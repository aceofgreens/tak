#include <list>
#include <vector>
#include <utility>
#include "tak.h"


struct Piece {
    public:
        const int id; // A numeric index
        const int player; // 1 or 2
        const bool capstone; // Whether it's a capstone
        bool flat; // Is it a flat stone, only meaningful when placed
        loc pos; // Current position, (-1, -1) indicates not placed;
        Piece(const int _id, const int _player, const bool _capstone, const bool _flat);
        bool is_placed();
};
