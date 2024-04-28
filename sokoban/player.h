#ifndef PLAYER_H
#define PLAYER_H

#include "level.h"

#include <cstddef>

class Player {
public:
    Player(size_t row, size_t column) {
        player_row = row;
        player_column = column;
    }

    ~Player() {
        player_column = 0;
        player_row = 0;
    }

    [[nodiscard]] size_t get_player_row() const {
        return player_row;
    }

    [[nodiscard]] size_t get_player_column() const {
        return player_column;
    }

    void spawn_player(size_t row, size_t column);

    void move(int next_row, int next_column);

private:
    size_t player_row = 0;
    size_t player_column = 0;
};

#endif // PLAYER_H
