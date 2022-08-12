#include <iostream>
#include <list>
#include <vector>
#include <utility>
#include <algorithm>
#include "tak.h"
#include "game.h"


TakGame::TakGame(size_t _board_size = 5): board_size(_board_size), carry_limit(_board_size) {
    reset_game();
};
        
size_t TakGame::get_size() {
    return board_size;
}

size_t TakGame::get_carry_limit() {
    return carry_limit;
}

void TakGame::reset_game() {
    // Create board
    board = TakBoard(board_size, std::vector<std::list<Piece*>>(board_size, std::list<Piece*>()));

    // Create players
    players = std::pair<Player, Player>(Player {one}, Player {two});
    
    // Create player one pieces
    player_one_pieces = std::vector<Piece>();
    if (board_size != 5) {std::cout << "Please set board size to 5";}
    for (int ix = 0; ix < 21; ix++) {
        player_one_pieces.push_back(Piece {ix, 1, false, true});
    }
    player_one_pieces.push_back(Piece {21, 1, true, false});
    player_one_pieces_placed = 0;
    player_one_capstone_placed = false;

    // Create player two pieces
    player_two_pieces = std::vector<Piece>();
    for (int ix = 0; ix < 21; ix++) {
        player_two_pieces.push_back(Piece {ix, 2, false, true});
    }
    player_two_pieces.push_back(Piece {21, 2, true, false});
    player_two_pieces_placed = 0;
    player_two_capstone_placed = false;
}

int TakGame::add_piece_to_board(std::pair<int, int> pos, int player, bool capstone = false, bool standing = false) {
    // Check that the position is within the board
    if (pos.first < 0 || pos.second < 0 || pos.first >= static_cast<int>(board_size) || pos.second >= static_cast<int>(board_size)) {
        std::cout << "Invalid placement, position outside of board" << std::endl;
        return -1;
    }

    // Check that the position is empty
    if (board[pos.first][pos.second].size() > 0) {
        std::cout << "Invalid placement, position not empty" << std::endl;
        return -1;
    }

    // Check that not all normal stones are placed
    if (capstone == false && (player_one_pieces_placed >= 21 || player_two_pieces_placed >= 21)) {
        std::cout << "Invalid placement, all normal stones already placed" << std::endl;
        return -1;
    }

    if (player == 1) { // First do it for player 1
        if (capstone) {
            // A capstone should not be already placed
            if (player_one_capstone_placed == true) {
                std::cout << "Invalid placement, capstone already placed" << std::endl;
                return -1;
            }

            // Place the capstone
            player_one_pieces[21].pos = pos;
            player_one_capstone_placed = true;
            board[pos.first][pos.second].push_back(&(player_one_pieces[21]));
        } else {
            // Place the stone, if it's standing, indicate it, add it to the board
            player_one_pieces[player_one_pieces_placed].pos = pos;
            if (standing == true) {player_one_pieces[player_one_pieces_placed].flat = false;}
            board[pos.first][pos.second].push_back(&(player_one_pieces[player_one_pieces_placed]));
            player_one_pieces_placed += 1;
        }
    } else if (player == 2) { // Do the same if it's player two
        if (capstone) {
            // A capstone should not be already placed
            if (player_two_capstone_placed == true) {
                std::cout << "Invalid placement, capstone already placed" << std::endl;
                return -1;
            }

            // Place the capstone
            player_two_pieces[21].pos = pos;
            player_two_capstone_placed = true;
            board[pos.first][pos.second].push_back(&(player_two_pieces[21]));
        } else {
            // Place the stone, if it's standing, indicate it
            player_two_pieces[player_two_pieces_placed].pos = pos;
            if (standing == true) {player_two_pieces[player_two_pieces_placed].flat = false;}
            board[pos.first][pos.second].push_back(&(player_two_pieces[player_two_pieces_placed]));
            player_two_pieces_placed += 1;
        }
    } else {
        std::cout << "Invalid placement, invalid player specified" << std::endl;
        return -1;
    }
    return 0;
}

