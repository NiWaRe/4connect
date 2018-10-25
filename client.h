#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDataStream>
#include <QTcpSocket>
#include <QObject>
#include <iostream>
#include "net.h"

//Darf nicht "test_logic" enthalten

class Network;
//class QTcpSocket;
//class QNetworkSession;

//Muss von QObject erben, damit "Q_OBJECT" deiniert ist und so auch die Slots und Signals definiert
// Schon durch parent
class Client : public Net
{
    Q_OBJECT

public:
    /**TODO: Base-Logic class referenz übergeben für connects */
    explicit Client(Network *parent, QString qIPAddress, quint16 port);
    ~Client();
private slots:

signals:

private:
    //Default init.
    const QString _qIPAddress = "localhost";
    const quint16 _port;

};

#endif
