#include "server.h"
#include <QtNetwork>
#include <QtCore>
#include <iostream>
#include "logic.h"
#include <assert.h>
#include "network.h"

Server::Server(Network *parent, quint16 port)
    :_port(port)
{
    std::cout << "Here comes dat port-boy " << _port << std::endl;
    tcpServer = new QTcpServer(this);
        if (!tcpServer->listen(QHostAddress::Any, port)) { //Server dazu Anweisen auf nächste Client-Request zu warten
            /*Hat nicht funktioniert*/
            std::cerr<<"Fortune Server: Unable to start the server"<<std::endl;
            setConSuccess(false);
            emit netConnected(conSuccess);
            return;
        }
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            if (ipAddressesList.at(i) != QHostAddress::LocalHost && ipAddressesList.at(i).toIPv4Address()) {
                ipAddress = ipAddressesList.at(i).toString();
                break;
            }
        }
        // if we did not find one, use IPv4 localhost
        if (ipAddress.isEmpty())
            ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
            std::cout<<"The server is running on\n\nIP: "<<ipAddress.toStdString()<<"\nport:"<<tcpServer->serverPort()<<"\n\n"<<std::endl;

        //CONNECTS
        //NETWORK-INTERNAL
        //For creation of TcpSockt
        connect(tcpServer, &QTcpServer::newConnection, this, &Server::sessionOpened);


        //NETWORK-LOGIC
        //NETWORK-LOGIC
        //Incoming: Signal FROM logic
        connect(parent, &Network::netSendGameParameters,
                this, &Server::netSendGameParameters);
        connect(parent, &Network::netSendRequestStartRound,
                this, &Server::netSendRequestStartRound);
        connect(parent, &Network::netSendMove,
                this, &Server::netSendMove);
        connect(parent, &Network::netSendResponseOnRequest,
                this, &Server::netSendResponseOnRequest);
        connect(parent, &Network::netSendResponseOnMove,
                this, &Server::netSendResponseOnMove);
        connect(parent, &Network::netSendNickName,
                this, &Server::netSendNickName);
        connect(parent, &Network::netSendChat,
                this, &Server::netSendChat);
        //Outgoing: Signal TO logic
        connect(this, &Server::netConnected,
                parent, &Network::netConnected);

        connect(this, &Server::netRecGameParameters,
                parent, &Network::netRecGameParameters);
        connect(this, &Server::netRecRequestStartRound,
                parent, &Network::netRecRequestStartRound);
        connect(this, &Server::netRecMove,
                parent, &Network::netRecMove);
        connect(this, &Server::netRecResponseOnRequest,
                parent, &Network::netRecResponseOnRequest);
        connect(this, &Server::netRecResponseOnMove,
                parent, &Network::netRecResponseOnMove);
        connect(this, &Server::netRecChatMsg,
                parent, &Network::netRecChat);
        connect(this, &Server::netRecErrorLog,
                parent, &Network::netRecChat);

        connect(this, &Server::netRecNickName,
                parent, &Network::netRecNickName);
}


void Server::sessionOpened()
{
    //MULTIPLE CLICK SAFE
    //To not allow multiple client-connections
    if(!alreadyConnected){
        alreadyConnected = !alreadyConnected;
        //Make Connection out of requested connection from client (connection is still pending) and store the returned TcpSocket
        tcpSocket = tcpServer->nextPendingConnection();

        //Use QDataStream to serialize Data (convert into bits) and stream into QTcpSocket (in generel into a QIODevice)
        iostream.setDevice(tcpSocket);

        connect(tcpSocket, &QIODevice::readyRead, this, &Server::receive);
        //To know whether error happened -> QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error) gibt es nicht
        //Auch Methoden der Parent-Klasse werden durch Server-objekt referenziert
        connect(tcpSocket, static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),this, &Server::displayError);
         //Delete Socket after disconnecting
        connect(tcpSocket, &QAbstractSocket::disconnected, tcpSocket, &QObject::deleteLater);
        //Tell logic that disconnected
        connect(tcpSocket, &QAbstractSocket::disconnected, this, &Net::setOpponentDisconnected);

        //TODO: Was für Fehler können bei Server auftreten?

        std::cout<<"SERVER: Session created, TcpSocket and Datatream init."<<std::endl;
        emit netConnected(conSuccess); //Wird 100% nach Net::displayError ausgeführt und erst wenn connection schon passt

    }
    //For receiving
    //connect(tcpSocket, &QIODevice::readyRead, this, &Server::readFortune);

    /*
    //send signal to logic

    //test-send
    char d[2];
    d[0] = '0x01';
    d[1] = '0x02';
    iostream.writeRawData(d, 2);

    std::cout<<"SERVER: SEND"<<d[0]<<d[1]<<std::endl;
    */
}


void Server::netSendGameParameters(quint8 width, quint8 height, quint8 rounds, quint8 beginner){

    std::cout<<"NETWORK::SENDING GAMEPARAMETERS"<<std::endl;
    char msg[6];
    msg[0] = 0x01;
    msg[1] = 0x04;
    msg[2] = width;
    msg[3] = height;
    msg[4] = rounds;
    msg[5] = beginner;
    iostream.writeRawData(msg, 6);
}

Server::~Server(){
    std::cout<<"DISCONNECT"<<std::endl;
    tcpSocket->abort();
    tcpSocket->disconnectFromHost();
}

