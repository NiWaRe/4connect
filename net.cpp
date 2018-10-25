#include "net.h"
#include <iostream>
#include <QtNetwork>
#include <QtCore>
#include <vector>

//Bei Client können ein par Cases nicht vorkommen
//-> So aber trotzdem weniger Aufwand als z.B mit polymorphie
void Net::receive(){
    std::cout<<"RECEIVED!!"<<std::endl;
    char msgHeader[2]; //char = quint8
    std::vector<char> msgBody;
    int iBytesAvailable = tcpSocket->bytesAvailable();
    //For later if more than a message was inputed: Read without emptying!
    QByteArray completeMsg = tcpSocket->peek(iBytesAvailable);

    std::cout<<"PEEK: "<<static_cast<int>(tcpSocket->bytesAvailable())<<std::endl;

    std::vector<char> msgExtra(completeMsg.begin(), completeMsg.end());
    bool extraMsg = false;
    bool unknownMsg = false;

    //Get MSG + Error-Routine
    if(iBytesAvailable < 2){
        //TODO: EMIT to Error-Window in GUI
        std::cerr<<"Zu wenige Bytes erhalten: Keine Message erkennbar"<<std::endl;
    }else if(iBytesAvailable >= 2){
        iostream.readRawData(msgHeader, 2);
        //NUR ganzen befehl holen (nicht unbedingt = gesamte msg!)

        //Für geschickte QString -> unnötig in Vektoren zu schreiben -> einfach direkt in char
        if(msgHeader[0] != 7 && msgHeader[0] != 9){
            for(quint8 i = 0; i<static_cast<quint8>(msgHeader[1]); i++){
                char temp2;
                iostream.readRawData(&temp2, 1);
                msgBody.push_back(temp2);
            }
        }
    }

//    std::cout<<"FIRST MSG BYTES: "<<static_cast<int>(msgBody.size()+3)<<std::endl;

    //Switch-Case-Auswertung -> Alle params in quint8 (uncod. char)
    //Übergabe "by-value"!! sonst laufen vars nach diesem slot ausser scope!
    switch(msgHeader[0]){
        //0x01 -> Dezimal
        case 1:
        {
            //Wenn Länge nicht stimmt -> Nicht Bekannte Nachricht
            if(msgHeader[1] != 4){
                   unknownMsg = true;
                   break;
            }
            //TODO: Logik parsed bei beginner: 0x00 -> S, 0x01 -> C
            //temp-var. because vector.at(...) gives back reference!
            quint8 width = msgBody.at(0);
            quint8 height = msgBody.at(1);
            quint8 rounds = msgBody.at(2);
            quint8 beginner = msgBody.at(3);
            emit netRecGameParameters(width,
                                      height,
                                      rounds,
                                      beginner);
            break;
        }
        case 2:
        {
            if(msgHeader[1] != 2){
                   unknownMsg = true;
                   break;
            }
            quint8 round_number = msgBody.at(0);
            quint8 beginner_this_round = msgBody.at(1);
            emit netRecRequestStartRound(round_number,
                                         beginner_this_round);
            break;
        }
        case 3:
        {
            if(msgHeader[1] != 1){
                   unknownMsg = true;
                   break;
            }
            quint8 x_coordinate = msgBody.at(0);
            std::cout << "NETWORK:: RECEIVED MOVE " << static_cast<int>(x_coordinate) << std::endl;
            emit netRecMove(x_coordinate);
            break;
        }
        case 16:
        {
            if(msgHeader[1] != 1){
                   unknownMsg = true;
                   break;
            }
            quint8 status_code = msgBody.at(0);
            emit netRecResponseOnRequest(status_code);
            break;
        }
        case 17:
        {
            if(msgHeader[1] != 1){
                   unknownMsg = true;
                   break;
            }
            quint8 status_code = msgBody.at(0);
            emit netRecResponseOnMove(status_code);
            break;
        }
        case 9:
        {
            //ES GIBT KEINE VORGEGEBENE LÄNGE!

            char temp3[msgHeader[1]];

            std::cout<<"CHAT-LÄNGE: "<<int(msgHeader[1])<<std::endl;

            iostream.readRawData(temp3, msgHeader[1]);

            /*  Convert Const char * to QString */
            QString msgString;
            msgString = QString::fromUtf8(temp3, msgHeader[1]);

            std::cout<<"CHAT-MSG: "<<msgString.toStdString()<<std::endl;

            emit netRecChatMsg(msgString);
            break;
        }
        case 7:
        {
            char nickNameLength;
            iostream.readRawData(&nickNameLength, 1);

            char nickNamesConst[nickNameLength];
            char colorConst[msgHeader[1]-nickNameLength];

            iostream.readRawData(nickNamesConst, nickNameLength);

            iostream.readRawData(colorConst, msgHeader[1]-nickNameLength);

            /*  Convert Const char * to QString */
            QString nickNames;
            nickNames = QString::fromUtf8(nickNamesConst);

            QString color;
            color = QString::fromUtf8(colorConst);

            std::cout<<"NickName: "<<nickNames.toStdString()<<std::endl;
            std::cout<<"Color: "<<color.toStdString()<<std::endl;
            emit netRecNickName(nickNames, color);
            break;
        }

        default:
        {
            unknownMsg = true;

            char msg[msgHeader[1]];

            iostream.readRawData(msg, msgHeader[1]);

            /*  Convert Const char * to QString */
            QString msgString;
            msgString = QString::fromUtf8(msg);

            std::cout<<"DEFAULT-MSG: "<<msgString.toStdString()<<std::endl;

            emit netRecErrorLog("Unknown Content: "+msgString);

            break;
         }
    }


    std::cout<<"BYTES AVAILABLE: "<<static_cast<int>(tcpSocket->bytesAvailable())<<std::endl;

    //Check wether additional bytes are in stream
    if(tcpSocket->bytesAvailable() > 0){
        extraMsg = true;
        std::cout<<"EXTRA!!!"<<std::endl;
    }

                int number = msgExtra.size() - msgHeader[1] - 3;

                std::cout<<"EXTRA_NUMB: "<<number<<std::endl;

                int temp = msgHeader[1];

                for(int a = 0; a < number; a++){
                   std::cout<<"EXTRA: "<<int(msgExtra.at(a+msgHeader[1]+3))<<std::endl;
                }
                for (int i = 0; i < number; ++i)
                {
                    char temp;
                    iostream.readRawData(&temp, 1);

                }

    //Check if extra message is already a new message:
    if(extraMsg){
        unsigned char msgCode[7] = {1, 2, 3, 16, 17, 9, 7};
        //msgHeader[1]+2 = insg. Anzahl an ausgelesenen Bytes
        char msgHeaderNew = msgExtra.at(msgHeader[1]+3); //NEIN -> DOCH -1 da nullbasiert
//        std::cout<<"NEW: "<<msgHeaderNew<<std::endl;
        for(int i = 0; i<7; i++){
            if(msgHeaderNew==msgCode[i]){
                //Recursive: Recall slot to reuse parsing mechanism
                //because of usage of "peek" data is still in stream
                extraMsg = false;

                std::cout<<"DOUBLE-MSG"<<std::endl;

                Net::receive();
            }
        }
        unknownMsg = true;
      }

    if(unknownMsg){
            //No new Message -> Error | OR random message
            //readall() to empty buffer isn't possible because data could be written into the stream during thsi processing
            //Number of data still in stream:
            int number = msgExtra.size() - msgHeader[1] - 3; //Nur für TEST beim ersten

            char msg[number];
            //Emtpy Stream by reading out
            if(number != 0){

                iostream.readRawData(msg, number);
            }
            /*  Convert Const char * to QString */

            QString msgString;
            msgString = QString::fromUtf8(msg);
            std::cout<<"Unknown (EXTRA) Content: "<<msgString.toStdString()<<std::endl;

            emit netRecErrorLog(msgString);
        }
}

