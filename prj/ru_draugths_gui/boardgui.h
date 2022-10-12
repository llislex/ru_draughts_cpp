#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <QTableWidget>
#include <QResizeEvent>
#include <QStyledItemDelegate>

class GameUnitDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    GameUnitDelegate(QWidget* parent = 0) : QStyledItemDelegate(parent),
        w_dam(":/images/w_dam.png"),
        w_unit(":/images/w_d.png"),
        b_dam(":/images/b_dam.png"),
        b_unit(":/images/b_d.png") {}
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
protected:
    QImage w_dam;
    QImage w_unit;
    QImage b_dam;
    QImage b_unit;
};

class BoardGui : public QTableWidget
{
    Q_OBJECT
protected:
    unsigned N;
    bool acitve_site(int r, int c) const {return ((r + c) % 2) != 0;}
    void resizeEvent(QResizeEvent *event);
public:
    GameUnitDelegate unit_delegate;

    QTableWidgetItem* itemByIndex(unsigned index) const;
    explicit BoardGui(QWidget *parent = 0);
    void init(int N = 8);
    void setGameItem(unsigned index, char ch);
    void setSelectable(unsigned index);
    void setNotSelectable(unsigned index);
    unsigned item_index(int r, int c) const;
    unsigned max_index() const {return N * N / 2;}
signals:

public slots:

};

#endif // BOARDGUI_H
