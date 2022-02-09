#ifndef AI_H
#define AI_H

#include "..\rules\rules.h"
#include <list>
#include <vector>
#include <assert.h>

#define MAX_EVAL 126
#define MIN_EVAL -126

struct EvaluatedMove
{
    Rules::Move move;
    int eval;
    EvaluatedMove(const Rules::Move& m, int eval) : move(m), eval(eval){}
};

typedef std::vector<EvaluatedMove> EvaluatedMoves;

inline EvaluatedMoves moves_with_eval(const EvaluatedMoves& moves, int eval)
{
    EvaluatedMoves result;
    for(EvaluatedMoves::const_iterator it = moves.begin(); it != moves.end(); ++it)
        if(eval == it->eval)
            result.push_back(*it);
    return result;
}

inline int negamax(const BoardBin& b, const Rules& r, bool white, unsigned depth, int alpha, int beta)
{
    if(depth == 0)
    {
        int eval = BoardStat(b, r.bg).evaluate();
        return white ? eval : -eval;
    }
    int result = MIN_EVAL;
    Rules::Moves moves;
    bool is_hit = white ? r.move_list(b, moves) : r.move_list_enemy(b, moves);
    if (!is_hit)
        --depth;
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        int eval = negamax(m->b, r, !white, depth, -beta, -alpha);
        result = std::max(result, -eval);
        alpha = std::max(result, alpha);
        if (alpha >= beta)
            break;
    }
    return result;
}


inline int build_game_tree(const BoardBin& b, const Rules& r, bool white, unsigned depth, EvaluatedMoves& eval_moves)
{
    int result = MIN_EVAL;
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;
    Rules::Moves moves;
    bool is_hit = white ? r.move_list(b, moves) : r.move_list_enemy(b, moves);
    if (!is_hit)
        --depth;
    if(moves.size() == 1)
    {
        eval_moves.push_back(EvaluatedMove(*moves.begin(), 0));
        return 0;
    }
    for(Rules::Moves::const_iterator m = moves.begin(); m != moves.end(); ++m)
    {
        int eval = negamax(m->b, r, !white, depth, -beta, -alpha);
        result = std::max(result, -eval);
        //alpha = std::max(alpha, result);
        eval_moves.push_back(EvaluatedMove(*m, -eval));
    }
    return result;
}


inline unsigned ply_policy_ex(unsigned units, unsigned dam_units)
{
    /*if (dam_units > 1)
        return 6;*/
    units += dam_units * 3;
    if(units < 4)
        return 12;
    if(units < 6)
        return 10;
    if(units < 12)
        return 8;
    if(units < 18)
        return 7;
    return 7;
}

inline unsigned ply_policy(unsigned units, unsigned dam_units)
{
    return ply_policy_ex(units, dam_units) + 3;
}

#endif // AI_H
