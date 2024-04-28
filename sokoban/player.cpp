#include "player.h"

void Player::spawn_player(size_t row, size_t column) {
    player_row = row;
    player_column = column;
}

void Player::move(int next_row, int next_column) {
    player_row = static_cast<size_t>(next_row);
    player_column = static_cast<size_t>(next_column);
}
