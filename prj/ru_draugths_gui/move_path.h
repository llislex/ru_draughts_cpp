#ifndef MOVE_PATH_H
#define MOVE_PATH_H

#include "../../../ru_draughts/ru_draughts/board.h"
#include "../../../ru_draughts/ru_draughts/rules.h"
#include <vector>

class MovePath
{
public:
    MovePath(const BoardBin& b, const Rules& r, bool white);
    std::vector<unsigned> path;
    std::vector<unsigned> end_point;
    std::vector<unsigned> start_point;
    Rules::Moves moves;
};

#endif // MOVE_PATH_H
