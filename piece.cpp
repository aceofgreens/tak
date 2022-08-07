#include <list>
#include <vector>
#include <utility>
#include "tak.h"
#include "piece.h"


Piece::Piece(const int _id, const int _player, const bool _capstone, const bool _flat): 
    id(_id), player(_player), capstone(_capstone), flat(_flat), pos(std::pair<int, int>(-1, -1)) {
};
        
bool Piece::is_placed() {
    return (pos.first >= 0 && pos.second >= 0) ? true : false;
}
