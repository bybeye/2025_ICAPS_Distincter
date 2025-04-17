#ifndef SEARCH_ENGINES_BATCH_EAGER_SEARCH_H
#define SEARCH_ENGINES_BATCH_EAGER_SEARCH_H

#include "../open_list.h"
#include "../search_engine.h"

#include <memory>
#include <vector>

#include "../heuristics/base_graph.h"

class Evaluator;
class PruningMethod;



namespace plugins {
class Feature;
}

namespace batch_eager_search {
class BatchEagerSearch : public SearchEngine {
    float best_h;
    int step_num = 0;
    const bool reopen_closed_nodes;

    const int DEFAULT_STATE_PRECISION = 10;
    const int DEFAULT_ACTION_START_NUM = 2000;
    int action_start_num;
    int state_precision;
    float action_sym_ratio = 0.3;
    bool action_pruning = false;
    bool state_pruning = false;
    
    std::unique_ptr<StateOpenList> open_list;
    std::unordered_set<std::string> state_key_set;

    std::vector<Evaluator *> path_dependent_evaluators;
    std::shared_ptr<Evaluator> heuristic;

    void start_f_value_statistics(EvaluationContext &eval_context);
    void update_f_value_statistics(EvaluationContext &eval_context);
    void reward_progress();

protected:
    virtual void initialize() override;
    virtual SearchStatus step() override;

public:
    explicit BatchEagerSearch(const plugins::Options &opts);
    virtual ~BatchEagerSearch() = default;
    virtual void print_statistics() const override;
    std::string md5(const torch::Tensor &tensor);
    void dump_search_space() const;
};

extern void add_options_to_feature(plugins::Feature &feature);
}

#endif
