#ifndef LOGICCONNECTION_H
#define LOGICCONNECTION_H
#include <QObject>
#include <iostream>
#include <QColor>
#include "game.h"

class Logic;

/**
 * @brief The LogicConnection class is responsible for managing everything inside a single connection between 2 players
 */
class LogicConnection: public QObject
{
    Q_OBJECT

private:
    Game *_game = nullptr;
    GameParameters _game_parameters;
    const CT _connection_type;
    CV _connection_var;

    void _changeBeginner();

public:
    LogicConnection(Logic *logic_ptr, GameParameters game_parameters, CT connection_type);
    ~LogicConnection();

    void createGameObj();
    void deleteGameObj();

    /**
     * @brief buildGameField making necessary preparations to build gamefield and sending s signal to Gui
     */
    void buildGameField();
    /**
     * @brief startGame creating a game object and giving
     */
    void startGame();

    GameParameters  getGameParameters() { return _game_parameters;}
    CT  getConnectionType() { return _connection_type;}
    CV   getConnectionVar()  { return _connection_var;}

    void setGameParameters(GameParameters new_value){ _game_parameters = new_value;}
    void setGameParameters(quint8 width, quint8 height, quint8 rounds_n, Player beginner);
    void setConnectionVar (CV  new_value){ _connection_var  = new_value;}

    // Converting enum classes to int or QString
    quint8 beginnerLog2Net(Player log_beginner);
    Player beginnerNet2Log(quint8 net_beginner);
    QString colorLog2Str(QColor color_log);
    QColor  colorStr2Log(QString color_str);



public slots:

    // GAME -> CONNECTION
    /**
     * @brief gameOver  signal coming from Game meaning the game is over
     * @param result    win, loss, draw, disconnected
     * @param ended_by  whose move ended the game
     */
    void gameOver(Result result, Player ended_by);

    // GUI -> LOGIC -> CONNECTION
    void guiNewRound();                                                             //

    // NETWORK -> LOGIC -> CONNECTION
    void netConnected();                                                            //
    void netFailedToConnect(quint8 status_code);                                       // X

    void netRecGameParameters(quint8 width, quint8 height, quint8 rounds_n, quint8 beginner);   // X NUR CLIENT
    void netRecRequestStartRound(quint8 current_round, quint8 beginner_this_round);       // X
    void netRecResponseOnRequest(quint8 status_code);                                  // X
    void netRecNickName(QString nickname, QString color);                                       // X

    void netRecMoveConnection(quint8 x);
    void netRecResponseOnMoveConnection(quint8 status_code);


signals:

    // CONNECTION -> LOGIC -> NETWORK
    void netSendGameParameters(quint8 width, quint8 height, quint8 rounds_n, quint8 beginner);  // X NUR SERVER
    void netSendRequestStartRound(quint8 current_round, quint8 beginner_this_round);      // X
    void netSendResponseOnRequest(quint8 status_code);                                 // X
    void netSendNickName(QString nickname, QString color);                                      // X

    // CONNECTION -> LOGIC -> GUI
    void guiBuildGameField(quint8 width,
                           quint8 height,
                           quint8 rounds_n,
                           QColor color1,
                           QColor color2,
                           std::string nick_name1,
                           std::string nick_name2);
    void guiStartGame(quint8 current_round, QString player_name, quint8 score1, quint8 score2, quint8 round_n);                             // X
    void guiGameOver(Result result, bool more_rounds);                              // X
    void guiOpponentReadyToContinue();                                              //

    // GUI -> LOGIC -> CONNECTION -> GAME
    void guiRecMove(quint8 x);

    // NETWORK -> LOGIC -> CONNECTION -> GAME
    void netRecMoveGame(quint8 x);
    void netRecResponseOnMoveGame(quint8 status_code);

    // GAME -> CONNECTION -> LOGIC -> NETWORK
    void netSendMove(quint8 x);
    void netSendResponseOnMove(quint8 status_code);

    // GAME -> CONNECTION -> LOGIC -> GUI
    void guiSetStone(quint8 x, quint8 y, QColor color, QString player_name);
    void guiSet3Warning(std::set<std::pair<quint8, quint8>> pairs_set);

};

#endif // LOGICCONNECTION_H
