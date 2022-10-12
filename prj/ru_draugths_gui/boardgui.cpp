#include "boardgui.h"
#include <QPainter>

void GameUnitDelegate::paint(QPainter *painter, const QStyleOptionViewItem &opt, const QModelIndex &index) const
{
    if(index.data().canConvert<int>())
    {
        int ch = qvariant_cast<int>(index.data());
        bool selected = opt.state & QStyle::State_Selected;
        painter->fillRect(opt.rect, selected ? opt.palette.highlight() : opt.palette.window());
        switch(ch)
        {
        case 'x':
            painter->drawImage(opt.rect, b_unit);
            break;
        case 'X':
            painter->drawImage(opt.rect, b_dam);
            break;
        case 'o':
            painter->drawImage(opt.rect, w_unit);
            break;
        case 'O':
            painter->drawImage(opt.rect, w_dam);
            break;
        }
    }
    else
        QStyledItemDelegate::paint(painter, opt, index);
}

BoardGui::BoardGui(QWidget *parent) :
    QTableWidget(parent),
    N(0),
    unit_delegate(parent)
{
    this->setItemDelegate(&unit_delegate);
}

QTableWidgetItem* new_active_site()
{
    QTableWidgetItem* it = new QTableWidgetItem(QTableWidgetItem::UserType);
    it->setBackground(QBrush(QColor(255, 255, 255)));
    it->setForeground(QBrush(QColor(0,0,0)));
    return it;
}

QTableWidgetItem* passive_site()
{
    QTableWidgetItem* it = new QTableWidgetItem(QTableWidgetItem::UserType);
    it->setFlags(Qt::NoItemFlags);
    return it;
}

void BoardGui::init(int N)
{
    this->N = N;
    setRowCount(N);
    setColumnCount(N);

    for(int r = 0; r < N; ++r)
    {
        setColumnWidth(r, rowHeight(r));
        for(int c = 0; c < N; ++c)
        {
            QTableWidgetItem* it = this->acitve_site(r, c) ? new_active_site() : passive_site();
            this->setItem(r, c, it);
        }
    }
}

void BoardGui::resizeEvent(QResizeEvent* evt)
{
    if(N > 0)
    {
        int size = qMin(evt->size().width(), evt->size().height()) / N ;
        for (int i = 0; (unsigned) i < N; ++i)
        {
            setColumnWidth(i, size);
            setRowHeight(i, size);
        }
    }
    QTableWidget::resizeEvent(evt);
}

void BoardGui::setGameItem(unsigned index, char ch)
{
    QTableWidgetItem* it = itemByIndex(index);
    it->setTextAlignment(Qt::AlignCenter);
    if(it)
    {
        it->setText(QString(QChar(ch)));
        int i = ch;
        it->setData(0, QVariant(i));
    }
}

void BoardGui::setSelectable(unsigned index)
{
    QTableWidgetItem* d = itemByIndex(index);
    if(d)
    {
        d->setFlags(d->flags() & ~(Qt::NoItemFlags));
    }
}

void BoardGui::setNotSelectable(unsigned index)
{
    QTableWidgetItem* d = itemByIndex(index);
    if(d)
        d->setFlags(Qt::NoItemFlags);
}

QTableWidgetItem* BoardGui::itemByIndex(unsigned index) const
{
    if(index < max_index())
    {
        int row = index * 2 / (N);
        int col = index * 2 % (N) + (row + 1) % 2;
        return item(row, col);
    }
    return 0;
}

unsigned BoardGui::item_index(int r, int c) const
{
    if(acitve_site(r, c))
    {
        unsigned index = r * N / 2 + c / 2;
        assert(index < max_index());
        return index;
    }
    return max_index();
}
