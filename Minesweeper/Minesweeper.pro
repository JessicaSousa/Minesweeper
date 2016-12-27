# -------------------------------------------------
# Project created by QtCreator 2010-10-28T13:39:17
# -------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += sql
TARGET = Minesweeper
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    minesweeper.cpp \
    helpwindow.cpp \
    aboutwindow.cpp \
    minesweeperbutton.cpp \
    savescore.cpp \
    topten.cpp
HEADERS += mainwindow.h \
    minesweeper.h \
    helpwindow.h \
    aboutwindow.h \
    minesweeperbutton.h \
    savescore.h \
    topten.h
FORMS += mainwindow.ui \
    helpwindow.ui \
    aboutwindow.ui \
    savescore.ui \
    topten.ui
RESOURCES += images.qrc
