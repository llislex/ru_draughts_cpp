#ifndef BOARDGUI_H
#define BOARDGUI_H

#include <QTableWidget>
#include <QResizeEvent>

class BoardGui : public QTableWidget
{
    Q_OBJECT
protected:
    unsigned N;
    bool acitve_site(int r, int c) const {return ((r + c) % 2) != 0;}
    void resizeEvent(QResizeEvent *event);
public:
    QTableWidgetItem* itemByIndex(unsigned index) const;
    explicit BoardGui(QWidget *parent = 0);
    void init(int N = 8);
    void setGameItem(unsigned index, char ch);
    void setGameItem(unsigned index, const QIcon& img);
    void setSelectable(unsigned index);
    void setNotSelectable(unsigned index);
    unsigned item_index(int r, int c) const;
    unsigned max_index() const {return N * N / 2;}
signals:

public slots:

};

#endif // BOARDGUI_H
