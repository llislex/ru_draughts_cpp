#include "boardgui.h"


BoardGui::BoardGui(QWidget *parent) : QTableWidget(parent), N(0)
{
}


QTableWidgetItem* new_active_site()
{
    QTableWidgetItem* it = new QTableWidgetItem(QTableWidgetItem::UserType);
    it->setBackground(QBrush(QColor(0,0,0)));
    //it->setTextColor(QColor(255,255,255));
    it->setForeground(QBrush(QColor(255, 255, 255)));
    it->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
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
            if(this->acitve_site(r, c))
            {
                QTableWidgetItem* it = new_active_site();
                this->setItem(r, c, it);
            }
            else
            {
                QTableWidgetItem* it = passive_site();
                this->setItem(r, c, it);
            }
        }
    }
}

int font_resize(int size, const QFont& base_font)
{
    for(int fontSize = 1; true; ++fontSize)
    {
        QFont f(base_font);
        f.setPixelSize( fontSize );
        QRect r = QFontMetrics(f).boundingRect("X");
        if (r.width() >= size || r.height() >= size)
            return fontSize - 1;
    }
    return base_font.pixelSize();
}

void BoardGui::resizeEvent(QResizeEvent* evt)
{
    if(N > 0)
    {
        int size = qMin(evt->size().width(), evt->size().height()) / N ;
        for (int i = 0; (unsigned) i < N; ++i)
        {
            /*int fsz = font_resize(size / 2, font());
            QFont f(font());
            f.setPixelSize(fsz);
            setFont(f);*/

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
    if(it) it->setText(QString(QChar(ch)));
}

void BoardGui::setGameItem(unsigned index, const QIcon& img)
{
    QTableWidgetItem* it = itemByIndex(index);
    if(it) it->setIcon(img);
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
