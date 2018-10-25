/*#ifndef STONE_H
#define STONE_H
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>


class Stone:public QWidget
{
    
 Q_OBJECT  
    
public:
     Stone(QWidget *parent = 0);
     void setFloatBased(bool floatBased);
     void setAntialiased(bool antialiased);
     QSize minimumSizeHint() const override;
     QSize sizeHint() const override;

 public slots:
     void nextAnimationFrame();
 
 protected:
     void paintEvent(QPaintEvent *painter) override;
     bool floatBased;
     bool antialiased;
     int frameNo;


 };


#endif // STONE_H


*/


    

  
