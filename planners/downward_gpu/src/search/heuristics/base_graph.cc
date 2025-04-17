#include "base_graph.h"

#include <chrono>

// gen graph
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/isomorphism.hpp>
#include <openssl/evp.h>
#include <chrono> // For chrono::seconds


std::string BaseGraph::read_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: '" << filename << "'" << std::endl;
        throw std::runtime_error("Failed to open file.");
    }
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}


BaseGraph::BaseGraph(const std::string& domain_file, const std::string& problem_file) {
    try{
	std::string domain_content = read_file(domain_file);
	ParseDomain domain_parser(domain_content);
	types = domain_parser.parse_types();
	constants = domain_parser.parse_constants();
	predicates = domain_parser.parse_predicates();
	largest_predicate = domain_parser.get_largest_param_num();

	std::string problem_content = read_file(problem_file);
        ParseProblem problem_parser(problem_content);
        objects = problem_parser.parse_objects();
        inits = problem_parser.parse_inits();
        goals = problem_parser.parse_goals();

	// std::this_thread::sleep_for(std::chrono::seconds(1));
        // std::cout << "inits..." << std::endl;

	gen_node_classes();
    } catch (const std::exception& e) {
	std::cerr << "Error: " << e.what() << std::endl;
    }
}


void BaseGraph::gen_node_classes() {
    node_classes.insert({
        {"state", 0},
        {"goal", 1}
    });
    
    for (const auto &type : types) {
	node_classes.emplace(type, node_classes.size());
    }

    for (const auto &predicate : predicates) {
	node_classes.emplace(predicate, node_classes.size());            
	// node_classes.emplace(pair.second[0], node_classes.size());     
	// if (largest_predicate < static_cast<int>(pair.second.size())) {
	//     largest_predicate = static_cast<int>(pair.second.size());
        // }
    }
}


void BaseGraph::gen_nodes() {
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
	nodes_index[predicate] = nodes.size() / 2;
	nodes.emplace_back(0);     // unachievd,
	nodes.emplace_back(node_classes[predicate]);
    }
}


void BaseGraph::init_missing_preds(const std::vector<std::pair<std::string, std::vector<int>>> &states) {
    if (!check_init) {
	check_init = true;
        
	std::set<std::string> pres_set;
	for (const auto& state : states) {
	    pres_set.insert(state.first);
	}

	std::set<std::string> miss_predicates;
	for (const auto& init : inits){
	    if (pres_set.find(init) == pres_set.end()) {
		miss_predicates.insert(init);
	    }
	}

	if (!miss_predicates.empty()) {
	    std::cout << "Missing predicates: ";
	    for (const auto& mp : miss_predicates) {
		std::cout << mp << ", ";
            }
            std::cout << std::endl;
	    gen_init(miss_predicates, "init");
	}
    }
}


std::tuple<std::vector<std::pair<int, int>>, std::vector<int>> BaseGraph::get_edges_colors(const std::vector<std::pair<std::string, std::vector<int>>> &state) {
    init_missing_preds(state);
    auto [new_nodes, new_edges_1, new_edges_2, new_edge_attrs] = gen_nodes_edges(state);

    std::vector<std::pair<int, int>> edges;
    for (size_t i = 0; i < new_edges_1.size(); ++i) {
        edges.emplace_back(new_edges_1[i], new_edges_2[i]);
    }

    std::vector<int> colors;
    for (size_t i = 0; i < new_nodes.size(); i += 2) {
        int color = new_nodes[i] * 1000 + new_nodes[i + 1];
        colors.emplace_back(color);
    }

    return {edges, colors};
}


