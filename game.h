#ifndef GAME_H
#define GAME_H
#include <QObject>
#include "board.h"

class LogicConnection;


class Game: public QObject
{
    Q_OBJECT
public:
    Game(LogicConnection *logic_connection_ptr, GameParameters game_parameters);

private:
    /**
     * @brief _board Obj containing a 2D Matrix representing the game field and functions to set stones
     */
    Board _board;
    QColor _color1;         // Color of Player1
    QColor _color2;         // Color of Player2
    std::string _nickname1; // Nickname of Player1
    std::string _nickname2; // Nickname of Player1
    Player _turn;           // Player whose turn it is

    void _changeTurn();

public slots:

    // GUI -> LOGIC -> CONNECTION -> GAME
    /**
     * @brief guiRecMove    Receive a move from Gui and set a stone if the move is valid
     * @param x             x-coordinate
     */
    void guiRecMove(quint8 x);

    // NETWORK -> LOGIC -> CONNECTION -> GAME
    /**
     * @brief netRecMove    Receive a move from Network and set a stone if the move is valid
     * @param x             x-coordinate
     */
    void netRecMove(quint8 x);
    void netRecResponseOnMove(quint8 status_code);

signals:

    // GAME -> CONNECTION
    /**
     * @brief gameOver      send a Signal to LogicConnection if the game ended
     * @param result
     * @param ended_by
     */
    void gameOver(Result result, Player ended_by);

    // GAME -> CONNECTION -> LOGIC -> GUI
    void guiSetStone(quint8 x, quint8 y, QColor color, QString player_name);
    void guiSet3Warning(std::set<std::pair<quint8, quint8>> pairs_set);

    // GAME -> CONNECTION -> LOGIC -> NETWORK
    void netSendMove(quint8 x);
    void netSendResponseOnMove(quint8 status_code);

};

#endif // GAME_H
