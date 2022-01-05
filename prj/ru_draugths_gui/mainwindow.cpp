#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include <QIcon>
#include <string>
#include <iostream>
#include <sstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    const unsigned size = 8;
    ui->setupUi(this);
    ui->tw->init(size);
    move_maker = 0;
    bg = new BoardGeometry(size);
    r = new Rules(*bg);
    ui->actionBlack_AI->setChecked(true);
    start_new_game();
    connect(ui->actionNew_Game, SIGNAL(triggered()), this, SLOT(start_new_game()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete r;
    delete bg;
}

void MainWindow::on_tw_cellPressed(int row, int column)
{
    if(is_human_move())
    {
        unsigned i = ui->tw->item_index(row, column);
        //setWindowTitle(QString::number(i));
        unsigned num = moves_number_with(i);
        if (num > 0)
        {
            if((unsigned)start_move_index == i)
            {
                ui->tw->item(row, column)->setSelected(false);
            }
            else
            {
                if (start_move_index != -1)
                {
                    ui->tw->itemByIndex(start_move_index)->setSelected(false);
                }
                start_move_index = i;
                if(num == 1)
                {
                    apply_move(*first_move_with(i));
                }
                else
                {
                    ui->tw->itemByIndex(start_move_index)->setSelected(true);
                }
            }
        }
        else
        {
            Rules::Moves::const_iterator it = is_end_move(i, start_move_index);
            if (it != moves.end())
            {
                if (start_move_index != -1)
                {
                    ui->tw->itemByIndex(start_move_index)->setSelected(false);
                }
                apply_move(*it);
            }
        }
    }
}

void MainWindow::apply_move(const Rules::Move& m, int eval, int depth)
{
    QString title = QString(white_move ? is_hit ? "%1 x %2" : "%1 - %2" : is_hit ? ".. %1 x %2" : ".. %1 - %2").arg(m.n0).arg(m.n);
    title += QString("  (eval %1 depth %2)").arg(eval).arg(depth);
    setWindowTitle(title);
    start_move_index = -1;
    white_move = !white_move;
    set_board(m.b);
    moves.clear();
    is_hit = white_move ? r->move_list(b, moves) : r->move_list_enemy(b, moves);

    if(!is_human_move())
    {
        move_maker = new MoveMaker();
        connect(move_maker, &QThread::finished, this, &MainWindow::ai_move);
        move_maker->init(b, white_move, r);
        move_maker->start();
    }
}

void MainWindow::set_board(const BoardBin& bx)
{
    b = bx;
    for(unsigned i = 0; i < r->bg.N; ++i)
    {
        const unsigned mask = 1 << i;
        char c = b.own & mask ? b.dam & mask ? 'O' : 'o' : b.enemy & mask ? b.dam & mask ? 'X' : 'x' : ' ';
        ui->tw->setGameItem(i, c);
    }
}

unsigned MainWindow::moves_number_with(unsigned i) const
{
    unsigned result = 0;
    for(Rules::Moves::const_iterator it = moves.begin(); it != moves.end(); ++it)
        if(it->n0 == i)
            ++result;
    return result;
}

Rules::Moves::const_iterator MainWindow::first_move_with(unsigned i) const
{
    for(Rules::Moves::const_iterator it = moves.begin(); it != moves.end(); ++it)
        if(it->n0 == i)
            return it;
    return moves.end();
}

Rules::Moves::const_iterator MainWindow::is_end_move(unsigned i, int start_index) const
{
    for(Rules::Moves::const_iterator it = moves.begin(); it != moves.end(); ++it)
        if(it->n == i && it->n0 == (unsigned) start_index)
            return it;
    return moves.end();
}

void MainWindow::start_new_game()
{
    white_move = true;
    white_player = ui->actionWhite_AI->isChecked() ? AI : Human;
    black_player = ui->actionBlack_AI->isChecked() ? AI : Human;
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
    set_board(brd(std::string(str)));
    moves.clear();

    is_hit = white_move ? r->move_list(b, moves) : r->move_list_enemy(b, moves);
    start_move_index = -1;

    if(!is_human_move())
    {
        move_maker = new MoveMaker();
        connect(move_maker, &QThread::finished, this, &MainWindow::ai_move);
        move_maker->init(b, white_move, r);
        move_maker->start();
    }
}

void MainWindow::ai_move()
{
    if(move_maker)
    {
        bool valid = move_maker->valid_move;
        Rules::Move m = move_maker->best_move;
        int depth = move_maker->depth;
        int eval = move_maker->eval;
        disconnect(move_maker, &QThread::finished, this, &MainWindow::ai_move);
        delete move_maker;
        move_maker = 0;

        if(valid)
        {
            apply_move(m, eval, depth);
        }
    }
}

