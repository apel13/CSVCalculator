//
// Created by apel on 25.12.2020.
//

#include "CSVCalculator.h"

bool CSVCalculator::load(std::istream &file) {
    _read_csv_file(file);
    _is_load = true;
    return true;
}

bool CSVCalculator::unload(std::ostream &file) {
    if (!_is_load) {
        throw std::runtime_error("csv file not load");
    }
    if (file.bad()) {
        return false;
    }
    for (auto &_original_column : _original_columns) {
        file << _delimiter << _original_column;
    }
    file << std::endl;
    for (auto &pair: _rows) {
        auto &row_idx = pair.first;
        auto &cells = pair.second;
        file << row_idx << _delimiter;
        for (size_t col_idx = 0; col_idx < cells.size(); ++col_idx) {
            file << cells[col_idx];
            if (col_idx != cells.size() - 1) {
                file << _delimiter;
            }
        }
        file << std::endl;
    }
    return true;
}

void CSVCalculator::calculate() {
    for (auto &pair : _rows) {
        auto &row_idx = pair.first;
        auto &cells = pair.second;
        for (size_t column_idx = 0; column_idx < cells.size(); ++column_idx) {
            this->_update_cell(row_idx, column_idx);
        }
    }
}

void CSVCalculator::_update_number_of_columns(const std::string &line) {
    this->_number_of_columns = std::count(line.begin(), line.end(), _delimiter);
}

uint64_t CSVCalculator::_parse_operand(const std::string &operand) {
    static const auto operand_regex = std::regex(R"((\w*?)(\d+))");
    std::smatch pieces_match;

    if (std::regex_match(operand, pieces_match, operand_regex)) {
        if (pieces_match[1].str().empty()) {
            //operand is number
            return std::stod(pieces_match[2].str());
        } else {
            //operand is cell
            size_t column_index = _columns_indexes[pieces_match[1].str()];
            size_t row_index = std::stoul(pieces_match[2].str());
            return this->_update_cell(row_index, column_index);
        }
    }
    throw std::runtime_error("can't parse operand: " + operand);
}

uint64_t CSVCalculator::_update_cell(size_t i, size_t j) {
    if (_rows[i][j].at(0) != '=') {
        return std::stod(_rows[i][j]);
    }
    static const auto expression_regex = std::regex(R"(=\s*(\w*?\d+)\s*([\+\*-\/])\s*(\w*?\d+))");
    std::smatch pieces_match;
    if (std::regex_match(_rows[i][j], pieces_match, expression_regex)) {
        uint64_t first_operand = this->_parse_operand(pieces_match[1].str());
        auto operand_operator = _operators[pieces_match[2].str()[0]];
        uint64_t second_operand = this->_parse_operand(pieces_match[3].str());

        uint64_t result = operand_operator(first_operand, second_operand);
        _rows[i][j] = std::to_string(result);
        return result;
    }
    throw std::runtime_error("can't parse expression: " + _rows[i][j]);
}

auto CSVCalculator::_parse_csv_row(const std::string &line, bool isHeader) {
    std::vector<std::string> result;
    result.reserve(this->_number_of_columns);

    std::stringstream lineStream(line);
    std::string cell;

    std::getline(lineStream, cell, _delimiter);
    size_t line_no = 0;
    if (!isHeader)
        line_no = stoul(cell);

    while (std::getline(lineStream, cell, _delimiter)) {
        if (!cell.empty()) {
            result.push_back(cell);
        }
    }
    return std::make_pair(line_no, result);
}

void CSVCalculator::_read_csv_file(std::istream &file) {
    //headers
    std::string current_row;
    getline(file, current_row);
    _update_number_of_columns(current_row);
    _original_columns = std::move(_parse_csv_row(current_row, true).second);

    for (size_t i = 0; i < _original_columns.size(); ++i) {
        _columns_indexes[_original_columns[i]] = i;
    }

    //_rows
    while (getline(file, current_row)) {
        _rows.insert(_parse_csv_row(current_row));
    }
}
