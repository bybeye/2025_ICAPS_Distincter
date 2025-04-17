import time
import copy
import logging
from enum import Enum
from typing import List, Tuple
from abc import abstractmethod

import torch
import torch.nn.functional as F
from .ParseProblem import ParseProblem
from .ParseDomain import ParseDomain

logger = logging.getLogger('Logger')


class NodeType(Enum):
    STATE = 0
    GOAL = 1
    ACHIEVED = 1


# class BaseGraph(Representation):
class BaseGraph():

    # for representation class
    lifted = True
    directed = False
    n_edge_labels = 1

    def __init__(self, domain_pddl, problem_pddl):

        # Problems PDDL
        with open(problem_pddl, "r", encoding='UTF-8') as fp:
            problem_pddl = fp.read()
            problem_parser = ParseProblem(problem_pddl)

        self.goals = problem_parser.parse_goals()
        self.check_init = False
        self.inits = problem_parser.parse_inits()
        self.objects = problem_parser.parse_objects()

        # Domain PDDL
        with open(domain_pddl, "r", encoding='UTF-8') as fp:
            domain_pddl = fp.read()
            domain_parser = ParseDomain(domain_pddl)

        self.types = domain_parser.parse_types()
        self.constants = domain_parser.parse_constants()
        self.predicates = domain_parser.parse_predicates()

        # node and edge types
        self.largest_predicate = 1
        self.node_class = self.gen_node_class()

        # Graph
        self.nodes, self.nodes_index, self.node_names, self.node_colors = self.gen_nodes()

        self.edges = []  # {i: [] for i in range(self.largest_predicate + 1)}
        self.edge_attrs = []
        self.gen_init(self.goals, "goal")

    def convert_to_pyg(self):
        return

    def _compute_graph_representation(self):
        return

    def state_to_cgraph(self, state: List[Tuple[str, List[str]]]):
        return

    def str_to_state(self, s) -> List[Tuple[str, List[str]]]:
        """Used in dataset construction to convert string representation of facts into a (pred, [args]) representation"""
        state = []
        for fact in s:
            fact = fact.replace(")", "").replace("(", "")
            toks = fact.split()
            if toks[0] == "=":
                continue
            if len(toks) > 1:
                state.append((toks[0], toks[1:]))
            else:
                state.append((toks[0], ()))
        return state

    def gen_node_class(self):
        node_classes = ["state", "goal"] + self.types

        for key in self.predicates.keys():
            node_classes.append(key)
            if self.largest_predicate < len(self.predicates[key]):
                self.largest_predicate = len(self.predicates[key])

        return node_classes

    @abstractmethod
    def gen_nodes(self):
        pass

    @abstractmethod
    def gen_init(self, predicates, node_type, debug=False):
        pass

    @abstractmethod
    def gen_nodes_edges(self, predicates, node_type, debug=False):
        pass

    def state_to_tensor(self, predicates: List[Tuple[str, List[str]]]):
        # current states from fast-downward
        # start_time = time.time()

        if not self.check_init:
            print("pog init")
            self.check_init = True
            miss_predicates = []

            current_states = set()
            for predicate, params in predicates:
                if predicate in ('', ' '):
                    continue

                if len(params) > 0:
                    current_states.add(predicate + " " + " ".join(params))
                else:
                    current_states.add(predicate)

            for init_predicate in self.inits:
                if init_predicate not in current_states:
                    miss_predicates.append(init_predicate)

            if len(miss_predicates) > 0:
                print(f"Missing: {miss_predicates}")
                # self.predicate_index
                self.gen_init(miss_predicates, "state")

            # print(self.nodes_index)

        # add state edges:
        x, edge_index, edge_attrs = self.gen_nodes_edges(predicates, "state")

        # print(nodes)
        # return x, edge_indices
        # edge_index = [edge_index[i]).t().contiguous() for i in range(self.largest_predicate + 1)]

        # print(sum(len(fact[1]) + 1 for fact in state), [len(fact[1]) + 1 for fact in state])
        # to_add = sum(len(fact[1]) + 1 for fact in state)
        # x = torch.nn.functional.pad(x, (0, 0, 0, to_add), "constant", 0)

        # edge_attrs = torch.tensor(edge_attrs, dtype=torch.int)
        # dim = int(torch.max(edge_attrs).item() + 1)
        # edge_index = [edge_index[:, edge_attrs==i] for i in range(dim)]

        # end_time = time.time()
        # time_taken = end_time - start_time
        # print(f"Time taken to run the function: {time_taken} seconds")

        # print(state)
        # print(x)
        # time.sleep(1)
        return x, edge_index, edge_attrs  # , nodes_index
