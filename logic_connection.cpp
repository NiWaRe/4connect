#include "logic_connection.h"
#include "logic.h"


LogicConnection::LogicConnection(Logic *logic_ptr, GameParameters game_parameters, CT connection_type)
    :_game_parameters(game_parameters), _connection_type(connection_type), _connection_var(CV::init)
{
    std::cout << "Building LogicConnetction " << (connection_type==CT::server ? "server" : "client") << std::endl;    

    // NETWORK -> LOGIC -> CONNECTION
    connect(logic_ptr, &Logic::netConnected,             this, &LogicConnection::netConnected);
    connect(logic_ptr, &Logic::netFailedToConnect,       this, &LogicConnection::netFailedToConnect);

    connect(logic_ptr, &Logic::netRecRequestStartRound,  this, &LogicConnection::netRecRequestStartRound);
    connect(logic_ptr, &Logic::netRecResponseOnRequest,  this, &LogicConnection::netRecResponseOnRequest);
    connect(logic_ptr, &Logic::netRecNickName,       this, &LogicConnection::netRecNickName);

    // CONNECTION -> LOGIC -> NETWORK
    connect(this, &LogicConnection::netSendRequestStartRound,   logic_ptr, &Logic::netSendRequestStartRound);
    connect(this, &LogicConnection::netSendResponseOnRequest,   logic_ptr, &Logic::netSendResponseOnRequest);
    connect(this, &LogicConnection::netSendNickName,        logic_ptr, &Logic::netSendNickName);

    // CONNECTION -> LOGIC -> GUI
    connect(this, &LogicConnection::guiBuildGameField,          logic_ptr, &Logic::guiBuildGameField);
    connect(this, &LogicConnection::guiStartGame,               logic_ptr, &Logic::guiStartGame);
    connect(this, &LogicConnection::guiGameOver,                logic_ptr, &Logic::guiGameOver);
    connect(this, &LogicConnection::guiOpponentReadyToContinue, logic_ptr, &Logic::guiOpponentReadyToContinue);

    // GUI -> LOGIC -> CONNECTION
    connect(logic_ptr, &Logic::guiNewRound,         this, &LogicConnection::guiNewRound);

    // GUI -> LOGIC -> CONNECTION -> GAME
    connect(logic_ptr, &Logic::guiRecMove,          this, &LogicConnection::guiRecMove);

    // NETWORK -> LOGIC -> CONNECTION -> GAME
    connect(logic_ptr, &Logic::netRecMove,              this, &LogicConnection::netRecMoveConnection);
    connect(logic_ptr, &Logic::netRecResponseOnMove,    this, &LogicConnection::netRecResponseOnMoveConnection);

    // GAME -> CONNECTION -> LOGIC -> NETWORK
    connect(this, &LogicConnection::netSendMove,            logic_ptr, &Logic::netSendMove);
    connect(this, &LogicConnection::netSendResponseOnMove,  logic_ptr, &Logic::netSendResponseOnMove);

    // GAME -> CONNECTION -> LOGIC -> GUI
    connect(this, &LogicConnection::guiSetStone,            logic_ptr, &Logic::guiSetStone);
    connect(this, &LogicConnection::guiSet3Warning,         logic_ptr, &Logic::guiSet3Warning);



    if (connection_type == CT::server){
        connect(this, &LogicConnection::netSendGameParameters, logic_ptr, &Logic::netSendGameParameters);

    }
    else if (connection_type == CT::client){
        connect(logic_ptr, &Logic::netRecGameParameters,   this, &LogicConnection::netRecGameParameters);
    }

}


void LogicConnection::_changeBeginner(){
    switch (_game_parameters.beginner){
        case Player::Player1:
            _game_parameters.beginner = Player::Player2;
            break;
        case Player::Player2:
            _game_parameters.beginner = Player::Player1;
            break;
    }
    std::cout << "Changed beginner" << std::endl;
}


LogicConnection::~LogicConnection(){
    if (_game){
        delete _game;
    }
}


void LogicConnection::createGameObj(){
    if (!_game){
        _game = new Game(this, _game_parameters);
    }
}


void LogicConnection::deleteGameObj(){
    if (_game){
        delete _game;
    }
}


