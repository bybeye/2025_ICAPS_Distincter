import random
import sys
import time
import logging
import math
import torch
import numpy as np


logger = logging.getLogger('Logger')


class AverageMeter():
    """Computes and stores the average and current value"""

    def __init__(self, name, fmt=':f'):
        self.name = name
        self.fmt = fmt
        self.reset()

    def reset(self):
        self.val = 0
        self.avg = 0
        self.sum = 0
        self.count = 0

    def update(self, val, n=1):
        self.val = val
        self.sum += val * n
        self.count += n
        self.avg = self.sum / self.count

    def __str__(self):
        fmtstr = '{name} {avg' + self.fmt + '}'
        return fmtstr.format(**self.__dict__)


class ProgressMeter():
    def __init__(self, num_batches, meters, prefix=""):
        self.batch_fmtstr = self._get_batch_fmtstr(num_batches)
        self.meters = meters
        self.prefix = prefix

    def display(self, batch):
        entries = [self.prefix + self.batch_fmtstr.format(batch)]
        entries += [str(meter) for meter in self.meters]
        logger.info('\t'.join(entries))

    def _get_batch_fmtstr(self, num_batches):
        num_digits = len(str(num_batches // 1))
        fmt = '{:' + str(num_digits) + 'd}'
        return '[' + fmt + '/' + fmt.format(num_batches) + ']'


def worker_init_fn(worker_id):
    np.random.seed(worker_id)
    random.seed(worker_id)


def evaluate(model, device, val_loader, criterion, prefix):
    losses = AverageMeter('Loss', ':3.6f')
    model.eval()

    with torch.no_grad():
        for data in val_loader:
            data = data.to(device)
            h_true = data.y.float().to(device)

            if data.edge_attr is not None:
                output = model.forward(data.x, data.edge_index, data.edge_attr, data.batch)
            else:
                output = model.forward(data.x, data.edge_index, data.batch)

            if isinstance(output, tuple):
                _, h_pred = output
            else:
                h_pred = output

            loss = criterion.forward(h_pred, h_true)
            losses.update(loss.item(), data[0].size(0))

    logger.info(f"{prefix} loss: {losses.avg:4.6f}")
    return losses.avg


def adjust_learning_rate(optimizer, init_lr, epoch, num_epochs, warmup_epochs=5):
    """Decays the learning rate with half-cycle cosine after warmup"""
    if epoch < warmup_epochs:
        lr = init_lr * epoch / warmup_epochs
    else:
        lr = init_lr * 0.5 * (1. + math.cos(math.pi * (epoch - warmup_epochs) / (num_epochs - warmup_epochs)))

    for param_group in optimizer.param_groups:
        param_group['lr'] = lr
    return lr


def train(epoch, model, device, train_loader, criterion, optimizer, lr, num_epochs, warmup=5):
    batch_time = AverageMeter('Time', ':6.2f')
    data_time = AverageMeter('Data', ':6.2f')
    losses = AverageMeter('Loss', ':6.4f')
    iter_num = len(train_loader)

    model.train()
    end = time.time()
    for i, data in enumerate(train_loader):
            
        data_time.update(time.time() - end)
        data = data.to(device)
        h_true = data.y.float().to(device)
        adjust_learning_rate(optimizer, lr, epoch + i / iter_num, num_epochs, warmup)

        if data.edge_attr is not None:
            output = model.forward(data.x, data.edge_index, data.edge_attr, data.batch)
        else:
            output = model.forward(data.x, data.edge_index, data.batch)

        if isinstance(output, tuple):
            _, h_pred = output
        else:
            h_pred = output

        loss = criterion.forward(h_pred, h_true)
        optimizer.zero_grad(set_to_none=True)
        loss.backward()
        optimizer.step()

        losses.update(loss.item(), data[0].size(0))
        batch_time.update(time.time() - end)
        end = time.time()

    progress = ProgressMeter(len(train_loader), [batch_time, data_time, losses], prefix=f"Train Epoch: [{epoch + 1}]")
    progress.display(0)
    return losses.avg


def setup_logging(log_file, level):
    logger = logging.getLogger("Logger")
    logger.setLevel(level)
    logger.handlers = []
    formatter = logging.Formatter('%(asctime)s | %(levelname)s | %(message)s', datefmt='%Y-%m-%d,%H:%M:%S')

    console_handler = logging.StreamHandler(sys.stdout)
    console_handler.setLevel(level)
    console_handler.setFormatter(formatter)
    logger.addHandler(console_handler)
    return logger


def save_jit_model(model, model_dir, save_file):
    logger.info(f"Save: {model_dir + save_file}")
    jit_model = torch.jit.script(model)
    frozen_module = torch.jit.freeze(jit_model.eval())
    frozen_module = torch.jit.optimize_for_inference(frozen_module)
    torch.jit.save(frozen_module, model_dir + save_file)


def get_train_val(data_set, val_rate=0.1):
    # get unique plan namesy
    aset = set()
    for item in data_set:
        if item.plan_name not in aset:
            aset.add(item.plan_name)

    train_plans = []
    val_plans = []
    sorted_plan_names = sorted(list(aset))

    for index, item in enumerate(sorted_plan_names):
        # Due to uneven difficulty for different problems, randomly selecting them may lead that the diffculty of problems are changed dramatically.
        # So, I select the last one of the intervals in val set.
        if (val_rate != 0 and index % (1 / val_rate) == (1 / val_rate) - 1):  # or item_num < 30:
            val_plans.append(item)
        else:
            train_plans.append(item)

    train_set = []
    val_set = []
    for item in data_set:
        if not hasattr(item, 'sub_plan_name'):
            item.sub_plan_name = ""

        if item.y == 0 or item.sub_plan_name.startswith("i"):  # or item.sub_plan_name.startswith("c"):
            continue

        if item.plan_name in train_plans:
            train_set.append(item)
        else:
            if item.sub_plan_name == "":
                val_set.append(item)

    logger.info(f"train size: {len(train_set)}, validation size: {len(val_set)}")
    return train_set, val_set


def get_trainset(dataset):
    train_dataset = []
    for _, item in enumerate(dataset):
        if item.optim == 1:
            train_dataset.append(item)

    return train_dataset


def get_val_dict(dataset):
    # val_dict all validation keys, which can get indexes of validation data in a step.
    val_dict = {}
    for index, item in enumerate(dataset):

        if hasattr(item, 'sub_plan_name') and item.sub_plan_name != "":
            continue

        key = item.problem + "_" + str(item.g)
        if key in val_dict:
            val_dict[key].append(index)
        else:
            val_dict[key] = [index]

    # remove keys whose list size is 1
    keys_to_remove = [key for key in val_dict if len(val_dict[key]) < 2]  # remove keys with length 1
    for key in keys_to_remove:
        del val_dict[key]

    return val_dict


def cal_accuracy(val_hs, val_dict):
    correct_num = 0
    val_hs = [int(h * 10) for h in val_hs]
    for key in val_dict.keys():
        val_step_hlist = [val_hs[i] for i in val_dict[key]]   # get hs with indexs
        min_value = min(val_step_hlist)  # Find the minimum value in the list

        try:
            val_step_hlist.index(min_value, 1)  # Find minimum value only at 0
        except ValueError:
            correct_num += 1

    total_num = len(val_dict.keys())
    accuracy = correct_num / total_num * 100
    return accuracy


def evaluate_accuracy(model, device, val_loader, criterion, val_dict, prefix):
    losses = AverageMeter('Loss', ':3.6f')
    model.eval()

    hs_list = []
    with torch.no_grad():
        for data in val_loader:
            data = data.to(device)
            h_true = data.y.float().to(device)

            if data.edge_attr is not None:
                output = model.forward(data.x, data.edge_index, data.edge_attr, data.batch)
            else:
                output = model.forward(data.x, data.edge_index, data.batch)

            if isinstance(output, tuple):
                _, h_pred = output
            else:
                h_pred = output

            loss = criterion.forward(h_pred, h_true)
            h_pred = h_pred.cpu()
            hs_list.extend(h_pred)
            losses.update(loss.item(), data[0].size(0))

    accuracy = cal_accuracy(hs_list, val_dict)
    logger.info(f"{prefix} accuracy:  {accuracy:.2f}, loss: {losses.avg:4.4f}")
    return accuracy, losses.avg
