#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <torch/torch.h>

#include "../search_space.h"
#include "../abstract_task.h"
#include "parse_domain.cc"
#include "parse_problem.cc"

// for gen Graph
#include <boost/graph/adjacency_list.hpp>

struct VertexProperty {
    float property1;
    float property2;
};

struct EdgeProperty {
    float attribute;
};

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperty, EdgeProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef std::pair<int, int> EdgePair;

class BaseGraph {

protected:    
    // domain pddl
    std::vector<std::string> types;
    std::map<std::string, std::vector<std::string>> constants;
    // std::map<std::string, std::vector<std::string>> predicates;
    std::vector<std::string> predicates;    

    // problem pddl
    std::map<std::string, std::vector<std::string>> objects;
    std::set<std::string> inits;
    std::set<std::string> goals;
    Graph current_g;

    // fixed data
    std::vector<int> nodes;
    std::vector<int> edges_1;
    std::vector<int> edges_2;
    std::vector<int> edge_attrs;

    // dict
    std::unordered_map<std::string, int> nodes_index;       // for objects and predicate names
    std::unordered_map<std::string, int> nodes_goal_index;  // for goals
    std::unordered_map<std::string, int> node_classes;

    // internal states
    bool check_init = false;
    int largest_predicate = 1;

    void gen_node_classes();

    virtual void gen_nodes() = 0;

    virtual void gen_init(const std::set<std::string> &predicates_strs, const std::string &node_type) = 0;
    
    virtual std::tuple<std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>> gen_nodes_edges(const std::vector<std::pair<std::string, std::vector<int>>>& states) = 0;

public:
  
    BaseGraph(const std::string &domain_file, const std::string &problem_file);

    virtual ~BaseGraph() = default;

    std::tuple<std::vector<std::pair<int, int>>, std::vector<int>> get_edges_colors(const std::vector<std::pair<std::string, std::vector<int>>>& states);
    
    void init_missing_preds(const std::vector<std::pair<std::string, std::vector<int>>> &states);

    void update_current_g(const std::vector<std::pair<std::string, std::vector<int>>> &state);
    
    std::string get_removed_node_wL_hash(const std::string &param_names);
    
    std::string state_graph_hash(const std::vector<std::pair<std::string, std::vector<int>>>& states);
    
    std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> state_to_tensor(const std::vector<std::pair<std::string, std::vector<int>>> &states);
    
    std::unordered_map<std::string, int> get_nodes_index() {return nodes_index;}

    std::string read_file(const std::string& filename);

    int get_number_Vectors() { return largest_predicate; }

    std::string weisfeiler_lehman_graph_hash(const Graph &graph);
    std::string graph_wl_hash(const Graph &graph);
    std::string compute_md5(const std::string &input);        
    void print_details();

    void print_nodes_index() {
	std::cout << "nodes index: ";
	for (const auto& pair : nodes_index) {
	    std::cout << pair.first << ": " << pair.second << ", ";
        }
	std::cout << std::endl;
    }

};
