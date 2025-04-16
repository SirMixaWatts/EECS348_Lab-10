#include "calculator.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    if (!input || !output) {
        std::cerr << "Error opening file.\n";
        return 1;
    }

    std::string line;
    while (std::getline(input, line)) {
        // Expecting input like "25.7 + -123.456"
        size_t plus_pos = line.find('+');
        if (plus_pos == std::string::npos) continue;

        std::string lhs = line.substr(0, plus_pos);
        std::string rhs = line.substr(plus_pos + 1);
        
        // Trim whitespace
        lhs.erase(0, lhs.find_first_not_of(" \t\n\r"));
        lhs.erase(lhs.find_last_not_of(" \t\n\r") + 1);
        rhs.erase(0, rhs.find_first_not_of(" \t\n\r"));
        rhs.erase(rhs.find_last_not_of(" \t\n\r") + 1);

        std::string result = add_doubles_as_strings(lhs, rhs);
        output << result << std::endl;
    }

    input.close();
    output.close();
    return 0;
}

