#include "mainwindow.h"
#include <QApplication>
#include "base.h"
#include "enums_structs.h"
#include "netbroadcast.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Base base;

    //Ausführen des Broadcasting Funktionalität
    //netBroadcast *nB = new netBroadcast();
    return a.exec();
}
