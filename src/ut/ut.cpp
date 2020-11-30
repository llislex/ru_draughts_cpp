#include "ut.h"

#include <iostream>

#include "..\rules\board.h"
#include "..\rules\rules.h"
#include "..\utils\utils.h"
#include "..\ai\ai.h"


using namespace std;

//unit tests
#if 0
const char* dir_name[BoardGeometry::Directions] = { "left    up", "right   up", "left  down", "right down" };
for (unsigned n = 0; n < g.N; ++n)
{
    cout << n << endl;
    for (int d = 0; d < BoardGeometry::Directions; ++d)
    {
        unsigned near = g.near(n, (BoardGeometry::Direction) d);
        cout << "    " << dir_name[d] << " near " << near << endl;
    }
}
#endif

bool ut_to_text()
{
    const  char*  str =
        "  .  x  .  ."
        "o  .  .  .  "
        "  x  x  x  ."
        ".  .  .  .  "
        "  .  x  x  ."
        ".  o  .  .  "
        "  .  .  .  ."
        ".  .  .  .  "
        ;
    cout << "d 4x4" << endl;
    BoardGeometry g(8);
    cout << g.N << endl;
    BoardBin b = { 0x80, 0x1, 0x81 };
    BoardStat bs(b, g);
    cout << bs << endl;

    b = brd(string(str));
    cout << BoardStat(b, g) << endl;
    return true;
}

bool ut_rules_check()
{
    BoardGeometry g(8);
    Rules r(g);
#if 1
    cout << "way" << endl;
    for (unsigned i = 0; i < g.N; ++i)
    {
        BoardBin b = { 0, 0, 0 };
        b.own = 1 << i;
        b.enemy = r.ways[i];
        BoardStat bs(b, g);
        cout << i << endl << bs << endl;
    }

    cout << "way 2" << endl;
    for (unsigned i = 0; i < g.N; ++i)
    {
        BoardBin b = { 0, 0, 0 };
        b.own = 1 << i;
        b.enemy = r.ways2[i];
        BoardStat bs(b, g);
        cout << i << endl << bs << endl;
    }
#endif
    cout << "dam_ways" << endl;
    for (unsigned i = 0; i < g.N; ++i)
    {
        BoardBin b = { 0, 0, 0 };
        b.own = 1 << i;
        b.enemy = r.dam_ways[i];
        BoardStat bs(b, g);
        cout << i << endl << bs << endl;
    }
#if 1
    cout << "dam_way_dir" << endl;
    for (unsigned i = 0; i < g.N; ++i)
    {
        cout << i << endl;
        for (int d = 0; d < 4; ++d)
        {
            BoardBin b = { 0, 0, 0 };
            b.own = 1 << i;
            b.enemy = r.dam_way_dir[d][i];
            BoardStat bs(b, g);
            cout << d << endl << bs << endl << hex << b.enemy << endl;
        }
    }
#endif
    return true;
}

bool ut_hit_unit()
{
    const  char*  str =
        "  .  x  .  ."
        "o  .  .  .  "
        "  x  x  x  ."
        ".  .  .  .  "
        "  .  x  x  ."
        ".  o  .  .  "
        "  .  .  .  ."
        ".  .  .  .  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    std::vector<Rules::Move> lst;
    unsigned n = 4;
    cout << "hit n " << n << endl;
    r._hit(b.own & ~(1 << n), b.enemy, n, lst);
    cout << lst << endl << "done " << n << endl;

    return true;
}

bool ut_hit_dam()
{
   const  char*  str =
        "  .  x  .  ."
        "O  .  .  .  "
        "  x  X  x  ."
        ".  .  .  .  "
        "  .  x  x  ."
        ".  .  .  .  "
        "  .  x  .  ."
        ".  .  .  .  "
        ;

    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    std::vector<Rules::Move> lst;
    unsigned n = 4;
    cout << "hit n " << n << endl;
    r._hit_dam(b.own & ~(1 << n), b.enemy, n, lst);
    r._adjust_hit_list(lst, n, b.dam);
    cout << lst << endl << "done " << n << endl;

    return true;
}

bool ut_hit_dam_turkey()
{
    const  char*  str =
        "  .  .  .  ."
        ".  .  .  .  "
        "  .  .  .  ."
        ".  x  x  .  "
        "  .  x  .  O"
        ".  .  x  x  "
        "  .  x  .  ."
        ".  .  .  .  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    std::vector<Rules::Move> lst;
    unsigned n = 19;
    cout << "hit n " << n << endl;
    r._hit_dam(b.own & ~(1 << n), b.enemy, n, lst);
    cout << lst << endl << "done " << n << endl;

    bool result = lst.size() == 1 && lst[0].n == 18;
    return result;
}

