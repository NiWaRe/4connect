#ifndef WIN_H
#define WIN_H

#include <QDialog>
#include <QGraphicsScene>
#include <QLabel>
#include <QMovie>


namespace Ui {
class Win;
}

class Win : public QDialog
{
    Q_OBJECT

public:
    explicit Win(QWidget *parent = 0);
    ~Win();

private:
    Ui::Win *ui;
};

#endif // WIN_H
