#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <set>
#include <algorithm>
#include <cctype>

class ParseProblem {
public:
    ParseProblem(const std::string& input_pddl) {
        pddl_content = remove_comments(input_pddl);
	pddl_content = std::regex_replace(pddl_content, std::regex("\\t"), " ");

        // Transform the string to lowercase
        std::transform(pddl_content.begin(), pddl_content.end(), pddl_content.begin(), ::tolower);
        
        if (!check_valid(pddl_content)) {
            throw std::invalid_argument("PDDL file error, stop working.");
        }
    }

    std::set<std::string> parse_goals() {
        std::set<std::string> goals;
        // std::regex goal_regex(":goal([\\s\\S]*)");
        // std::smatch match;
        // if (std::regex_search(pddl_content, match, goal_regex) && !match.str().empty()) {

	std::string goal_content = extractSubstring(pddl_content, ":goal", "-1");
	if(!goal_content.empty()){
        // std::string goal_content = extractSubstring(pddl_content, ":goal(", ")");
            // std::string goal_content = match.str(1);
	    parse_nested_content(goal_content, goals);

	    // std::cout << "goal_content: " << goal_content << std::endl;
            // Extract nested content within the goal section
        }
        return goals;
    }


    std::string extractSubstring(const std::string& input, const std::string& startKeyword, const std::string& endKeyword) {
	// Find the start index after the startKeyword
	size_t startIndex = input.find(startKeyword);
	if (startIndex == std::string::npos) {
	    return "Start keyword not found.";
	}
	// Adjust startIndex to be after the keyword
	startIndex += startKeyword.length();

        // Find the end index using the endKeyword

	if (endKeyword != "-1"){
	    size_t endIndex = input.find(endKeyword, startIndex);
	    if (endIndex == std::string::npos) {
		return "End keyword not found.";
	    }

	    // Extract the substring between the start and end indices
            return input.substr(startIndex, endIndex - startIndex);
        } else {
            return input.substr(startIndex);
	}
    }
        
    std::set<std::string> parse_inits() {
        std::set<std::string> inits;

        std::string init_content = extractSubstring(pddl_content, ":init", "(:goal");
	// std::cout << init_content << std::endl;
        if (!init_content.empty()){
	    parse_nested_content(init_content, inits);
	} 

	// std::cout << "Into inits..." << std::endl;

	// auto begin = std::sregex_iterator(pddl_content.begin(), pddl_content.end(), init_regex);
	// auto end = std::sregex_iterator();
	// for (std::sregex_iterator i = begin; i != end; ++i) {
	//     std::smatch match = *i;
	//     std::cout << "Match found: " << match.str(1) << std::endl;
	//     // Additional processing
	// }

	// try {
	//     if (std::regex_search(pddl_content, match, init_regex)) {
	// 	std::cout << "Regex search successful." << std::endl;
	// 	if (!match.str().empty()){
	// 	    std::string init_content = match.str(1);
	// 	    std::cout << "init_content:" << init_content << std::endl;
	// 	    // Extract nested content within the init section
	// 	    parse_nested_content(init_content, inits);
	// 	} 
	//     }
	// } catch (const std::regex_error& e) {
	//     std::cerr << "Regex error: " << e.what() << std::endl;
	//     // Handle regex-specific errors
	// } catch (const std::exception& e) {
	//     std::cerr << "General exception: " << e.what() << std::endl;
	//     // Handle other exceptions
	// }
        
        return inits;
    }

    std::map<std::string, std::vector<std::string> > parse_objects() {
        std::map<std::string, std::vector<std::string> > objects;
        // std::regex object_regex(":objects\\s+([^)]+)");
        // std::smatch match;
        // if (std::regex_search(pddl_content, match, object_regex) && !match.str().empty()) {
	// std::string object_content = match.str(1);
        std::string object_content = extractSubstring(pddl_content, ":objects", ")");
	if(!object_content.empty()){
	    // object_content = std::regex_replace(object_content, std::regex("\\)"), "");

	    // std::cout << "object content: " << object_content << std::endl;
            // Split by newline or space to separate objects and types
            std::istringstream iss(object_content);
            std::string line;
	    std::vector<std::string> temp_objs;
            while (std::getline(iss, line)) {
                std::regex type_regex("(.*)- (.+)");
                std::smatch type_match;
                if (std::regex_match(line, type_match, type_regex)) {
		    std::string type = type_match[2];
		    if (!temp_objs.empty()) {
			objects[type].insert(objects[type].end(), temp_objs.begin(), temp_objs.end());
			temp_objs.clear();
                    }
                    
                    std::istringstream obj_iss(type_match[1]);
		    std::string obj;                    
                    while (obj_iss >> obj) {
                        objects[type].push_back(obj);
                    }
                    
                } else {
                    std::istringstream temp_iss(line);
		    std::string obj;
		    while (temp_iss >> obj) {
                        temp_objs.push_back(obj);
                    }
                }
            }

	    if (!temp_objs.empty()) {
		objects["object"].insert(objects["object"].end(), temp_objs.begin(), temp_objs.end());
	    }
        }
        return objects;
    }

private:
    std::string pddl_content;