void BaseGraph::update_current_g(const std::vector<std::pair<std::string, std::vector<int>>> &state) {
    init_missing_preds(state);
    auto [new_nodes, new_edges_1, new_edges_2, new_edge_attrs] = gen_nodes_edges(state);

    Graph g;
    // Add vertices with properties
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < new_nodes.size(); i += 2) {
        Vertex v = boost::add_vertex(g);
        g[v].property1 = new_nodes[i];
        g[v].property2 = new_nodes[i + 1];
        vertices.push_back(v);
    }

    // Add edges with attributes
    for (size_t i = 0; i < new_edges_1.size(); ++i) {
        Edge e;
        bool inserted;
        boost::tie(e, inserted) = boost::add_edge(vertices[new_edges_1[i]], vertices[new_edges_2[i]], g);
        if (inserted) {
            g[e].attribute = new_edge_attrs[i];
        }
    }
    current_g = g;
}

std::string BaseGraph::get_removed_node_wL_hash(const std::string &param_name) {
    int node_id = nodes_index[param_name];
    Graph copy_g = current_g; // copy graph g
    clear_vertex(node_id, copy_g);  // Remove all edges connected to vertex 1
    remove_vertex(node_id, copy_g); // Remove vertex 1
    return graph_wl_hash(copy_g);
}


std::string BaseGraph::state_graph_hash(const std::vector<std::pair<std::string, std::vector<int>>>& states) {
    init_missing_preds(states);

    Graph g;
    auto [new_nodes, new_edges_1, new_edges_2, new_edge_attrs] = gen_nodes_edges(states);

    // Add vertices with properties
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < new_nodes.size(); i += 2) {
        Vertex v = boost::add_vertex(g);
        g[v].property1 = new_nodes[i];
        g[v].property2 = new_nodes[i + 1];
        vertices.push_back(v);
    }

    // Add edges with attributes
    for (size_t i = 0; i < new_edges_1.size(); ++i) {
        Edge e;
        bool inserted;
        boost::tie(e, inserted) = boost::add_edge(vertices[new_edges_1[i]], vertices[new_edges_2[i]], g);
        if (inserted) {
            g[e].attribute = new_edge_attrs[i];
        }
    }
    
    return graph_wl_hash(g);
}


