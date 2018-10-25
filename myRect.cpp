#include "myRect.h"
#include "ui_dialog.h"
#include<QtGui>
#include<QtCore>
#include <QWidget>
#include <QPaintEvent>
#include <QDebug>
#include <iostream>


myRect::myRect(QWidget *parent)
    : QWidget(parent)
{


}

void myRect::paintEvent(QPaintEvent *)
{
    qDebug() << "in myRect::paintEvent";
    QPainter painter(this);
    QRect rec(0,0,70,70);
    QPen framepen(Qt::red);

    for( x=0 ; x<7; x++){
         qDebug()<< "ax" << x << "and ay " << y;
        for( y=0;y<7;y++){
 qDebug()<< "x" << x << "and y " << y;
    rec.setRect(x*95,y*95,95,95);
    painter.drawRect(rec);
    painter.drawEllipse(rec);



        }


    }

}






/*void Rect::paintEvent(QPaintEvent *e)
{


         QPainter painter(this);
         QRect rec(10,10,100,100);
         QPen framepen(Qt::red);

         for(int x=0;x<7;x++){
             for(int y=0;y<7;y++){
         rec.setRect(10+y*120,10+x*120,100,100);
         painter.drawRect(rec);
         painter.drawEllipse(rec);
         ui->mahmoud->setVerticalSpacing(0);
         ui->mahmoud->setHorizontalSpacing(0);
         ui->mahmoud->addWidget()


             }
         }

}*/

