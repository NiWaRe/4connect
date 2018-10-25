#include "game.h"
#include "logic_connection.h"
#include <iostream>

Game::Game(LogicConnection *logic_connection_ptr, GameParameters game_parameters)
    :_board(game_parameters.width,
            game_parameters.height),
      _color1(game_parameters.color1),
      _color2(game_parameters.color2),
      _nickname1(game_parameters.nick_name1),
      _nickname2(game_parameters.nick_name2),
      _turn(game_parameters.beginner)
{
    std::cout << "Building Game Class " << std::endl;

    // GUI -> LOGIC -> CONNECTION -> GAME
    connect(logic_connection_ptr, &LogicConnection::guiRecMove,             this, &Game::guiRecMove);

    // NETWORK -> LOGIC -> CONNECTION -> GAME
    connect(logic_connection_ptr, &LogicConnection::netRecMoveGame,         this, &Game::netRecMove);
    connect(logic_connection_ptr, &LogicConnection::netRecResponseOnMoveGame,   this, &Game::netRecResponseOnMove);

    // GAME -> CONNECTION -> LOGIC -> NETWORK
    connect(this, &Game::netSendMove,               logic_connection_ptr, &LogicConnection::netSendMove);
    connect(this, &Game::netSendResponseOnMove,     logic_connection_ptr, &LogicConnection::netSendResponseOnMove);

    // GAME -> CONNECTION -> LOGIC -> GUI
    connect(this, &Game::guiSetStone,           logic_connection_ptr, &LogicConnection::guiSetStone);
    connect(this, &Game::guiSet3Warning,        logic_connection_ptr, &LogicConnection::guiSet3Warning);

    // GAME -> CONNECTION
    connect(this, &Game::gameOver,              logic_connection_ptr, &LogicConnection::gameOver);

}


void Game::_changeTurn(){
    switch (_turn){
        case Player::Player1:
            _turn = Player::Player2;
            break;
        case Player::Player2:
            _turn = Player::Player1;
            break;
    }
    std::cout << "Changed turn" << std::endl;
}


void Game::guiRecMove(quint8 x){
    std::cout << "MOVE " << static_cast<int>(x) << " FROM GUI RECEIVED" << std::endl;
    if (_turn == Player::Player1){
        std::pair<qint16, qint16> result = _board.makeMove(x, _turn);
        if ((result.first >= 0) && (result.second >= 0)){
            std::cout << "MOVE " << static_cast<int>(x) << " FROM GUI ACCEPTED" << std::endl;

            emit netSendMove(x);
            emit guiSetStone(static_cast<quint8>(result.first),
                             static_cast<quint8>(result.second),
                             _color1,
                             QString::fromStdString(_nickname2));

            std::set<std::pair<quint8, quint8>> pairs_set = _board.check3Sets(Player::Player2);
            emit guiSet3Warning(pairs_set);


            bool win = _board.checkWinCondition(_turn);
            bool columns_full = _board.checkAllColumnsFull();
            if (win){
                emit gameOver(Result::win, Player::Player1);
            }
            else if (!win && columns_full){
                emit gameOver(Result::draw, Player::Player1);
            }
            else{
                _changeTurn();
            }

        }
    }
}


void Game::netRecMove(quint8 x){
    std::cout << "MOVE " << x << " FROM NET RECEIVED" << std::endl;
    if (_turn == Player::Player2){
        std::pair<qint16, qint16> result = _board.makeMove(x, _turn);
        if ((result.first == -1) && (result.second == -1)){
            emit netSendResponseOnMove(0x11);                                           // Außerhalb des Spielfeldes
        }
        else if ((result.first == -2) && (result.second == -2)){
            emit netSendResponseOnMove(0x12);                                           // Spalte ist voll
        }
        else {
            std::cout << "MOVE " << x << " FROM NET ACCEPTED" << std::endl;

            emit netSendResponseOnMove(0x00);  // OK
            emit guiSetStone(static_cast<quint8>(result.first),
                             static_cast<quint8>(result.second),
                             _color2,
                             QString::fromStdString(_nickname1));

            std::set<std::pair<quint8, quint8>> pairs_set = _board.check3Sets(Player::Player2);
            emit guiSet3Warning(pairs_set);

            bool loss = _board.checkWinCondition(_turn);
            bool columns_full = _board.checkAllColumnsFull();
            if (loss){
                emit gameOver(Result::loss, Player::Player2);
            }
            else if (!loss && columns_full){
                emit gameOver(Result::draw, Player::Player2);
            }
            else{
                _changeTurn();
            }
        }
    }
    else{
        emit netSendResponseOnMove(0x10);                                               // Nicht an der Reihe
    }
}


void Game::netRecResponseOnMove(quint8 status_code){
    // received a response on a move from GUI.
    switch (status_code){
        case 0x00:
            // TODO decide if do something and Send move to Gui
            // RETODO just send the move to gui from guiRecMove if it's right and don't wait for OK from net
            break;
        case 0x01:  // 4er, Spielende
            // Handled in LogicConnection::netRecResponseOnMoveConnection
            break;
        case 0x02:  // Unentschieden, Spielende
            // Handled in LogicConnection::netRecResponseOnMoveConnection
            break;
        case 0x10:  // Nicht an der Reihe
            // logic error by opponent
            break;
        case 0x11:  // Außerhalb des Spielfeldes
            // logic error by opponent
            break;
        case 0x12:  // Spalte ist voll
            // logic error by opponent
            break;
        case 0x13:  // kein laufendes Spiel
            // logic error by opponent
            break;
        case 0x20:  // Sonstiger Fehler
            // logic error by opponent
            break;
    }
}
