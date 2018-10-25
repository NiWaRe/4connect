#include "base.h"
#include <set>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

Base::Base()
    :_logic(), _gui(), _network()
{
    // GUI -> LOGIC
    connect(&_gui, &Widget::startConnectionServer,      &_logic, &Logic::startConnectionServer);
    connect(&_gui, &Widget::startConnectionClient,      &_logic, &Logic::startConnectionClient);
    connect(&_gui, &Widget::disconnect,                 &_logic, &Logic::disconnect);

    // GUI to NETWORK
    connect(&_gui, &Widget::sendMsg,                    &_network, &Network::netSendChat);
    connect(&_gui, &Widget::startBroadcast,             &_network, &Network::netStartBroadcast);
    connect(&_gui, &Widget::endBroadcast,               &_network, &Network::netEndBroadcast);
    connect(&_gui, &Widget::disconnect,                 &_network, &Network::netDisconnect);

    // LOGIC -> NETWORK
    connect(&_logic, &Logic::netCreateHost,             &_network, &Network::netCreateHost);
    connect(&_logic, &Logic::netConnectToHost,          &_network, &Network::netConnectToHost);
    connect(&_logic, &Logic::netDisconnect,             &_network, &Network::netDisconnect);

    // NETWORK to GUI
    connect(&_network, &Network::netConnected,             &_gui, &Widget::netConnected);
    connect(&_network, &Network::netFailedToConnect,       &_gui, &Widget::netFailedToConnect);
    connect(&_network, &Network::netRecChat,               &_gui, &Widget::displayMsg);

    // NETWORK -> LOGIC -> CONNECTION
    connect(&_network, &Network::netConnected,             &_logic, &Logic::netConnected);
    connect(&_network, &Network::netFailedToConnect,       &_logic, &Logic::netFailedToConnect);

    connect(&_network, &Network::netRecGameParameters,     &_logic, &Logic::netRecGameParameters);
    connect(&_network, &Network::netRecRequestStartRound,  &_logic, &Logic::netRecRequestStartRound);
    connect(&_network, &Network::netRecResponseOnRequest,  &_logic, &Logic::netRecResponseOnRequest);
    connect(&_network, &Network::netRecNickName,           &_logic, &Logic::netRecNickName);


    // CONNECTION -> LOGIC -> NETWORK
    connect(&_logic, &Logic::netSendGameParameters,     &_network, &Network::netSendGameParameters);
    connect(&_logic, &Logic::netSendRequestStartRound,  &_network, &Network::netSendRequestStartRound);
    connect(&_logic, &Logic::netSendResponseOnRequest,  &_network, &Network::netSendResponseOnRequest);
    connect(&_logic, &Logic::netSendNickName,       &_network, &Network::netSendNickName);


    // CONNECTION -> LOGIC -> GUI
    connect(&_logic, &Logic::guiBuildGameField,             &_gui, &Widget::buildGameField);
    connect(&_logic, &Logic::guiStartGame,                  &_gui, &Widget::startGame);
    connect(&_logic, &Logic::guiGameOver,                   &_gui, &Widget::gameOver);
    connect(&_logic, &Logic::guiOpponentReadyToContinue,    &_gui, &Widget::opponentReadyToContinue);

    // GUI -> LOGIC -> CONNECTION
    connect(&_gui, &Widget::newRound,              &_logic, &Logic::guiNewRound);

    // GUI -> LOGIC -> CONNECTION -> GAME
    connect(&_gui, &Widget::makeMove,              &_logic, &Logic::guiRecMove);

    // NETWORK -> LOGIC -> CONNECTION -> GAME
    connect(&_network, &Network::netRecMove,           &_logic, &Logic::netRecMove);
    connect(&_network, &Network::netRecResponseOnMove, &_logic, &Logic::netRecResponseOnMove);

    // GAME -> CONNECTION -> LOGIC -> NETWORK
    connect(&_logic, &Logic::netSendMove,           &_network, &Network::netSendMove);
    connect(&_logic, &Logic::netSendResponseOnMove, &_network, &Network::netSendResponseOnMove);

    // GAME -> CONNECTION -> LOGIC -> GUI
    connect(&_logic, &Logic::guiSetStone,       &_gui, &Widget::setStone);
    connect(&_logic, &Logic::guiSet3Warning,    &_gui, &Widget::set3Warning);


    _gui.show();
//    _network.netStartBroadcast();
//    _gui.test1();
//    _gui.test2();


}






















