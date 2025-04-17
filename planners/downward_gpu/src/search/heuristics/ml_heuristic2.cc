#include "ml_heuristic2.h"

#include "../plugins/plugin.h"
#include "../task_utils/task_properties.h"

#include <cstddef>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <torch/script.h> // One-stop header.

using std::string;

namespace ml_heuristic2 {

MLHeuristic2::MLHeuristic2(const plugins::Options &opts) : Heuristic(opts), device(torch::kCPU) {

    // Read init values
    model_path = opts.get<string>("model_path");
    domain_file = opts.get<string>("domain_file");
    problem_file = opts.get<string>("instance_file");
    rep_type = opts.get<string>("rep_type");
    pruning = opts.get<string>("pruning");
    
    repre = create_representation(rep_type, domain_file, problem_file);
    initialise_lifted_facts(repre->get_nodes_index());
    repre->print_nodes_index();

    // limit the use of multiple cores in torch lib
    setenv("OMP_NUM_THREADS", "1", 1);
    setenv("MKL_NUM_THREADS", "1", 1);
    torch::set_num_threads(1);
    torch::set_num_interop_threads(1);    
    if(torch::cuda::is_available()) device = torch::Device(torch::kCUDA);
    // torch::globalContext().setBenchmarkCuDNN(true);
    
    try {
        std::cout << "Trying to load model from file: " << model_path << ", Device: " << device << " ...\n";
        model = torch::jit::load(model_path);
	model.to(device);       
	model.eval();
	std::cout << "Successfully loaded model!!" << std::endl;
    }catch (const c10::Error& e) {
        std::cerr << "Error: Loading the model error!!!\n";
	std::cerr << e.what() << std::endl;        
	exit(-1);        
    }
}

std::unique_ptr<BaseGraph> MLHeuristic2::create_representation(const std::string& rep_type, const std::string& domain_file, const std::string& problem_file) {
    if (rep_type == "pog2") {
        std::cout << "Create pog2 class!" << std::endl;
        return std::make_unique<PredicateObjectGraph2>(domain_file, problem_file);
    } else {
        std::cout << "Create default pog class!" << std::endl;
        return std::make_unique<PredicateObjectGraph>(domain_file, problem_file);
    }
}

void MLHeuristic2::initialise_lifted_facts(const std::unordered_map<std::string, int>& nodes_index) {
    FactsProxy facts(*task);
    for (FactProxy fact : facts) {
        string name = fact.get_name();

        // Convert from FDR var-val pairs back to propositions
        if (name == "<none of those>") {
            continue;
        } else {
            if (name.substr(0, 5) == "Atom ") {
                name = name.substr(5);
            } else if (name.substr(0, 12) == "NegatedAtom ") {
                continue;
            } else {
                std::cout << "Substring of downward fact does not start with 'Atom ': "
                          << "or 'NegatedAtom '" << name << std::endl;
                exit(-1);
            }
        }

        // replace all occurrences of '(' and ')' by ' '
        std::replace(name.begin(), name.end(), '(', ' ');
        std::replace(name.begin(), name.end(), ')', ' ');

        // Remove occurrences of ','
        name.erase(std::remove(name.begin(), name.end(), ','), name.end());

        // Trim string
        size_t first = name.find_first_not_of(' ');
	if (first == std::string::npos)
	    continue;
        
	size_t last = name.find_last_not_of(' ');
        name = name.substr(first, (last - first + 1));

        if (name.length() == 0)
            continue;

	if (name.length() < 3)
            std::cout << "ERROR Name:" << name << "!" << std::endl; 

        std::istringstream iss(name);
        std::string token;
        std::vector<int> args;
        while (iss >> token) {
	    int token_id = -1;
            auto it = nodes_index.find(token);
	    if (it != nodes_index.end()) {
		token_id = it->second;
                if (token_id < 0) {
		    std::cerr << "Dict arg is negative: " << token  << ", value: " << token_id << std::endl;
                }
                
                args.push_back(token_id);
	    } else {
		std::cerr << "Dict Args not found: [" << token << "]"<< std::endl;
	    }
        }
	
        std::pair<std::string, std::vector<int>> lifted_fact(name, args);
        dict_fact_to_obj.insert({fact.get_pair(), lifted_fact});
    }
}


std::vector<std::pair<std::string, std::vector<int>>> MLHeuristic2::fd_state_to_ml_state(const State &ancestor_state) {
    State state = convert_ancestor_state(ancestor_state);
    std::vector<std::pair<std::string, std::vector<int>>> predicates;
    for (FactProxy fact : state) {
	auto it = dict_fact_to_obj.find(fact.get_pair());
	if (it != dict_fact_to_obj.end()) {
	    predicates.push_back(it->second);
        }
        // else {
	    // std::cerr << "FD State not found: " << fact.get_pair() << std::endl;
	// }
    }
    return predicates;
}


float MLHeuristic2::compute_heuristic(const State &ancestor_state) {
    std::vector<State> ancestor_states = {ancestor_state};
    return compute_heuristic_batch(ancestor_states)[0];
}

std::vector<float> MLHeuristic2::compute_heuristic_batch(const std::vector<State> &ancestor_states) {
    auto func_start = std::chrono::high_resolution_clock::now();
    std::vector<float> ret;
    // the length of each graph varies dramatically, so only set a limit for the batch size is not right. 
    // size_t batch_size = 256;
    int batch_node_limit = 1024 * 1024; // within GPU 4GB 

    std::vector<torch::Tensor> cpp_x_list;
    std::vector<torch::Tensor> cpp_edges_list;
    std::vector<torch::Tensor> cpp_edge_attrs_list;
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& ancestor_state : ancestor_states){
	// Get states from python function and transfter to cpp types.
	// count prepare data time
	std::vector<std::pair<std::string, std::vector<int>>> predicates = fd_state_to_ml_state(ancestor_state);
        auto [cpp_x, cpp_edges, cpp_edge_attrs] = repre->state_to_tensor(predicates);
        // std::cout << "Received nodes_tensor: " << cpp_x << std::endl;
	// std::cout << "Received edges_tensor: " << cpp_edges.t() << std::endl;
	// std::cout << "Received edge_attrs_tensor: " << cpp_edge_attrs.t() << std::endl;
	// repre->print_details();    

        cpp_x_list.push_back(cpp_x);
        cpp_edges_list.push_back(cpp_edges);
	cpp_edge_attrs_list.push_back(cpp_edge_attrs);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    cumulate_data_time += duration.count();

    size_t j = 0;
    while(j < ancestor_states.size()){
        // for (size_t i = 0; i < ancestor_states.size(); i += batch_size) {
        // size_t current_batch_size = std::min(batch_size, ancestor_states.size() - i);
	int graph_id = 0;
        int edge_start_num = 0;
	std::vector<torch::Tensor> batch_nodes;
	std::vector<torch::Tensor> batch_edges;  // for oog
	std::vector<torch::Tensor> batch_edge_attrs;  // for oog   
	std::vector<int64_t> batch_graph_ids;
        // for (size_t j = i, graph_id = 0; j < i + current_batch_size; ++j, graph_id++) {
	while(edge_start_num < batch_node_limit){
            torch::Tensor cpp_x = cpp_x_list[j];
            torch::Tensor cpp_edges = cpp_edges_list[j];
            torch::Tensor updated_edges = cpp_edges + edge_start_num;
	    torch::Tensor cpp_edge_attrs = cpp_edge_attrs_list[j];
            
	    batch_nodes.push_back(cpp_x);
            batch_edges.push_back(updated_edges);
            batch_edge_attrs.push_back(cpp_edge_attrs);
	    // batch_graph_ids is for identifying the node in which graphs.
	    batch_graph_ids.insert(batch_graph_ids.end(), cpp_x.size(0), graph_id);

	    // update start number for next item's edge
            edge_start_num += cpp_x.size(0); // node number
	    j++;
	    graph_id++;
            if (j == ancestor_states.size()){
                break;
            }
	}

	// Prepare input structure
	std::vector<c10::IValue> inputs;
	torch::Tensor batch_nodes_tensor = torch::cat(batch_nodes, 0).to(device);
	torch::Tensor batch_edges_tensor = torch::cat(batch_edges, 1).to(torch::kInt64).to(device);
	torch::Tensor batch_edge_attrs_tensor = torch::cat(batch_edge_attrs, 0).to(device);
	torch::Tensor batch_graph_tensor = torch::tensor(batch_graph_ids, torch::dtype(torch::kInt64)).to(device);        
	inputs.push_back(batch_nodes_tensor); // nodes
	inputs.push_back(batch_edges_tensor);  // edges
	inputs.push_back(batch_edge_attrs_tensor);  // edge attributes
	inputs.push_back(batch_graph_tensor); // node graph id
            
	// count model time
	auto start = std::chrono::high_resolution_clock::now();
        c10::IValue outputs = model(inputs);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
        cumulate_model_time += duration.count();
	model_invoke_times += 1;

	torch::Tensor h_output;
	if (outputs.isTensor()) {
	    h_output = outputs.toTensor();            
	} else if (outputs.isTuple()) {   
	    auto result_tuple = outputs.toTuple();
            h_output = result_tuple->elements()[1].toTensor();
        }
        
	h_output = h_output.contiguous().to(torch::kCPU, torch::kFloat32);
	auto h_data_ptr = h_output.data_ptr<float>();            
	ret.insert(ret.end(), h_data_ptr, h_data_ptr + h_output.numel());
    }

    auto func_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> func_duration = func_end - func_start;
    cumulate_heuristic_time += func_duration.count();
    return ret;
}


ModelOutputs MLHeuristic2::compute_heuristic_batch2(const std::vector<State> &succ_states) {
    auto func_start = std::chrono::high_resolution_clock::now();
    // the length of each graph varies dramatically, so only set a limit for the batch size is not right. 
    const int batch_node_limit = 1024 * 1024; // within GPU 4GB 

    size_t j = 0;
    ModelOutputs outputs;
    while(j < succ_states.size()){
	int graph_id = 0;
        int edge_start_num = 0;
	std::vector<torch::Tensor> batch_nodes;
	std::vector<torch::Tensor> batch_edges;  // for oog
	std::vector<torch::Tensor> batch_edge_attrs;  // for oog   
	std::vector<int64_t> batch_graph_ids;

        auto start = std::chrono::high_resolution_clock::now();
        while (edge_start_num < batch_node_limit) {
	    State succ_state = succ_states[j];
	    std::vector<std::pair<std::string, std::vector<int>>> predicates = fd_state_to_ml_state(succ_state);
            auto [cpp_x, cpp_edges, cpp_edge_attrs] = repre_state_to_tensor(predicates);            
            torch::Tensor updated_edges = cpp_edges + edge_start_num;
            
	    batch_nodes.push_back(cpp_x);
            batch_edges.push_back(updated_edges);
            batch_edge_attrs.push_back(cpp_edge_attrs);
	    // batch_graph_ids is for identifying the node in which graphs.
	    batch_graph_ids.insert(batch_graph_ids.end(), cpp_x.size(0), graph_id);

	    // update start number for next item's edge
            edge_start_num += cpp_x.size(0); // node number
	    j++;
	    graph_id++;
            if (j == succ_states.size()){
                break;
            }
	}

	// Prepare input structure
	std::vector<c10::IValue> inputs;
	torch::Tensor batch_nodes_tensor = torch::cat(batch_nodes, 0).to(device);
	torch::Tensor batch_edges_tensor = torch::cat(batch_edges, 1).to(torch::kInt64).to(device);
	torch::Tensor batch_edge_attrs_tensor = torch::cat(batch_edge_attrs, 0).to(device);
	torch::Tensor batch_graph_tensor = torch::tensor(batch_graph_ids, torch::dtype(torch::kInt64)).to(device);        
	inputs.push_back(batch_nodes_tensor); // nodes
	inputs.push_back(batch_edges_tensor);  // edges
	inputs.push_back(batch_edge_attrs_tensor);  // edge attributes
        inputs.push_back(batch_graph_tensor);       // node graph id
	// calculate parse graph time
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	incr_cumulate_data_time(duration.count());
        
	// count model time
	start = std::chrono::high_resolution_clock::now();
	c10::IValue result = model(inputs);
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
        cumulate_model_time += duration.count();
	model_invoke_times += 1;
        
	if (result.isTensor()) {
	    torch::Tensor h_output = result.toTensor();
	    h_output = h_output.contiguous().to(torch::kCPU, torch::kFloat32);
            
	    auto h_data_ptr = h_output.data_ptr<float>();
	    outputs.h_outputs.insert(outputs.h_outputs.end(), h_data_ptr, h_data_ptr + h_output.numel());
	} else if (result.isTuple()) {   
	    auto result_tuple = result.toTuple();
	    torch::Tensor x_output = result_tuple->elements()[0].toTensor();
            torch::Tensor h_output = result_tuple->elements()[1].toTensor();
	    x_output = x_output.contiguous().to(torch::kCPU, torch::kFloat32);
            h_output = h_output.contiguous().to(torch::kCPU, torch::kFloat32);
            auto h_data_ptr = h_output.data_ptr<float>();
            outputs.h_outputs.insert(outputs.h_outputs.end(), h_data_ptr, h_data_ptr + h_output.numel());
            
	    // std::cout << "x_output.dim(): " << x_output.dim() << ",h_output.size(0): " << h_output.size(0) << ",x_output.size(0): " << x_output.size(0) << ",x_output.size(1): " <<  x_output.size(1) << std::endl;
	    // Collect elements of x_output (features) and convert each row to string
            for (int j = 0; j < x_output.size(0); ++j) {
                torch::Tensor feature = x_output[j];

		// std::ostringstream oss;
		// for (int i = 0; i < feature.size(0); ++i) {
		//     oss << feature[i].item<float>();
		// }
		// std::string feature_str = oss.str();
		// std::cout << "feature str: " << feature_str << ", " << feature_md5 << std::endl;
		outputs.x_outputs.push_back(feature);
            }
        }
    }

    auto func_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> func_duration = func_end - func_start;
    cumulate_heuristic_time += func_duration.count();
    return outputs;
}

class MLHeuristic2Feature : public plugins::TypedFeature<Evaluator, MLHeuristic2> {
  public:
    MLHeuristic2Feature() : TypedFeature("ml2") {
        document_title("ML heuristic2");
        document_synopsis("TODO");

        add_option<string>("rep_type", "rep type", "default_value");        
        add_option<string>("model_path", "path to trained model or model weights", "default_value");
        add_option<string>("domain_file", "Path to the domain file.", "default_file");
        add_option<string>("instance_file", "Path to the instance file.", "default_file");
        add_option<string>("pruning", "action_state", "default_value");                

        Heuristic::add_options_to_feature(*this);

        document_language_support("action costs", "not supported");
        document_language_support("conditional effects", "not supported");
        document_language_support("axioms", "not supported");

        document_property("admissible", "no");
        document_property("consistent", "no");
        document_property("safe", "yes");
        document_property("preferred operators", "no");
    }
};

static plugins::FeaturePlugin<MLHeuristic2Feature> _plugin;

} // namespace ml_heuristic