bool TakGame::dfs(const std::pair<size_t, size_t> pos, const bool west_east, std::vector<std::vector<bool>>& visited) {
    std::cout << "dfs called at pos" << pos.first << pos.second << std::endl;
    size_t i = pos.first;
    size_t j = pos.second;
    bool reached_other_side;
    int current_player = board[i][j].back()->player;
    visited[i][j] = true; // Mark current position as visited
    
    // Initially, west-east are always started from the west, so if we have
    // reached east, then there is a path. Similarly for north-south.
    if (west_east == true) {
        reached_other_side = (j == board_size - 1) ? true : false;
        if (reached_other_side == true) {return true;}
    } else {
        reached_other_side = (i == board_size - 1) ? true : false;
        if (reached_other_side == true) {return true;}
    }

    // The conditions for being a valid neighbor are:
    // 1) Needs to be within the board
    // 2) Needs to have a stone placed of the current player's color
    // 3) The stone needs to be one that counts towards the road
    // 4) The position needs to be not visited

    // Find neighbors of current position
    std::vector<std::pair<size_t, size_t>> neighbors;
    // Neighbor right above current position
    if (i - 1 >= 0 && i - 1 < board_size && board[i - 1][j].size() > 0 && (board[i - 1][j].back()->capstone || board[i - 1][j].back()->flat)
        && board[i - 1][j].back()->player == current_player && !visited[i - 1][j]) {
        neighbors.push_back(std::pair<size_t, size_t>(i - 1, j));
    }
    // Neighbor right below current position
    if (i + 1 >= 0 && i + 1 < board_size && board[i + 1][j].size() > 0 && (board[i + 1][j].back()->capstone || board[i + 1][j].back()->flat)
        && board[i + 1][j].back()->player == current_player && !visited[i + 1][j]) {
        neighbors.push_back(std::pair<size_t, size_t>(i + 1, j));
    }
    // Neighbor to the left of current position
    if (j - 1 >= 0 && j - 1 < board_size && board[i][j - 1].size() > 0 && (board[i][j - 1].back()->capstone || board[i][j - 1].back()->flat)
        && board[i][j - 1].back()->player == current_player && !visited[i][j - 1]) {
        neighbors.push_back(std::pair<size_t, size_t>(i, j - 1));
    }
    // Neighbor to the right of current position
    if (j + 1 >= 0 && j + 1 < board_size && board[i][j + 1].size() > 0 && (board[i][j + 1].back()->capstone || board[i][j + 1].back()->flat)
        && board[i][j + 1].back()->player == current_player && !visited[i][j + 1]) {
        neighbors.push_back(std::pair<size_t, size_t>(i, j + 1));
    }

    // Call DFS recursively on the neighbors
    for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
        if (dfs(*it, west_east, visited) == true) {
            return true;
        }
    }
    return false;
}

std::pair<bool, bool> TakGame::victory() {
    bool player_one_wins = false;
    bool player_two_wins = false;
    std::vector<std::vector<bool>> visited 
        {std::vector<std::vector<bool>>(board_size, std::vector<bool>(board_size, false))}; // Visited indicators

    for (size_t i = 0; i < board_size; i++) { // First check for west-east road
        // If there are stones placed and their owner is not a winner yet
        if ((board[i][0].size() > 0) && (board[i][0].back()->player == 1 ? player_one_wins == false : player_two_wins == false)) {
            if (board[i][0].back()->flat || board[i][0].back()->capstone) { // If they count towards the road
                std::cout << "Callig DFS with "<< i << 0 << std::endl;
                bool res = dfs(std::pair<size_t, size_t>{i, 0}, true, visited); // Call DFS starting from that position
                if (res == true) { // If there is a road, there is a winner
                    if (board[i][0].back()->player == 1) { // If it's player one, he wins
                        player_one_wins = true;
                    } else {
                        player_two_wins =  true; // If it's player two, he wins
                    }
                }
            }
        }
    }

    std::vector<std::vector<bool>> visited_north_south 
        {std::vector<std::vector<bool>>(board_size, std::vector<bool>(board_size, false))}; // Visited indicators
    for (size_t j = 0; j < board_size; j++) { // Then check for north-south road
        // If there are stones placed and their owner is not a winner yet
        if ((board[0][j].size() > 0) && (board[0][j].back()->player == 1 ? player_one_wins == false : player_two_wins == false)) {
            if (board[0][j].back()->flat || board[0][j].back()->capstone) { // If they count towards the road
                std::cout << "Callig DFS with "<< 0 << j << std::endl;
                bool res = dfs(std::pair<size_t, size_t>{0, j}, false, visited_north_south); // Call DFS starting from that position
                if (res == true) { // If there is a road, there is a winner
                    if (board[0][j].back()->player == 1) { // If it's player one, he wins
                        player_one_wins = true;
                    } else {
                        player_two_wins =  true; // If it's player two, he wins
                    }
                }
            }
        }
    }
    return std::pair<bool, bool>(player_one_wins, player_two_wins);
}

