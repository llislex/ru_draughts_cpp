#ifndef PLAYER_H
#define PLAYER_H

#include <QThread>
#include <QMutex>
#include <QDebug>
#include "rules.h"
#include "board.h"
#include "ai.h"
#include "utils.h"

class MoveMaker: public QThread
{
    Q_OBJECT
public:
    Rules* m_r;
    bool white_turn;
    BoardBin m_b;
    Rules::Move best_move;
    bool valid_move;
    int eval;
    int depth;
    uint64_t took_time;

    void init(const BoardBin& b, bool is_white, Rules* r)
    {
        white_turn = is_white;
        m_b = b;
        m_r = r;
        valid_move = false;
        took_time = 0;
    }
    void run() override
    {
        TookTime tt;
        BoardStat bs(m_b, m_r->bg);
        unsigned ply = ply_policy(bs.own_units + bs.enemy_units, bs.own_dams + bs.enemy_dams);
        EvaluatedMoves moves;
        eval = build_game_tree(m_b, *m_r, white_turn, ply, moves);
        depth = ply;
        valid_move = moves.size() > 0;
        if(valid_move)
        {
            EvaluatedMoves bm = moves_with_eval(moves, eval);
            srand(time(0));
            unsigned choise = rand() % bm.size();
            best_move = bm[choise].move;
        }
        took_time = tt.took_time_ms();
    }
};

#endif // PLAYER_H