//Before sending coding must be done! -> Switch as with receive would have been easier...
void Net::netSendMove(quint8 x_coordinate){

    std::cout << "NETWORK:: SENDING MOVE " << static_cast<int>(x_coordinate) << std::endl;
    char msg[3];
    msg[0] = 0x03;
    msg[1] = 0x01;
    //TODO: CHECKEN OF ANSTELLE CHAR EINFACH QINT8 BENUTZT WERDEN KANN
    msg[2] = x_coordinate;
    iostream.writeRawData(msg, 3);
}

void Net::netSendResponseOnRequest(quint8 statuscode){
    char msg[3];
    msg[0] = 0x10;
    msg[1] = 0x01;
    msg[2] = statuscode;
    iostream.writeRawData(msg, 3);
}

void Net::netSendResponseOnMove(quint8 status_code){
    char msg[3];
    msg[0] = 0x11;
    msg[1] = 0x01;
    msg[2] = status_code;
    iostream.writeRawData(msg, 3);
}

void Net::netSendRequestStartRound(quint8 round_number, quint8 beginner_this_round){
//    std::cout<<"TESSSSSSSSST"<<std::endl;
    quint8 temp = 65;
    char msg[4]; //4
    // Ohne '': -> Kommen so im System wieder heraus!
    //Mit '':  -> Werden so direkt Ausgegeben
    //Beim Ausgeben wird die Zahl ohne '' als ASCII codierung interpretiert
    msg[0] = 0x02;
    msg[1] = 0x02;
    msg[2] = round_number;
    msg[3] = beginner_this_round;
    iostream.writeRawData(msg, 4);
}

