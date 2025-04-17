#ifndef ML_HEURISTIC_H
#define ML_HEURISTIC_H

#include <chrono>
#include <torch/torch.h>
#include <torch/script.h>
#include <torch/python.h>

#include <torch/extension.h>
#include <ATen/ATen.h>

#include <torchscatter/scatter.h>
#include <torchsparse/sparse.h>

#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>

#include "../heuristic.h"

/*
  Modified from Florian's FD-Hypernet c++ code

  Mainly used as a wrapper for Python models. Calls Python models during heuristic evaluation.
  Used for GNN and kernel heuristics. For optimised kernel heuristics, see ml_linear_regression.h
*/

namespace ml_heuristic {
class MLHeuristic : public Heuristic {
    void initialise_model(const plugins::Options &opts);
    void initialise_grounded_facts();
    void initialise_lifted_facts();
    void initialise_facts();

    void print_model(const torch::Device &device, const torch::jit::script::Module &model);

    
    // Required for pybind. Once this goes out of scope python interaction is no
    // longer possible.
    //
    // IMPORTANT: since member variables are destroyed in reverse order of
    // construction it is important that the scoped_interpreter_guard is listed as
    // first member variable (therefore destroyed last), otherwise calling the
    // destructor of this class will lead to a segmentation fault.
    pybind11::scoped_interpreter guard;

    // Python object which computes the heuristic
    pybind11::object rep;

    // TorchScript model
    torch::jit::script::Module model;
    torch::Device device;

    std::string rep_type = "ilg";
    size_t numberOfVectors = 0;
    size_t re_size = 0;

    double cumulate_data_time = 0;
    double cumulate_model_time = 0;
    double cumulate_heuristic_time = 0;
    
    // Dictionary that maps FD proposition strings to (pred o_1 ... o_n)
    // proposition strings in the case of grounded ML, or (pred, args) tuples.
    // We only want to do this translation once,
    // hence we store it here. This could be ignored if we change the format of
    // propositions in ML.
    std::map<FactPair, std::string> fact_to_grounded_ml_input;
    std::map<FactPair, std::pair<std::string, std::vector<std::string>>> fact_to_lifted_ml_input;

    bool lifted_ml;

    pybind11::list fd_state_to_ml_state(const State &ancestor_state);

  protected:
    virtual float compute_heuristic(const State &ancestor_state) override;
    virtual std::vector<float> compute_heuristic_batch(const std::vector<State> &ancestor_states) override;
    virtual std::vector<float> bak_compute_heuristic_batch(const std::vector<State> &ancestor_states);
    virtual float bak_compute_heuristic(const State &ancestor_state);
  public:
    explicit MLHeuristic(const plugins::Options &opts);
    double get_cumulate_data_time() const {return cumulate_data_time;}
    double get_cumulate_model_time() const { return cumulate_model_time; }
    double get_cumulate_heuristic_time() const { return cumulate_heuristic_time; }
};

} // namespace ml_heuristic


class GnnDataset : public torch::data::Dataset<GnnDataset> {
private:
    torch::Tensor nodes_;
    torch::Tensor edges_;

public:
    GnnDataset(torch::Tensor nodes, torch::Tensor edges)
        : nodes_(nodes), edges_(edges) {}

    torch::data::Example<> get(size_t index) override {
        return {nodes_[index], edges_[index]};
    }

    torch::optional<size_t> size() const override {
        return nodes_.size(0);
    }
};


#endif
