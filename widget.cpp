#include "widget.h"
#include "ui_widget.h"
#include "enums_structs.h"

#include <QPropertyAnimation>
#include <QDebug>
#include <iostream>

Widget::Widget(QWidget *parent) :
    QWidget(parent),ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->Server->hide();
    ui->Client->hide();
    connect(ui->radioclient,&QRadioButton::clicked,ui->Client,&QFrame::show);
    connect(ui->radioclient,&QRadioButton::clicked,ui->Server,&QFrame::hide);
    connect(ui->radioserver,SIGNAL(clicked()),ui->Server,SLOT(show()));
    connect(ui->radioserver,SIGNAL(clicked()),ui->Client,SLOT(hide()));

//    connect(ui->StratGame,SIGNAL(clicked()),this,SLOT(OpenGame()));
    connect(ui->radioserver,&QPushButton::clicked, [this](){
        auto animation = new QPropertyAnimation(ui->Server, "size");
        animation->setDuration(800);
        animation->setStartValue(QSize(0, 322));
        animation->setEndValue(QSize(550, 322));
        animation->start();
    });

    connect(ui->radioclient,&QRadioButton::clicked, [this](){
    auto animation1 = new QPropertyAnimation(ui->Client, "size");
    animation1->setDuration(800);
    animation1->setStartValue(QSize(0, 321));
    animation1->setEndValue(QSize(550, 321));
    animation1->start();
    });
    connect(ui->ServerConnect,&QPushButton::clicked,this, &Widget::serverinit);
    connect(ui->CLientConnect,&QPushButton::clicked,this, &Widget::clientinit);

}


void Widget::buildGameField(quint8 width,
                            quint8 height,
                            quint8 rounds_n,
                            QColor color1,
                            QColor color2,
                            std::string nick_name1,
                            std::string nick_name2){

    game = new Dialog(this, width, height, rounds_n, 0);
    game->show();
    this->hide();
    emit infoGameField(color1, color2, nick_name1,nick_name2);

}


void Widget::rewrite()
{

}





void Widget::serverinit()
{

QString port = ui->ServerPortText->text();

QString width = ui->width->text();

QString height = ui->height->text();

QString round = ui->Round->text();

QString nickname = ui->ServerName->text();

int beginner = -1;


if (ui->ServerFirst->isChecked())
{
    beginner = 0;

}

else if (ui->RandomFirst->isChecked())
{
    beginner = 2;
}

else if (ui->ClientFirst->isChecked())
{
    beginner = 1;
}

else{
    ui->output->append("Please Choose who goes First!");
}
spalten = width.toInt();
zeilen =height.toInt();
round_n= round.toInt();

QString color = ui->ServerColor->currentText();

QColor color_to_send;
if (color=="magenta") {
    color_to_send = Qt::magenta;
}
else if (color=="blue") {
    color_to_send = Qt::blue;
}
else if (color=="green") {
    color_to_send = Qt::green;
}
else if (color=="yellow") {
    color_to_send = Qt::yellow;
}
else if (color=="cyan") {
    color_to_send = Qt::cyan;
}
else{
    color_to_send = Qt::magenta;
}

emit startConnectionServer(port.toInt(), width.toInt(), height.toInt(), round.toInt(), beginner, nickname.toStdString(), color_to_send);
}


void Widget::clientinit()
{
QString  host = ui->ClientIPText->text();

QString port = ui->ClientPortText-> text();

QString nickname = ui->ClientName->text();

QString color = ui->ClientColor->currentText();

QColor color_to_send;
if (color=="magenta") {
    color_to_send = Qt::magenta;
}
else if (color=="blue") {
    color_to_send = Qt::blue;
}
else if (color=="green") {
    color_to_send = Qt::green;
}
else if (color=="yellow") {
    color_to_send = Qt::yellow;
}
else if (color=="cyan") {
    color_to_send = Qt::cyan;
}
else{
    color_to_send = Qt::magenta;
}

emit startConnectionClient(host.toStdString(),  port.toInt(),  nickname.toStdString(), color_to_send);
}


void Widget::startGame(quint8 current_round, QString player_name, quint8 score1, quint8 score2, quint8 round_n)
{

        emit tostartGameDIA(current_round, player_name,score1,score2,round_n);
}


void Widget::gameOver(Result result, bool more_rounds){

        emit togameOverDIA(result, more_rounds);

}


void Widget::opponentReadyToContinue()
{
        emit toopponentReadyToContinueDIA();
}


void Widget::netConnected()
{

}


void Widget::netFailedToConnect(quint8 status_code)
{

}


void Widget::displayMsg(QString msg){

    emit chatMsg(msg);

}

void Widget::chatconnection(QString msg){

    emit sendMsg(msg);
}

Widget::~Widget()
{
    delete ui;
}



