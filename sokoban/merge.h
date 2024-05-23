#ifndef MERGE_H
#define MERGE_H

#include "vector"

#include "globals.h"
#include "iostream"
#include "level.h"
#include "player.h"
#include "parser.h"

class merge {
public:
    merge() {
        player = new Player(0, 0);
        levels = parser::decode_levels_from_file("data/levels.sl");
    };

    void load_next_level(){
        ++level_index;
        if (level_index >= level_count) {
            level_index = 0;
            game_state = VICTORY_STATE;
            create_victory_menu_background();
        }

        current_level = levels[level_index];

        for (size_t row = 0; row < current_level.get_rows(); ++row) {
            for (size_t column = 0; column < current_level.get_columns(); ++column) {
                char cell = levels[level_index].get_level_cell(row, column);
                if (cell == PLAYER) {
                    current_level.set_level_cell(row, column, FLOOR);
                    player->spawn_player(row, column);
                } else if (cell == PLAYER_ON_GOAL) {
                    current_level.set_level_cell(row, column, GOAL);
                    player->spawn_player(row, column);
                } else {
                    current_level.set_level_cell(row, column, cell);
                }
            }
        }
        level_struct current_level_struct = level_struct(current_level.get_rows(),
                                                         current_level.get_columns(),
                                                         current_level.get_data());
        derive_graphics_metrics_from_loaded_level(&current_level_struct);
    }

    void move_player(int dx, int dy) {
        int next_row = static_cast<int>(player->get_player_row()) + dy;
        int next_column = static_cast<int>(player->get_player_column()) + dx;
        if (!current_level.is_cell_inside_level(next_row, next_column)) {
            return;
        }

        char cell = current_level.get_level_cell(static_cast<size_t>(next_row), static_cast<size_t>(next_column));
        if (cell == FLOOR || cell == GOAL) {
            player->move(next_row, next_column);
        } else if (cell == BOX || cell == BOX_ON_GOAL) {
            int target_row = next_row + dy;
            int target_column = next_column + dx;
            if (!current_level.is_cell_inside_level(target_row, target_column)) {
                return;
            }
            char target_cell = current_level.get_level_cell(static_cast<size_t>(target_row),
                                                            static_cast<size_t>(target_column));
            if (target_cell == FLOOR || target_cell == GOAL) {
                cell = cell == BOX ? FLOOR : GOAL;
                if (target_cell == FLOOR) {
                    target_cell = BOX;
                } else {
                    target_cell = BOX_ON_GOAL;
                    play_sound(goal_sound);
                }
                current_level.set_level_cell(target_row, target_column, target_cell);
                current_level.set_level_cell(next_row, next_column, cell);
                player->move(next_row, next_column);

                bool level_solved = true;
                for (size_t row = 0; level_solved && row < current_level.get_rows(); ++row) {
                    for (size_t column = 0; level_solved && column < current_level.get_columns(); ++column) {
                        char cell_to_test = current_level.get_level_cell(row, column);
                        if (cell_to_test == GOAL) {
                            level_solved = false;
                        }
                    }
                }
                if (level_solved) {
                    load_next_level();
                    play_sound(exit_sound);
                }
            }
        }
    }

    void update_game() {
        switch (game_state) {
            case MENU_STATE:
                SetExitKey(KEY_ESCAPE);
                if (IsKeyPressed(KEY_ENTER)) {
                    game_state = GAME_STATE;
                }
                break;
            case GAME_STATE:
                SetExitKey(0);
                if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
                    move_player(0, -1);
                    return;
                } else if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
                    move_player(0, 1);
                    return;
                } else if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
                    move_player(-1, 0);
                    return;
                } else if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
                    move_player(1, 0);
                    return;
                } else if (IsKeyPressed(KEY_ESCAPE)) {
                    game_state = RELOAD_REQ_STATE;
                }
                break;
            case RELOAD_REQ_STATE:
                if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
                    game_state = GAME_STATE;
                } else if (IsKeyPressed(KEY_R)) {
                    --level_index;
                    load_next_level();
                    game_state = GAME_STATE;
                }
                break;
            case VICTORY_STATE:
                SetExitKey(KEY_ESCAPE);
                if (IsKeyPressed(KEY_ENTER)) {
                    game_state = MENU_STATE;
                }
                break;
        }
    }

    void draw_game() {
        ++game_frame;
        level_struct lvl_struct = level_struct(current_level.get_rows(),
                                               current_level.get_columns(),
                                               current_level.get_data());
        player_struct p_struct = player_struct(player->get_player_row(),
                                               player->get_player_column());
        switch (game_state) {
            case MENU_STATE:
                draw_menu();
                break;
            case GAME_STATE:
                draw_loaded_level(&lvl_struct);
                draw_player(&p_struct);
                draw_player_level();
                break;
            case RELOAD_REQ_STATE:
                draw_reload_req_menu();
                break;
            case VICTORY_STATE:
                draw_victory_menu();
                break;
        }
    }

    void draw_player_level() const {
        const float level_font_size = GAME_LEVEL_FONT_SIZE * screen_scale;
        const float level_top_margin = GAME_LEVEL_Y_SHIFT * screen_scale;

        std::string level_text = std::string("Level ");
        level_text += std::to_string(level_index + 1);
        level_text += " out of ";
        level_text += std::to_string(level_count);

        Vector2 level_size = MeasureTextEx(menu_font, level_text.c_str(), level_font_size, 1);
        Vector2 level_position = {(screen_width - level_size.x) * 0.5f, level_top_margin};
        DrawTextEx(menu_font, level_text.c_str(), level_position,
                   level_font_size, 1, GAME_LEVEL_COLOR1);
        level_position.x -= 4 * screen_scale;
        level_position.y -= 4 * screen_scale;
        DrawTextEx(menu_font, level_text.c_str(), level_position,
                   level_font_size, 1, GAME_LEVEL_COLOR2);
    }

private:
    level current_level;
    size_t level_index = -1;
    size_t level_count = 3;
    std::vector<level> levels;
    Player *player;
};

#endif // MERGE_H