void Net::netSendChat(QString msg){

    std::cout<<"PREPARE CHAT: "<<msg.toStdString()<<std::endl;

    int msgSize = msg.size();

    std::cout<<"PREPARE CHAT LENGTH: "<<msgSize<<std::endl;

    char msgChat[msgSize+2];
    msgChat[0] = 0x09;          //does't matter if hex or not
    msgChat[1] = msgSize;

    QByteArray msgByte;
    const char *msgString;

    msgByte = msg.toUtf8();
    msgString = msgByte.constData();

    for(int i = 0; i < msgSize;i++){
        msgChat[i+2] = msgString[i];
    }

    iostream.writeRawData(msgChat, msgSize+2);
}

void Net::netSendNickName(QString nickName, QString color){
    int msgSize = nickName.size()+color.size(); //6, da Spiel-Params am Anfang einfach mitgeschickt werden (Ohne Header), Nickname max. 10, Color max. 6
    char msgChat[msgSize+3]; //Header (inkl. Länge von nickName)
    msgChat[0] = 0x07;
    msgChat[1] = msgSize;
    msgChat[2] =nickName.size();

    QByteArray nickNameBytes;
    const char *nickNameConst;

    QByteArray colorBytes;
    const char *colorConst;

    //Erster Teil der MSG
    nickNameBytes = nickName.toUtf8();
    nickNameConst = nickNameBytes.constData();

    //Zweiter Teil der MSG
    colorBytes = color.toUtf8();
    colorConst = colorBytes.constData();

    //Spiel-Paramter kommen danach hinterher

    for(int i = 0; i < msgSize; i++){
        if(i < nickName.size()){
            msgChat[i+3] = *(nickNameConst+i);
        }else if(i >= nickName.size()){
            msgChat[i+3] = *(colorConst+i-nickName.size());
        }
    }
    for(int x = 0; x < msgSize+3; x++){
        std::cout<<"SEND: "<<msgChat[x]<<std::endl;
    }

    iostream.writeRawData(msgChat, msgSize+2);
}


void Net::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        setConSuccess(false);
        break;
    case QAbstractSocket::HostNotFoundError:
        setConSuccess(false);
        std::cerr<<"The host was not found. Please check the host name and port settings."<<std::endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        setConSuccess(false);
        std::cerr<<"The connection was refused by the peer. Server running? IP/Port alright?"<<std::endl;
        break;
    default:
        setConSuccess(false);
        std::cerr<<"The following error occurred: "<<tcpSocket->errorString().toStdString()<<std::endl;
    }
}


void Net::setConSuccess(bool success){
    conSuccess = success;
}


//Tell Logic that disconnected
void Net::setOpponentDisconnected(){
    //Tell Logic
//    std::cout<<"TELL DISCONNECTED"<<std::endl;
    emit opponentDisconnected();
}
