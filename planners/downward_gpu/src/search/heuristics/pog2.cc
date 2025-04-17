#include "pog2.h"

#include <chrono>


PredicateObjectGraph2::PredicateObjectGraph2(const std::string& domain_file, const std::string& problem_file) : BaseGraph(domain_file, problem_file) {
    // Graph
    gen_nodes();
    gen_init(goals, "goal");
}        

void PredicateObjectGraph2::gen_nodes() {
    // Create a set of all node types from constants and objects
    std::set<std::string> all_node_types;
    for (const auto& pair : constants) {
	all_node_types.insert(pair.first);
    }
    for (const auto& pair : objects) {
	all_node_types.insert(pair.first);
    }

    // Iterate over sorted node types
    for (const auto& key : all_node_types) {
	if (constants.find(key) != constants.end()) {
            for (const auto &obj : constants[key]) {
		nodes_index[obj] = nodes.size() / 2;
                nodes.emplace_back(0);     // unachievd,
		nodes.emplace_back(node_classes[key]);
            }
	}
	if (objects.find(key) != objects.end()) {
            for (const auto &obj : objects[key]) {
		nodes_index[obj] = nodes.size() / 2;
		nodes.emplace_back(0);     // unachievd,
		nodes.emplace_back(node_classes[key]);
            }
        }
    }

    // Generate nodes for predicate types
    for (const auto &predicate : predicates) {
	// Since we don't create predicate nodes, we use their class index instead.
        nodes_index[predicate] = node_classes[predicate];
    }
}


void PredicateObjectGraph2::gen_init(const std::set<std::string>& predicates_strs, const std::string& node_type) {
    for (const auto& predicate_str : predicates_strs) {
	std::istringstream iss(predicate_str);
	std::string predicate;
	std::vector<std::string> items;
        std::string item;
        
	while (iss >> item) {
	    items.push_back(item);
	}

	if (items.empty()) continue;
	int param_nums = items.size();

	// Add fact (goal / state) node
        int fact_index = nodes.size() / 2;
        if (node_type == "goal"){
	    nodes.emplace_back(1);   // unachieved
	    nodes_goal_index[predicate_str] = fact_index;  // goal
	} else {
	    nodes.emplace_back(0);   // current state             
        }

	nodes.emplace_back(nodes_index[items[0]]);
        for (int i = 1; i < param_nums; i++) {
            if (items[i].empty())
		continue;
	    // Add edges, predicate to fact
            edges_1.emplace_back(fact_index);
            edges_2.emplace_back(nodes_index[items[i]]);
            edges_1.emplace_back(nodes_index[items[i]]);
            edges_2.emplace_back(fact_index);
            edge_attrs.push_back(i - 1);
	    edge_attrs.push_back(i - 1);            
        }
    }
}

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>> PredicateObjectGraph2::gen_nodes_edges(const std::vector<std::pair<std::string, std::vector<int>>>& states) {
    // Use the copy constructor
    int fact_index = nodes.size() / 2;
    std::vector<int> new_nodes(nodes);
    std::vector<int> new_edges_1(edges_1);
    std::vector<int> new_edges_2(edges_2);    
    std::vector<int> new_edge_attrs(edge_attrs);

    for (const auto &state : states) {
	const std::string& predicate_str = state.first;
	const std::vector<int>& items = state.second;
        int param_nums = items.size();

	if (predicate_str.empty()) continue;

	if (nodes_goal_index.find(predicate_str) != nodes_index.end()) {
	    int index = nodes_goal_index.at(predicate_str) * 2; // * 2 changes to the original length
	    new_nodes[index] = 2;
	    continue;
	}

        new_nodes.emplace_back(0);            // unachievd,
	new_nodes.emplace_back(items[0]);     // current state, 
        for (int i = 1; i < param_nums; i++) {
            // int node_id = nodes_index.at(items[i]);
	    int node_id = items[i];
            new_edges_1.emplace_back(fact_index);
	    new_edges_2.emplace_back(node_id);
            new_edges_1.emplace_back(node_id);		
            new_edges_2.emplace_back(fact_index);
	    new_edge_attrs.emplace_back(i - 1);
            new_edge_attrs.emplace_back(i - 1);
	}
	fact_index+=1;
    }

    return {new_nodes, new_edges_1, new_edges_2, new_edge_attrs};
}
