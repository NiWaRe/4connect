#ifndef NETBROADCAST_H
#define NETBROADCAST_H

#include <QWidget>
#include <QtNetwork>
#include <iostream>
#include <QUdpSocket>
#include <QObject>
#include <QTime>

/***TODO***/
//1. Kann ich nur eine einzige Klasse schreiben -> Kombination Sender/Receiver
//--> ZIEL: Spiel starten OHNE JEGLICHEN KONTAKT davor! -> Dazu müssen beide beides können
//--> IDEE: Computer-Spezifische Verzögerung in Form der MAC-Addresse in
//          a) Sende-Startpunkt oder
//          b) Nach emit von readyRead:
//2. Von Sender-Seite her, wie implementiere ich Timer, der Package jede x-ms schickt
//3. GUI bekommt in jedem Fall Signal mit QString IP-Addresse -> Usr wählt aus und spiel startet normal
//--> Es wird nicht durch alle Ports gelooped, sondern es gibt c4-Port: 43334!

class netBroadcast : public QObject
{
    Q_OBJECT

public:
    explicit netBroadcast();
    //NUR FÜR PROTECTED:
    //Object kann nur zerstört werden, durch Kinder oder mit Kindern
    //TODO: Testen ob das Sinn macht und in normale net implementieren
//    ~netBroadcast();

signals:
    //Both instances are both anyway, only for organisation

    //B-Receiver-Signals
    /**
     * @brief Signal das emittiert wird , sobald eine Broadcast-instanz zweimal die spezielle Nachricht empfangen hat
     */
    void serverFound();

    /**
     * @brief Signal das emittiert wird, sobald zukünftige Server-instanz bestätigung zurück gesendet hat
     */
    void clientFound();


    //B-Sender-Signals
    //For both the partner-computer and the local computer
    /**
     * @brief send
     */
    void send();
    /**
     * @brief sende Datagram-Signal
     */
    void broadcastDatagram();

public slots:
    //B-Receiver-Signals
    /**
     * @brief Verarbeite Datagrame
     */
    void processPendingDatagrams();
//    void startGame(playerMode pM);

    /**
     * @brief korrespondierende Slot zu obigem Signal
     */
    void serverFoundSlot();

    /**
     * @brief korrespondierende Slot zu obigem Signal
     */
    void clientFoundSlot();

    //B-Sender-Signals
    /**
     * @brief sende Datagram-Slot
     */
    void broadcastDatagramSlot();
private:
    //Im Allgemeinen keiner getter/setter nötig, da sowieso enum! (+ keine Zeit)

//    enum playerMode {SERVER, CLIENT};
    QUdpSocket *_udpSocketSender = nullptr;
    QUdpSocket *_udpSocketReceiver = nullptr;
    //For now static for c4 in general: c4-PORT (as 80 is for HTTP)
    const quint16 _port = 43334;
    QHostAddress *_opponentAddress = nullptr;
    QHostAddress *_oAddress = nullptr;
    int _sendingNumb = 4;
    int _sendingNumbIterator = 0;
    enum searchingMode {START, STOP} _sM = START;
//    searchingMode _sM;
    char* _datagram= nullptr;
    int _rMessages = 0;
    int _rMessages2 = 0;
    QTimer *_timer = nullptr;
    QTimer *_timer2 = nullptr;
    int _timerInterval = 4000;

    //Alternating Sender/Receiver
    bool _sending = false;

    bool start = true;

    bool end = false;

    int _randNumb = 0;
    int _addNumb = 222222222;

     QString _id;
     QString _id2;
};

#endif // NETBROADCAST_H
