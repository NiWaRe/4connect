#ifndef MYRECT_H
#define MYRECT_H
#include<QWidget>
#include<QtGui>
#include<QtCore>
#include <QPaintEvent>
#include <QPainter>

class myRect:public QWidget
{
   Q_OBJECT

public:
    myRect(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

private:
    int x;
    int y;

};

#endif // MYRECT_H


