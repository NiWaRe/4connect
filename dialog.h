#ifndef DIALOG_H
#define DIALOG_H

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

#include <vector>
#include <set>
#include "stone.h"
#include "win.h"
#include "enums_structs.h"
#include "token.h"
#include <QPropertyAnimation>

class Widget;
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    int xMax;
    int yMax;
    int Round;
    explicit Dialog(Widget * _widget_ptr, int width , int height,int round_n, QDialog *parent = 0);
    ~Dialog();


    std::vector<std::vector<Token *>> myTokens;

private:
    Ui::Dialog *ui;
    QPropertyAnimation *animation;
    bool isTyping = false;
    QString textBuffer;
    Win *w;

    void onColumnClicked(quint8 x);
    void onColumenter(int x);
    void onColumleave(int x);


public slots:

    void startGameDIA(quint8 current_round, QString player_name, quint8 score1, quint8 score2, quint8 round_n);
    void gameOverDIA(Result result, bool more_rounds);
    void opponentReadyToContinueDIA();
    void send();
    void setStoneDIA(quint8 x, quint8 y, QColor color, QString player_name);
    void set3Warning(std::set<std::pair<quint8, quint8>> pairs_set);

    void Displaychat(QString msg);
    void infogameField(QColor color1, QColor color2, std::string nick_name1, std::string nick_name2);
signals:
    void columnClicked(int col);
    void Sendmsg(QString text);
    void openend();
    void lose();
    void win();

};




#endif // DIALOG_H
