#include "ml_heuristic.h"

#include "../plugins/plugin.h"
#include "../task_utils/task_properties.h"

#include <fstream>
#include <iostream>

#include <torch/script.h> // One-stop header.

namespace py = pybind11;

using std::string;

namespace ml_heuristic {
MLHeuristic::MLHeuristic(const plugins::Options &opts) : Heuristic(opts), device(torch::kCPU) {
    if(torch::cuda::is_available()){
	device = torch::Device(torch::kCUDA);
    }

    initialise_model(opts);
    initialise_facts();
}

void MLHeuristic::initialise_model(const plugins::Options &opts) {
    // Add ML submodule to the python path
    auto gnn_path = std::getenv("ML");
    if (!gnn_path) {
	std::cout << "ML env variable not found. Aborting." << std::endl;
	exit(-1);
    }
    std::string path(gnn_path);
    std::cout << "ML path is " << path << std::endl;

    // Append python module directory to the path
    // py::module sys = py::module::import("sys");
    // sys.attr("path").attr("append")(path);

    // Force all output being printed to stdout. Otherwise INFO logging from
    // python will be printed to stderr, even if it is not an error.
    // sys.attr("stderr") = sys.attr("stdout");

    // Read paths
    std::string model_path = opts.get<string>("model_path");
    std::string domain_file = opts.get<string>("domain_file");
    std::string instance_file = opts.get<string>("instance_file");
    rep_type = opts.get<string>("rep_type");

    // Throw everything into Python code depending on model type
    std::cout << "Trying to load model from file " << model_path << " ...\n";

    try {
	// device = torch::Device(torch::kCPU);
	// Deserialize the ScriptModule from a file using torch::jit::load().
        model = torch::jit::load(model_path);
	model.to(device); //, torch::kHalf);        
	model.eval();
        // model.to(device);
	// print_model(device, model);
    }catch (const c10::Error& e) {
        std::cerr << "error loading the model\n";
	std::cerr << e.what() << std::endl;        
	exit(-1);        
    }

    std::cout << "Successfully loaded model!!" << std::endl;

    // Append python module directory to the path
    py::module sys = py::module::import("sys");
    sys.attr("path").attr("append")(path);
    sys.attr("stderr") = sys.attr("stdout");

    // Throw everything into Python code depending on model type
    std::cout << "Trying to load python methods..." << std::endl;
    // std::cout << model.attr("rep_type") << domain_file << instance_file << std::endl;

    // std::string rep_type = "ilg";
    // std::string rep_type = model.attr("rep_type").toStringRef();
    std::cout << "rep_type:" << rep_type << std::endl;
    
    py::module util_module = py::module::import("util.save_load");
    rep = util_module.attr("update_representation")(rep_type, domain_file, instance_file);
    std::cout << "Loaded rep! Rep lifted: " << rep.attr("lifted").cast<bool>() << std::endl;
    lifted_ml = rep.attr("lifted").cast<bool>();

    // lifted_ml = model.attr("lifted_state_input")().cast<bool>();
}

void MLHeuristic::print_model(const torch::Device &device, const torch::jit::script::Module &model){
  std::cout << "Model name: RGNN" << std::endl;
  std::cout << "Deivce: " << device << std::endl;

  // count layers
  int num_layers = 0;
  // std::cout << model.named_parameters().size() << std::endl; 
  for (const auto& pair : model.named_parameters()) {
      // std::cout << pair.name << std::endl;
      std::size_t found = pair.name.find("weight");
      if (found!=std::string::npos){
	  num_layers++;
      }
  }
  std::cout << "Number of layers: " << num_layers << std::endl; // << pair.value

  // print model attributes  
  // for (const auto& attr : model.named_attributes())   
  //     std::cout << "attributes: " << attr.name << std::endl;

  // Get current value of `init_func` 
  // std::cout << model.attr("device") << std::endl;  
  // std::cout << model.attr("jit") << std::endl;  
  std::cout << "rep_type: " << model.attr("rep_type") << std::endl;  

  // print model methods
  // for (const auto& method : model.get_methods())  
  //     std::cout << "method name:" << method.nme() << "\n";

  // print parameters sizes
  // for (const auto& p : model.parameters()) {
  //     std::cout << torch:: p.size(0) << std::endl;
  // }

  // Print the schema to see the signature of the forward method
  auto forward_method = model.get_method("forward");
  const auto& function_schema = forward_method.function().getSchema();
  std::cout << function_schema << std::endl;
}



void MLHeuristic::initialise_facts() {
    if (lifted_ml) {
        initialise_lifted_facts();
    } else {
        initialise_grounded_facts();
    }
}


void MLHeuristic::initialise_grounded_facts() {
    FactsProxy facts(*task);
    for (FactProxy fact : facts) {
        // TODO(DZC) this is an artifact of FD-Hypernet/STRIPS-HGN code
        // we can remove this conversion in both the code here and in python
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
        if (std::isspace(name[0])) {
            name.erase(0, 1);
        }
        if (std::isspace(name.back())) {
            name.erase(name.end() - 1, name.end());
        }

        // Add parentheses around string
        name = "(" + name + ")";
        fact_to_grounded_ml_input.insert({fact.get_pair(), name});

#ifndef NDEBUG
        std::cout << name << " ";
#endif
    }

#ifndef NDEBUG
    std::cout << std::endl;
#endif
}

void MLHeuristic::initialise_lifted_facts() {
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
        if (std::isspace(name[0])) {
            name.erase(0, 1);
        }
        if (std::isspace(name.back())) {
            name.erase(name.end() - 1, name.end());
        }

        std::istringstream iss(name);
        std::string s;
        std::string pred = "";
        std::vector<std::string> args;

        while (std::getline(iss, s, ' ')) {
            if (pred == "") {
                pred = s;
            } else {
                args.push_back(s);
            }
        }
        std::pair<std::string, std::vector<std::string>> lifted_fact(pred, args);

        fact_to_lifted_ml_input.insert({fact.get_pair(), lifted_fact});

#ifndef NDEBUG
        std::cout << name << " ";
#endif
    }

#ifndef NDEBUG
    std::cout << std::endl;
#endif
}


