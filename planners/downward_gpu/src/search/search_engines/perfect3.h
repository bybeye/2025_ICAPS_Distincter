#ifndef SEARCH_ENGINES_PERFECT_H
#define SEARCH_ENGINES_PERFECT_H

#include "../open_list.h"
#include "../search_engine.h"

#include <memory>
#include <vector>
#include <map>
#include <fstream>

class Evaluator;
class PruningMethod;

namespace plugins {
class Feature;
}

namespace perfect3 {
class Perfect3 : public SearchEngine {
    std::map<FactPair, std::string> fact_to_string;
    std::string plan_path;
    std::string states_path;

    void initialise_grounded_facts();
    void write_state_to_file_3(std::ofstream &plan_file, const State& s, const std::string op_name, const int g, const int optim);

protected:
    virtual void initialize() override;
    virtual SearchStatus step() override;

public:
    explicit Perfect3(const plugins::Options &opts);
    virtual void print_statistics() const override;
    void dump_search_space() const;
};

extern void add_options_to_feature(plugins::Feature &feature);
}

#endif
