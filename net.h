#ifndef CONNECTION_H
#define CONNECTION_H

//UPDATE: No need for abstract there are no methods that have to exist in client and server and that need to have a different implementation
//except the constrcutor:

//2 ways of achieving that: pure virtual public destrctor or protected constrcutor

//Abstract base class: connection -> Abstract classes can`t be initialized / Abstract member function directly leads to abstract class
//Ptr on abstract classes can be declared and later be init. with the right child class

#include <QDataStream>
#include <QTcpSocket>
#include <QObject>


class Net : public QObject
{
    Q_OBJECT


    //TODO: Virtual destrcutor

protected:
  Net(){} //connection object shouldn't be initalized
  QTcpSocket *tcpSocket = nullptr;
  QDataStream iostream;
  bool conSuccess = true;
  bool alreadyConnected = false;

  void setConSuccess(bool success);
signals:

     /**
       * @brief wird gerufen, wenn TCP-Verbindung aufgebaut wurde
       * @param successful -> um zurückzugeben ob erfolgreich oder nicht
       */
      void netConnected(bool successful);

      //Tell logic, oponent disconnected
      /**
       * @brief Verbindung wird unerwartet getrennt
       */
      void opponentDisconnected();

      //Verhandeln der Spielparameter
      /**
       * @brief netRec->Im Allgemeinen EMPFANGENE Daten: Spiel-Parameter wurden ausgetauscht
       * @param width
       * @param height
       * @param rounds_n
       * @param beginner
       */
      void netRecGameParameters(quint8 width, quint8 height, quint8 rounds_n, quint8 beginner);

      //Anforderung Rundenbeginn
      /**
       * @brief Es wurde eine Runden-Anfrage empfangen
       * @param current_round
       * @param beginner_this_round
       */
      void netRecRequestStartRound(quint8 current_round, quint8 beginner_this_round);

      //Zug
      /**
       * @brief Es wurde ein Zug empfangen
       * @param x -> X-Koordinate: Es müssen nur Spalten beobachtet werden
       */
      void netRecMove(quint8 x);

      //Antwort auf Anfrage
      /**
       * @brief Antwort auf Anfrgae
       * @param status_code
       */
      void netRecResponseOnRequest(quint8 status_code);

      //Antwort auf Zug
      /**
       * @brief Bestätigung meines eigenen Zuges wurde empfangen
       * @param status_code
       */
      void netRecResponseOnMove(quint8 status_code);

      //Chat-Nachricht
      /**
       * @brief Es wurde eine Chat-Nachricht empfangen
       * @param msg
       */
      void netRecChatMsg(QString msg);

      //Fehler der auf GUI angezeigt werden soll, impliziert "otherMessage", da Logic sowieso nich für handeln ausgelegt ist
      /**
       * @brief Auf dem Chat-Window: Fehler anzeigen lassen
       * @param msg
       */
      void netRecErrorLog(QString msg);

      /**
       * @brief Ich empfange den NickName meines Gegenspielers
       * @param nickname
       * @param color
       */
      void netRecNickName(QString nickname, QString color);

public slots:
    //All Methods for client and server object

    //FOR CONNECTING: In constrcutors directly establishing of connection

    //FOR RECEIVING:same for both s & c
      /**
     * @brief Allgemeine Empfang-Methode/Slot: Für beide Knoten identisch
     * Nachricht wird aufgeteilt in Header (WAS und WIE LANG), Body (Nachricht selbst), Extra (falls zuviel gesendet wurde)
     * Auswertungs-Logik bestehen aus grosser Switch-Case
     */
    void receive();

    //Tell logic, oponent disconnected
    /**
     * @brief Allgemein im folgenden die Sende-Gegenstücke zu den obigen Signals
     */
    void setOpponentDisconnected();

    //FOR SENDING: Additional methods in server class

    //Anforderung Rundenbeginn: Beide s & c
    /**
     * @brief Spielbeginn/Rundenbeginn anfordern
     * @param round_number
     * @param beginner_this_round
     */
    void netSendRequestStartRound(quint8 round_number, quint8 beginner_this_round);

    //Antwort auf Anfrage: Beide s & c
    /**
     * @brief Antwort auf Anfrage senden
     * @param status_code
     */
    void netSendResponseOnRequest(quint8 status_code);

    //Zug: Beide s & c
    /**
     * @brief Zug senden
     * @param x
     */
    void netSendMove(quint8 x);

    //Antwort auf Zug: Beide s & c
    /**
     * @brief Antwort auf Zug (des Gegners) senden
     * @param status_code
     */
    void netSendResponseOnMove(quint8 status_code);

    //Chat
    /**
     * @brief Chat-Nachricht senden
     * @param msg
     */
    void netSendChat(QString msg);

    //Other: Beide s & c (LOGIK-FEHLER)
    /**
     * @brief Gegner seinen Nickname senden
     * @param nickname
     * @param color
     */
    void netSendNickName(QString nickname, QString color);

    //CONNECTION-FEHLER
    /**
     * @brief Error anzeigen lassen (Netzwerk-Fehler)
     * @param socketError
     */
    void displayError(QAbstractSocket::SocketError socketError);


//    void netSendNickName(QString msg);
};

#endif // CONNECTION_H
