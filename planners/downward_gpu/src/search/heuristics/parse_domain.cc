#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <cctype> 

class ParseDomain {

private:
    int largest_param_num = 0;
    
public:
    ParseDomain(const std::string& input_pddl) {
        pddl_content = remove_comments(input_pddl);
	// std::cout << pddl_content << std::endl;
        // pddl_content = std::regex_replace(pddl_content, std::regex("\n"), " ");
        pddl_content = std::regex_replace(pddl_content, std::regex("\\r"), " ");
        pddl_content = std::regex_replace(pddl_content, std::regex("\\t"), " ");

	// Transform the string to lowercase
        std::transform(pddl_content.begin(), pddl_content.end(), pddl_content.begin(), ::tolower);
        
	// pddl_content = std::regex_replace(pddl_content, std::regex("\\n"), " ");
	// std::cout << pddl_content << std::endl;
        if (pddl_content.find(":typing") != std::string::npos) {
	    support_typing = true;
        }
        
        if (!check_valid(pddl_content)) {
            throw std::invalid_argument("PDDL file error, stop working.");
        }

        std::cout << "PDDL is valid." << std::endl;
    }

    int get_largest_param_num() {return largest_param_num;}

    std::vector<std::string> parse_types() {
        std::vector<std::string> types;
        std::smatch match;
        std::regex type_pattern(":types([\\s\\S]*?)\\)");

        if (std::regex_search(pddl_content, match, type_pattern) && match.size() > 1) {
            std::string types_str = match[1].str();

	    if (types_str.find("- ") != std::string::npos){
		// std::cout << "types_str" << types_str << std::endl;
		std::string line;
		std::istringstream const_stream(types_str);
		while (std::getline(const_stream, line)) {
		    std::istringstream line_stream(line);
		    std::string token;
		    while (line_stream >> token) {
                        if (token != "-") {
			    auto it = std::find(types.begin(), types.end(), token);
			    if (it == types.end()) {
                                types.push_back(token);
			    }                                
			}else{
			    break;
			}
		    }
		}
            }else{
		std::string type;
		std::istringstream type_stream(types_str);
	        while (type_stream >> type) {
		    types.push_back(type);
		}
	    }
        } else {
            types.push_back("object");
        }

        return types;
    }

    std::map<std::string, std::vector<std::string>> parse_constants() {
        std::map<std::string, std::vector<std::string>> constants;
        std::smatch match;
        std::regex constant_pattern(":constants(.*?)\\)");

        // std::cout << pddl_content << std::endl;
        if (std::regex_search(pddl_content, match, constant_pattern) && match.size() > 1) {
            std::istringstream const_stream(match[1].str());
            std::string line;
            while (std::getline(const_stream, line)) {
                std::istringstream line_stream(line);
                std::string token;
                std::vector<std::string> tokens;

                while (line_stream >> token) {
                    tokens.push_back(token);
                }

                if (support_typing and tokens.size() > 1) {
		    std::string type = tokens.back(); // Last element is type
		    tokens.pop_back();                // Remove type
		    tokens.pop_back();                // Remove -
		    constants[type].insert(constants[type].end(), tokens.begin(), tokens.end());
                } else if (tokens.size() > 1) {
                    std::string type = "object";
		    constants[type].insert(constants[type].end(), tokens.begin(), tokens.end());
                }
            }
        }
        return constants;
    }

