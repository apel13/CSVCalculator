#include <iostream>

#include "CSVCalculator.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "the program requires one argument: the path to the csv file" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    std::ifstream file(filename, std::ios::in);
    if (!file.is_open()) {
        std::cout << "can't open the file " << filename << std::endl;
        return 2;
    }

    CSVCalculator calc;
    calc.load(file);
    calc.calculate();
    calc.unload(std::cout);
    file.close();
    return 0;
}