py::list MLHeuristic::fd_state_to_ml_state(const State &ancestor_state) {
    State state = convert_ancestor_state(ancestor_state);

    py::list ml_state;
    if (lifted_ml) {
        for (FactProxy fact : state) {
            ml_state.append(fact_to_lifted_ml_input[fact.get_pair()]);
        }
    } else {
        for (FactProxy fact : state) {
            ml_state.append(fact_to_grounded_ml_input[fact.get_pair()]);
        }
    }
    return ml_state;
}


float MLHeuristic::bak_compute_heuristic(const State &ancestor_state) {
    // Convert state into Python object and feed into ML.
    // std::cout << "compute_heuristic" << std::endl;
    py::list ml_state = fd_state_to_ml_state(ancestor_state);    
    py::tuple re = rep.attr("state_to_tensor")(ml_state);
    py::object py_x = re[0];
    py::list py_edge_list = re[1];
    // std::cout << "re type :" << re[0].get_type() << re[1].get_type() << std::endl;

    // Change input data to C++ format and move into GPU
    torch::Tensor cpp_x = THPVariable_Unpack(static_cast<PyObject *>(py_x.ptr()));
    cpp_x = cpp_x.to(device);

    // torch::Tensor cpp_edge = THPVariable_Unpack(static_cast<PyObject *>(py_edge_list.ptr()));
    // torch::Tensor list_of_edge_index = cpp_edge.to(device);
    
    std::vector<torch::Tensor> list_of_edge_index;
    for (auto &item : py_edge_list) {
	torch::Tensor cpp_edge = THPVariable_Unpack(static_cast<PyObject *>(item.ptr()));
        cpp_edge = cpp_edge.to(device);
	list_of_edge_index.push_back(cpp_edge);
    }

    // Prepare input structure
    std::vector<c10::IValue> inputs;
    inputs.push_back(cpp_x);
    inputs.push_back(list_of_edge_index);
    inputs.push_back(c10::IValue()); // for None

    // Invoke model
    // int h = model.attr("h")(ml_state).cast<int>();
    c10::IValue result = model(inputs);
    float h = result.toTensor().item<float>();   
    std::cout << "compute_heuristic return h:" << h << std::endl;
    return h;
}

