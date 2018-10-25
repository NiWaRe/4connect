#ifndef BOARD_H
#define BOARD_H
#include <QObject>
#include <set>
#include "enums_structs.h"

using int2d = std::vector<std::vector<int>>;

class Board: public QObject
{
    Q_OBJECT
public:
    Board(quint8 width, quint8 height);

    std::pair<qint16, qint16> makeMove(quint8 x, Player player);

    int  getStone(quint8 x, quint8 y);
    void setStone(quint8 x, quint8 y, Player player);
    void removeStone(quint8 x, quint8 y);

    quint8  getLowestRow(quint8 x);
    bool    checkValidity(quint8 column);
    bool    checkWinCondition(Player player);
    bool    checkAllColumnsFull();
    int     getHistory(quint8 row, quint8 col);
    std::set<std::pair<quint8, quint8>> check3Sets(Player player);

private:
    const quint8  _width;
    const quint8  _height;
    int   _stones_count;
    int2d _Matrix;
};

#endif // BOARD_H
