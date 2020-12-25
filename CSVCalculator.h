//
// Created by apel on 25.12.2020.
//

#ifndef CSVCALCULATOR_CSVCALCULATOR_H
#define CSVCALCULATOR_CSVCALCULATOR_H

#include <iostream>
#include <fstream>
#include <regex>
#include <map>

class CSVCalculator {
public:
    bool load(std::istream &file);

    bool unload(std::ostream &file);

    void calculate();

private:
    bool _is_load = false;
    char _delimiter = ',';
    std::uint64_t _number_of_columns;

    std::vector<std::string> _original_columns;
    std::map<std::string, std::uint64_t> _columns_indexes;
    std::map<std::uint64_t, std::vector<std::string>> _rows;

    std::map<char, std::function<uint64_t(uint64_t, uint64_t)>>
            _operators = {{'+', std::plus<>()},
                          {'-', std::minus<>()},
                          {'*', std::multiplies<>()},
                          {'/', std::divides<>()}};

    void _update_number_of_columns(const std::string &line);

    uint64_t _parse_operand(const std::string &operand);

    uint64_t _update_cell(std::size_t i, std::size_t j);

    auto _parse_csv_row(const std::string &line, bool isHeader = false);

    void _read_csv_file(std::istream &file);
};


#endif //CSVCALCULATOR_CSVCALCULATOR_H