void LogicConnection::buildGameField(){
    // call Logic functions to build the game.
    // For now, it is not needed as the game will start _logically_ only after opponent verifies game start


    std::cout << "BUILDING GAME FIELD" << std::endl;
    setConnectionVar(CV::gamefield_built);
    emit guiBuildGameField(_game_parameters.width,
                           _game_parameters.height,
                           _game_parameters.rounds_n,
                           _game_parameters.color1,
                           _game_parameters.color2,
                           _game_parameters.nick_name1,
                           _game_parameters.nick_name2);
    if (_connection_type == CT::server){
        emit netSendRequestStartRound(_game_parameters.current_round,
                                      beginnerLog2Net(_game_parameters.beginner));

        std::cout << "START REQUEST SENT" << std::endl;
        setConnectionVar(CV::start_request_sent);
    }

}


void LogicConnection::startGame(){
    // TODO logically startGame();

    QString a = colorLog2Str(_game_parameters.color1);
    QString b = colorLog2Str(_game_parameters.color2);

    std::cout << "color1 = " << a.toStdString() << std::endl;
    std::cout << "color2 = " << b.toStdString() << std::endl;

    std::cout << "nickname1 = " << _game_parameters.nick_name1 << std::endl;

    std::cout << "nickname2 = " << _game_parameters.nick_name2 << std::endl;

    std::cout << "current_round = " << _game_parameters.current_round << std::endl;

    createGameObj();
    _connection_var = CV::game_started;

    QString starting_player;
    if (_game_parameters.beginner == Player::Player1){
        starting_player = QString::fromStdString(_game_parameters.nick_name1);
    }
    else{
        starting_player = QString::fromStdString(_game_parameters.nick_name2);
    }

    std::cout << "GAME STARTED" << std::endl;
    emit guiStartGame(_game_parameters.current_round, starting_player,
                      _game_parameters.score1,
                      _game_parameters.score2,
                      _game_parameters.rounds_n);
}


void LogicConnection::setGameParameters(quint8 width, quint8 height, quint8 rounds_n, Player beginner){
    std::cout << "Setting game parameters" << std::endl
              << "width: " << width << std::endl
              << "height: " << height << std::endl
              << "rounds_n: " << rounds_n << std::endl;
    _game_parameters.width = width;
    _game_parameters.height = height;
    _game_parameters.rounds_n = rounds_n;
    _game_parameters.beginner = beginner;
}


quint8 LogicConnection::beginnerLog2Net(Player log_beginner){
    quint8 net_beginner;
    switch (_connection_type)
    {
        case CT::server:
            switch (log_beginner) {
                case Player::Player1:
                    net_beginner = 0;
                    break;
                case Player::Player2:
                    net_beginner = 1;
                    break;
            } break;
        case CT::client:
            switch (log_beginner) {
                case Player::Player1:
                    net_beginner = 1;
                    break;
                case Player::Player2:
                    net_beginner = 0;
                    break;
            } break;
    }
    return net_beginner;
}


Player LogicConnection::beginnerNet2Log(quint8 net_beginner){
    Player log_beginner;
    switch (_connection_type)
    {
        case CT::server:
            switch (net_beginner) {
                case 0:
                    log_beginner = Player::Player1;
                    break;
                case 1:
                    log_beginner = Player::Player2;
                    break;
                default:
                    throw "Player::None";
            } break;
        case CT::client:
            switch (net_beginner) {
                case 1:
                    log_beginner = Player::Player1;
                    break;
                case 0:
                    log_beginner = Player::Player2;
                    break;
                default:
                    throw "Player::None";
            } break;
    }
    return log_beginner;
}


QString LogicConnection::colorLog2Str(QColor color_log){

    QString color_str;
    if (color_log == Qt::magenta) {
        color_str = "red   ";
    }
    else if (color_log == Qt::blue) {
        color_str = "blue  ";
    }
    else if (color_log == Qt::green) {
        color_str = "green ";
    }
    else if (color_log == Qt::yellow) {
        color_str = "yellow";
    }
    else if (color_log == Qt::cyan) {
        color_str = "cyan";
    }
    else {
        color_str = "magenta";
    }
    return color_str;
}


QColor LogicConnection::colorStr2Log(QString color_str){
    QColor color_log;

    if (color_str == "magenta") {
        color_log = Qt::magenta;
    }
    else if (color_str == "blue") {
        color_log = Qt::blue;
    }
    else if (color_str == "green") {
        color_log = Qt::green;
    }
    else if (color_str == "yellow") {
        color_log = Qt::yellow;
    }
    else if (color_str=="cyan") {
        color_log = Qt::cyan;
    }
    else{
        color_log = Qt::magenta;
    }

    return color_log;
}


