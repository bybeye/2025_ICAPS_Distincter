#ifndef ML_HEURISTIC2_H
#define ML_HEURISTIC2_H

#include <chrono>
#include <torch/torch.h>
#include <torch/script.h>
#include <torch/python.h>
#include <torch/extension.h>

#include <ATen/ATen.h>

#include <torchscatter/scatter.h>
#include <torchsparse/sparse.h>

#include "../heuristic.h"
#include "pog.h"
#include "pog2.h"
#include "base_graph.h"

/*
  Modified from Florian's FD-Hypernet c++ code

  Mainly used as a wrapper for Python models. Calls Python models during heuristic evaluation.
  Used for GNN and kernel heuristics. For optimised kernel heuristics, see ml_linear_regression.h
*/

namespace ml_heuristic2 {

struct ModelOutputs {
    std::vector<torch::Tensor> x_outputs;
    std::vector<float> h_outputs;
};

class MLHeuristic2 : public Heuristic {
    // void initialise_lifted_facts(std::unordered_map<std::string, int>);    
    std::string model_path;
    std::string domain_file;
    std::string problem_file;
    std::string rep_type = "ilg";
    std::string pruning;
    
    // pybind11::object rep;

    // TorchScript model
    torch::jit::script::Module model;
    torch::Device device;
    std::unique_ptr<BaseGraph> repre;
        
    size_t numberOfVectors = 0;
    size_t re_size = 0;

    // statistics
    double cumulate_data_time = 0;
    double cumulate_sym_time = 0;
    double cumulate_model_time = 0;
    double cumulate_heuristic_time = 0;
    double model_invoke_times = 0;
    
    std::map<FactPair, std::pair<std::string, std::vector<int>>> dict_fact_to_obj;

    void initialise_model(const plugins::Options &opts);
    void initialise_lifted_facts(const std::unordered_map<std::string, int> &nodes_index);
    std::unique_ptr<BaseGraph> create_representation(const std::string& rep_type, const std::string& domain_file, const std::string& problem_file);

protected:
    virtual float compute_heuristic(const State &ancestor_state) override;
    virtual std::vector<float> compute_heuristic_batch(const std::vector<State> &ancestor_states) override;

public:
    explicit MLHeuristic2(const plugins::Options &opts);

    BaseGraph *get_graph() const { return repre.get(); }
    double get_cumulate_data_time() const { return cumulate_data_time; }
    double get_cumulate_model_time() const { return cumulate_model_time; }
    double get_cumulate_heuristic_time() const { return cumulate_heuristic_time; }
    double get_cumulate_sym_time() const { return cumulate_sym_time; }
    int get_model_invoke_times() const { return model_invoke_times; }
    void incr_cumulate_data_time(double duration) { cumulate_data_time += duration; }
    void incr_cumulate_sym_time(double duration) { cumulate_sym_time += duration; }
    std::string get_pruning() { return pruning; }
    std::string repre_state_graph_hash(const std::vector<std::pair<std::string, std::vector<int>>> &states) const { return repre->state_graph_hash(states); }
    std::tuple<torch::Tensor, torch::Tensor, torch::Tensor> repre_state_to_tensor(const std::vector<std::pair<std::string, std::vector<int>>> &states) const { return repre->state_to_tensor(states); }
    
    std::vector<std::pair<std::string, std::vector<int>>> fd_state_to_ml_state(const State &ancestor_state);
    ModelOutputs compute_heuristic_batch2(const std::vector<State> &succ_states);

    };

} // namespace ml_heuristic2

#endif
