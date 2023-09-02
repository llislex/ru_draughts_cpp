#ifndef MOVEMAKER_H
#define MOVEMAKER_H

#include "rules.h"
#include <vector>

using namespace std;

class MoveMaker
{
    const Rules& r;
    bool is_white;
    int m_depth;
    vector<BoardBin> moves;
    vector<vector<BoardBin>> endpoint;
public:
    MoveMaker(const Rules& r, bool is_white, int depth = 8): r(r), is_white(is_white), m_depth(depth){}
    void init(const BoardBin& b);
    void prepare_for_next_move(const BoardBin& own_move, const BoardBin& enemy_move);
    BoardBin make_a_best_move(int depth);
};

#endif // MOVEMAKER_H