void LogicConnection::gameOver(Result result, Player ended_by){
    if (ended_by == Player::Player1){
        setConnectionVar(CV::game_ended_by_me);
        if (result == Result::win){
            _game_parameters.score1++;
        }
        else if (result == Result::draw){
        }
    }
    else{
        setConnectionVar(CV::game_ended_by_op);
        if (result == Result::loss){
            _game_parameters.score2++;
        }
        else if (result == Result::draw){
        }
    }

    quint8 rounds_n = _game_parameters.rounds_n;
    quint8 score1 = _game_parameters.score1;
    quint8 score2 = _game_parameters.score2;
    quint8 current_round = _game_parameters.current_round;

    quint8 rounds_left = rounds_n - current_round;
    int round_diff_me  = score1 - score2;
    int round_diff_op  = score2 - score1;
    if (round_diff_me > rounds_left){
//        emit guiSeriesOver(Result::win,
//                           _game_parameters.score1,
//                           _game_parameters.score2,
//                           _game_parameters.rounds_n);
    }
    else if(round_diff_op > rounds_left){
//        emit guiSeriesOver(Result::loss,
//                           _game_parameters.score1,
//                           _game_parameters.score2,
//                           _game_parameters.rounds_n);
    }
    else{
        // no early victory
        if (_game_parameters.current_round < _game_parameters.rounds_n){
            emit guiGameOver(result, true);
        }
        else{
            // Series is over
            emit guiGameOver(result, false);

            if (score1 > score2){
//                emit guiSeriesOver(Result::loss,
//                                   _game_parameters.score1,
//                                   _game_parameters.score2,
//                                   _game_parameters.rounds_n);
            }
            else if (score2 < score1){
//                emit guiSeriesOver(Result::loss,
//                                   _game_parameters.score1,
//                                   _game_parameters.score2,
//                                   _game_parameters.rounds_n);
            }
            else{
//                emit guiSeriesOver(Result::draw,
//                                   _game_parameters.score1,
//                                   _game_parameters.score2,
//                                   _game_parameters.rounds_n);
            }
        }
    }
    deleteGameObj();
}


void LogicConnection::guiNewRound(){
    if (_connection_var == CV::game_end_accepted){
        std::cout << "LOGIC:: Starting new round" << std::endl;

        setConnectionVar(CV::gamefield_built);
        _game_parameters.current_round++;
        _changeBeginner();

        if (_connection_type == CT::server){
            emit netSendRequestStartRound(_game_parameters.current_round,
                                          beginnerLog2Net(_game_parameters.beginner));

            std::cout << "START REQUEST SENT" << std::endl;
            setConnectionVar(CV::start_request_sent);
        }
    }
}


void LogicConnection::netConnected(){
    if (_connection_var == CV::init){
        // TODO check data from opponent and answer if it came wrongly
        _connection_var = CV::connected;

        std::cout << "CONNECTED" << std::endl;

        if (_connection_type == CT::server){
//            QString color1_str = colorLog2Str(_game_parameters.color1);
//            emit netSendNickName(QString::fromStdString(_game_parameters.nick_name1), color1_str);
//            std::cout << "SENDING NICKNAME" << std::endl;
//            setConnectionVar(CV::nickname_sent);
            std::cout<<"LOGIC::EMIT SENDGAMEPARAMETERS"<<std::endl;
            emit netSendGameParameters(_game_parameters.width,
                                           _game_parameters.height,
                                           _game_parameters.rounds_n,
                                           beginnerLog2Net(_game_parameters.beginner));
            setConnectionVar(CV::game_parameters_sent);
        }
    }
}


void LogicConnection::netFailedToConnect(quint8 status_code){
    delete this;
}


void LogicConnection::netRecGameParameters(quint8 width, quint8 height, quint8 rounds_n, quint8 beginner){
//    if (_connection_var == CV::nickname_exchanged
    if (_connection_var == CV::connected
        && _connection_type == CT::client){
        // TODO check data from opponent and answer if it came wrongly

        std::cout << "RECEIVED PARAMETERS" << std::endl;
        setGameParameters(width, height, rounds_n, beginnerNet2Log(beginner));
        emit netSendResponseOnRequest(0x00);  // OK

        std::cout << "PARAMETERS EXCHANGED" << std::endl;
        setConnectionVar(CV::game_parameters_exchanged);
        buildGameField();
    }
}


