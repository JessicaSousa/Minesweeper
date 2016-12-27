/**
 * Main
 * We drive the game. yep.
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */
#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    MainWindow w;
    QIcon icon(":/images/awesome_face.png");
    w.setWindowIcon(icon);
    w.show();

    return a.exec();
}