std::vector<float> MLHeuristic::bak_compute_heuristic_batch(const std::vector<State> &ancestor_states) {
    std::vector<float> ret;
    for (size_t i = 0; i < ancestor_states.size(); i++) {
	py::list ml_state = fd_state_to_ml_state(ancestor_states[i]);    
	py::tuple re = rep.attr("state_to_tensor")(ml_state);
	py::object py_x = re[0];
	py::list py_edge_list = re[1];
	// std::cout << "re type :" << re[0].get_type() << re[1].get_type() << std::endl;

	// Change input data to C++ format and move into GPU
	torch::Tensor cpp_x = THPVariable_Unpack(static_cast<PyObject *>(py_x.ptr()));
	cpp_x = cpp_x.to(device);

	// torch::Tensor cpp_edge = THPVariable_Unpack(static_cast<PyObject *>(py_edge_list.ptr()));
	// torch::Tensor list_of_edge_index = cpp_edge.to(device);
    
	std::vector<torch::Tensor> list_of_edge_index;
	for (auto &item : py_edge_list) {
	    torch::Tensor cpp_edge = THPVariable_Unpack(static_cast<PyObject *>(item.ptr()));
	    cpp_edge = cpp_edge.to(device);
	    list_of_edge_index.push_back(cpp_edge);
	}

	// Prepare input structure
	std::vector<c10::IValue> inputs;
	inputs.push_back(cpp_x);
	inputs.push_back(list_of_edge_index);
	inputs.push_back(c10::IValue()); // for None

	c10::IValue result = model(inputs);
	float h = result.toTensor().item<float>();
        ret.push_back(h);
    }
 
    // Print the vector elements
    std::cout << "Compute heuristic batch: ";
    for (const auto& val : ret) {
	std::cout << val << " ";
    }
    std::cout << std::endl;    
    return ret;
}

int MLHeuristic::compute_heuristic(const State &ancestor_state) {
    std::vector<State> ancestor_states = {ancestor_state};
    return compute_heuristic_batch(ancestor_states)[0];
}

