#include <vector>
#include <array>

#include "widget.h"
#include "dialog.h"
#include "ui_dialog.h"



Dialog::Dialog(Widget * widget_ptr, int width, int height,int round_n,  QDialog *parent):
    QDialog(parent),
    ui(new Ui::Dialog)
  ,xMax(width)
  ,yMax(height)
  ,Round(round_n)
{
    ui->setupUi(this);
    setFixedSize(1050, 600);
    w = new Win();

     qDebug() << "width"<< xMax<<"height"<<yMax<<"Round"<<Round;

    for(auto x = 0; x < xMax ; ++x)
    {
        myTokens.push_back(std::vector<Token *>());
        for(auto y = 0; y <yMax; ++y)
        {
            Token *z = new Token(60,60,x,y,false);
            myTokens[x].push_back(z);
            ui->grid->addWidget(z,y,x);
            connect(z, &Token::clicked, [this, x](){onColumnClicked(x);});
            connect(z, &Token::leave, [this, x](){onColumleave(x);});
            connect(z, &Token::enter, [this, x](){onColumenter(x);});
            connect(ui->input, &QLineEdit::textChanged, [this](){
                if(!isTyping) {
                    textBuffer = ui->output->toPlainText();
                    qDebug() << textBuffer;
                    ui->output->append("Typing...");
                    isTyping = true;
                }
            });

            
        }
    }
    ui->frame_2->hide();
    ui->frame_3->hide();
    ui->frame_4->hide();
    connect(ui->continue_2,&QPushButton::clicked,widget_ptr, &Widget::newRound);
    connect(ui->Exit,&QPushButton::clicked, widget_ptr,&Widget::disconnect);
    connect(ui->continue_2,&QPushButton::clicked,ui->frame_2,&QFrame::hide);
    connect(ui->Exit,&QPushButton::clicked,ui->frame_2,&QFrame::hide) ;
    connect(ui->continue_2,&QPushButton::clicked,ui->frame_3,&QFrame::hide);
    connect(ui->Exit,&QPushButton::clicked,ui->frame_3,&QFrame::hide) ;

    connect(ui->input, &QLineEdit::returnPressed, this, &Dialog::send);
    connect(ui->pushButton,&QPushButton::clicked, this, &Dialog::send);
    connect(widget_ptr, &Widget::tostartGameDIA, this, &Dialog::startGameDIA);
    connect(widget_ptr, &Widget::togameOverDIA, this, &Dialog::gameOverDIA);
    connect(widget_ptr, &Widget::toopponentReadyToContinueDIA, this, &Dialog::opponentReadyToContinueDIA);
    connect(this, &Dialog::columnClicked, widget_ptr, &Widget::makeMove);
    connect(widget_ptr, &Widget::setStone, this, &Dialog::setStoneDIA);
    connect(widget_ptr, &Widget::set3Warning,   this, &Dialog::set3Warning);
    connect(widget_ptr,&Widget::chatMsg, this, &Dialog::Displaychat);
    connect(this ,&Dialog::Sendmsg, widget_ptr, &Widget::chatconnection);
    connect(widget_ptr,&Widget::infoGameField, this, &Dialog::infogameField);
    connect(this, &Dialog::openend, [this](){
                auto animation = new QPropertyAnimation(ui->frame_2, "size");
                animation->setDuration(800);
                animation->setStartValue(QSize(300, 0));
                animation->setEndValue(QSize(300, 300));
                animation->start();
    });

    connect(this, &Dialog::lose, [this](){
                auto animation = new QPropertyAnimation(ui->frame_3, "size");
                animation->setDuration(800);
                animation->setStartValue(QSize(0, 300));
                animation->setEndValue(QSize(550, 330));
                animation->start();
    });

    connect(this, &Dialog::win, [this](){
                auto animation = new QPropertyAnimation(ui->frame_4, "size");
                animation->setDuration(800);
                animation->setStartValue(QSize(0, 330));
                animation->setEndValue(QSize(550, 330));
                animation->start();
    });
}





