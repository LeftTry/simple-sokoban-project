#include <stdexcept>
#include "level.h"

bool level::is_cell_inside_level(int row, int column) const {
    return row < rows && column < columns;
}

char level::get_level_cell(size_t row, size_t column) {
    return data[row * columns + column];
}

void level::set_level_cell(size_t row, size_t column, char cell) {
    data[row * columns + column] = cell;
}
