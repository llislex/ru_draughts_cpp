#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>

#include "board.h"
#include "rules.h"

#include "move_maker.h"


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
protected:
    typedef enum {Human, AI} PlayerType;

    BoardBin b;
    Rules* r;
    BoardGeometry* bg;
    bool white_move;
    Rules::Moves moves;
    bool is_hit;

    MoveMaker* move_maker;

    PlayerType white_player;
    PlayerType black_player;
    bool is_human_move() const {return white_move ? white_player == Human : black_player == Human;}
    unsigned moves_number_with(unsigned i) const;
    Rules::Moves::const_iterator first_move_with(unsigned i) const;
    Rules::Moves::const_iterator is_end_move(unsigned i, int start_index) const;
    int start_move_index;

    void apply_move(const Rules::Move& m, int eval = 0, int depth = 0, uint64_t took_time = 0);
    void set_board(const BoardBin& bx);
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:


private slots:
    void on_tw_cellPressed(int row, int column);
    void start_new_game();
    void ai_move();
signals:
    void request_ai_move(const BoardBin& b, const Rules& r, bool is_white);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
