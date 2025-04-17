#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "../search_space.h"
#include "../abstract_task.h"
#include "base_graph.h"


class PredicateObjectGraph : public BaseGraph{
protected:
  
    void gen_nodes() override;

    void gen_init(const std::set<std::string> &predicates_strs, const std::string &node_type) override;
    
    std::tuple<std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>> gen_nodes_edges(const std::vector<std::pair<std::string, std::vector<int>>>& states) override;

public:
    PredicateObjectGraph(const std::string& domain_file, const std::string& problem_file);
    
};
