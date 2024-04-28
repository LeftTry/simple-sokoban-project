#ifndef SOKOBAN_PARSER_H
#define SOKOBAN_PARSER_H

#include "globals.h"
#include "level.h"
#include "vector"
#include "fstream"

using namespace std;

class parser {
public:

    parser() = default;

    static vector<level> decode_levels_from_file(const string &path) {
        vector<level> decoded_levels;
        ifstream iss(path);
        int line_number = 0;
        bool is_separated = true;

        for (string line; getline(iss, line);) {
            line_number++;
            if (line.empty())
                continue;

            if (line[0] == ';') {
                if (is_separated)
                    throw runtime_error(" separation symbol must have a symbol before it");

                is_separated = true;
                continue;
            }

            try {
                if (!is_separated)
                    throw runtime_error(" must be separation between levels");

                decoded_levels.push_back(decode_level(line));
                is_separated = false;
            }
            catch (runtime_error &e) {
                cerr << "Error on " << to_string(line_number) << ":" << e.what() << "\n";
            }
        }
        return decoded_levels;
    }
private:
    static level decode_level(string &text) {
        vector<pair<vector<char>, size_t>> data(1);
        size_t rows = 1;
        size_t columns = 0;
        size_t curr_cols = 0;
        int current_read_position = 0;
        int player_symbol_position = -1;
        string num;

        for (char current: text) {
            current_read_position++;
            if (current == ';')
                throw runtime_error(to_string(current_read_position)
                + ": level separation symbol can't be on the same line");

            if (current == '|') {
                rows++;
                columns = max(columns, curr_cols);
                data[rows - 2].second = curr_cols;
                curr_cols = 0;
                data.emplace_back();
                continue;
            }

            if (isdigit(current)) {
                num += current;
            } else {
                int characters_quantity = num.empty() ? 1 : stoi(num);
                if (current == PLAYER) {
                    if (characters_quantity > 1) {
                        throw runtime_error(to_string(current_read_position - num.length())
                        + ": player can't be spawned more than 1 times");
                    }
                    player_symbol_position = current_read_position;
                }
                curr_cols += characters_quantity;
                num = "";
                for (int i = 0; i < characters_quantity; i++)
                    data[rows - 1].first.push_back(current);
            }
        }

        if (player_symbol_position == -1)
            throw runtime_error(" level doesn't contain a player");

        vector<char> decoded_data;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns - data[i].second; j++)
                data[i].first.push_back(' ');
            decoded_data.insert(decoded_data.end(), data[i].first.begin(), data[i].first.end());
        }
        return {rows, columns, decoded_data};
    }

};

#endif //SOKOBAN_PARSER_H