    std::string remove_comments(const std::string& input) {
	return std::regex_replace(input, std::regex(";[^\n]*\n"), "\n");
    }

    bool check_valid(const std::string& input) {
        int balance = 0;
        for (char ch : input) {
            if (ch == '(') balance++;
            else if (ch == ')') balance--;
            if (balance < 0) return false;
        }
        return balance == 0;
    }

    void parse_nested_content(const std::string &content, std::set<std::string> &container) {
        std::string replaced_content = std::regex_replace(content, std::regex("\\(and"), " ");

	// std::cout << "replace content: " << replaced_content << std::endl;
        std::regex nested_regex("\\(([^)]+)\\)");
	// std::regex nested_regex("\\(([^)]+?)\\)");  // Non-greedy match        
        std::sregex_iterator iter(replaced_content.begin(), replaced_content.end(), nested_regex);
        std::sregex_iterator end;
        for (; iter != end; ++iter) {
	    std::string captured = iter->str(1);
            std::string trimmed = trim(captured);
	    // std::cout << trimmed << std::endl;
	    container.insert(trimmed); 
        }
    }

    std::string trim(const std::string& str) {
	size_t first = str.find_first_not_of(' ');
	if (first == std::string::npos)
	    return ""; // All spaces

	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
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
        // domain_names.push_back("./problem_pddl/satellite_p05.pddl");
        // domain_names.push_back("./problem_pddl/sokoban_p03.pddl");
        // domain_names.push_back("./problem_pddl/blocksworld_p01.pddl");
        // domain_names.push_back("./problem_pddl/ferry_p10.pddl");
        // domain_names.push_back("./problem_pddl/miconic_p05.pddl");
        // domain_names.push_back("./problem_pddl/satellite_p03.pddl");
        // domain_names.push_back("./problem_pddl/transport_p03.pddl");
        // domain_names.push_back("./problem_pddl/childsnack_p05.pddl");
        // domain_names.push_back("./problem_pddl/floortile_p12.pddl");
        // domain_names.push_back("./problem_pddl/rovers_p01.pddl");
        // domain_names.push_back("./problem_pddl/spanner_p05.pddl");
// 	domain_names.push_back("./problem_pddl/sokoban_medium_p03.pddl");

//         while(!domain_names.empty()){
// 	    std::string filename = domain_names.back();
//             std::cout << filename << std::endl;
//             domain_names.pop_back();

// 	    std::string content = read_file(filename);
// 	    ParseProblem parser(content);
// 	    auto goals = parser.parse_goals();
// 	    auto inits = parser.parse_inits();
// 	    auto objects = parser.parse_objects();

// 	    std::cout << "Goals:" << std::endl;
// 	    for (const auto& goal : goals) {
// 		std::cout << goal << "." <<std::endl;
// 	    }

// 	    std::cout << "Init:" << std::endl;
// 	    for (const auto& init : inits) {
// 		std::cout << init << "." << std::endl;
// 	    }

// 	    std::cout << "Objects:" << std::endl;
// 	    for (const auto& pair : objects) {
// 		std::cout << pair.first << ": ";
// 		for (const auto& obj : pair.second) {
// 		    std::cout << obj << ". ";
// 		}
// 		std::cout << std::endl;
//             }

// 	    std::cout << "------------------------------" << std::endl;
// 	}
//     } catch (const std::exception& e) {
// 	std::cerr << "Error: " << e.what() << std::endl;
//     }
//     return 0;
// }
