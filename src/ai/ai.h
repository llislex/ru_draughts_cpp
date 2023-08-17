#ifndef AI_H
#define AI_H

#include "rules.h"
#include "bit_op.h"
#include <list>
#include <vector>
#include <assert.h>
#include <algorithm>

#define MAX_EVAL (126 * 256)
#define MIN_EVAL -(MAX_EVAL)

struct EvaluatedMove
{
    BoardBin move;
    int eval;
    EvaluatedMove(const BoardBin& m, int eval) : move(m), eval(eval){}
};

typedef std::vector<EvaluatedMove> EvaluatedMoves;

#if 0
inline int evaluate(const BoardBin& b)
{
    const int own_units = bitcount(b.own);
    if (own_units == 0)
        return MIN_EVAL;
    const int enemy_units = bitcount(b.enemy);
    if(enemy_units == 0)
        return MAX_EVAL;

    int eval = own_units - enemy_units;
    if(b.dam)
    {
        eval += bitcount(b.own & b.dam) << 1;
        eval -= bitcount(b.enemy & b.dam) << 1;
    }
    return eval;
}
#endif

inline int extended_evaluate(const BoardBin& b)
{
    const int own_units = bitcount(b.own);
    if (own_units == 0)
        return MIN_EVAL;
    const int enemy_units = bitcount(b.enemy);
    if(enemy_units == 0)
        return MAX_EVAL;

    int eval = own_units - enemy_units;
    if(b.dam)
    {
        eval += bitcount(b.own & b.dam) << 1;
        eval -= bitcount(b.enemy & b.dam) << 1;
    }
    eval <<= 8;

#if 1
    if (b.dam == 0)
    {
        const BoardBin ca = Rules::control_area(b);
        eval += bitcount(ca.own) - bitcount(ca.enemy);
    }
    else
    {
        //TODO
    }
#endif
    return eval;
}

inline EvaluatedMoves moves_with_eval(const EvaluatedMoves& moves, int eval)
{
    EvaluatedMoves result;
    for(EvaluatedMoves::const_iterator it = moves.begin(); it != moves.end(); ++it)
        if(eval == it->eval)
            result.push_back(*it);
    return result;
}

inline int negamax(const BoardBin& b, bool white, unsigned depth, int alpha, int beta)
{
    if(depth == 0)
    {
        //int eval = BoardStat(b, r.bg).evaluate();
        int eval = extended_evaluate(b);
        return white ? eval : -eval;
    }
    int result = MIN_EVAL;
    Rules::Moves moves;
    bool is_hit = Rules::move_list(b, moves);
    if (!is_hit)
        --depth;
    for(auto brd: moves)
    {
        int eval = negamax(brd, !white, depth, -beta, -alpha);
        result = std::max(result, -eval);
        alpha = std::max(result, alpha);
        if (alpha >= beta)
            break;
    }
    return result;
}


inline int build_game_tree(const BoardBin& b, bool white, unsigned depth, EvaluatedMoves& eval_moves)
{
    //TODO
    int result = MIN_EVAL;
    int alpha = MIN_EVAL;
    int beta = MAX_EVAL;
    Rules::Moves moves;
    bool is_hit = Rules::move_list(b, moves);
    if (!is_hit)
        --depth;
    if(moves.size() == 1)
    {
        eval_moves.push_back(EvaluatedMove(*moves.begin(), 0));
        return 0;
    }
    for(auto m: moves)
    {
        int eval = negamax(m, !white, depth, -beta, -alpha);
        result = std::max(result, -eval);
        //alpha = std::max(alpha, result);
        eval_moves.push_back(EvaluatedMove(m, -eval));
    }
    return result;
}


inline unsigned ply_policy_ex(unsigned units, unsigned dam_units)
{
    /*if (dam_units > 1)
        return 6;*/
    units += dam_units * 2;
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
#if RUSSIAN_DAM_RULES
    return ply_policy_ex(units, dam_units) + 2;
#else
    return ply_policy_ex(units, dam_units);
#endif
}

#endif // AI_H
