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
        const int player;
        const bool capstone;
        bool flat;
        Piece(const int _player, const bool _capstone, const bool _flat): player(_player), capstone(_capstone), flat(_flat) {}
};

typedef std::vector<std::vector<std::list<Piece>>> TakBoard;

class Board {
    private:
        size_t n; // Size of board
        int carry_limit; 
    public:
        TakBoard board; // The board
        Board(size_t _n = 5): n(_n), carry_limit(_n),
            board {_n, std::vector<std::list<Piece>>(_n, std::list<Piece>())} {};
        
        size_t get_size() {
            return n;
        }

        int get_carry_limit() {
            return carry_limit;
        }

        void add_piece(Piece& p, size_t i, size_t j) {
            board[i][j].push_back(p);
        }

        bool dfs(TakBoard& board, const int k, const bool west_east) {
            size_t n = get_size();
            int i; 
            int j;
            if (west_east == true) {
                i = k;
                j = 0;
            } else {
                i = 0;
                j = k;
            }
            int current_player = board[i][j].back().player;
        };

        std::pair<bool, bool> victory(TakBoard& board) {
            n = get_size();
            bool player_one_wins = false;
            bool player_two_wins = false;
            for (size_t i = 0; i < n; i++) { // First check for west-east road
                // If there are stones placed and their owner is not a winner yet
                if ((board[i][0].size() > 0) && (board[i][0].back().player == 1 ? player_one_wins == false : player_two_wins == false)) {
                    if (board[i][0].back().flat || board[i][0].back().capstone) { // If they count towards the road
                        bool res = dfs(board, i, true); // Call DFS starting from that position
                        if (res == true) { // If there is a road, there is a winner
                            if (board[i][0].back().player == 1) { // If it's player one, he wins
                                player_one_wins = true;
                            } else {
                                player_two_wins =  true; // If it's player two, he wins
                            }
                            continue; // If there's a winner, we cantinue
                        }
                    }
                }
            }

            for (size_t j = 0; j < n; j++) { // Then check for north-south road
                // If there are stones placed and their owner is not a winner yet
                if ((board[0][j].size() > 0) && (board[0][j].back().player == 1 ? player_one_wins == false : player_two_wins == false)) {
                    if (board[0][j].back().flat || board[0][j].back().capstone) { // If they count towards the road
                        bool res = dfs(board, j, false); // Call DFS starting from that position
                        if (res == true) { // If there is a road, there is a winner
                            if (board[0][j].back().player == 1) { // If it's player one, he wins
                                player_one_wins = true;
                            } else {
                                player_two_wins =  true; // If it's player two, he wins
                            }
                            continue; // If there's a winner, we cantinue
                        }
                    }
                }
            }
        return std::pair<bool, bool>(player_one_wins, player_two_wins);
        }

        void print_board() {
            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < n; j++) {
                    if (board[i][j].size() == 0) {
                        print("|   |", " ");
                    } else {
                        std::list<Piece>::iterator it;
                        for (it = board[i][j].begin(); it != board[i][j].end(); ++it) {
                            if (it->capstone) {
                                std::cout << it->player << "c, ";
                            } else if (! it->flat) {
                                std::cout << it->player << "s, ";
                            } else {
                                std::cout << it->player << " ";
                            }
                            }
                    }   
                }
                print("\n", " ");
            }
        }
    };
    

int main() {
    Board board {5};
    Piece x1 {1, true, false};
    Piece x2 {2, true, true};

    std::cout << board.board.size() << std::endl;
    board.add_piece(x1, 2, 3);
    board.add_piece(x1, 1, 4);
    board.add_piece(x2, 2, 3);

    print(board.get_size());
    board.print_board();
    std::pair<bool, bool> result = board.victory(board.board);
    std::cout << result.first << " " << result.second << std::endl;
    return 0;
}