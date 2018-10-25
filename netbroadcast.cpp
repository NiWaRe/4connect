#include "netbroadcast.h"
#include <random>
#include <chrono>


netBroadcast::netBroadcast():
    _udpSocketReceiver(new QUdpSocket(this)),
    _udpSocketSender(new QUdpSocket(this)),
    _timer(new QTimer(this)),
    _timer2(new QTimer(this)),
    _datagram(new char[3])
    //Hat keine Auswirkung -> Seeden mit MAC-Addresse: Einzige Unterschied also unterschiedliche anfangszeit
    {

    std::srand(std::time(0));
    _randNumb = std::rand();
    QString base = "c4%1";
    _id = base.arg(_randNumb);
    _id2 = base.arg(_addNumb);

    std::cout<<"RANDOM-ID: "<<_id.toStdString()<<std::endl;
    std::cout<<"RANDOM-ID2: "<<_id2.toStdString()<<std::endl;

    /*******RECEIVER_IMPL******/
    //Bind to certain port = Listen for data-packages on this port
    //"ShareAddress" means that port and ip-address should still be open to other services!
    _udpSocketReceiver->bind(_port, QUdpSocket::ShareAddress);
    std::cout<<"Listening!"<<std::endl;

    //TODO: connects machen
    //Same as TcpSocket -> When something is received, readyRead() Signal is outputed
    connect(_udpSocketReceiver, &QUdpSocket::readyRead, this, &netBroadcast::processPendingDatagrams);

    //connect(this, SIGNAL(serverFound()), this, SLOT(start()));
    connect(_timer, SIGNAL(timeout()), this, SLOT(broadcastDatagramSlot()));
    connect(this, SIGNAL(clientFound()), this, SLOT(clientFoundSlot()));
    connect(this, SIGNAL(serverFound()), this, SLOT(serverFoundSlot()));
    connect(this, SIGNAL(broadcastDatagram()), this, SLOT(broadcastDatagramSlot()));

    std::cout<<"TIME-Interval: "<<_timerInterval<<std::endl;

    _timer->start(_timerInterval);



//ETWAS HINTER SIGNAL STEHEN ZU HABEN FUNKTIONIERT NICHT

    //TODO: GUI-connects
//    connect(this, SIGNAL(), this, SLOT());
//    connect(this, SIGNAL(), this, SLOT());

}