void LogicConnection::netRecRequestStartRound(quint8 current_round, quint8 beginner_this_round){
    GameParameters game_par = getGameParameters();

    if (_connection_var == CV::gamefield_built
            && _connection_type == CT::client){
        quint8 my_net_beginner = beginnerLog2Net(_game_parameters.beginner);

        if (game_par.current_round == current_round
                && my_net_beginner == beginner_this_round){

            emit netSendResponseOnRequest(0x00);  // OK
            std::cout << "START REQUEST ACCEPTED" << std::endl;
            setConnectionVar(CV::start_request_accepted);
            startGame();
        }
    }
}


void LogicConnection::netRecResponseOnRequest(quint8 status_code){
    switch (_connection_var){
//        case CV::connected:
        case CV::game_parameters_sent:
            if (_connection_type == CT::server){
                if (status_code == 0){

                    std::cout << "PARAMETERS EXCHANGED" << std::endl;
                    setConnectionVar(CV::game_parameters_exchanged);
                    buildGameField();
                }
            } break;
        case CV::start_request_sent:
            if (status_code == 0){

                std::cout << "START REQUEST ACCEPTED" << std::endl;
                setConnectionVar(CV::start_request_accepted);
                startGame();

            } break;
        case CV::game_ended_by_op:
            if (status_code == 0){
                setConnectionVar(CV::game_end_accepted);           // OK
                emit guiOpponentReadyToContinue();
            } break;
        default:
            break;
    }
}

void LogicConnection::netRecNickName(QString nickname, QString color){
    if (_connection_type == CT::client){
        if (_connection_var == CV::connected){
            _game_parameters.nick_name2 = nickname.toStdString();

            std::cout << "RECEIVED NICKNAME" << std::endl;
            QColor color2;
            QColor color_input_log = colorStr2Log(color);

            if (_game_parameters.color1 == color_input_log){
                if (_game_parameters.color1 == Qt::magenta){
                    color2 = Qt::cyan;
                }
                else{
                    color2 = Qt::magenta;
                }
            }
            else{
                color2 = color_input_log;
            }
            _game_parameters.color2 = color2;

            QString color1_str = colorLog2Str(_game_parameters.color1);
            std::cout << "SENDING NICKNAME IN RETURN" << std::endl;
            emit netSendNickName(QString::fromStdString(_game_parameters.nick_name1), color1_str);
            setConnectionVar(CV::nickname_exchanged);
        }
    }
    else if (_connection_type == CT::server){
        if (_connection_var == CV::nickname_sent){
            setConnectionVar(CV::nickname_exchanged);
            std::cout << "RECEIVED NICKNAME BACK" << std::endl;

            _game_parameters.nick_name2 = nickname.toStdString();
            QColor color2;
            QColor color_input_log = colorStr2Log(color);

            if (_game_parameters.color1 == color_input_log){
                if (_game_parameters.color1 == Qt::magenta){
                    color2 = Qt::cyan;
                }
                else{
                    color2 = Qt::magenta;
                }
            }
            else{
                color2 = color_input_log;
            }
            _game_parameters.color2 = color2;


            setConnectionVar(CV::game_parameters_sent);
            std::cout<<"LOGIC::EMIT SENDGAMEPARAMETERS"<<std::endl;
            emit netSendGameParameters(_game_parameters.width,
                                       _game_parameters.height,
                                       _game_parameters.rounds_n,
                                       beginnerLog2Net(_game_parameters.beginner));
        }
    }
}


void LogicConnection::netRecMoveConnection(quint8 x){
    if (_game){
        emit netRecMoveGame(x);
    }
    else{
        emit netSendResponseOnMove(0x13);  // kein laufendes Spiel
    }
}


void LogicConnection::netRecResponseOnMoveConnection(quint8 status_code){
    switch (status_code){
        case 0x00: case 0x01: case 0x02: case 0x10: case 0x11: case 0x12: case 0x13: case 0x20:
            if (_game){
                if (status_code == 0x01 || status_code == 0x02){                               // 4er, Spielende
                    if (_connection_var == CV::game_ended_by_me){
                        emit netSendResponseOnRequest(0x00);
                        setConnectionVar(CV::game_end_accepted);           // OK
                        emit guiOpponentReadyToContinue();
                    }
                    else{
                        emit netSendResponseOnRequest(0x01);            // Abgelehnt oder nicht bereit
                    }
                }
                // Other status_codes get redirected to Game object
                else{
                    emit netRecResponseOnMoveGame(status_code);
                }
            }
            // If there is no game going
            else{
                emit netSendResponseOnRequest(0x01);                    // Abgelehnt oder nicht bereit
            }
            break;
        // not accepted status_code
        default:
            emit netSendResponseOnRequest(0x02);                        // Nicht unterstuzt
            break;

    }

}