bool ut_move_list_1()
{
    const  char*  str =
        "  .  .  .  ."
        ".  .  .  o  "
        "  .  .  .  ."
        ".  x  x  .  "
        "  .  x  .  O"
        ".  .  x  .  "
        "  .  x  .  ."
        "o  .  .  .  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    Rules::Moves moves;
    cout << __FUNCTION__<< endl;
    r.move_list(b, moves);
    cout << moves << endl;
    return true;
}

bool ut_move_list_2()
{
    const  char*  str =
        "  .  .  .  ."
        ".  .  .  o  "
        "  .  .  .  ."
        ".  x  x  .  "
        "  .  x  o  O"
        ".  .  x  .  "
        "  .  x  .  ."
        "o  .  O  .  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    Rules::Moves moves;
    cout << __FUNCTION__<< endl;
    r.move_list(b, moves);
    cout << moves << endl;
    return true;
}


bool ut_move_list_3()
{
    const  char*  str =
        "  .  .  .  ."
        ".  .  .  o  "
        "  .  .  .  ."
        ".  x  x  .  "
        "  .  x  .  O"
        ".  .  x  .  "
        "  .  x  .  ."
        "o  .  .  .  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    Rules::Moves moves;
    cout << __FUNCTION__<< endl;
    r.move_list_enemy(b, moves);
    cout << moves << endl;
    return true;
}

bool ut_move_list_4()
{
    const  char*  str =
        "  .  .  .  ."
        ".  .  .  o  "
        "  .  .  .  ."
        ".  x  x  .  "
        "  .  x  o  O"
        ".  .  x  .  "
        "  .  x  .  ."
        "o  .  O  .  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    Rules::Moves moves;
    cout << __FUNCTION__<< endl;
    r.move_list_enemy(b, moves);
    cout << moves << endl;
    return true;
}

bool ut_move_list_5()
{
    const  char*  str =
            " x x x x"
            "x x x x "
            " x . . x"
            "o . . x "
            " . . . ."
            ". * o o "
            " o o o o"
            "o o o o "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    Rules::Moves moves;
    cout << __FUNCTION__<< endl;
    r.move_list_enemy(b, moves);
    cout << moves << endl;
    return true;
}

bool ut_move_list_6()
{
    const  char*  str =
            " . . . ."
            ". x . x "
            " . . . ."
            "o . . . "
            " . . x ."
            ". o . . "
            " . . . o"
            ". . X o "
        ;
    const  char* expected =
            " . . . ."
            ". x . x "
            " . . . ."
            "o . . . "
            " X . x ."
            ". . . . "
            " . . . o"
            ". . * o "
            ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    Rules::Moves moves;
    cout << __FUNCTION__<< endl;
    r.move_list_enemy(b, moves);
    cout << moves << endl;
    bool result = moves.size() == 1 && moves[0].n == 16 && moves[0].n0 == 30;
    if(!result)
        cout << "expected:" << endl << expected << endl;
    return result;
}


bool ut_dumb_play()
{
    const  char*  str =
        "  x  x  x  x"
        "x  x  x  x  "
        "  x  x  x  x"
        ".  .  .  .  "
        "  .  .  .  ."
        "o  o  o  o  "
        "  o  o  o  o"
        "o  o  o  o  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, g);
    cout << bs << endl;

    int i = 0;
    while(true)
    {
        Rules::Moves m;
        bool white = (i % 2) == 0;
        white ? r.move_list(b, m) : r.move_list_enemy(b, m);
        if(m.size() == 0)
        {
            cout << (white ? "black win" : "white win") << endl;
            break;
        }
        int random_choise = rand() % m.size();
        ++i;
        cout << i << ". " << m[random_choise] << endl;
        b = m[random_choise].b;
    }
    return true;
}

#if 0
bool ut_ai_test()
{
    const  char*  str =
        "  x  x  x  x"
        "x  x  x  x  "
        "  x  x  x  x"
        ".  .  .  .  "
        "  .  .  .  ."
        "o  o  o  o  "
        "  o  o  o  o"
        "o  o  o  o  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    GameTree tree;
    TookTime t;
    build_game_tree(tree, 0, b, r, true, 6);
    uint64_t t0 = t.took_time_ms();

    cout << "tree size " << tree.size() << endl;
    cout << "took time " << t0 << " ms" <<endl;
    cout << "all took time " << t.took_time_ms() << " ms" <<endl;
    return true;
}

