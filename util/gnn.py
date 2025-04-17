import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import time
import warnings
from abc import ABC, abstractmethod

from torch_geometric.nn import MessagePassing
from torch.nn import Sequential, Linear, ReLU, Dropout, LeakyReLU, BatchNorm1d
from torch.nn.parameter import Parameter
from torch.nn.modules.module import Module
from torch.nn import Embedding
from torch import Tensor
from typing import Optional, List, FrozenSet
from torch_geometric.loader import DataLoader
from torch_geometric.data import Data
from torch_geometric.nn.inits import glorot, zeros
from torch_geometric.nn import global_add_pool, global_max_pool, global_mean_pool
from torch_geometric.nn import GATConv, GATv2Conv, NNConv, RGCNConv, GCNConv, FastRGCNConv, RGATConv, CuGraphRGCNConv
from torch_geometric.nn.conv import ECConv
from torch_geometric.nn import PNAConv

from representation import REPRESENTATIONS


class RGCN_HX(torch.nn.Module):
    def __init__(self, num_node_types, num_edge_types, hidden_dim=32, num_layers=4):
        super().__init__()
        self.num_layers = num_layers
        self.num_node_types = num_node_types

        self.convs = torch.nn.ModuleList()
        self.convs.append(RGCNConv(num_node_types, hidden_dim, num_relations=num_edge_types, aggr='mean').jittable())
        for _ in range(num_layers - 1):
            self.convs.append(RGCNConv(hidden_dim, hidden_dim, num_relations=num_edge_types, aggr='mean').jittable())
        self.bns = torch.nn.ModuleList([torch.nn.BatchNorm1d(hidden_dim) for _ in range(num_layers)])
        self.linear = Linear(hidden_dim, 1)
        self.relu = torch.nn.LeakyReLU(negative_slope=0.01)

    def forward(self, x: Tensor, edge_index: Tensor, edge_attrs: Tensor, batch: Optional[Tensor]):
        with torch.no_grad():
            node_type = x[:, 0]
            x = F.one_hot(x[:, 1].to(torch.int64), num_classes=self.num_node_types).float()
            x[:, 0] = node_type + x[:, 0]

        # Apply GNN layers
        for i, (conv, bn) in enumerate(zip(self.convs, self.bns)):
            if i != self.num_layers - 1:
                x = self.relu(bn(conv(x, edge_index, edge_attrs)))
            else:
                x = conv(x, edge_index, edge_attrs)

        # Global pooling and final layer for the heuristic prediction
        feature = global_add_pool(x, batch)
        h = self.linear(feature)
        return feature, h.squeeze(1)


