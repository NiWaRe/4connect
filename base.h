#ifndef BASE_H
#define BASE_H
#include <QObject>
#include "logic.h"
#include "widget.h"
#include "network.h"


class Base: public QObject
{
    Q_OBJECT
public:
    Base();

private:
    Logic _logic;
    Widget _gui;
    Network _network;

};

#endif // BASE_H
