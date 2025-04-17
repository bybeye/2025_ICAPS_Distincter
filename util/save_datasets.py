import time
import argparse
import itertools
import pickle
import os
import sys
import random
import itertools

import numpy as np
import torch
from torch_geometric.loader import DataLoader
from sklearn.model_selection import train_test_split
from torch_geometric.data import Data
import torch.nn.functional as F


sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
import representation
# from util.stats import get_stats
from representation import REPRESENTATIONS


def parse_args(domain='blocksworld'):
    parser = argparse.ArgumentParser()
    parser.add_argument("--domain", type=str, default='blocksworld', choices=IPC2023_LEARNING_DOMAINS)

    # model params
    parser.add_argument("--rep", type=str, default='pog2')
    parser.add_argument("--planner", type=str, default="fd", choices=["fd", "pwl"])
    tmp_args = parser.parse_args([])
    # domain = tmp_args.domain
    tmp_args.domain_pddl = f"../benchmarks/{domain}/domain.pddl"
    tmp_args.tasks_dir = f"../benchmarks/subplans/{domain}/"
    tmp_args.plans_dir = f"../benchmarks/subplans/{domain}/"

    return tmp_args

_DOWNWARD = "../planners/downward_gpu/fast-downward.py"
_POWERLIFTED =""

def get_plan_info(domain_pddl, problem_pddl, plan_file, args, planner="fd"):
    states = []
    actions = []
    with open(plan_file, "r") as f:
        for line in f.readlines():
            if ";" in line:
                continue
            actions.append(line.replace("\n", ""))

    aux_garbage = repr(hash((domain_pddl, problem_pddl, plan_file, repr(args))))
    aux_garbage = aux_garbage.replace("-", "n")
    state_output_file = aux_garbage + ".states"
    sas_file = aux_garbage + ".sas"

    cmd = {
        "pwl": f"export PLAN_PATH={plan_file} "
        + f"&& {_POWERLIFTED} -d {domain_pddl} -i {problem_pddl} -s perfect "
        + f"--plan-file {state_output_file}",
        "fd": f"export PLAN_INPUT_PATH={plan_file} "
        + f"&& export STATES_OUTPUT_PATH={state_output_file} "
        + f"&& {_DOWNWARD} --sas-file {sas_file} {domain_pddl} {problem_pddl} "
        + f"--search 'perfect([blind()])'",  # need filler h
    }[planner]
    output = os.popen(cmd).readlines()
    if output:
        pass  # this is so syntax highlighting sees `output`
    print("cmd", cmd)
    # print("output", output)
    # os.system(cmd)
    with open(state_output_file, "r") as f:
        for line in f.readlines():
            if ";" in line:
                continue
            line = line.replace("\n", "")
            s = set()
            for fact in line.split():
                if "(" not in fact:
                    lime = f"({fact})"
                else:
                    pred = fact[: fact.index("(")]
                    fact = fact.replace(pred + "(", "").replace(")", "")
                    args = fact.split(",")[:-1]
                    lime = "(" + " ".join([pred] + args) + ")"
                s.add(lime)
            states.append(s)
    if os.path.exists(sas_file):
        os.remove(sas_file)
    if os.path.exists(state_output_file):
        os.remove(state_output_file)

    schema_cnt = {}
    for action in actions:
        schema = action.replace("(", "").split()[0]
        if schema not in schema_cnt:
            schema_cnt[schema] = 0
        schema_cnt[schema] += 1

    ret = []
    for i, state in enumerate(states):
        if i == len(actions):
            ret.append((state, {0:0}))
        else:
            action = actions[i]
            schema = action.replace("(", "").split()[0]
            ret.append((state, schema_cnt.copy()))
            schema_cnt[schema] -= 1
    return ret


node_types = None
edge_types = None
def gen_graphs(args, debug=False):
    global node_types
    global edge_types
    print("Generating graphs from plans...")
    graphs = []
    
    for plan_name in sorted(list(os.listdir(args.plans_dir))):
        if plan_name.endswith(".plan"):
            problem_set = []
            problem_pddl = f"{args.tasks_dir}/{plan_name.replace('.plan', '.pddl')}"
            # print(args.domain_pddl)
            # print(problem_pddl)

            if "_i" in plan_name:
                continue

            if os.path.exists(problem_pddl):
                plan_file_dir = f"{args.plans_dir}/{plan_name}"
                rep = REPRESENTATIONS[args.rep](domain_pddl=args.domain_pddl, problem_pddl=problem_pddl)
                if args.rep == "ilg" or  args.rep == "llg" :
                    rep.convert_to_pyg()
                plan = get_plan_info(args.domain_pddl, problem_pddl, plan_file_dir, args)

                print("node_class:", rep.node_class)
                for state, schema_cnt in plan:
                    state = rep.str_to_state(state)
                    x, edge_index, edge_attrs = rep.state_to_tensor(state)

                    if debug:
                        print("x:", x.shape, ", edge_index:", edge_index.shape, ", edge_attrs:", edge_attrs.shape)
                        print("x:", x)
                        print("edge index:", edge_index.T)
                        print("edge_attrs:", edge_attrs)
                        print("nodes index:", rep.nodes_index)

                        node_type = x[:, 0]
                        temp_x = F.one_hot(x[:, 1].to(torch.int64), num_classes=10).float()
                        temp_x[:, 0] = node_type + temp_x[:, 0]
                        print(temp_x)

                        list_of_edge_index = [edge_index[(edge_attrs == i).nonzero()].squeeze(1).T for i in range(3)]
                        print(list_of_edge_index)
                        
                    y = sum(schema_cnt.values())
                    if edge_attrs is not None:
                        graph = Data(x=x, edge_index=edge_index, edge_attr=edge_attrs, y=y)
                    else:
                        graph = Data(x=x, edge_index=edge_index, y=y)
   
                    graph.plan_name = plan_name
                    plan_sub_names = plan_name.replace('.plan', '').split("_")
                    print(plan_sub_names)
                    if len(plan_sub_names) == 3:
                        graph.plan_name = plan_sub_names[1]
                        graph.sub_plan_name = plan_sub_names[2]
                    elif len(plan_sub_names) == 2:
                        graph.plan_name = plan_sub_names[1]
                        graph.sub_plan_name = ""

                    graphs.append(graph)
            
            if debug:
                break

    print("Graphs generated!")
    return graphs


IPC2023_LEARNING_DOMAINS = [
    "blocksworld",     
    "ferry",
    "childsnack",
    "floortile",
    "miconic",
    "rovers",
    "satellite",
    "sokoban",
    "spanner",
    "transport"
]

for domain in IPC2023_LEARNING_DOMAINS:
    args = parse_args(domain)
    print(args)
    graphs = gen_graphs(args)

    with open(domain + ".data", "wb") as fp:
        pickle.dump(graphs, fp)
