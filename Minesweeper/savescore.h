/**
 * Minesweeper header files
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */

#ifndef SAVESCORE_H
#define SAVESCORE_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
    class SaveScore;
}

class SaveScore : public QDialog {
    Q_OBJECT
public:
    SaveScore(int, QWidget *parent = 0);
    ~SaveScore();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SaveScore *ui;
    int score;

private slots:
        void save();
};

#endif // SAVESCORE_H
