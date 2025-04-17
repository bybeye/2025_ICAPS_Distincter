#include "perfect3.h"

#include "../evaluation_context.h"
#include "../evaluator.h"
#include "../open_list_factory.h"
#include "../pruning_method.h"

#include "../algorithms/ordered_set.h"
#include "../plugins/options.h"
#include "../task_utils/successor_generator.h"
#include "../utils/logging.h"

#include <cassert>
#include <cstdlib>
#include <memory>
#include <map>
#include <optional>
#include <set>
#include <typeinfo>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>

using namespace std;

namespace perfect3 {
Perfect3::Perfect3(const plugins::Options &opts) : SearchEngine(opts) {
    states_path = std::getenv("STATES_OUTPUT_PATH");
    plan_path = std::getenv("PLAN_INPUT_PATH");
    std::cout << "Plan_path: " << plan_path << ", states_path:" << states_path << std::endl;
}

void Perfect3::initialize() {
    initialise_grounded_facts();
    // State initial_state = state_registry.get_initial_state();
}

void Perfect3::print_statistics() const {
    statistics.print_detailed_statistics();
    search_space.print_statistics();
}

void Perfect3::initialise_grounded_facts() {
  FactsProxy facts(*task);
  for (FactProxy fact : facts) {
    string fact_str = fact.get_name();
    std::regex rex("[\\)]");
    fact_str = std::regex_replace(fact_str, rex, "");
    std::regex rex2("[\\(]");
    fact_str = std::regex_replace(fact_str, rex2, ", ");
    
    // Convert from FDR var-val pairs back to propositions
    if (fact_str == "<none of those>") {
      continue;
    } else if (fact_str == "") {
      continue;
    } else {
      if (fact_str.substr(0, 5) == "Atom ") {
        fact_str = fact_str.substr(5);
      } else if (fact_str.substr(0, 12) == "NegatedAtom ") {
        continue;
      } else {
        std::cout << "Substring of downward fact does not start with 'Atom ': "
                  << "or 'NegatedAtom '" << fact_str << std::endl;
        exit(-1);
      }
    }
    
    fact_to_string.insert({fact.get_pair(), fact_str});
  }
}

/**
   byb added
   Save states with the pddl goal format.
   Using ';' to seperate goals.
   Eaxmple: (arm-empty);(clear a)(on a b);(ontable b);
**/
void Perfect3::write_state_to_file_3(std::ofstream &plan_file, const State& s, const std::string op_name, const int g, const int optim) {
  State state = task_proxy.convert_ancestor_state(s);
  for (FactProxy fact : state) {
    std::string fact_str = fact_to_string[fact.get_pair()];
    if (fact_str == ""){
	continue;
    }    
    plan_file << fact_str << ";;";
  }
  
  plan_file << "||" << op_name << "||" << g << "||" << optim << endl;
}


inline bool is_goal_state(TaskProxy task, const State &state) {
    for (FactProxy goal : task.get_goals()) {
        if (state[goal.get_variable()] != goal)
            return false;
    }
    return true;
}

SearchStatus Perfect3::step() {
  State next_s = state_registry.get_initial_state();
  std::ofstream output_file(states_path);
  write_state_to_file_3(output_file, next_s, "none", 0, 1);

  std::map<std::string, int> name_to_op_id;
  for (size_t i = 0; i < task_proxy.get_operators().size(); i++) {
      OperatorProxy op = task_proxy.get_operators()[i];      
      name_to_op_id[op.get_name()] = int(i);
      std::cout << op.get_name() << ":" << i << std::endl;
  }

  std::string line;
  ifstream file(plan_path);
  int step_num = 0;
  if (file.is_open()) {
      while (getline(file, line)) {
          if (line[0] == ';') {  // finished parsing
              break;
          }

	  step_num++;
          line = line.substr(1, line.size() - 2);
          int optim_op_id = -1;
          if (name_to_op_id.count(line)) {
              optim_op_id = name_to_op_id[line];	               
	  } else {
	      cout << "invalid plan because cannot find action " << line << endl;
          }

	  State optim_s = state_registry.get_initial_state();          
	  vector<OperatorID> applicable_ops;
          successor_generator.generate_applicable_ops(next_s, applicable_ops);
	  int num_candidates = 0;
          for (OperatorID op_id : applicable_ops) {
              OperatorProxy op = task_proxy.get_operators()[op_id];
	      OperatorProxy optim_op = task_proxy.get_operators()[optim_op_id];              
	      State succ_state = state_registry.get_successor_state(next_s, op);
              SearchNode succ_node = search_space.get_node(succ_state);
              if (succ_node.is_dead_end()) {
		  cout << "dead end node:" << op.get_name() << endl;
		  continue;
              }

              if (succ_node.is_new()) {
		  if(op.get_id() == optim_op.get_id()){
                      optim_s = succ_state;
		      cout << "execute operation:" << op.get_name() << endl;                      
                      write_state_to_file_3(output_file, succ_state, op.get_name(), step_num, 1);
		  }else{
		      write_state_to_file_3(output_file, succ_state, op.get_name(), step_num, 0);
		  }
              } else {
		  cout << "ERROR: node is not new:" << op.get_name() << endl;
              }
	      num_candidates++;
	  }
	  next_s = optim_s;              
	  cout << "num_candidates:" << num_candidates << endl;
      }

      file.close();
      if (!is_goal_state(task_proxy, next_s)) {
          cout << "invalid plan because final state is not a goal state" << endl;
          return FAILED;
      }
  } else {
      cout << "Unable to open file";
      return FAILED;
  }
  output_file << "; GOOD";
  return SOLVED;
}

void Perfect3::dump_search_space() const {
    search_space.dump(task_proxy);
}

void add_options_to_feature(plugins::Feature &feature) {
    SearchEngine::add_pruning_option(feature);
    SearchEngine::add_options_to_feature(feature);
}
}
