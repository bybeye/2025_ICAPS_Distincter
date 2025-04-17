import os
os.environ["CUBLAS_WORKSPACE_CONFIG"] = ":4096:8"

import copy
import math
import pickle
import random
import logging
import argparse
import warnings
import numpy as np
from pathlib import Path
warnings.filterwarnings("ignore", category=FutureWarning)
warnings.filterwarnings("ignore", category=UserWarning)

import torch
from torch import optim
from torch_geometric.loader import DataLoader

from util.gnn import RGCN_HX
from util.loss import RMSELoss
from util.tools import train, save_jit_model, setup_logging, get_trainset, evaluate_accuracy, get_val_dict, worker_init_fn
from util.testing_evaluate import testing_evaluate



def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--seed", type=int, default=1)
    parser.add_argument("--domain", type=str, default='blocksworld')
    parser.add_argument("--data_root", type=str, default="./benchmarks/datasets/pog_datasets/")
    parser.add_argument("--model_dir", type=str, default="./models/")
    parser.add_argument('--val_rate', type=float, default=0.1)

    # for train
    parser.add_argument("--lr", type=float, default=1e-3)
    parser.add_argument("--warmup", type=int, default=10)
    # parser.add_argument("--batch_size", type=int, default=1024)
    parser.add_argument("--num_cycles", type=int, default=1)
    parser.add_argument("--cycle_epochs", type=int, default=20)
    parser.add_argument('--weight_decay', type=float, default=0)

    # for evaluate
    parser.add_argument("--p_num", type=int, default=1, help="process number")
    parser.add_argument("--fail_limit", type=int, default=5)
    parser.add_argument("--timeout", type=int, default=1800, help="timeout seconds")
    parser.add_argument("--pruning", type=str, default="action_state")

    # model params
    parser.add_argument("--network", type=str, default="RGCN")
    parser.add_argument("--nlayers", type=int, default=3)
    parser.add_argument("--nhid", type=int, default=64)
    parser.add_argument("--rep", type=str, default='pog2')  #  We rename POG2 to TILG in the paper.
    return parser.parse_args()


def train_model():
    logger.info(args)
    random.seed(args.seed)
    np.random.seed(args.seed)
    torch.manual_seed(args.seed)
    torch.cuda.manual_seed(args.seed)
    torch.backends.cudnn.benchmark = False
    torch.backends.cudnn.deterministic = True
    torch.use_deterministic_algorithms(True)
    os.environ['PYTHONHASHSEED'] = str(args.seed)

    if torch.cuda.is_available():
        args.device = torch.device("cuda")
    else:
        args.device = torch.device("cpu")
        logger.info("No GPU exists, run with CPU!!!")

    with open(args.data_root + args.domain + ".data", "rb") as fp:
        dataset = pickle.load(fp)
        trainset = get_trainset(dataset)
        val_dict = get_val_dict(dataset)
        num_node_types = 0
        num_edge_types = 0
        for _, data in enumerate(trainset):
            num_node_types = int(max(torch.max(data.x).item() + 1, num_node_types))
            num_edge_types = int(max(torch.max(data.edge_attr).item() + 1, num_edge_types))
        logger.info(f"dataset size: {len(dataset)}, trainset size: {len(trainset)}, valset size: {len(val_dict.keys())}")

    # keep iteration number same
    args.batch_size = int(len(trainset) / 100)
    logger.info(f"Batch_size: {args.batch_size}")
    train_loader = DataLoader(trainset, batch_size=args.batch_size, shuffle=True, drop_last=True, num_workers=0, worker_init_fn=worker_init_fn)
    val_loader = DataLoader(dataset, batch_size=args.batch_size, shuffle=False, drop_last=False)

    # init model
    logger.info(f"Init RGCN network with node and edge types: {num_node_types}, {num_edge_types}")
    model = RGCN_HX(num_node_types, num_edge_types, hidden_dim=args.nhid, num_layers=args.nlayers).to(args.device)
    logger.info(f"Model params size: {sum(p.numel() for p in model.parameters())}")
    criterion = RMSELoss()
    optimizer = optim.SGD(model.parameters(), lr=args.lr, momentum=0.9, weight_decay=args.weight_decay)

    best_epoch = 0
    best_metric = 0
    best_loss = 999
    bak_model = None
    total_epochs = args.num_cycles * args.cycle_epochs + args.warmup
    for epoch in range(total_epochs):
        train_loss = train(epoch, model, args.device, train_loader, criterion, optimizer, args.lr, args.cycle_epochs, args.warmup)
        if math.isnan(train_loss):
            logger.error("Nan occurs, exit!")
            return -1

        if len(val_dict.keys()) != 0:
            val_acc, val_loss = evaluate_accuracy(model, args.device, val_loader, criterion, val_dict, "Val Epoch: [" + str(epoch + 1) + "]")
            if (val_acc > best_metric or (val_acc == best_metric and best_loss > val_loss)) and epoch > args.warmup:
                best_epoch = epoch + 1
                best_metric = val_acc
                best_loss = val_loss
                bak_model = copy.copy(model)
        elif epoch == total_epochs - 1:
            bak_model = copy.copy(model)

    logger.info(f"Best_val_acc {best_metric:.2f} at epoch {best_epoch}")
    args.model_file = f'{args.domain}_{args.network}_{args.seed}.dt'
    save_jit_model(bak_model, args.model_dir, args.model_file)
    return 0


if __name__ == '__main__':
    global logger
    args = parse_args()
    Path(args.model_dir).mkdir(exist_ok=True)
    Path("logs/aux").mkdir(parents=True, exist_ok=True)

    logger = setup_logging(None, logging.DEBUG)
    for _ in range(10):
        status = train_model()
        if status != 0:
            args.seed += 10
        else:
            break

    # Test evaluate
    if torch.cuda.is_available() and status == 0:
        logger.info("Begin test evaluate...")
        torch.cuda.empty_cache()

        if args.pruning != 'ablation':
            pruning_types = [args.pruning]
        else:
            pruning_types = ['none', 'action', 'state', 'action_state']

        for pruning_type in pruning_types:
            args.pruning = pruning_type
            logger.info(f"--------------------Begine pruning {args.pruning}------------")
            coverage = testing_evaluate(args)
            logger.info(f"coverage: {coverage}")
            logger.info(f"--------------------End pruning {args.pruning}---------------")
    else:
        logger.error("Cannot find the suitable seed. Exit without evaluation!!!")
