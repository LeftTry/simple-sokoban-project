#include <stdexcept>
#include "level.h"

bool level::is_cell_inside_level(int row, int column) const {
    return row < rows && column < columns;
}

char level::get_level_cell(size_t row, size_t column) {
    if(data.empty()) throw std::runtime_error("Data is null");
    return data[row * columns + column];
}

void level::set_level_cell(size_t row, size_t column, char cell) {
    if(data.empty()) throw std::runtime_error("Data is null");
    data[row * columns + column] = cell;
}