    std::vector<std::string> parse_predicates() {
    // std::map<std::string, std::vector<std::string>> parse_predicates() {
        // std::map<std::string, std::vector<std::string>> predicates;
	std::vector<std::string> predicates;        
        std::smatch match;
        // std::regex predicate_pattern(":predicates(.*?)\\)");
	// std::regex predicate_pattern(":predicates(.*?)\\(:action");
        std::regex predicate_pattern(":predicates((?:[^:]|:(?!action))+)");

	// std::cout << "pddl content: " << pddl_content << std::endl;

        if (std::regex_search(pddl_content, match, predicate_pattern) && match.size() > 0) {
            // std::cout << match[0] << std::endl;
	    // std::cout << match[1].str() << std::endl;
            std::string predicates_str = match[1].str();
            std::regex predicate_expr("\\(([^)]+)\\)");
            std::sregex_iterator iter(predicates_str.begin(), predicates_str.end(), predicate_expr);
            std::sregex_iterator end;

            for (; iter != end; ++iter) {
		// std::cout << "output param:" << iter->str(1) << std::endl;
		std::string predicate_str = iter->str(1);
                std::istringstream pred_stream(predicate_str);
                std::string pred_name;
                pred_stream >> pred_name; // First token is predicate name
                // std::cout << "pred_name: " << pred_name << std::endl;

		std::string param;
		std::vector<std::string> params;
		params.push_back(pred_name);
                if (predicate_str.find("- ") != std::string::npos) {
		    int same_type_param_num = 0;
		    while (pred_stream >> param) {
			if (param.find('?') != std::string::npos) {
			    same_type_param_num += 1;
			} else if (param.find('-') != std::string::npos){
			    pred_stream >> param;
			    for (int i=0; i<same_type_param_num; i++){
				params.push_back(param);
			    }
			    same_type_param_num = 0;
			}
                    }
                } else {
                    while (pred_stream >> param) {
			params.push_back(param);
		    }
                }
                // predicates[predicate_str] = params;
                predicates.push_back(params[0]);

                if (int(params.size()) - 1 > largest_param_num) {
                    largest_param_num = params.size() - 1;
                }
            }
        } else {
	    std::cout<< "Predicates nothing found!" << std::endl;
        }

        return predicates;
    }

private:
    std::string pddl_content;
    bool support_typing = false;

    std::string remove_comments(const std::string& input) {
        return std::regex_replace(input, std::regex(";[^\n]*\n"), "\n");
    }

    bool check_valid(const std::string& input) {
        std::regex parentheses_pattern("[()]");
        std::sregex_iterator iter(input.begin(), input.end(), parentheses_pattern);
        std::sregex_iterator end;

        int valid_num = 0;
        int i = 0;
        int total = std::distance(iter, end);

        for (; iter != end; ++iter, ++i) {
            if (iter->str() == "(") {
                ++valid_num;
            } else if (iter->str() == ")") {
                --valid_num;
            }

            if (valid_num < 0) {
                std::cout << "valid_num is less than 0" << std::endl;
                return false;
            }

            if (valid_num == 0 && i != total - 1) {
                std::cout << "PDDL format error: early end parentheses" << std::endl;
                return false;
            }
        }

        if (valid_num != 0) {
            std::cout << "PDDL format error: parentheses format is wrong!" << std::endl;
            return false;
        }

        return true;
    }
};

// std::string read_file(const std::string& filename) {
//     std::ifstream file(filename);
//     if (!file.is_open()) {
//         std::cerr << "Failed to open file: '" << filename << "'" << std::endl;
//         throw std::runtime_error("Failed to open file.");
//     }
//     return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
// }

// int main() {
//     try {
// 	std::vector<std::string> domain_names;
//         domain_names.push_back("./domain_files/childsnack.pddl");
// 	domain_names.push_back("./domain_files/satellite.pddl");
//         domain_names.push_back("./domain_files/blocksworld.pddl");
// 	domain_names.push_back("./domain_files/ferry.pddl");
//         domain_names.push_back("./domain_files/miconic.pddl");
//         domain_names.push_back("./domain_files/rovers.pddl");
//         domain_names.push_back("./domain_files/floortile.pddl");
// 	domain_names.push_back("./domain_files/sokoban.pddl");
//         domain_names.push_back("./domain_files/spanner.pddl");
// 	domain_names.push_back("./domain_files/transport.pddl");

//         while (!domain_names.empty()){
//             std::string filename = domain_names.back();
// 	    std::cout << filename << std::endl;
//             domain_names.pop_back();
            
// 	    std::string file_content = read_file(filename);
// 	    ParseDomain parser(file_content);

// 	    auto types = parser.parse_types();
// 	    auto constants = parser.parse_constants();
// 	    auto predicates = parser.parse_predicates();

// 	    std::cout << "Types: ";
// 	    for (const auto& type : types) {
// 		std::cout << type << " ";
// 	    }
// 	    std::cout << std::endl;

// 	    std::cout << "Constants:" << std::endl;
// 	    for (const auto& [key, values] : constants) {
// 		std::cout << key << ": ";
// 		for (const auto& value : values) {
// 		    std::cout << value << " ";
// 		}
// 		std::cout << std::endl;
// 	    }

// 	    std::cout << "Predicates:" << std::endl;
// 	    for (const auto& [key, params] : predicates) {
// 		std::cout << key << ": ";
// 		for (const auto& param : params) {
// 		    std::cout << param << " ";
// 		}
// 		std::cout << std::endl;
//             }
// 	    std::cout << "------------------------" << std::endl;
// 	}
       
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << std::endl;
//     }
//     return 0;
// }
