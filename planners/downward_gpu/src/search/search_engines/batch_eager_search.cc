#include "batch_eager_search.h"

#include "../evaluation_context.h"
#include "../evaluator.h"
#include "../open_list_factory.h"
#include "../pruning_method.h"

#include "../algorithms/ordered_set.h"
#include "../plugins/options.h"
#include "../task_utils/successor_generator.h"
#include "../utils/logging.h"

// #include "../heuristics/ml_heuristic.h"
#include "../heuristics/ml_heuristic2.h"
#include "check_params.h"

#include <cassert>
#include <cstdlib>
#include <memory>
#include <optional.hh>
#include <set>
#include <typeinfo>
#include <openssl/md5.h>
#include <openssl/evp.h> 
#include <torch/torch.h>


using namespace std;

namespace batch_eager_search {
BatchEagerSearch::BatchEagerSearch(const plugins::Options &opts)
    : SearchEngine(opts), reopen_closed_nodes(opts.get<bool>("reopen_closed")),
      open_list(opts.get<shared_ptr<OpenListFactory>>("open")->create_state_open_list()) {
    const vector<shared_ptr<Evaluator>> &evals = opts.get_list<shared_ptr<Evaluator>>("evals");
    heuristic = evals[0];
}

void BatchEagerSearch::initialize() {
    log << "Conducting batch gbfs" << (reopen_closed_nodes ? " with" : " without")
      << " reopening closed nodes, (real) bound = " << bound << endl;
    assert(open_list);

    std::set<Evaluator *> evals;
    open_list->get_path_dependent_evaluators(evals);

    path_dependent_evaluators.assign(evals.begin(), evals.end());

    State initial_state = state_registry.get_initial_state();
    for (Evaluator *evaluator : path_dependent_evaluators) {
	evaluator->notify_initial_state(initial_state);
    }

    /*
      Note: we consider the initial state as reached by a preferred
      operator.
    */
    EvaluationContext eval_context(initial_state, 0, true, &statistics);
    statistics.inc_evaluated_states();

    if (open_list->is_dead_end(eval_context)) {
	log << "Initial state is a dead end." << endl;
    } else {
	if (search_progress.check_progress(eval_context))
	    statistics.print_checkpoint_line(0);
	SearchNode node = search_space.get_node(initial_state);
	node.open_initial();
	open_list->insert(eval_context, initial_state.get_id());
    }

    print_initial_evaluator_values(eval_context);
    best_h = eval_context.get_result(heuristic.get()).get_evaluator_value();

    ml_heuristic2::MLHeuristic2 *ml_ptr2 = dynamic_cast<ml_heuristic2::MLHeuristic2 *>(heuristic.get());
    std::string pruning = ml_ptr2->get_pruning();
    std::cout << "pruning type: " << pruning << std::endl;
    state_precision = DEFAULT_STATE_PRECISION;
    action_start_num = DEFAULT_ACTION_START_NUM;
    if (pruning == "action_state") {
	action_pruning = true;
	state_pruning = true;
    } else if (pruning == "action") {
        action_pruning = true;
	action_start_num = 3;
    } else if (pruning == "state") {
        state_pruning = true;
    }
}

void BatchEagerSearch::print_statistics() const {
    statistics.print_detailed_statistics();
    search_space.print_statistics();
}


std::string BatchEagerSearch::md5(const torch::Tensor& tensor) {
    // tensor = tensor.contiguous(); // Ensure tensor is contiguous
    if (!tensor.is_contiguous()) {
        throw std::runtime_error("Tensor is not contiguous");
    }

    if (tensor.scalar_type() != torch::kFloat32) {
        throw std::runtime_error("Tensor is not a float32 tensor");
    }
    
    const unsigned char* data = reinterpret_cast<const unsigned char*>(tensor.data_ptr<float>());
    size_t length = tensor.numel() * sizeof(float);
    
    EVP_MD_CTX *context = EVP_MD_CTX_new();
    if (context == nullptr) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    if (EVP_DigestInit_ex(context, EVP_md5(), nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to initialize digest");
    }

    if (EVP_DigestUpdate(context, data, length) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to update digest");
    }

    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_len = 0;

    if (EVP_DigestFinal_ex(context, digest, &digest_len) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to finalize digest");
    }

    EVP_MD_CTX_free(context);

    std::ostringstream oss;
    for (unsigned int i = 0; i < digest_len; ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)digest[i];
    }
    return oss.str();
}


SearchStatus BatchEagerSearch::step() {
    tl::optional<SearchNode> node;
    step_num++;

    // Get first node in queue that is not closed
    while (true) {
	if (open_list->empty()) {
	    log << "Completely explored state space -- no solution!" << endl;
	    return FAILED;
        }

        StateID id = open_list->remove_min();
        State s = state_registry.lookup_state(id);

        node.emplace(search_space.get_node(s));
	
	if (node->is_closed())
            continue;

	node->close();
	assert(!node->is_dead_end());
	statistics.inc_expanded();
	break;
    }

    const State &s = node->get_state();
    if (check_goal_and_set_plan(s))
	return SOLVED;

    ml_heuristic2::MLHeuristic2* ml_ptr2 = dynamic_cast<ml_heuristic2::MLHeuristic2*>(heuristic.get());
    vector<OperatorID> applicable_ops;
    successor_generator.generate_applicable_ops(s, applicable_ops);

    // get successor states
    std::vector<State> succ_states;
    std::vector<OperatorProxy> ops;
    int num_applicable_ops = int(applicable_ops.size());
    int action_pruning_num = -1;
    std::vector<OperatorID> filtered_ops;  
    if (action_pruning && num_applicable_ops > action_start_num) {
	auto sym_start = std::chrono::high_resolution_clock::now();
        std::vector<std::pair<std::string, std::vector<int>>> state = ml_ptr2->fd_state_to_ml_state(s);
        Check_param check_param;
	BaseGraph* ptr_graph = ml_ptr2->get_graph();
        filtered_ops = check_param.check_operators2(state, ptr_graph, task_proxy, applicable_ops);
	auto sym_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = sym_end - sym_start;

        // if (duration.count() > 0) std::cout << "Calculate orbits time: " << duration.count() << " Remove: " << removed_op << std::endl;
	action_pruning_num = applicable_ops.size() - filtered_ops.size();
        statistics.inc_total_action(action_pruning_num);
	statistics.inc_action_invoke_times();
	ml_ptr2->incr_cumulate_sym_time(duration.count());

        // if symmetries is too less, increase action start number
	float remove_ratio = float(action_pruning_num) / applicable_ops.size();
        if(remove_ratio < action_sym_ratio && num_applicable_ops > action_start_num){
	    log << "Action pruning start number increases from: " << action_start_num << ", removed_ratio and applicable_ops: " << remove_ratio << ", " << num_applicable_ops << std::endl;
            action_start_num = num_applicable_ops;
        }
    } else {
	filtered_ops = applicable_ops;
    }

    for (OperatorID op_id : filtered_ops) {
	// std::cout<< "op_id: " << op_id.get_index() << std::endl;
	OperatorProxy op = task_proxy.get_operators()[op_id];
	if ((node->get_real_g() + op.get_cost()) >= bound)
	    continue;

	State succ_state = state_registry.get_successor_state(s, op);
	statistics.inc_generated();

	SearchNode succ_node = search_space.get_node(succ_state);
	// Previously encountered dead end. Don't re-evaluate.
	if (succ_node.is_dead_end())
	    continue;

        if (succ_node.is_new()) {
	    succ_states.push_back(succ_state);
	    ops.push_back(op);
	    succ_node.open(*node, op, get_adjusted_cost(op));
        }
    }

    // std::cout << "succ_states: ";
    // for (const State& state : succ_states) {
    // 	std::cout << state.get_id() << " ";
    // }
    // std::cout << std::endl;

    // batch evaluate heuristics
    float tmp_h = 99999;
    std::string tmp_op;
    size_t n_succs = succ_states.size();
    if (n_succs > 0) {
	// sml_ptr2 = dynamic_cast<ml_heuristic2::MLHeuristic2*>(heuristic.get());
	ml_heuristic2::ModelOutputs outputs = ml_ptr2->compute_heuristic_batch2(succ_states);
	std::vector<float> hs = outputs.h_outputs;
        std::vector<torch::Tensor> graph_keys = outputs.x_outputs;

	// std::cout << "hs: ";
	// for (const float& num : hs) {
	//     std::cout << num << " ";
	// }
	// std::cout << std::endl;
        
	statistics.inc_evaluated_states(n_succs);
	statistics.inc_evaluations(n_succs);
	statistics.update_heuristic_time(ml_ptr2->get_cumulate_heuristic_time());
	statistics.update_data_time(ml_ptr2->get_cumulate_data_time());
	statistics.update_model_time(ml_ptr2->get_cumulate_model_time());
	statistics.update_sym_time(ml_ptr2->get_cumulate_sym_time());            
	statistics.update_model_invoke_times(ml_ptr2->get_model_invoke_times());

	// Sort hs
	// Create a vector of indices
	std::vector<int> indices(hs.size());
	for (size_t i = 0; i < hs.size(); i++) {
	    indices[i] = i;
	}
        std::sort(indices.begin(), indices.end(), [&hs](int i1, int i2) {
	    // return hs[i1] > hs[i2];  // sort largest first for LIFO
	    return hs[i1] < hs[i2];  // sort smallest first for FIFO
	});

        std::vector<float> sorted_hs(hs.size());
	std::vector<torch::Tensor> sorted_graph_keys(hs.size());
        std::vector<OperatorProxy> sorted_ops;
        std::vector<State> sorted_succ_states;
	for (size_t i = 0; i < indices.size(); i++) {
	    sorted_hs[i] = hs[indices[i]];
	    sorted_graph_keys[i] = graph_keys[indices[i]];
            sorted_ops.push_back(ops[indices[i]]);
	    sorted_succ_states.push_back(succ_states[indices[i]]);
        }
	// Sort End

	int remove_sym_num = 0;
	for (size_t i = 0; i < n_succs; i++) {
            float h = sorted_hs[i];                   // sort
            OperatorProxy op = sorted_ops[i];         // sort
            State succ_state = sorted_succ_states[i]; // sort

            // state pruning
	    bool skip_insertion = false;
	    if (state_pruning) {
		torch::Tensor graph_feature = sorted_graph_keys[i];
		graph_feature = torch::round(graph_feature * state_precision) / state_precision;
		graph_feature = graph_feature.contiguous();
		std::string graph_key = md5(graph_feature);
                             
                if (state_key_set.find(graph_key) == state_key_set.end()) {
                    // log << "Insert state: h: " << h << ", op: " << op.get_name() << ", graph_key: " << graph_key << ", state_key_set size: " << state_key_set.size() << std::endl;
		    state_key_set.insert(graph_key);
		} else {
                    // log << "Filter state: h: " << h << ", op: " << op.get_name() << ", graph_key: " << graph_key << ", state_key_set size: " << state_key_set.size() << std::endl;
		    remove_sym_num++;
                    statistics.inc_total_state();
		    skip_insertion = true;                    
		}
            }
            
	    if (h < tmp_h) {
		tmp_h = h;
		tmp_op = op.get_name();
            }
            
	    if (!skip_insertion) {
                open_list->insert(int(h), succ_state.get_id());
	    }                
        }

	if (tmp_h <= best_h) {
	    best_h = tmp_h;
	    log << "h=" << tmp_h << ", " << "action=(" << tmp_op << "), ";
	    statistics.print_checkpoint_line(node->get_g() + 1);
        }
    }
    
    if (step_num % 1000 == 0) {
	log << "Iteration print " << step_num << ", h=" << tmp_h << ", " << "action=(" << tmp_op << "), ";
	statistics.print_checkpoint_line(node->get_g() + 1);
    }
    
    return IN_PROGRESS;
}


void BatchEagerSearch::reward_progress() {
  // Boost the "preferred operator" open lists somewhat whenever
  // one of the heuristics finds a state with a new best h value.
  open_list->boost_preferred();
}

void BatchEagerSearch::dump_search_space() const { search_space.dump(task_proxy); }

void add_options_to_feature(plugins::Feature &feature) {
  SearchEngine::add_pruning_option(feature);
  SearchEngine::add_options_to_feature(feature);
}
}  // namespace batch_eager_search
