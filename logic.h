#ifndef LOGIC_H
#define LOGIC_H
#include <QObject>
#include "logic_connection.h"

class Logic: public QObject
{
    Q_OBJECT
public:
    Logic();
    ~Logic();

private:
    LogicConnection *_logic_connection = nullptr;

public slots:

    // GUI -> LOGIC
    /**
     * @brief startConnectionServer Creates a objects in Logic and Network responsible for handling server connection
     * @param port      port for TCP connection
     * @param width     board width
     * @param height    board height
     * @param rounds_n  number of rounds to be played
     * @param beginner  0 for server, 1 for client, 2 for random
     * @param nickname  user's nickname
     * @param color     user's nickname stones color
     */
    void startConnectionServer(quint16 port, quint8 width, quint8 height, quint8 rounds_n, quint8 beginner,
                               std::string nickname, QColor color);
    /**
     * @brief startConnectionClient Creates a objects in Logic and Network responsible for handling client connection
     * @param host      Ip-adress of the server
     * @param port      Port of the server
     * @param nickname  user's nickname
     * @param color     user's nickname stones color
     */
    void startConnectionClient(std::string host, quint16 port,
                               std::string nickname, QColor color);
    /**
     * @brief disconnect Deletes Connection object in Logic and Network. Closes TCP connection
     */
    void disconnect();

signals:

    // LOGIC -> NETWORK
    void netCreateHost(quint16 port);
    void netConnectToHost(std::string host, quint16 port);
    void netDisconnect();

    // NETWORK -> LOGIC -> CONNECTION
    void netConnected();
    void netFailedToConnect(quint8 status_code);

    void netRecGameParameters(quint8 width, quint8 height, quint8 rounds_n, quint8 beginner);     // NUR CLIENT
    void netRecRequestStartRound(quint8 current_round, quint8 beginner_this_round);
    void netRecResponseOnRequest(quint8 status_code);
    void netRecNickName(QString nickname, QString color);

    // CONNECTION -> LOGIC -> NETWORK
    void netSendGameParameters(quint8 width, quint8 height, quint8 rounds_n, quint8 beginner);    // NUR SERVER
    void netSendRequestStartRound(quint8 current_round, quint8 beginner_this_round);
    void netSendResponseOnRequest(quint8 status_code);
    void netSendNickName(QString nickname, QString color);

    // CONNECTION -> LOGIC -> GUI
    void guiBuildGameField(quint8 width,
                           quint8 height,
                           quint8 rounds_n,
                           QColor color1,
                           QColor color2,
                           std::string nick_name1,
                           std::string nick_name2);
    void guiStartGame(quint8 current_round, QString player_name, quint8 score1, quint8 score2, quint8 round_n);
    void guiGameOver(Result result, bool more_rounds);
    void guiOpponentReadyToContinue();

    // GUI -> LOGIC -> CONNECTION
    void guiNewRound();

    // GUI -> LOGIC -> CONNECTION -> GAME
    void guiRecMove(quint8 x);

    // NETWORK -> LOGIC -> CONNECTION -> GAME
    void netRecMove(quint8 x);
    void netRecResponseOnMove(quint8 status_code);

    // GAME -> CONNECTION -> LOGIC -> NETWORK
    void netSendMove(quint8 x);
    void netSendResponseOnMove(quint8 status_code);

    // GAME -> CONNECTION -> LOGIC -> GUI
    void guiSetStone(quint8 x, quint8 y, QColor color, QString player_name);
    void guiSet3Warning(std::set<std::pair<quint8, quint8>> pairs_set);

};

#endif // LOGIC_H
