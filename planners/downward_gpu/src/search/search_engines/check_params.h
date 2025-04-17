#include <string>
#include <vector>
#include "../operator_id.h"
#include "../heuristic.h"
#include "../heuristics/base_graph.h"

#include <iostream>
#include <algorithm>

extern "C" {
#include <nausparse.h>
#include <traces.h>
}

class Check_param {

    DEFAULTOPTIONS_SPARSEGRAPH(options);
    statsblk stats;
    sparsegraph sg;  // Declare sparse graph structure
    
    std::vector<std::string> splitStringBySpace(const std::string &str);
    std::vector<int> calculate_auto_orbits(const std::vector<std::pair<int, int>>& edges, const std::vector<int>& colors);    

public:
    Check_param();
    std::map<std::string, int> gen_orbit_maps(const std::vector<int>& orbit_list, const std::unordered_map<std::string, int>& nodes_index);
    std::vector<OperatorID> check_operators2(const std::vector<std::pair<std::string, std::vector<int>>> &state, BaseGraph* gnn_graph, const TaskProxy &task_proxy, std::vector<OperatorID> &op_ids);
};
