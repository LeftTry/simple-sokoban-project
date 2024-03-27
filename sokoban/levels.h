#ifndef LEVELS_H
#define LEVELS_H

#include "globals.h"
#include "player.h"
#include "graphics.h"
#include "iostream"

#include <cstddef>

void load_next_level() {
    ++level_index;
    if (level_index >= LEVEL_COUNT) {
        level_index = 0;
        game_state = VICTORY_STATE;
        create_victory_menu_background();
    }

    level.set_rows(LEVELS[level_index]->get_rows());
    level.set_columns(LEVELS[level_index]->get_columns());
    level.set_data(new char[level.get_rows() * level.get_columns()]);

    for (size_t row = 0; row < level.get_rows(); ++row) {
        for (size_t column = 0; column < level.get_columns(); ++column) {
            char cell = LEVELS[level_index]->get_data()[row * level.get_columns() + column];
            if (cell == PLAYER) {
                std::cout << "OK" << std::endl;
                set_level_cell(row, column, FLOOR);
                spawn_player(row, column);
            } else if (cell == PLAYER_ON_GOAL) {
                set_level_cell(row, column, GOAL);
                spawn_player(row, column);
            } else {
                set_level_cell(row, column, cell);
            }
        }
    }

    derive_graphics_metrics_from_loaded_level();
}

void unload_level() {
    level.set_rows(0);
    level.set_columns(0);
    level.set_data(nullptr);
}

bool is_cell_inside_level(int row, int column) {
    return row < level.get_rows() && column < level.get_columns();
}

char& get_level_cell(size_t row, size_t column) {
    return level.get_data()[row * level.get_columns() + column];
}

void set_level_cell(size_t row, size_t column, char cell) {
    level.get_data()[row * level.get_columns() + column] = cell;
}

#endif // LEVELS_H
