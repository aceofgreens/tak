#include <iostream>
#include <list>
#include <vector>
#include <utility>


template<typename T>
void print(T x, std::string sep = "\n") {
    std::cout << x << sep;
}

struct Piece {
    public:
        const int id; // A numeric index
        const int player; // 1 or 2
        const bool capstone; // Whether it's a capstone
        bool flat; // Is it a flat stone, only meaningful when placed
        std::pair<int, int> pos; // Current position, (-1, -1) indicates not placed;
        Piece(const int _id, const int _player, const bool _capstone, const bool _flat): 
            id(_id), player(_player), capstone(_capstone), flat(_flat), pos(std::pair<int, int>(-1, -1)) {}
        
        bool is_placed() {
            return (pos.first >= 0 && pos.second >= 0) ? true : false;
        }
};

enum Player {
    one = 1,
    two = 2
};

typedef std::vector<std::vector<std::list<Piece&>>> TakBoard;

// class Board {
//     private:
//         size_t n; // Size of board
//         int carry_limit; 
//     public:
//         TakBoard board; // The board
//         Board(size_t _n = 5): n(_n), carry_limit(_n),
//             board {_n, std::vector<std::list<Piece>>(_n, std::list<Piece>())} {};
        
//         size_t get_size() {
//             return n;
//         }

//         int get_carry_limit() {
//             return carry_limit;
//         }

//         void add_piece(Piece& p, size_t i, size_t j) {
//             if (check_if_valid_to_add(p, i, j) == true) {
//                 board[i][j].push_back(p);
//             }
//         }

//         // TODO: This should be a static method 
//         bool dfs(TakBoard& board, const std::pair<size_t, size_t> pos, const bool west_east, std::vector<std::vector<bool>>& visited) {
//             std::cout << "dfs called at pos" << pos.first << pos.second << std::endl;
//             size_t n = get_size();
//             size_t i = pos.first;
//             size_t j = pos.second;
//             bool reached_other_side;
//             int current_player = board[i][j].back().player;
//             visited[i][j] = true; // Mark current position as visited
            
//             // Initially, west-east are always started from the west, so if we have
//             // reached east, then there is a path. Similarly for north-south.
//             if (west_east == true) {
//                 reached_other_side = (j == n - 1) ? true : false;
//                 if (reached_other_side == true) {return true;}
//             } else {
//                 reached_other_side = (i == n - 1) ? true : false;
//                 if (reached_other_side == true) {return true;}
//             }

//             // The conditions for being a valid neighbor are:
//             // 1) Needs to be within the board
//             // 2) Needs to have a stone placed of the current player's color
//             // 3) The stone needs to be one that counts towards the road
//             // 4) The position needs to be not visited

//             // Find neighbors of current position
//             std::vector<std::pair<size_t, size_t>> neighbors;
//             // Neighbor right above current position
//             if (i - 1 >= 0 && i - 1 < n && board[i - 1][j].size() > 0 && (board[i - 1][j].back().capstone || board[i - 1][j].back().flat)
//                 && board[i - 1][j].back().player == current_player && !visited[i - 1][j]) {
//                 neighbors.push_back(std::pair<size_t, size_t>(i - 1, j));
//             }
//             // Neighbor right below current position
//             if (i + 1 >= 0 && i + 1 < n && board[i + 1][j].size() > 0 && (board[i + 1][j].back().capstone || board[i + 1][j].back().flat)
//                 && board[i + 1][j].back().player == current_player && !visited[i + 1][j]) {
//                 neighbors.push_back(std::pair<size_t, size_t>(i + 1, j));
//             }
//             // Neighbor to the left of current position
//             if (j - 1 >= 0 && j - 1 < n && board[i][j - 1].size() > 0 && (board[i][j - 1].back().capstone || board[i][j - 1].back().flat)
//                 && board[i][j - 1].back().player == current_player && !visited[i][j - 1]) {
//                 neighbors.push_back(std::pair<size_t, size_t>(i, j - 1));
//             }
//             // Neighbor to the right of current position
//             if (j + 1 >= 0 && j + 1 < n && board[i][j + 1].size() > 0 && (board[i][j + 1].back().capstone || board[i][j + 1].back().flat)
//                 && board[i][j + 1].back().player == current_player && !visited[i][j + 1]) {
//                 neighbors.push_back(std::pair<size_t, size_t>(i, j + 1));
//             }

//             // Call DFS recursively on the neighbors
//             for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
//                 if (dfs(board, *it, west_east, visited) == true) {
//                     return true;
//                 }
//             }
//             return false;
//         };

//         std::pair<bool, bool> victory(TakBoard& board) {
//             n = get_size();
//             bool player_one_wins = false;
//             bool player_two_wins = false;
//             std::vector<std::vector<bool>> visited {std::vector<std::vector<bool>>(n, std::vector<bool>(n, false))}; // Visited indicators

//             for (size_t i = 0; i < n; i++) { // First check for west-east road
//                 // If there are stones placed and their owner is not a winner yet
//                 if ((board[i][0].size() > 0) && (board[i][0].back().player == 1 ? player_one_wins == false : player_two_wins == false)) {
//                     if (board[i][0].back().flat || board[i][0].back().capstone) { // If they count towards the road
//                         std::cout << "Callig DFS with "<< i << 0 << std::endl;
//                         bool res = dfs(board, std::pair<size_t, size_t>{i, 0}, true, visited); // Call DFS starting from that position
//                         if (res == true) { // If there is a road, there is a winner
//                             if (board[i][0].back().player == 1) { // If it's player one, he wins
//                                 player_one_wins = true;
//                             } else {
//                                 player_two_wins =  true; // If it's player two, he wins
//                             }
//                         }
//                     }
//                 }
//             }