std::vector<float> MLHeuristic::compute_heuristic_batch(const std::vector<State> &ancestor_states) {
    // auto start = std::chrono::high_resolution_clock::now();
    // std::cout << "compute_heuristic_batch" << std::endl;
    auto func_start = std::chrono::high_resolution_clock::now();
    std::vector<int> ret;
    size_t batch_size = 64;

    // edge shapes
    if (numberOfVectors == 0){
	py::list ml_state = fd_state_to_ml_state(ancestor_states[0]);
	py::tuple re = rep.attr("state_to_tensor")(ml_state);
        numberOfVectors = py::len(re[1]);
	re_size = re.size();
    }
    // std::cout << "tuple size:" << re.size() << std::endl;
    // std::cout << "states size:" <<  ancestor_states.size() << std::endl;
    
    for (size_t i = 0; i < ancestor_states.size(); i += batch_size) {
	size_t current_batch_size = std::min(batch_size, ancestor_states.size() - i);
        
        int edge_start_num = 0;
	std::vector<torch::Tensor> batch_nodes;
        std::vector<std::vector<torch::Tensor>> array_batch_edges(numberOfVectors);  //for llg and ilg
	std::vector<torch::Tensor> batch_edges;  // for oog
	std::vector<torch::Tensor> batch_edge_attrs;  // for oog   
	std::vector<int64_t> batch_graph_ids;
	for (size_t j = i, graph_id = 0; j < i + current_batch_size; ++j, graph_id++) {
            // std::cout << i << j << graph_id << std::endl;
            // Get states from python function and transfter to cpp types.

	    // count prepare data time
	    auto start = std::chrono::high_resolution_clock::now();
	    py::list ml_state = fd_state_to_ml_state(ancestor_states[j]);
            py::tuple re = rep.attr("state_to_tensor")(ml_state);
            auto end = std::chrono::high_resolution_clock::now();
	    std::chrono::duration<double> duration = end - start;
	    cumulate_data_time += duration.count();

	    py::object py_x = re[0];            
	    torch::Tensor cpp_x = THPVariable_Unpack(static_cast<PyObject *>(py_x.ptr()));
	    batch_nodes.push_back(cpp_x);

	    if (re.size() == 2) {
		// llg and ilg
		py::list py_edge_list = re[1];
		// batch * 6 * [tesnors], stack into 6 * [batch * tensors]            
		for (size_t ii = 0; ii < numberOfVectors; ii+=1) {
		    torch::Tensor cpp_edges = THPVariable_Unpack(static_cast<PyObject *>(py_edge_list[ii].ptr()));
		    // update edges' index;
		    // std::cout << "edge start num:" << edge_start_num << std::endl;
		    torch::Tensor updated_edges = cpp_edges + edge_start_num;
		    array_batch_edges[ii].push_back(updated_edges);

                    // auto shape = cpp_edge.sizes();
                    // std::cout << "Original Shape: [";
                    // for (size_t i = 0; i < shape.size(); ++i) {
                    //    std::cout << shape[i];
                    //    if (i < shape.size() - 1) {
                    //        std::cout << ", ";
                    //    }
                    // }
                    // std::cout << "]" << std::endl;
		}
            // } else if (re.size() == 2 && rep_type=="pog") {
	    // 	// pog edges
            //     py::object py_edges = re[1];
	    // 	torch::Tensor cpp_edges = THPVariable_Unpack(static_cast<PyObject *>(py_edges.ptr()));
	    // 	torch::Tensor updated_edges = cpp_edges + edge_start_num;
            //     batch_edges.push_back(updated_edges);
                
            } else {
                // oog edges
                py::object py_edges = re[1];
		torch::Tensor cpp_edges = THPVariable_Unpack(static_cast<PyObject *>(py_edges.ptr()));
		torch::Tensor updated_edges = cpp_edges + edge_start_num;
                batch_edges.push_back(updated_edges);

                // oog edge attrs
                py::object py_edge_attrs = re[2];
		torch::Tensor cpp_edge_attrs = THPVariable_Unpack(static_cast<PyObject *>(py_edge_attrs.ptr()));
                batch_edge_attrs.push_back(cpp_edge_attrs);
            }

	    // batch_graph_ids is for identifying the node in which graphs.
	    batch_graph_ids.insert(batch_graph_ids.end(), cpp_x.size(0), graph_id);

	    // update start number for next item's edge
	    edge_start_num += cpp_x.size(0);
	}

	// Prepare input structure
	std::vector<c10::IValue> inputs;
	torch::Tensor batch_nodes_tensor = torch::cat(batch_nodes, 0).to(device);
	inputs.push_back(batch_nodes_tensor); // nodes

        if (re_size == 2) {
	    // for llg and ilg batch edges
	    std::vector<torch::Tensor> batch_edges_tensor;
	    for (const auto &batch_edges : array_batch_edges) {
		torch::Tensor stacked_edges = torch::cat(batch_edges, 1);  // stack with dimension 1
		stacked_edges = stacked_edges.to(device);
		batch_edges_tensor.push_back(stacked_edges);
	    }
	    inputs.push_back(batch_edges_tensor);
        // } else if (re.size() == 2 && rep_type=="pog") {
	//     // for pog 
        //     std::cout << "rep:" << rep_type << std::endl;
        //     torch::Tensor batch_edges_tensor = torch::cat(batch_edges, 1).to(device);
	//     inputs.push_back(batch_edges_tensor);  // edges
        } else if (re_size == 3) {
            // for oog 
	    torch::Tensor batch_edges_tensor = torch::cat(batch_edges, 1).to(device);
	    inputs.push_back(batch_edges_tensor);  // edges
            
            torch::Tensor batch_edge_attrs_tensor = torch::cat(batch_edge_attrs, 0).to(device);
	    inputs.push_back(batch_edge_attrs_tensor);  // edge attributes
	}
        
	torch::Tensor batch_graph_tensor = torch::tensor(batch_graph_ids, torch::dtype(torch::kInt64)).to(device);
	inputs.push_back(batch_graph_tensor); // node graph id

	// std::cout << "Check input shape:" << batch_nodes_tensor.size(0) << ", " <<  batch_nodes_tensor.size(1) << ", " << batch_graph_tensor.size(0) << std::endl;

	// count model time
	auto start = std::chrono::high_resolution_clock::now();
	c10::IValue result = model(inputs);
	torch::Tensor result_tensor = result.toTensor();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	cumulate_model_time += duration.count();
        
	// Ensure tensor move to CPU and is contiguous.
	// Get a pointer to the tensor data
	// Insert data into a std::vector<int>
        result_tensor = result_tensor.contiguous().to(torch::kCPU, torch::kInt32);;
	auto data_ptr = result_tensor.data_ptr<float>();
        ret.insert(ret.end(), data_ptr, data_ptr + result_tensor.numel());
	// Print the vector elements
	// std::cout << "Compute heuristic batch: ";
	// for (const auto& val : ret) {
	// 	std::cout << val << " ";
	// }
	// std::cout << std::endl;
    }

    // auto stop = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast < std::chrono::milliseconds>(stop - start);
    // std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;

    auto func_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> func_duration = func_end - func_start;
    cumulate_heuristic_time += func_duration.count();
    return ret;
}


class MLHeuristicFeature : public plugins::TypedFeature<Evaluator, MLHeuristic> {
  public:
    MLHeuristicFeature() : TypedFeature("ml") {
        document_title("ML heuristic");
        document_synopsis("TODO");

        add_option<string>("rep_type", "rep type", "default_value");        
        add_option<string>("model_path", "path to trained model or model weights", "default_value");
        add_option<string>("domain_file", "Path to the domain file.", "default_file");
        add_option<string>("instance_file", "Path to the instance file.", "default_file");

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

static plugins::FeaturePlugin<MLHeuristicFeature> _plugin;

} // namespace ml_heuristic