void Dialog::send() {

    if(isTyping) {
        isTyping = false;
        ui->output->setText(textBuffer);

    }
    QString text = ui->input->text();
    ui->output->append(text);
    emit Sendmsg(text);
    ui->input->clear();
}

void Dialog::Displaychat(QString msg){
    ui->output->append(msg);

}

void Dialog::onColumnClicked(quint8 x) {
    qDebug() << "Column " << x << " clicked";
    emit columnClicked(x);

}

void Dialog::setStoneDIA(quint8 x, quint8 y, QColor color, QString player_name)
{
    qDebug() << player_name;
    // TODO
    // Here a stone at x,y should be painted in color_to_set
    ui->yourTurnName->setText(player_name);
    qDebug()<<"the  elemet";
    Token* Ptr = myTokens[x][y];
     //Ptr-> m_Pressed = true;
     Ptr->color = color;
     repaint();

}


void Dialog::onColumenter(int x){

//    qDebug() << "enter"<< x;
    for(auto it = myTokens[x].begin(); it != myTokens[x].end(); ++it) {
        (*it)->_enter = true;
         repaint();
    }


}


void Dialog::onColumleave(int x){

//    qDebug() << "leave"<< col;

    for(auto it = myTokens[x].begin(); it != myTokens[x].end(); ++it) {

        (*it)->_enter=false;
        repaint();
    }



}


void Dialog::startGameDIA(quint8 current_round, QString player_name, quint8 score1, quint8 score2, quint8 round_n){

    ui->roundsn->display(current_round);
    ui->yourTurnName->setText(player_name);
    ui->score1->display(score1);
    ui->score2->display(score2);
    ui->roundsn_2->display(round_n);


}

void Dialog::gameOverDIA(Result result, bool more_rounds){
    if (result == Result::win){
     ui->frame_4->show();
     emit win();

     w->show();
    }
    if (result == Result::loss){
     ui->frame_3->show();
     emit lose();

    }

    ui->frame_2->show();
    emit openend();

}


void Dialog::opponentReadyToContinueDIA(){
    qDebug()<<"ready\;";
}

void Dialog::infogameField(QColor color1, QColor color2, std::string nick_name1, std::string nick_name2)
{
    QString color_str1;
    if (color1 == Qt::magenta) {
        color_str1 = "background-color:magenta;   ";
    }
    else if (color1== Qt::blue) {
        color_str1 = "background-color:blue;  ";
    }
    else if (color1 == Qt::green) {
        color_str1 = "background-color:green; ";
    }
    else if (color1 == Qt::yellow) {
        color_str1 = "background-color:yellow;";
    }
    else if (color1 == Qt::cyan) {
        color_str1 = "background-color:cyan;";
    }

    QString color_str2;
    if (color2 == Qt::magenta) {
        color_str2 = "background-color:magenta;   ";
    }
    else if (color2== Qt::blue) {
        color_str2 = "background-color:blue;  ";
    }
    else if (color2 == Qt::green) {
        color_str2 = "background-color:green; ";
    }
    else if (color2 == Qt::yellow) {
        color_str2 = "background-color:yellow;";
    }
    else if (color2 == Qt::cyan) {
        color_str2 = "background-color:cyan;";
    }
    ui->Player1->setText(QString::fromStdString(nick_name1));
    ui->Player2->setText(QString::fromStdString(nick_name2));
    ui->color1->setStyleSheet(color_str1);
    ui->color2->setStyleSheet(color_str2);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::set3Warning(std::set<std::pair<quint8, quint8>> pairs_set){


    for(quint8 x=0; x < xMax; x++) {
        for(quint8 y=0; y < yMax; y++){
          Token* Ptr = myTokens[x][y];
          Ptr->m_Pressed = false;
          repaint();
        }
    }


    for(auto f : pairs_set) {
      quint8 x = f.first;
      quint8 y = f.second;
      Token* Ptr = myTokens[x][y];
      Ptr->m_Pressed = true;
      repaint();
    }


    // TODO
    // 1. remove highlightning from old stones
    // highlight every Stone in the vector
    // there can be duplicates
}










