#include "token.h"
#include "dialog.h"
#include <QPropertyAnimation>



Token::Token(int _width, int _height, int _x, int _y, bool enter, bool leave ,bool Pressed, QWidget *parent) : QWidget(parent), width(_width), height(_height), x(_x), y(_y), _enter(enter),_leave(leave),m_Pressed(Pressed)
{    /*  connect(ui->radioserver,&QPushButton::clicked, [this](){
        auto animation = new QPropertyAnimation(ui->Server, "pos");
        animation->setDuration(800);
        animation->setStartValue(QPoint(0, 0));
        animation->setEndValue(QPoint(0, 6));
        animation->start();*/

}





void Token::paintEvent(QPaintEvent *e)
{
    //qDebug() << "in myRect::paintEvent1"<<x++<<y++;
    QPainter painter(this);
    QRect rec(0,0,width+45,height+10);
//    QPen framepen(Qt::red);
    painter.setBrush(Qt::black);
    painter.drawRect(rec);


    if(m_Pressed){
         painter.setBrush(Qt::red);
       painter.drawRect(rec);
        }
    if(!m_Pressed){
        painter.setBrush(Qt::black);
        painter.drawRect(rec);
    }

    if(_enter){
       painter.setBrush(Qt::green);
       painter.drawRect(rec);
    }

    if (_leave){
        painter.setBrush(Qt::black);
        painter.drawRect(rec);

    }



    painter.setBrush(color);
    painter.drawEllipse(rec);







}

void Token::mouseReleaseEvent(QMouseEvent *event) {

    if(event->button() == Qt::LeftButton) {
        emit clicked();
    }

}


void Token::enterEvent(QEvent *event){
    emit enter();
}

void Token::leaveEvent(QEvent *event){
    emit leave();
}


     /*connect(ui->tester,&QPushButton::clicked, [this](){
     Stone *stone = new Stone;
     ui->gridLayout->addWidget(stone,0,0);
     animation = new QPropertyAnimation(Stone, "geometry");
     animation->setDuration(1000);
     animation->setStartValue(QRect (0,0,200,200));
     animation->setEndValue(QRect(0,7,200,200));
     animation->start();
*/



