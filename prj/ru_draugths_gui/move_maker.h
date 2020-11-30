#ifndef PLAYER_H
#define PLAYER_H

#include <QThread>
#include <QMutex>
#include "rules.h"
#include "board.h"
#include "ai.h"

class MoveMaker: public QThread
{
    Q_OBJECT
public:
    Rules* m_r;
    bool white_turn;
    BoardBin m_b;
    Rules::Move best_move;
    bool valid_move;

    void init(const BoardBin& b, bool is_white, Rules* r)
    {
        white_turn = is_white;
        m_b = b;
        m_r = r;
        valid_move = false;
    }
    void run() override
    {
        BoardStat bs(m_b, m_r->bg);
        unsigned ply = ply_policy(bs.own_units + bs.enemy_units, bs.own_dams + bs.enemy_dams);
        EvaluatedMoves moves;
        int eval = build_game_tree(m_b, *m_r, white_turn, ply, moves);
        EvaluatedMoves bm = best_moves(moves, eval);
        valid_move = moves.size() > 0;
        if(valid_move)
        {
            unsigned choise = rand() % bm.size();
            best_move = bm[choise].move;
            valid_move = true;
        }
    }
};

#endif // PLAYER_H