// Function to compute the MD5 hash of a string using EVP interface
std::string BaseGraph::compute_md5(const std::string &input) {
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (ctx == nullptr) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    if (EVP_DigestInit_ex(ctx, EVP_md5(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize EVP digest context");
    }

    if (EVP_DigestUpdate(ctx, input.data(), input.size()) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to update EVP digest");
    }

    if (EVP_DigestFinal_ex(ctx, digest, &digest_len) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize EVP digest");
    }

    EVP_MD_CTX_free(ctx);

    std::ostringstream oss;
    for (unsigned int i = 0; i < digest_len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    return oss.str();
}


// Function to compute the Weisfeiler-Lehman hash of a graph
std::string BaseGraph::weisfeiler_lehman_graph_hash(const Graph &graph) {
    int iterations = 1;    
    std::map<Vertex, std::string> labels;
    for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
        labels[v] = std::to_string(graph[v].property1) + "_" + std::to_string(graph[v].property2);
    }

    for (int i = 0; i < iterations; ++i) {
        std::map<Vertex, std::string> new_labels;
        for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
            std::vector<std::string> neighbor_labels;
            for (auto adj_v : boost::make_iterator_range(boost::adjacent_vertices(v, graph))) {
                neighbor_labels.push_back(labels[adj_v]);
            }
            std::sort(neighbor_labels.begin(), neighbor_labels.end());
            std::ostringstream oss;
            oss << labels[v];
            for (const auto &label : neighbor_labels) {
                oss << "_" << label;
            }
            new_labels[v] = compute_md5(oss.str());
        }
        labels = new_labels;
    }

    std::vector<std::string> final_labels;
    for (const auto &label_pair : labels) {
        final_labels.push_back(label_pair.second);
    }
    std::sort(final_labels.begin(), final_labels.end());

    std::ostringstream oss;
    for (const auto &label : final_labels) {
        oss << label << "_";
    }

    return compute_md5(oss.str());
}

// Function to compute the hash of a graph
std::string BaseGraph::graph_wl_hash(const Graph &graph) {
    if (boost::num_vertices(graph) == 0) {
        throw std::invalid_argument("Input graph must have at least one vertex.");
    }

    // Compute the Weisfeiler-Lehman graph hash
    std::string wl_hash = weisfeiler_lehman_graph_hash(graph);

    // Compute the MD5 hash of the Weisfeiler-Lehman hash
    return compute_md5(wl_hash);
}


std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> BaseGraph::state_to_tensor(const std::vector<std::pair<std::string, std::vector<int>>>& states) {
    init_missing_preds(states);

    auto [new_nodes, new_edges_1, new_edges_2, new_edge_attrs] = gen_nodes_edges(states);
    // transfer data type to tensor
    torch::Tensor nodes_tensor = torch::from_blob(new_nodes.data(), {static_cast<long>(new_nodes.size())}, torch::kInt32).to(torch::kFloat);
    nodes_tensor = nodes_tensor.view({int(new_nodes.size() / 2), 2});

    torch::Tensor edges_1_tensor = torch::from_blob(new_edges_1.data(), {static_cast<long>(new_edges_1.size())}, torch::kInt32).to(torch::kFloat);
    torch::Tensor edges_2_tensor = torch::from_blob(new_edges_2.data(), {static_cast<long>(new_edges_2.size())}, torch::kInt32).to(torch::kFloat);
    torch::Tensor edges_tensor = torch::stack({edges_1_tensor, edges_2_tensor}, 0);
    // torch::Tensor first_row = tmp_tensor.slice(0, 0, tmp_tensor.size(0), 2);  // Start at index 0, step by 2
    // torch::Tensor second_row = tmp_tensor.slice(0, 1, tmp_tensor.size(0), 2); // Start at index 1, step by 2
    
    torch::Tensor edge_attrs_tensor = torch::from_blob(new_edge_attrs.data(), {static_cast<long>(new_edge_attrs.size())}, torch::kInt32).to(torch::kFloat);

    // std::cout << "new_nodes: " << nodes_tensor << std::endl;
    // std::cout << "edges_tensor: " << edges_tensor << std::endl;    
    // std::cout << "edge_attrs_tensor: " << edge_attrs_tensor << std::endl;
    return std::make_tuple(nodes_tensor, edges_tensor, edge_attrs_tensor);
}


void BaseGraph::print_details() {
    std::cout << "node classes:" << std::endl;
    for (const auto& node_class : node_classes){
	std::cout << node_class.first << " " << node_class.second << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "constants:" << std::endl;
    for (const auto& pair : constants) {
	std::cout << pair.first << ": ";
	for (const auto& item : pair.second) {
	    std::cout << item << " ";
	}
	std::cout << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "objects:" << std::endl;
    for (const auto& pair : objects) {
	std::cout << pair.first << ": ";
	for (const auto& item : pair.second) {
	    std::cout << item << " ";
	}
	std::cout << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "goals:" << std::endl;
    for (const auto& goal : goals){
	std::cout << goal << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "inits:" << std::endl;
    for (const auto& init : inits){
	std::cout << init << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "predicates:" << std::endl;
    for (const auto& pair : predicates) {
	// std::cout << pair.first << ": ";
	// for (const auto& item : pair.second) {
	//     std::cout << item << " ";
	// }
	// std::cout << std::endl;
	std::cout << pair << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "nodes:" << std::endl;
    for (const auto& node : nodes){
	std::cout << node << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "node index:" << std::endl;
    for (const auto& node_index : nodes_index){
	std::cout << node_index.first << " " << node_index.second << std::endl;
    }
    std::cout << "------------------------------" << std::endl;

    std::cout << "edges:" << std::endl;
    auto it1 = edges_1.begin();
    auto it2 = edges_2.begin();
    for (; it1 != edges_1.end() && it2 != edges_2.end(); ++it1, ++it2) {
	const auto& edge_1 = *it1;
	const auto& edge_2 = *it2;
	std::cout << edge_1 << edge_2  << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
    
    std::cout << "edges attr:" << std::endl;
    for (const auto& attr : edge_attrs){
	std::cout << attr << std::endl;
    }
    std::cout << "------------------------------------------------------------" << std::endl;
}

// int main() {
//     std::vector<std::string> domain_names;
//     domain_names.push_back("./domain_files/blocksworld.pddl");
//     // domain_names.push_back("./domain_files/childsnack.pddl");
//     // domain_names.push_back("./domain_files/satellite.pddl");
//     // domain_names.push_back("./domain_files/ferry.pddl");
//     // domain_names.push_back("./domain_files/miconic.pddl");
//     // domain_names.push_back("./domain_files/rovers.pddl");
//     // domain_names.push_back("./domain_files/floortile.pddl");
//     // domain_names.push_back("./domain_files/sokoban.pddl");
//     // domain_names.push_back("./domain_files/spanner.pddl");
//     // domain_names.push_back("./domain_files/transport.pddl");

//     std::vector<std::string> problem_names;
//     problem_names.push_back("./problem_pddl/blocksworld_p15.pddl");
//     // problem_names.push_back("./problem_pddl/blocksworld_p01.pddl");    
//     // problem_names.push_back("./problem_pddl/childsnack_p05.pddl");
//     // problem_names.push_back("./problem_pddl/satellite_p05.pddl");
//     // problem_names.push_back("./problem_pddl/ferry_p10.pddl");
//     // problem_names.push_back("./problem_pddl/miconic_p05.pddl");
//     // problem_names.push_back("./problem_pddl/rovers_p01.pddl");
//     // problem_names.push_back("./problem_pddl/floortile_p12.pddl");
//     // problem_names.push_back("./problem_pddl/sokoban_p03.pddl");
//     // problem_names.push_back("./problem_pddl/spanner_p05.pddl");
//     // problem_names.push_back("./problem_pddl/transport_p03.pddl");
    
//     // while(!domain_names.empty()){
//     std::string domain_file = domain_names.back();
//     std::cout << domain_file << std::endl;
//     domain_names.pop_back();
	
//     std::string problem_file = problem_names.back();
//     std::cout << problem_file << std::endl;
//     problem_names.pop_back();

//     PredicateObjectGraph pog(domain_file, problem_file);
//     // pog.print_details();
//     // }
   
//     int predicates_num = 100;
//     int run_times = 10000;
//     std::vector<std::pair<std::string, std::vector<int>>> state_list;

//     // Generating state_list
//     for (int i = 1; i <= 87; ++i) {
//         for (int j = 1; j <= 87; ++j) {
//             std::string state_obj1 = "b" + std::to_string(i);
//             std::string state_obj2 = "b" + std::to_string(j);
//             std::vector<int> objs = {0, 1};
//             state_list.push_back({"on " + state_obj1 + " " + state_obj2, objs});
//             if (static_cast<int>(state_list.size()) > predicates_num) {
//                 break;
//             }
//         }
//     }

//     auto start_time = std::chrono::high_resolution_clock::now();

//     // Run the main loop
//     for (int i = 0; i < run_times; ++i) {
//         pog.state_to_tensor(state_list);

//         if (i % 1000 == 0) {
//             auto current_time = std::chrono::high_resolution_clock::now();
//             auto time_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();
//             std::cout << i << " times cost: " << float(time_elapsed / 1000.0) << " seconds\n";
//         }
//     }

//     auto end_time = std::chrono::high_resolution_clock::now();
//     auto time_taken = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
//     std::cout << "Time taken to run the function: " << float(time_taken / 1000.0) << " seconds\n";    
    
//     return 0;
// }
