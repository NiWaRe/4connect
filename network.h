#ifndef NETWORK_H
#define NETWORK_H
#include <QObject>
#include <QString>
#include "net.h"
#include "server.h"
#include "client.h"
#include "netbroadcast.h"


class Network: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Referenz für Logik-Verbindung und connects
     */
    Network();
    /**
      @brief Genauso wie Server: Zum Löschen der selbsterstellten Pointer
      */
    ~Network();
    /**
     * @brief Pointer auf Parent-klasse -> Spezifiziert durch Init. mit Server oder Client
     */
    Net *_connect = nullptr;
    /**
     * @brief Broadcast-Server-Pointer -> Gibt nur einzelnes Objekt
     */
    netBroadcast *_broadcast = nullptr;


public slots:

    // LOGIC -> NETWORK
    /**
     * @brief Init. des allgemeinen Parent-Pointers als Server
     * @param port -> host einfach localhost
     */
    void netCreateHost(quint16 port){                                                  // NUR SERVER
        _connect = new Server(this, port);
    }

    /**
     * @brief Init. des allgemeinen Parent-Pointers als Server
     * @param host
     * @param port
     */
    void netConnectToHost(std::string host, quint16 port){                             // NUR CLIENT
        QString qIPAddress = QString::fromStdString(host);
        _connect = new Client(this, qIPAddress, port);
    }

    /**
     * @brief um Verbindungs-Objekt zu zerstören
     */
    void netDisconnect(){
        if (_connect){
            delete _connect;
        }
    }

    /**
     * @brief Starten des Broadcast-Servers
     */
    void netStartBroadcast(){
        // TODO change this if deleting _connect object isn't enough
        if (!_broadcast){
            _broadcast = new netBroadcast();
        }
    }

    /**
     * @brief Beenden des Broadcast-Servers | Freigeben der Objekte
     */
    void netEndBroadcast(){
        if (_broadcast){
            delete _broadcast;
        }
    }



signals:

    /**
      @brief Korrespondierende Signale zu net.h

    */

    // REDIRECTING SIGNAL FROM LOGIC THROUGH NETWORK TO NET
    // LOGIC -> NETWORK -> NET
    // Aushandeln Spielparameter
    void netSendGameParameters(quint8 width, quint8 height, quint8 rounds_n, quint8 beginner);  // NUR SERVER


    // Anforderung Rundenbeginn
    void netSendRequestStartRound(quint8 current_round, quint8 beginner_this_round);

    // Zug
    void netSendMove(quint8 x);

    // Antwort auf Anfrage
    void netSendResponseOnRequest(quint8 status_code);

    // Antwort auf Zug
    void netSendResponseOnMove(quint8 status_code);

    // Other
    void netSendNickName(QString nickname, QString color);


    // NET -> NETWORK -> LOGIC, GUI
    void netConnected();
    void netFailedToConnect(quint8 status_code);

    void netRecChat(QString msg);
    void netSendChat(QString msg);

    // NET -> NETWORK -> LOGIC
    // Aushandeln Spielparameter
    void netRecGameParameters(quint8 width, quint8 height, quint8 rounds_n, quint8 beginner);     // NUR CLIENT

    // Anforderung Rundenbeginn
    void netRecRequestStartRound(quint8 current_round, quint8 beginner_this_round);

    // Zug
    void netRecMove(quint8 x);

    // Antwort auf Anfrage
    void netRecResponseOnRequest(quint8 status_code);

    // Antwort auf Zug
    void netRecResponseOnMove(quint8 status_code);

    // Other
    void netRecNickName(QString nickname, QString color);


};

#endif // NETWORK_H
