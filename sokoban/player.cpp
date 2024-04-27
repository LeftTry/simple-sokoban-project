#include "player.h"

void Player::spawn_player(size_t row, size_t column) {
    player_row = row;
    player_column = column;
}

void Player::move(int next_row, int next_column) {
    player_row = static_cast<size_t>(next_row);
    player_column = static_cast<size_t>(next_column);
}

/*
bool player::move_player(int dx, int dy, level* current_level) {
    int next_row    = static_cast<int>(player_row)    + dy;
    int next_column = static_cast<int>(player_column) + dx;
    if (!current_level->is_cell_inside_level(next_row, next_column)) {
        return false;
    }

    char &cell = current_level->get_level_cell(static_cast<size_t>(next_row), static_cast<size_t>(next_column));
    if (cell == FLOOR || cell == GOAL) {
        player_row    = static_cast<size_t>(next_row);
        player_column = static_cast<size_t>(next_column);
    } else if (cell == BOX || cell == BOX_ON_GOAL) {
        int target_row    = next_row    + dy;
        int target_column = next_column + dx;
        if (!current_level->is_cell_inside_level(target_row, target_column)) {
            return false;
        }
        char &target_cell = current_level->get_level_cell(static_cast<size_t>(target_row), static_cast<size_t>(target_column));
        if (target_cell == FLOOR || target_cell == GOAL) {
            cell = cell == BOX ? FLOOR : GOAL;
            if (target_cell == FLOOR) {
                target_cell = BOX;
            } else {
                target_cell = BOX_ON_GOAL;
                play_sound(goal_sound);
            }

            player_row    = static_cast<size_t>(next_row);
            player_column = static_cast<size_t>(next_column);

            bool level_solved = true;
            for (size_t row = 0; level_solved && row < current_level->get_rows(); ++row) {
                for (size_t column = 0; level_solved && column < current_level->get_columns(); ++column) {
                    char cell_to_test = current_level->get_level_cell(row, column);
                    if (cell_to_test == GOAL) {
                        level_solved = false;
                    }
                }
            }
            return level_solved;
        }
    }
    return false;
}

player_struct player::make_player_struct() const {
    player_struct p_struct = player_struct(player_row, player_column);
    return p_struct;
}
*/