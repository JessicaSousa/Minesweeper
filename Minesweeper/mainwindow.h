/**
 * Minesweeper header files
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define BLANK_CELL 0
#define FLAGGED_CELL 1
#define QUESTION_CELL 2

#include <iostream>
#include <QMainWindow>
#include <QSignalMapper>
#include "minesweeper.h"
#include "minesweeperbutton.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    Minesweeper *game;
    QSignalMapper *signalMapper;
    QSignalMapper *signalMapper2;

    QTimer *timer;
    int currentTime;
    bool hasStarted;
    bool hasFinished;
    int cellsRevealed;
    int minesFlagged;
    int flagsFlagged;
    void lost();
    void won();
    void clear(int, int, bool);
    std::vector<std::vector<int> > mineStatus;
    void changeIcon(MineSweeperButton*, int, int);

private slots:
    void updateTimer();
    void hasRightClicked(QString);
    void reset();
    void handleSmileyFace();
    void handleTopTen();
    void handleAboutButton();
    void handleHelpButton();
    void handleButtonPressed();
    void handleButtonReleased();
    //void revealAll(QString);

public slots:

    void revealCell(QString);

};

#endif // MAINWINDOW_H
