#include <iostream>
#include <fstream>
#include <iostream>
#include <sstream>
#include "interpreter.h"

std::vector<std::string> splitStrings(const std::string& s) {
    std::stringstream ss;
    ss << s;
    std::vector<std::string> ans;
    std::string tmp;
    while (ss >> tmp) {
        ans.push_back(std::move(tmp));
    }
    return ans;
}

int main(int argc, char** argv) {
    std::string file_name;
    std::cin >> file_name;
    std::ifstream in(file_name);
    if (!in.is_open()) {
        std::cout << "Impossible to open file." << std::endl;
    } else {
        std::ostringstream output;
        interpret(in, output);
        std::cout << output.str();
    }
}