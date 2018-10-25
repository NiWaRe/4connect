#ifndef GAMEPARAMETERS_H
#define GAMEPARAMETERS_H
#include <QtGlobal>
#include <QColor>
#include <string>

enum class Result{
    win,
    loss,
    draw,
    disconnected,
};

enum class Player {
    Player1,  // Me
    Player2,  // Opponent
};


// Connection Type
enum class CT{
    server,
    client
};


// Connection Variable
enum class CV{
    init,
    connected,
    nickname_sent,
    nickname_exchanged,
    game_parameters_sent,
    game_parameters_exchanged,
    gamefield_built,            // game_created
    start_request_sent,
    start_request_accepted,
    game_started,
    game_ended_by_me,
    game_ended_by_op,
    game_end_accepted           // accepted by opponent
};


struct GameParameters{
    quint8 width;
    quint8 height;
    quint8 rounds_n;
    Player beginner;
    quint8 current_round;
    quint8 score1;
    quint8 score2;
    std::string nick_name1;
    std::string nick_name2;
    QColor color1;
    QColor color2;
};


struct Set3{
    quint8 x1;
    quint8 y1;
    quint8 x2;
    quint8 y2;
    quint8 x3;
    quint8 y3;
};

#endif // GAMEPARAMETERS_H
