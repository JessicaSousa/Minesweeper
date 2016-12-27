/**
 * Minesweeper header files
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */

#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QDialog>

namespace Ui {
    class HelpWindow;
}

class HelpWindow : public QDialog {
    Q_OBJECT
public:
    HelpWindow(QWidget *parent = 0);
    ~HelpWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::HelpWindow *ui;
};

#endif // HELPWINDOW_H
