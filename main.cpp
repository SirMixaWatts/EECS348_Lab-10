#include <iostream>
#include "calculator.hpp"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./calculator <filename>\n";
        return 1;
    }

    process_file(argv[1]);
    return 0;
}