//             std::vector<std::vector<bool>> visited_north_south {std::vector<std::vector<bool>>(n, std::vector<bool>(n, false))}; // Visited indicators
//             for (size_t j = 0; j < n; j++) { // Then check for north-south road
//                 // If there are stones placed and their owner is not a winner yet
//                 if ((board[0][j].size() > 0) && (board[0][j].back().player == 1 ? player_one_wins == false : player_two_wins == false)) {
//                     if (board[0][j].back().flat || board[0][j].back().capstone) { // If they count towards the road
//                         std::cout << "Callig DFS with "<< 0 << j << std::endl;
//                         bool res = dfs(board, std::pair<size_t, size_t>{0, j}, false, visited_north_south); // Call DFS starting from that position
//                         if (res == true) { // If there is a road, there is a winner
//                             if (board[0][j].back().player == 1) { // If it's player one, he wins
//                                 player_one_wins = true;
//                             } else {
//                                 player_two_wins =  true; // If it's player two, he wins
//                             }
//                         }
//                     }
//                 }
//             }
//         return std::pair<bool, bool>(player_one_wins, player_two_wins);
//         }

//     };



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
        TakGame(size_t _board_size = 5): board_size(_board_size), carry_limit(_board_size) {
            reset_game();
        };
        
        size_t get_size() {
            return board_size;
        }

        size_t get_carry_limit() {
            return carry_limit;
        }

        void reset_game() {
            // Create board
            board = TakBoard(board_size, std::vector<std::list<Piece&>>(board_size, std::list<Piece&>()));

            // Create players
            players = std::pair<Player, Player>(Player {one}, Player {two});
            
            // Create player one pieces
            player_one_pieces = std::vector<Piece>();
            if (board_size != 5) {std::cout << "Please set board size to 5";}
            for (int ix = 0; ix < 21; ix++) {
                player_one_pieces.push_back(Piece {ix, 1, false, false});
            }
            player_one_pieces.push_back(Piece {21, 1, true, false});

            // Create player two pieces
            player_two_pieces = std::vector<Piece>();
            for (int ix = 0; ix < 21; ix++) {
                player_two_pieces.push_back(Piece {ix, 2, false, false});
            }
            player_one_pieces.push_back(Piece {21, 2, true, false});
        }

        void add_piece_to_board(std::pair<int, int> pos, int player, bool capstone = false, bool standing = false) {
            // Check that the position is within the board
            if (pos.first < 0 || pos.second < 0 || pos.first >= static_cast<int>(board_size) || pos.second >= static_cast<int>(board_size)) {
                std::cout << "Invalid placement, position outside of board" << std::endl;
                return;
            }

            // Check that the position is empty
            if (board[pos.first][pos.second].size() > 0) {
                std::cout << "Invalid placement, position not empty" << std::endl;
                return;
            }

            // Check that not all normal stones are placed
            if (capstone == false && (player_one_pieces_placed >= 21 || player_two_pieces_placed >= 21)) {
                std::cout << "Invalid placement, all normal stones already placed" << std::endl;
            }

            if (player == 1) { // First do it for player 1
                if (capstone) {
                    // A capstone should not be already placed
                    if (player_one_capstone_placed == true) {
                        std::cout << "Invalid placement, capstone already placed" << std::endl;
                        return;
                    }

                    // Place the capstone
                    player_one_pieces[21].pos = pos;
                    player_one_capstone_placed = true;
                    board[pos.first][pos.second].push_back(player_one_pieces[21]);
                } else {
                    // Place the stone, if it's standing, indicate it, add it to the board
                    player_one_pieces[player_one_pieces_placed].pos = pos;
                    if (standing) {player_one_pieces[player_one_pieces_placed].flat = false;}
                    board[pos.first][pos.second].push_back(player_one_pieces[player_one_pieces_placed]);
                    player_one_pieces_placed += 1;
                }
            } else if (player == 2) { // Do the same if it's player two
                if (capstone) {
                    // A capstone should not be already placed
                    if (player_two_capstone_placed == true) {
                        std::cout << "Invalid placement, capstone already placed" << std::endl;
                        return;
                    }

                    // Place the capstone
                    player_two_pieces[21].pos = pos;
                    player_two_capstone_placed = true;
                    board[pos.first][pos.second].push_back(player_two_pieces[21]);
                } else {
                    // Place the stone, if it's standing, indicate it
                    player_two_pieces[player_two_pieces_placed].pos = pos;
                    if (standing) {player_two_pieces[player_two_pieces_placed].flat = false;}
                    board[pos.first][pos.second].push_back(player_two_pieces[player_two_pieces_placed]);
                    player_two_pieces_placed += 1;
                }
            } else {
                std::cout << "Invalid placement, invalid player specified" << std::endl;
                return;
            }
        }

        void print_board() {
            for (size_t i = 0; i < board_size; i++) {
                for (size_t j = 0; j < board_size; j++) {
                    if (board[i][j].size() == 0) {
                        std::cout << std::string(12, ' ') << "|";
                    } else {
                        std::list<Piece>::iterator it;
		            	int symbols_printed = 0;
                        for (it = board[i][j].begin(); it != board[i][j].end(); ++it) {
                            if (it->capstone) {
                                std::cout << it->player << "c,";
				                symbols_printed += 3;
                            } else if (! it->flat) {
                                std::cout << it->player << "s,";
				                symbols_printed += 3;
                            } else {
                                std::cout << it->player << ",";
				                symbols_printed += 2;
                            }
                        }
                        std::cout << std::string(12 - symbols_printed, ' ');
                        std::cout << "|";
                    }   
                }
                print("\n", "");
            }
        }
};


int main() {
    TakGame game {5};
    game.print_board();
    return 0;
}
