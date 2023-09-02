#include "movemaker.h"
#include <stdlib.h>
#include <chrono>
#include <thread>

void MoveMaker::init(const BoardBin& b)
{
    Rules::Moves mv;
    is_white ? r.move_list(b, mv) : r.move_list_enemy(b, mv);
    moves.clear();
    endpoint.clear();
    for(auto& m: mv)
    {
        moves.push_back(m.b);
    }
}
void MoveMaker::prepare_for_next_move(const BoardBin& own_move, const BoardBin& enemy_move)
{
    //TODO
    Rules::Moves mv;
    is_white ? r.move_list(enemy_move, mv) : r.move_list_enemy(enemy_move, mv);
    moves.clear();
    for(auto& m: mv)
    {
        moves.push_back(m.b);
    }
}
BoardBin MoveMaker::make_a_best_move(int depth)
{
    int index = rand() % moves.size();
    return moves[index];
}
