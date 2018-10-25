#ifndef TOKEN_H
#define TOKEN_H

#include<stone.h>


#include <QGraphicsEllipseItem>
#include <QDebug>
#include <QPainter>
#include <QtGui>
#include <QtCore>

#include <QDialog>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QString>
#include<QWidget>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QColor>

class Token : public QWidget
{
    Q_OBJECT
public:


    explicit Token(int width, int height, int x, int y , bool enter=false,bool leave=false,bool Pressed=false, QWidget *parent = 0);


int width, height, x, y;
//bool m_Pressed;
bool _enter;
bool _leave;
bool _myturn;
bool m_Pressed;
QColor color =Qt::lightGray;

public slots:

 // void Animationstone(int x,int y);

private:

signals:
    void clicked();
    void enter();
    void leave();


protected:
   void paintEvent(QPaintEvent *e);
   void mouseReleaseEvent(QMouseEvent *event);
   void enterEvent(QEvent *event);
   void leaveEvent(QEvent *event);
};

#endif // TOKEN_H