int TakGame::move_stack(std::pair<int, int> pos, int player, Direction direction, std::vector<int>& partition) {
    // Check that the position is within the board
    if (pos.first < 0 || pos.second < 0 || pos.first >= static_cast<int>(board_size) || pos.second >= static_cast<int>(board_size)) {
        std::cout << "Invalid movement, position outside of board" << std::endl;
        return -1;
    }

    // Check that the position is not empty
    if (board[pos.first][pos.second].size() == 0) {
        std::cout << "Invalid movement, position is empty" << std::endl;
        return -1;
    }

    // Check that the position is controlled by the player
    if (board[pos.first][pos.second].back()->player != player) {
        std::cout << "Invalid movement, position not controlled by player" << std::endl;
        return -1;
    }

    // Check the directions don't try to move us out of the board
    if (pos.first == 0 && direction == Direction::north) {
        std::cout << "Invalid movement, cannot move north" << std::endl; 
        return -1;
    } else if (pos.first == static_cast<int>(board_size) - 1 && direction == Direction::south) {
        std::cout << "Invalid movement, cannot move south" << std::endl; 
        return -1;
    } else if (pos.second == 0 && direction == Direction::west) {
        std::cout << "Invalid movement, cannot move west" << std::endl;
        return -1;
    } else if (pos.second == static_cast<int>(board_size) - 1 && direction == Direction::east) {
        std::cout << "Invalid movement, cannot move east" << std::endl; 
        return -1;
    }

    int i = pos.first;
    int j = pos.second;
    int num_pieces_in_position = board[i][j].size();
    int num_pieces_carried = std::min(num_pieces_in_position, static_cast<int>(carry_limit));
    int sum_of_partitions = 0;
    bool top_is_capstone = board[i][j].back()->capstone;

    // Check the first partition element
    if (partition[0] < 0) {
        std::cout << "Invalid movement, invalid partition specified" << std::endl;
        return -1;
    }
    
    // Check that all subsequent partitions leave at least 1 piece per space
    for (size_t ix=0; ix < partition.size(); ix++) {
        int v = partition[ix];
        if (ix >= 1 && v < 1) {
            std::cout << "Invalid movement, invalid partition specified" << std::endl;
            return -1;
        }
        sum_of_partitions += v;
    }

    // Check that the partition sums up to the number of pieces carried
    if (sum_of_partitions != num_pieces_carried) {
        std::cout << "Invalid movement, partition should sum to " << num_pieces_carried << std::endl;
        return -1;
    }

    // Check that there are no standing stones or capstones along the partition
    int new_i;
    int new_j;
    for (size_t ix = 1; ix < partition.size(); ix++) {                
        // Get the new position, consisting of new_i and new_j
        if (direction == Direction::north) {
            new_i = i - ix;
            new_j = j;
        } else if (direction == Direction::east) {
            new_i = i;
            new_j = j + ix;
        } else if (direction == Direction::south) {
            new_i = i + ix;
            new_j = j;
        } else {
            new_i = i;
            new_j = j - ix;
        }

        // if (board[new_i][new_j].size() > 0) { // There is a piece there
        //     if (board[new_i][new_j].back()->capstone) {
        //         std::cout << "Invalid movement, capstones or standing stones present along move path" << std::endl;
        //         return -1;
        //     } else if (board[new_i][new_j].back()->flat == false) { // Standing stone
        //         if (ix == partition.size() - 1) { // The standing stone is the last stone encountered in the path
        //             if (!top_is_capstone) {
        //                 std::cout << "Invalid movement, capstones or standing stones present along move path" << std::endl;
        //             }
        //         }
        //     }
        // }

        if (board[new_i][new_j].size() > 0 && \
            (board[new_i][new_j].back()->capstone || (board[new_i][new_j].back()->flat == false))) {
            std::cout << "Invalid movement, capstones or standing stones present along move path" << std::endl;
            return -1;
        }
    }
    
    // At this point, the partition should be valid and we just have to execute the move.
    // Set iterator pointing to middle of stack, before moving any pieces
    std::list<Piece*>::iterator it = board[i][j].begin();
    std::advance(it, static_cast<int>(board[i][j].size() - num_pieces_carried));

    for (size_t ix = 0; ix < partition.size(); ix++) {
        int v = partition[ix];
        
        // Get the new position, consisting of new_i and new_j
        if (direction == Direction::north) {
            new_i = i - ix;
            new_j = j;
        } else if (direction == Direction::east) {
            new_i = i;
            new_j = j + ix;
        } else if (direction == Direction::south) {
            new_i = i + ix;
            new_j = j;
        } else {
            new_i = i;
            new_j = j - ix;
        }

        for (int __ix = 0; __ix < v; __ix++) {
            // Move the current piece pointed at by iterator it to the new position new_pos
            if (ix == 0) {
                it++;
                continue;
            }
            (*it)->pos = std::pair<int, int>(new_i, new_j); // Update the position of the piece
            board[new_i][new_j].push_back(*it); // Add the pointer to the new stack
            it = board[i][j].erase(it); // Delete the pointer from the old stack


        }
    }
    return 0;
}

