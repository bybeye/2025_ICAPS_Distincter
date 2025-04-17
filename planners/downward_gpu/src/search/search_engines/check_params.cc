#include "check_params.h"

#include <iostream>
#include <set>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <chrono>

Check_param::Check_param() {
    options.writeautoms = FALSE;
    options.defaultptn = FALSE; // Ensure that partitions (color classes) are used
    options.getcanon = FALSE;    // Do not get canonical form
    options.digraph = FALSE;     // Indicate the graph is undirected
    SG_INIT(sg);  // Initialize sparse graph structure
}


std::vector<int> Check_param::calculate_auto_orbits(const std::vector<std::pair<int, int>>& edges, const std::vector<int>& colors) {
    int n = colors.size();
    int m = SETWORDSNEEDED(n);
    nauty_check(WORDSIZE, m, n, NAUTYVERSIONID);

    DYNALLSTAT(int, lab, lab_sz);
    DYNALLSTAT(int, ptn, ptn_sz);
    DYNALLSTAT(int, orbits, orbits_sz);

    DYNALLOC1(int, lab, lab_sz, n, "malloc");
    DYNALLOC1(int, ptn, ptn_sz, n, "malloc");
    DYNALLOC1(int, orbits, orbits_sz, n, "malloc");
    SG_ALLOC(sg, n, edges.size(), "malloc"); // Initialize sg with n vertices and given number of edges
    
    sg.nv = n;
    sg.nde = edges.size();

    // Add edges to the sparse graph
    int edge_count = 0;
    for (int i = 0; i < n; ++i) {
        sg.v[i] = edge_count;
        sg.d[i] = 0;
        for (const auto& edge : edges) {
            if (edge.first == i) {
                sg.e[edge_count++] = edge.second;
                sg.d[i]++;
            }
        }
    }

    // Initialize lab and ptn arrays with respect to colors
    std::vector<std::pair<int, int>> colored_vertices;
    for (int i = 0; i < n; ++i) {
        colored_vertices.emplace_back(colors[i], i);
    }

    // Sort vertices by color
    std::sort(colored_vertices.begin(), colored_vertices.end());

    for (int i = 0; i < n; ++i) {
        lab[i] = colored_vertices[i].second;
        if (i == n-1 || colored_vertices[i].first != colored_vertices[i+1].first) {
            ptn[i] = 0;  // End of a color class
        } else {
            ptn[i] = 1;  // Middle of a color class
        }
    }

    // Print lab and ptn arrays for verification
    // std::cout << "lab: ";
    // for (int i = 0; i < n; ++i) {
    //     std::cout << lab[i] << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "ptn: ";
    // for (int i = 0; i < n; ++i) {
    //     std::cout << ptn[i] << " ";
    // }
    // std::cout << std::endl;

    // Call sparsenauty to find automorphisms with vertex colors
    sparsenauty(&sg, lab, ptn, orbits, &options, &stats, NULL);
    
    // Output the results
    // std::cout << "Number of automorphisms: " << stats.grpsize1 << " * 10^" << stats.grpsize2 << std::endl;

    // Collect and return the orbits
    std::vector<int> orbit_list;
    for (int i = 0; i < sg.nv; ++i) {
        orbit_list.push_back(orbits[i]);
    }
    return orbit_list;
}


std::map<std::string, int> Check_param::gen_orbit_maps(const std::vector<int>& orbit_list, const std::unordered_map<std::string, int>& nodes_index){
    std::map<std::string, int> orbit_map;    
    for (const auto& item : nodes_index) {
        // std::cout << "Key: " << item.first << ", Value: " << item.second << std::endl;
	orbit_map[item.first] = orbit_list[item.second];
    }
    return orbit_map;
}


std::vector<OperatorID> Check_param::check_operators2(const std::vector<std::pair<std::string, std::vector<int>>> &state, BaseGraph* gnn_graph, const TaskProxy &task_proxy, std::vector<OperatorID> &op_ids) {
    // int removed_ops = 0;    
    std::vector<OperatorID> filtered_ops;
    std::unordered_set<std::string> param_names;
    std::unordered_set<std::string> hash_param_set;

    auto[edges, colors] = gnn_graph->get_edges_colors(state);
    std::unordered_map<std::string, int> nodes_index = gnn_graph->get_nodes_index();
    std::vector<int> orbit_list = calculate_auto_orbits(edges, colors);
    
    std::map<std::string, int> orbit_map = gen_orbit_maps(orbit_list, nodes_index);
    
    for (OperatorID op_id : op_ids) {
	OperatorProxy op = task_proxy.get_operators()[op_id];             
	std::string op_name = op.get_name();
        std::vector<std::string> pred_vector = splitStringBySpace(op_name);

        // gen hash string for functions
        std::ostringstream buffer;
        int param_num = pred_vector.size();
	buffer << pred_vector[0];
	for(int i=1; i<param_num; i++) {
            // Filter duplicated operators
            if (orbit_map.find(pred_vector[i]) != orbit_map.end()) {
                int orbit_value = orbit_map[pred_vector[i]];
                buffer << "_";
                buffer << orbit_value;
            } else {
                filtered_ops.push_back(op_id);
		std::cout << "ERROR: Unkown operator, " << op_id << std::endl;
                break;
            }
	}
            
	// check duplicate
	std::string hash_str = buffer.str();
	if (hash_param_set.find(hash_str) == hash_param_set.end()) {
            hash_param_set.insert(hash_str);
	    filtered_ops.push_back(op_id);
        }
        //else {
	    // std::cout << "Remove: " << op_name << std::endl;
	//    removed_ops++;
	//}	
    }

    // op_ids = filtered_ops;
    // return removed_ops;
    return filtered_ops;
}


std::vector<std::string> Check_param::splitStringBySpace(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string word;

    while (iss >> word) {
        result.push_back(word);
    }

    return result;
}
