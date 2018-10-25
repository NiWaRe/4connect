#ifndef SERVER
#define SERVER

#include <QString>
#include <iostream>
#include <QObject>
#include <QDataStream>
#include "net.h"

class Network;
class QTcpServer;
//class QNetworkSession;

//Muss immer von QObject erben -> schon durch parent

class Server : public Net
{
    Q_OBJECT

public:
    /**TODO: Base-Logic class referenz übergeben für connects */
    /**
     * @brief Server: Tcp-Socket-Objekt wird als Server init.|DataStream an Socket binden
     * @param parent: Für die internen connects
     * @param port
     */
    explicit Server(Network *parent, quint16 port);

    /**
      @brief Um alle Pointer die manuell auf dem Heap erstellt wurden wieder zu löschen
    */
    ~Server();
signals:

public slots:

    /**
     * @brief Um die readyRead...connects zu machen -> Ab da TcpSocket-Objekt init.
     */
    void sessionOpened();
    //Aushandeln der Spielparameter: Nur S
    /**
     * @brief Server-Exklusiv, am Anfang des Spiels
     * @param width
     * @param height
     * @param rounds
     * @param beginner
     */
    void netSendGameParameters(quint8 width, quint8 height,
                               quint8 rounds, quint8 beginner);
private:
    QString ipAddress = "localhost";
    QTcpServer *tcpServer = nullptr;
    const quint16 _port;

};

#endif // SERVER

