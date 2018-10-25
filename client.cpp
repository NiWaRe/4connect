#include <iostream>
#include <QtNetwork>
#include "client.h"
#include "network.h"

//WITHOUT GUI, a parent window -> the socket has to be deleted manually!!
Client::Client(Network *parent, QString qIPAddress, quint16 port)
    : _qIPAddress(qIPAddress),
      _port(port){
      //Input std::string to int to qint16 -> the params the method takes
    tcpSocket = new QTcpSocket(this);

    //data stream "in"
    iostream.setDevice(tcpSocket);

    //Make sure that other connections (that might still be in place) are disconnected (sockets deleted)
    tcpSocket->abort();

    //WHERE THE MAGIC HAPPENS!
    tcpSocket->connectToHost(_qIPAddress, _port);

    //CONNECTS

    //NETWORK-INTERNAL
    //SIGNAL readyRead is emitted very time A BLOCK IS RECEIVED (from the byte-array)
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::receive);
    //To know whether error happened -> QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error) gibt es nicht
    connect(tcpSocket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),this, &Client::displayError);
    //After disconnection, delete Socket
    connect(tcpSocket, &QAbstractSocket::disconnected, tcpSocket, &QObject::deleteLater);
    //Tell logic that disconnected
    connect(tcpSocket, &QAbstractSocket::disconnected, this, &Net::setOpponentDisconnected);

    //NETWORK-LOGIC
    //Incoming: Signal FROM logic

    connect(parent, &Network::netSendRequestStartRound,
            this, &Client::netSendRequestStartRound);
    connect(parent, &Network::netSendMove,
            this, &Client::netSendMove);
    connect(parent, &Network::netSendResponseOnRequest,
            this, &Client::netSendResponseOnRequest);
    connect(parent, &Network::netSendResponseOnMove,
            this, &Client::netSendResponseOnMove);
    connect(parent, &Network::netSendNickName,
            this, &Client::netSendNickName);
    connect(parent, &Network::netSendChat,
            this, &Client::netSendChat);
    //Outgoing: Signal TO logic

    connect(this, &Client::netConnected,
            parent, &Network::netConnected);

    connect(this, &Client::netRecGameParameters,
            parent, &Network::netRecGameParameters);
    connect(this, &Client::netRecRequestStartRound,
            parent, &Network::netRecRequestStartRound);
    connect(this, &Client::netRecMove,
            parent, &Network::netRecMove);
    connect(this, &Client::netRecResponseOnRequest,
            parent, &Network::netRecResponseOnRequest);
    connect(this, &Client::netRecResponseOnMove,
            parent, &Network::netRecResponseOnMove);
    connect(this, &Client::netRecChatMsg,
            parent, &Network::netRecChat);
    connect(this, &Client::netRecErrorLog,
            parent, &Network::netRecChat);


    connect(this, &Client::netRecNickName,
            parent, &Network::netRecNickName);

    std::cout<<"CLIENT: Session connected, TcpSocket and Datatream init."<<std::endl;

    emit netConnected(conSuccess);
}

Client::~Client(){
    tcpSocket->abort();
    //Disconnect!
    tcpSocket->disconnectFromHost();
}

/*
void Client::readFortune()
{

    QString nextFortune;
    QString DataAsString = QTextCodec::codecForMib(1015)->toUnicode(tcpSocket->readAll());
    std::cout<<"RECEIVED: "<<DataAsString.toStdString()<<std::endl;
    in.startTransaction();

    QString nextFortune;
    in >> nextFortune;

    if (!in.commitTransaction())
        return;

    if (nextFortune == currentFortune) {
        QTimer::singleShot(0, this, &Client::requestNewFortune);
        return;
    }

    currentFortune = nextFortune;
    std::cout<<"RECEIVED: "<<currentFortune.toStdString()<<std::endl;

    int iBytesAvailable = tcpSocket->bytesAvailable();
    while(iBytesAvailable > 0){
        if(tcpSocket->isValid()){
            char* pzBuff = new char[iBytesAvailable];
            int iReadBytes = tcpSocket->read(pzBuff, iBytesAvailable);
            if( iReadBytes > 0 ){
                std::cout<<"RECEIVED: "<<nextFortune.fromUtf8(tcpSocket->readAll())<<std::endl;
            }else{
                std::cerr<<"read bytes count is not valid !! "<<std::endl;
            }
        iBytesAvailable = tcpSocket->bytesAvailable();
        }
    }


    //WRITE

    test = '0x02';
    iostream.writeRawData(&test, 1);
    std::cout<<"SEND: "<<test<<std::endl;
}
*/
