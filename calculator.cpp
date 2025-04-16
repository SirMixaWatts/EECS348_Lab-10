#include "calculator.hpp"
#include <cctype>
#include <iostream>
#include <fstream>

bool is_valid_double(const std::string &s) {
    size_t i = 0;
    int len = s.length();

    if (len == 0) return false;

    // Optional +/-
    if (s[i] == '+' || s[i] == '-') i++;
    if (i == len) return false;

    bool has_digits = false;
    while (i < len && isdigit(s[i])) {
        has_digits = true;
        i++;
    }

    // Optional decimal point
    if (i < len && s[i] == '.') {
        i++;
        bool after_dot = false;
        while (i < len && isdigit(s[i])) {
            after_dot = true;
            i++;
        }
        return has_digits && after_dot && i == len;
    }

    return has_digits && i == len;
}

std::string add_doubles_as_strings(const std::string &a, const std::string &b) {
    // stub for now
    return "0.0";
}

void process_file(const std::string &filename) {
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open file\n";
        return;
    }

    while (std::getline(file, line)) {
        if (is_valid_double(line)) {
            std::string result = add_doubles_as_strings(line, "-123.456");
            std::cout << "Result of adding " << line << " and -123.456: " << result << "\n";
        } else {
            std::cout << "Invalid number: " << line << "\n";
        }
    }

    file.close();
}
