#ifndef LEVEL_H
#define LEVEL_H

#include <utility>

#include "vector"
#include "iostream"

class level {
public:
    level() {
        rows = 0;
        columns = 0;
        data = std::vector<char>();
    }

    level(size_t num_rows, size_t num_cols) {
        rows = num_rows;
        columns = num_cols;
        data = std::vector<char>();
    }

    level(size_t num_rows, size_t num_cols, std::vector<char> given_data) {
        rows = num_rows;
        columns = num_cols;
        data = std::move(given_data);
    }

    ~level() {
        rows = 0;
        columns = 0;
    }

    [[nodiscard]] size_t get_rows() const {
        return rows;
    }

    [[nodiscard]] size_t get_columns() const {
        return columns;
    }

    std::vector<char> get_data() {
        return data;
    }

    [[nodiscard]] bool is_cell_inside_level(int row, int column) const;

    char get_level_cell(size_t row, size_t column);

    void set_level_cell(size_t row, size_t column, char cell);

private:
    size_t rows = 0;
    size_t columns = 0;
    std::vector<char> data;
};


#endif // LEVEL_H
