#include "game.h"


int main(int argc, char** argv) {
    TakGame game {5};
    game.run();
    // game.add_piece_to_board(loc(2, 2), 1, false, false);
    // game.add_piece_to_board(loc(2, 3), 2, false, false);
    // game.add_piece_to_board(loc(2, 1), 1, false, false);
    // game.add_piece_to_board(loc(1, 2), 2, false, false);

    // game.print_board();
    // game.move_stack(loc(2, 3), 2, Direction(west), std::vector<int>{0, 1});
    // game.print_board();

    // game.move_stack(loc(2, 1), 2, Direction(west), std::vector<int>{0, 1});
    // game.move_stack(loc(2, 3), 2, Direction(west), std::vector<int>{0, 1});
    // game.move_stack(loc(2, 3), 2, Direction(west), std::vector<int>{0, 1});


    // game.run();
}