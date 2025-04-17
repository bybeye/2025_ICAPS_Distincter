import copy
import logging

import torch
from .base_graph import BaseGraph

logger = logging.getLogger('Logger')


class PredicateObjectGraph2(BaseGraph):

    def __init__(self, domain_pddl, problem_pddl):
        super().__init__(domain_pddl, problem_pddl)

    def gen_nodes(self):
        nodes = []
        nodes_index = {}
        node_names = []
        node_colors = []

        # gen objects
        all_node_types = set(list(self.constants.keys()) + list(self.objects.keys()))
        for key in sorted(all_node_types):
            # add constant nodes
            if key in self.constants:
                for obj in self.constants[key]:
                    nodes.append([0, self.node_class.index(key)])
                    node_names.append(obj)
                    node_colors.append(0)
                    nodes_index[obj] = len(nodes_index)

            if key in self.objects:
                for obj in self.objects[key]:
                    nodes.append([0, self.node_class.index(key)])
                    node_names.append(obj)
                    node_colors.append(0)
                    nodes_index[obj] = len(nodes_index)

        return nodes, nodes_index, node_names, node_colors

    def gen_init(self, predicates, node_type, debug=False):
        # gen facts
        for predicate_str in predicates:
            items = predicate_str.split(" ")
            param_nums = len(items) - 1
            predicate = items[0]
            predicate_str = predicate_str.replace(" ", "_")

            if debug:
                if predicate not in self.predicates:
                    print(f"ERROR: gen_goal_edges, {predicate} not in predicates {self.predicates}")

                if len(self.predicates[predicate]) != param_nums:
                    print(f"ERROR: The number of params is not right!!! {len(self.predicates[predicate])} {param_nums}")

            # add fact (goal / state) node
            # self.nodes.append([0, self.node_class.index(node_type)])  #  goal or init
            if node_type == "goal":
                self.nodes.append([1, self.node_class.index(predicate)])
                self.node_names.append(predicate)
                self.node_colors.append(2)
            else:
                self.nodes.append([0, self.node_class.index(predicate)])
                self.node_names.append(predicate)
                self.node_colors.append(1)
            self.nodes_index[predicate_str] = len(self.nodes_index)
            fact_index = len(self.nodes_index) - 1

            for i in range(param_nums):
                # fact to obj
                if items[i + 1] == "" or items[i + 1] == " ":
                    continue

                self.edges.append([fact_index, self.nodes_index[items[i + 1]]])
                self.edges.append([self.nodes_index[items[i + 1]], fact_index])
                self.edge_attrs.append(i)
                self.edge_attrs.append(i)

            # self.x = torch.cat(self.nodes, dim=0)
            self.x = torch.tensor(self.nodes)

    # Make sure this function invoking is after gen_nodes_edges
    def get_node_names_colors(self, predicates):
        node_names = copy.copy(self.node_names)
        node_colors = copy.copy(self.node_colors)
        for (predicate, items) in predicates:
            if predicate in ('', ' '):
                continue

            param_nums = len(items)
            predicate_str = predicate + "_" + "_".join(items) if param_nums > 0 else predicate
            if predicate_str in self.nodes_index:
                index = self.nodes_index[predicate_str]
                node_names[index] = predicate
                node_colors[index] = 3
                continue

            node_names.append(predicate)
            node_colors.append(1)

        return node_names, node_colors

    def gen_nodes_edges(self, predicates, node_type, debug=False):
        # gen facts
        nodes = self.x.clone()
        node_num = len(self.nodes_index)
        new_nodes = []
        new_edges = []  # [[] for i in range(self.largest_predicate + 1)]
        new_edge_attrs = []

        for (predicate, items) in predicates:
            # items = predicate_str.split(" ")
            # predicate = items[0]
            if predicate in ('', ' '):
                continue

            param_nums = len(items)
            predicate_str = predicate + "_" + "_".join(items) if param_nums > 0 else predicate
            # predicate_index.append(predicate_str)
            # print(predicate_str)
            if node_type == "state" and predicate_str in self.nodes_index:
                index = self.nodes_index[predicate_str]
                nodes[index, 0] = 2
                continue

            if debug:
                if predicate not in self.predicates:
                    print(f"ERROR: gen_goal_edges, {predicate} not in predicates {self.predicates}")

                if len(self.predicates[predicate]) != param_nums:
                    print(f"ERROR: The number of params is not right!!! {len(self.predicates[predicate])} {param_nums}")

            # add fact (goal / state) node
            new_nodes.append([0, self.node_class.index(predicate)])
            # new_nodes.append(self._one_hot_node(self.node_class.index(node_type), len(self.node_class)).unsqueeze(0))
            node_num += 1
            fact_index = node_num - 1

            for i in range(param_nums):
                # fact to obj
                new_edges.append([fact_index, self.nodes_index[items[i]]])
                new_edges.append([self.nodes_index[items[i]], fact_index])
                new_edge_attrs.append(i)
                new_edge_attrs.append(i)

        # edges = []
        # for i in range(self.largest_predicate + 1):
        #     edges.append(torch.cat((torch.tensor(self.edges[i], dtype=torch.long), torch.tensor(new_edges[i], dtype=torch.long)), dim=0).t().contiguous())

        nodes = torch.cat((nodes, torch.tensor(new_nodes)), dim=0)
        edges = torch.cat((torch.tensor(self.edges, dtype=torch.long), torch.tensor(new_edges, dtype=torch.long)), dim=0).t()
        new_edge_attrs = torch.cat((torch.tensor(self.edge_attrs, dtype=torch.long), torch.tensor(new_edge_attrs, dtype=torch.long)), dim=0)

        return nodes, edges, new_edge_attrs