void TakGame::run() {
    std::string command;
    int current_player = 1;
    while (true) {                
        print_board();

        // Get user command
        std::cout << "Player " << current_player << " command: ";
        std::getline(std::cin, command);

        // Extract command tokens
        std::string delimiter = " ";
        size_t current_position = 0;
        std::string token;
        std::vector<std::string> tokens;
        while ((current_position = command.find(delimiter)) != std::string::npos) {
            token = command.substr(0, current_position);
            tokens.push_back(token);
            command.erase(0, current_position + delimiter.length());
        }
        tokens.push_back(command.substr(0, command.length()));

        // Use "q" to quit and "r" to reset the game
        std::string main_command = tokens[0];
        if (main_command == "q") {
            break;
        } else if (main_command == "r") {
            reset_game();
            continue;
        }

        if (tokens.size() < 3) {
            std::cout << "Failed to parse command" << std::endl;
            continue;
        }

        // Parse the command tokens
        int i = stoi(tokens[1]);
        int j = stoi(tokens[2]);
        std::string specifier = tokens[3];
        bool valid = false;
        if (main_command == "p") { // A "p" stands for place, i.e. add a new piece
            int command_result;
            if (specifier == "c") {
                command_result = add_piece_to_board(std::pair<int, int>(i, j), current_player, true, false);
            } else if (specifier == "s") {
                command_result = add_piece_to_board(std::pair<int, int>(i, j), current_player, false, true);
            } else if (specifier == "f") {
                command_result = add_piece_to_board(std::pair<int, int>(i, j), current_player, false, false);
            } else {
                std::cout << "Failed to parse command, invalid specifier" << std::endl;
                valid = false;
            }
            if (command_result >= 0) {
                valid = true;
            }
        } else if (main_command == "m") { // m 2 3 r 0,1,1,1
            int command_result;
            std::vector<int> partition_vector;
            for (size_t _ix = 0; _ix < tokens[4].length(); _ix++) {
                if (tokens[4][_ix] == ',')
                    continue;
                partition_vector.push_back(tokens[4][_ix] - '0');
            }
            if (specifier == "n" || specifier == "u") {
                command_result = move_stack(std::pair<int, int>(i, j), current_player, Direction{north}, partition_vector);
            } else if (specifier == "e" || specifier == "r") {
                command_result = move_stack(std::pair<int, int>(i, j), current_player, Direction{east}, partition_vector);
            } else if (specifier == "s" || specifier == "d") {
                command_result = move_stack(std::pair<int, int>(i, j), current_player, Direction{south}, partition_vector);
            } else if (specifier == "w" || specifier == "l") {
                command_result = move_stack(std::pair<int, int>(i, j), current_player, Direction{west}, partition_vector);
            } else {
                std::cout << "Failed to parse command, invalid specifier" << std::endl;
                valid = false;
            }
            if (command_result >= 0) {
                valid = true;
            }
        }

        // If it's a valid command, change the player
        if (valid) {
            current_player = current_player == 1 ? 2 : 1;
        }
    }
}

void TakGame::print_board() {
    for (size_t i = 0; i < board_size; i++) {
        for (size_t j = 0; j < board_size; j++) {
            if (board[i][j].size() == 0) {
                std::cout << std::string(12, ' ') << "|";
            } else {
                std::list<Piece*>::iterator it;
                int symbols_printed = 0;
                for (it = board[i][j].begin(); it != board[i][j].end(); ++it) {
                    if ((*it)->capstone) {
                        std::cout << (*it)->player << "c,";
                        symbols_printed += 3;
                    } else if ((*it)->flat == false) {
                        std::cout << (*it)->player << "s,";
                        symbols_printed += 3;
                    } else {
                        std::cout << (*it)->player << ",";
                        symbols_printed += 2;
                    }
                }
                std::cout << std::string(12 - symbols_printed, ' ');
                std::cout << "|";
            }   
        }
        std::cout << std::endl;
    }
}