void netBroadcast::processPendingDatagrams(){

   // std::cout<<"*********RECEIVE********"<<std::endl;
    //Read received Datagram
    quint16 *port = nullptr;

        while (_udpSocketReceiver->hasPendingDatagrams()) {
                QByteArray datagram;
                //Adapt to udpSocket-DatagramSize
                datagram.resize(_udpSocketReceiver->pendingDatagramSize());
                //Where to store, How many bytes, Sender-IP, Sender-Port
                _udpSocketReceiver->readDatagram(datagram.data(), datagram.size(),
                                         _oAddress, port);
                delete port;
                _datagram = datagram.data();
                //std::cout<<"Received datagram: "<<_datagram<<std::endl;
                //If sender only sends once -> to also listen to possibly other IPs after certain amount of time
    //            if(){
    //                //TIMER-Implementation: After 10 Sek. change Sender IP-Var
    //            }
            }
        //Process Datagram
        //-> switch case with strings isn't possible: Possible with MAPS (Key-Value-Pairs)
        //Check if Broadcast is from c4 and if from same sender
        //&& _oAddress->toIPv4Address() != _opponentAddress->toIPv4Address()

        QString tempReceive = QString::fromUtf8(_datagram);


        if(_datagram[0] == 99 && _datagram[1] == 52){

            //std::cout<<"BINGO"<<std::endl;

            //TODO: What happens when 3.digit radnomly is 2 ?
            //if(_datagram[2] == 50){
            if(!QString::compare(tempReceive, _id2, Qt::CaseSensitive)){
                //std::cout<<"Case2"<<std::endl;


                _rMessages2++;
                                //Security reasons: Declare Server as found only when received twice the package
                if(_rMessages2 == 2){
                    _rMessages2 = 0;
                    emit serverFound();
                }
            }

            //Broadcast comes from c4-Game
            if(QString::compare(tempReceive, _id, Qt::CaseSensitive) && QString::compare(tempReceive, _id2, Qt::CaseSensitive)){
               //If Stings aren't equal -> returns 1 -> Wasn't send from own Socket
                //std::cout<<"Case1: "<<_rMessages<<std::endl;
                //Zuerst Inkrementieren
                _rMessages++;
                //Security reasons: Declare Server as found only when received twice the package
                if(_rMessages == 2){
                    _sM = STOP;
                    //std::cout<<"CRAZZZY"<<std::endl;
                    _rMessages = 0;
                    emit clientFound();
                }
            }
        }

//        if(_datagram[0] == 99 && _datagram[1] == 52 ){
//            std::cout<<"Shit"<<std::endl;

//            switch(_datagram[2]){
//                //searching phase
//                case 49:
//                    std::cout<<"Case1: "<<_rMessages<<std::endl;
//                    //Zuerst Inkrementieren
//                    ++_rMessages;
//                    //Security reasons: Declare Server as found only when received twice the package
//                    if(_rMessages == 2){
//                        _sM = STOP;
//                        std::cout<<"CRAZZZY"<<std::endl;
//                        _rMessages = 0;
//                        emit clientFound();
//                    }
//                    break;
//                //TCP-connection phase -> Received ACK from other PC
//                case 50:
//                    std::cout<<"Case2"<<std::endl;
//                    //Same var can be used because in this case IP HAS to be the same as in case one -> and there it was set to 0
//    //                ++_rMessages;
//    //                if(_rMessages == 2){
//    //                    _rMessages = 0;
//    //                    //START Normal Game as Client! So that sender alway alternates!
//    //                    emit startGame(CLIENT);
//    //                }

//                    //Davon ausgehen, dass in deisem case schon sicher c4 gemeint ist
//                    //2 ACK wurden gesendet, eins reicht!
//                    emit serverFound();
//                    break;
//                default:
//                    break;
//                }
        }
    //    if(_opponentAddress != nullptr){
    //        std::cout<<"Server found, processing"<<std::endl;
    //        emit serverFound();




void netBroadcast::broadcastDatagramSlot(){
//    if(!end){
//    _sending = !_sending;
//    }
    //std::cout<<"*********SEND********"<<std::endl;
//    if(_sending || true){

      //std::cout<<"Prepare Sending"<<std::endl;
      QByteArray datagram = "";
      switch (_sM){
         case START:
              datagram = _id.toUtf8();
              break;
          case STOP:
              datagram = _id2.toUtf8();
              break;
       }
      _udpSocketSender->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, _port);
      //std::cout<<"Broadcast send: "<<datagram.toStdString()<<std::endl;
//   }
}

void netBroadcast::serverFoundSlot(){
    //Send Signal to Net-Class normally GUI would send
    //emit startGame(CLIENT, _opponentAddress, )

    std::cout<<"TCP-CLIENT-START!"<<std::endl;

    //destroy udpsockets --> WIRD ERST DANACH AUSGEFÜHRT, RICHTIG???
//    ~netBroadcast();
}

void netBroadcast::clientFoundSlot(){
    //Send 2 ACK -> Between datagrams always 1000 ms pause
    //_sendingNumb = 2;
    //_sM = STOP;

//    _sending = !_sending;
//    end = true;
//    _timerInterval = 100;
    //Send Signal to Net-Class normally GUI would send
    //emit startGame(SERVER, _opponentAddress, )

    std::cout<<"TCP-SERVER-START!"<<std::endl;

    //DAMIT NICHT SELBST ALS SERVER STARTET!
    delete _udpSocketReceiver;

    //destroy udpsockets --> WIRD ERST DANACH AUSGEFÜHRT, RICHTIG???
//    ~netBroadcast();
}

