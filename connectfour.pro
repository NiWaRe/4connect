#-------------------------------------------------
#
# Project created by QtCreator 2018-09-22T10:43:07
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = connectfour
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    base.cpp \
    logic.cpp \
    network.cpp \
    game.cpp \
    board.cpp \
    logic_connection.cpp \
    widget.cpp \
    client.cpp \
    net.cpp \
    server.cpp \
    dialog.cpp \
    myRect.cpp \
    stone.cpp \
    token.cpp \
    win.cpp \
    netbroadcast.cpp

HEADERS += \
        mainwindow.h \
    base.h \
    logic.h \
    network.h \
    game.h \
    board.h \
    logic_connection.h \
    enums_structs.h \
    widget.h \
    client.h \
    net.h \
    server.h \
    dialog.cpp.autosave \
    dialog.h \
    myRect.h \
    stone.h \
    token.h \
    win.h \
    netbroadcast.h

FORMS += \
        mainwindow.ui \
    dialog.ui \
    stone.ui \
    widget.ui \
    win.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    uml_trial.qmodel

STATECHARTS +=

# QMAKE_MAC_SDK = macosx10.14
