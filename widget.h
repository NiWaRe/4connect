#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <iostream>
#include <QColor>
#include "enums_structs.h"
#include "dialog.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void test1(){
        std::cout << "emiting test signal " << std::endl;

        emit startConnectionServer(4567, 7, 7, 3, 1, "Alex", Qt::cyan);
    }
    void test2(){
        std::cout << "emiting test signal " << std::endl;

        emit startConnectionClient("localhost", 4567, "Sherif", Qt::magenta);
    }


public slots:
    void rewrite();
   // void OpenGame(quint8 width, quint8 height, quint8 rounds_n);
    void serverinit();
    void clientinit();

    // Show board, but don't let player play yet.
    // Waiting for a message from the opponent that he is ready to start
    // Happens only once in a connection. After the Game parameters where exchanged
    // and we now how big the board must be
    void buildGameField(quint8 width,
                           quint8 height,
                           quint8 rounds_n,
                           QColor color1,
                           QColor color2,
                           std::string nick_name1,
                           std::string nick_name2);
    // The opponent is ready
    // activate the playing field if IBeginn is true, if not, wait for the move from the opponent
    //
    // IT WILL ALSO BE USED
    // after the previous round ended and the new one begins.
    // If so, clear the game field and then activate it
    void startGame(quint8 current_round, QString player_name, quint8 score1, quint8 score2, quint8 round_n);

    // if the game ended from the logic side
    void gameOver(Result result, bool more_rounds);            // win, loss, draw, disconnected

    void opponentReadyToContinue();

    // connection succesful. Show some stuff on the display
    void netConnected();
    // connection failed. Show some stuff on the display
    void netFailedToConnect(quint8 status_code);
    void displayMsg(QString msg);
    void chatconnection(QString msg);

signals:

    void tostartGameDIA(quint8 current_round, QString player_name, quint8 score1, quint8 score2, quint8 round_n);
    void togameOverDIA(Result result, bool more_rounds);
    void toopponentReadyToContinueDIA();
    void startConnectionServer(quint16 port, quint8 width, quint8 height, quint8 rounds_n, quint8 beginner,  // 0 - server, 1 - client, 2 - random
                               std::string nickname, QColor color);  // from enum_structs Qt::red, Qt::green etc
    void startConnectionClient(std::string host, quint16 port,
                               std::string nickname, QColor color);

    void disconnect();

    void startBroadcast();
    void endBroadcast();

    void makeMove(quint8 x);
    void setStone(quint8 x, quint8 y, QColor color, QString player_name);
    void set3Warning(std::set<std::pair<quint8, quint8>> pairs_set);
    void sendMsg(QString msg);
    // After the Game is finished, the 'GAME OVER' window should appear.
    // If gameOver slot received more_rounds==true, there should be a grey 'Continue' button and maybe a subtitle 'waiting for opponent'
    // Gui is then waiting for opponentReadyToContinue() signal. If it appears, the
    // 'Continue' button should become green or blue or whatever
    // If the user clicks the 'Continue' Button, this signal should be emitted
    void newRound();
    void chatMsg(QString msg);
    void infoGameField(QColor color1, QColor color2, std::string nick_name1, std::string nick_name2);
    //put the stone of selected color in certain position

private:
    Ui::Widget *ui;
    Dialog *game;
    int spalten = 7;
    int zeilen = 7;
    int round_n=3;

};

#endif // WIDGET_H
