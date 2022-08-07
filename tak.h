#include <list>
#include <vector>
#include <utility>

#pragma once

enum Player {
    one = 1,
    two = 2
};

enum Direction {
    north = 1,
    east = 2,
    south = 3,
    west = 4
};


typedef std::pair<int, int> loc;
