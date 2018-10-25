#include "logic.h"

Logic::Logic(){
}

Logic::~Logic(){
    if (_logic_connection){
        delete _logic_connection;
    }
}


void Logic::startConnectionServer(quint16 port, quint8 width, quint8 height, quint8 rounds_n, quint8 beginner,
                                  std::string nickname, QColor color){
    if (!_logic_connection){
        Player log_beginner;
        if (beginner == 2){
            srand(static_cast<uint>(time(nullptr)));
            beginner = rand() % 100;
            beginner = beginner % 2;
        }
        if (beginner == 0){
            log_beginner = Player::Player1;
        }
        else {
            log_beginner = Player::Player2;
        }

        QColor opponent_color;
        if (color == Qt::yellow){
            opponent_color = Qt::red;
        }
        else{
            opponent_color = Qt::yellow;
        }

        GameParameters game_parameters;
        game_parameters.width = width;
        game_parameters.height = height;
        game_parameters.rounds_n = rounds_n;
        game_parameters.beginner = log_beginner;
        game_parameters.score1 = 0;
        game_parameters.score2 = 0;
        game_parameters.current_round = 1;
        game_parameters.nick_name1 = nickname;
        game_parameters.nick_name2 = "Opponent";
        game_parameters.color1 = color;
        game_parameters.color2 = opponent_color;

        _logic_connection = new LogicConnection(this, game_parameters, CT::server);

        emit netCreateHost(port);
    }
}


void Logic::startConnectionClient(std::string host, quint16 port,
                                  std::string nickname, QColor color){
    if (!_logic_connection){

        QColor opponent_color;
        if (color == Qt::yellow){
            opponent_color = Qt::red;
        }
        else{
            opponent_color = Qt::yellow;
        }

        GameParameters game_parameters;
        game_parameters.score1 = 0;
        game_parameters.score2 = 0;
        game_parameters.current_round = 1;
        game_parameters.nick_name1 = nickname;
        game_parameters.nick_name2 = "Opponent";
        game_parameters.color1 = color;
        game_parameters.color2 = opponent_color;

        // Parameters are not full here. The first 4 should be received from server
        _logic_connection = new LogicConnection(this, game_parameters, CT::client);

        emit netConnectToHost(host, port);
    }
}


void Logic::disconnect(){
    if (_logic_connection){
        delete _logic_connection;
        emit netDisconnect();
    }
}
