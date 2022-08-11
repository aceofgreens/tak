#include <iostream>
#include <list>
#include <vector>
#include <utility>
// #include <algorithm>
#include "tak.h"
#include "piece.h"

typedef std::vector<std::vector<std::list<Piece*>>> TakBoard;

class TakGame {
    private:
        TakBoard board;
        size_t board_size;
        size_t carry_limit;
        std::pair<Player, Player> players;
        std::vector<Piece> player_one_pieces;
        std::vector<Piece> player_two_pieces;
        int player_one_pieces_placed;
        int player_two_pieces_placed;
        bool player_one_capstone_placed;
        bool player_two_capstone_placed;
    public:
        TakGame(size_t _board_size);
        size_t get_size();
        size_t get_carry_limit();
        void reset_game();
        int add_piece_to_board(loc pos, int player, bool capstone, bool standing);
        bool dfs(const std::pair<size_t, size_t> pos, const bool west_east, std::vector<std::vector<bool>>& visited);
        std::pair<bool, bool> victory();
        int move_stack(loc pos, int player, Direction direction, std::vector<int>& partition);
        void run();
        void print_board();
};

