#include <iostream>
#include <list>
#include <vector>


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
    return 0;
}