bool ut_ai_test_2()
{
    const  char*  str =
        "  .  x  .  ."
        ".  .  .  .  "
        "  .  .  .  ."
        ".  .  x  .  "
        "  .  x  .  o"
        ".  .  x  o  "
        "  .  .  .  ."
        ".  o  .  o  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    GameTree tree;
    Node n = {0, 0, 0, 0};
    tree.push_back(n);
    TookTime t;
    build_game_tree(tree, 0, b, r, true, 6);
    uint64_t t0 = t.took_time_ms();

    int eval = maxi(tree, 0);
    std::vector<unsigned> children;
    get_children(tree, 0, children);
    uint64_t t1 = t.took_time_ms();
    for(std::vector<unsigned>::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        cout << tree[*it] << (eval == tree[*it].weight ?  " *" : " ") << endl;
    }
    cout << eval << endl;
    cout << "tree size " << tree.size() << endl;
    cout << "build tree: " << t0 << " ms minimax: " << t1 - t0 << " ms  all:" << t1 << endl;
    //cout << tree << endl;

    return true;
}

bool ut_ai_play()
{
    const  char*  str =
        "  x  x  x  x"
        "x  x  x  x  "
        "  x  x  x  x"
        ".  .  .  .  "
        "  .  .  .  ."
        "o  o  o  o  "
        "  o  o  o  o"
        "o  o  o  o  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);
    bool white_turn = true;

    for (int i = 0; i < 100; ++i)
    {
        BoardStat bs(b, r.bg);
        cout << bs << endl;

        GameTree tree;
        Node n = {0, 0, 0, 0};
        tree.push_back(n);
        build_game_tree(tree, 0, b, r, white_turn, 8);
        int eval = white_turn ? maxi(tree, 0) : mini(tree, 0);
        std::vector<unsigned> moves = get_best_moves(tree, eval);
        if (moves.size() == 0)
        {
            cout << (white_turn ? "Black win" : "White win") << endl;
            break;
        }
        unsigned choise = moves[rand() % moves.size()];
        b = r.apply_move(b, white_turn, tree[choise].n0, tree[choise].n1);
        white_turn = !white_turn;
    }
    return true;
}
#endif

bool ut_ai_test()
{
#if 1
    const  char*  str =
            " x . x ."
            "x x x x "
            " x . . x"
            "o x o . "
            " . . . o"
            "o . . . "
            " o o . o"
            "o . . o "
        ;
#else
    const  char*  str =
        "  .  .  .  ."
        ".  .  .  .  "
        "  .  .  .  x"
        ".  .  .  .  "
        "  x  .  o  ."
        ".  .  .  .  "
        "  .  .  .  ."
        "o  .  .  .  "
        ;
#endif
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);

    BoardStat bs(b, r.bg);
    cout << bs << endl;

    EvaluatedMoves moves;
    TookTime t;
    int eval = build_game_tree(b, r, false, 6, moves);
    uint64_t t0 = t.took_time_ms();

    //cout << "tree size " << tree.size() << endl;
    cout << "took time " << t0 << " ms" <<endl;
    cout << "all took time " << t.took_time_ms() << " ms" <<endl;
    //    cout << tree << endl;
    cout << "eval " << eval << endl;
    cout << moves << endl;

    cout << endl << "****  Best moves **** " << endl << endl;
    cout << best_moves(moves, eval) << endl;

    // cout << "eval " << eval << endl;
    return true;
}

bool ut_ai_play()
{
    const  char*  str =
        "  x  x  x  x"
        "x  x  x  x  "
        "  x  x  x  x"
        ".  .  .  .  "
        "  .  .  .  ."
        "o  o  o  o  "
        "  o  o  o  o"
        "o  o  o  o  "
        ;
    BoardBin b = brd(string(str));
    BoardGeometry g(8);
    Rules r(g);
    bool white_turn = true;

    unsigned ply = 8;

    for (int i = 0; i < 100; ++i)
    {
        //BoardStat bs(b, r.bg);
        //cout << bs << endl;

        TookTime t;
        EvaluatedMoves moves;
        int eval = build_game_tree(b, r, white_turn, ply, moves);
        EvaluatedMoves bm = best_moves(moves, eval);
        if (bm.size() == 0)
        {
            cout << (white_turn ? "Black win" : "White win") << endl;
            break;
        }
        unsigned choise = rand() % bm.size();
        uint64_t t0 = t.took_time_ms();
        b = bm[choise].move.b;
        white_turn = !white_turn;
        BoardStat bs(b, r.bg);
        cout << bm[choise] <<  t0 << " ms" << " ply " << ply << endl;
        ply = ply_policy(bs.own_units + bs.enemy_units, bs.own_dams + bs.enemy_dams);
    }
    return true;
}
