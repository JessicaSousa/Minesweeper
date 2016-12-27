/**
 * Minesweeper
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSignalMapper>
#include "minesweeper.h"
#include "minesweeperbutton.h"
#include "savescore.h"
#include "topten.h"
#include "aboutwindow.h"
#include "helpwindow.h"
#include <QTimer>
#include <QDebug>

int gridHeight = 4;
int gridWidth = 4;
int numberOfMines = 3;

/**
  * Constructor for MainWindow. It will initialize the entire board and create the necessary starting elements for the game
  */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //Global variables
    hasFinished = false; //Has the game finished?
    cellsRevealed = 0; //Number of current cells revealed
    flagsFlagged = 0; //Number of flags that have been flagged
    minesFlagged = 0; //Number of mines that have been flagged
    hasStarted = false; //If the game has started yet
    currentTime = 0; //The current time in seconds

    ui->setupUi(this);


    //Layout designs
    ui->mineContainer->setSpacing(0); //Forces the board cells to be spaced next to each other

    //Timer for the number of seconds that has passed by
    timer = new QTimer();

    //The display of the number of flags that have been put up (Mines left to solve)
    ui->lcdFlagCount->setDigitCount(3);
    ui->lcdFlagCount->display ( numberOfMines - flagsFlagged );

    //Initialize statuses
    // 0 = Empty, 1 = flagged, 2 = ?
    this->mineStatus.resize(gridHeight);
    for (int i = 0; i < gridHeight; ++i)
        this->mineStatus[i].resize(gridWidth);

    //Connect the UI elements
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(handleHelpButton()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(handleAboutButton()));
    connect(ui->action_Reset, SIGNAL(triggered()), this, SLOT(reset()));
    connect(ui->smileyFace, SIGNAL(clicked()), this, SLOT(handleSmileyFace()));
    connect(ui->actionTop_Ten, SIGNAL(triggered()), this, SLOT(handleTopTen()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));

    //Now handle the actual game.. enough of this extra feature stuff. Now for the real deal!
    game = new Minesweeper(gridWidth,gridHeight,numberOfMines);

    //We will need to map the click to an object's coordinates
    signalMapper = new QSignalMapper(this);
    signalMapper2 = new QSignalMapper(this);

    //Generate all the buttons for the game
    for( int i = 0; i < gridHeight; i++)
    {
        for( int j = 0; j < gridWidth; j++ )
        {
            MineSweeperButton* button = new MineSweeperButton("");

            //Button Styling
            button->setAttribute(Qt::WA_LayoutUsesWidgetRect); //Forces Mac OS X styled minesweeper to look like linux/windows
            button->setMaximumHeight(30);
            button->setMaximumWidth(30);
            button->setIcon (QIcon(QString(":/images/not_flat_button.png")));
            button->setIconSize (QSize(30,30));
            button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

            //Actually add the button to the container
            ui->mineContainer->addWidget(button, i, j);
            QString coordinates = QString::number(i)+","+QString::number(j); //Coordinate of the button
            //Map the coordinates to a particular MineSweeperButton
            signalMapper->setMapping(button, coordinates);
            signalMapper2->setMapping(button, coordinates);

            //Connections for the buttons
            connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
            connect(button, SIGNAL(rightButtonClicked()), signalMapper2, SLOT(map()));
            connect(button, SIGNAL(pressed()), this, SLOT(handleButtonPressed()));
            connect(button, SIGNAL(released()), this, SLOT(handleButtonReleased()));
        }
    }

    this->setMaximumHeight(30*gridHeight);
    this->setMaximumWidth(30*gridWidth);

    //Connect the signal mapper to this class so that we can handle its clicks
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(revealCell(QString))); //Left click
    connect(signalMapper2, SIGNAL(mapped(QString)), this, SLOT(hasRightClicked(QString))); //Right click
}

/**
  * hasRightClicked()
  * This is a slot that takes a coordinate of what was just right clicked and then handles the action
  * @param QString - The coordinates of the button that was clicked
  */
void MainWindow::hasRightClicked(QString coordinates)
{
    //If we've finished the game, we don't do anything
    if (hasFinished) return;

    //Retrieve the button that was pushed from the signal mapper
    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

    // Obtain its coordinates
    QStringList results = coordinates.split(",");

    int row = results.at(0).toInt();
    int column = results.at(1).toInt();

    //At this point, if we right click we are starting the game
    if ( !hasStarted )
    {
        timer->start(1000);
        hasStarted = true;
    }

    //If we're not flat, it means that this button has not been pushed yet
    if (! buttonPushed->isFlat() )
    {
            //0 = flat, 1 = flat with flag, 2 = ?
            if ( mineStatus[row][column] == BLANK_CELL)
            {
                    //We are now flagging the cell as it was blank
                    flagsFlagged++;

                    buttonPushed->setIcon (QIcon(QString(":/images/flag_no_flat_button.png")));
                    mineStatus[row][column] = FLAGGED_CELL;

                    ui->lcdFlagCount->display( numberOfMines - flagsFlagged );

                    //Did we get a mine?
                    if ( game->getValue(row, column) == MINE ) {
                        minesFlagged++;
                    }

            } else if ( mineStatus[row][column] == FLAGGED_CELL )
            {
                //We are now making the thing a question mark
                flagsFlagged--;
                ui->lcdFlagCount->display( numberOfMines - flagsFlagged ); //No longer flagged so we are going back up

                buttonPushed->setIcon (QIcon(":/images/unknown_no_flat_button.png"));
                mineStatus[row][column] = QUESTION_CELL;
            } else if ( mineStatus[row][column] == QUESTION_CELL )
            {
                ui->lcdFlagCount->display( numberOfMines - flagsFlagged );
                buttonPushed->setIcon (QIcon(QString(":/images/not_flat_button.png")));
                mineStatus[row][column] = BLANK_CELL;
            }
    } else {
        //The cell has been pushed now
        buttonPushed->setFlat(true);
    }
}

/**
  * updateTimer()
  * Increments the current game time and updates the LCD display this is run every second
  */
void MainWindow::updateTimer() {
        currentTime++;
        ui->lcdTimer->display(currentTime);
}

/**
  * revealCell()
  * Reveals the cell at the given coordinate, also handles losing / winning
  */
void MainWindow::revealCell(QString coordinates)
{
    //Obtain the coordinates
    QStringList results = coordinates.split(",");

    if ( results.size() != 2) //Ensure that we receive two coordinates
        qFatal("Less than two numbers received");

    int row = results.at(0).toInt();
    int column = results.at(1).toInt();


    // The first click in Minesweeper is always safe
    if ( !hasStarted )
    {
        game->safeClick(row,column);
    }

    //If we already finished the game.. we won't do anything here
    if (hasFinished)
    {
        //colocar parar contagem
        return;
    }
    else
    {
        cellsRevealed++;
    }

    //If it is flagged, we will ignore the mine
    if ( mineStatus[row][column] == FLAGGED_CELL || mineStatus[row][column] == QUESTION_CELL )
    {
            cellsRevealed--;
            return;
    }

    //Get the button we just pushed
    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));


    //If it is flat.. we already pushed it so ignore it
    if (buttonPushed->isFlat())
    {
            cellsRevealed--;
            return;
    }

    //If we have (gridHeight*gridWidth - numberOfMines) cells revealed (numberOfMines mines, gridHeight*gridWidth not mines), we win the game!
    if (cellsRevealed == (gridHeight*gridWidth - numberOfMines) && game->getValue(row, column) != MINE)
    {
        changeIcon(buttonPushed, row, column);
        won();
        return;
    }

    //Recrusively clear the squares if we reveal a zero
    if ( game->getValue (row, column) == 0 ) {
        cellsRevealed--;
        clear(row, column, true);
    }

    //Set the image according to the value of the cell
    changeIcon(buttonPushed, row, column);

    buttonPushed->setFlat(true);    // if revealed, set button flat

    //If we reveal a mine, we just lost :(
    if ( game->isMine( row, column ) )
    {
        lost();
        cellsRevealed--;
        return;
    } else {
        //Phew, it's not a mine! We're happy!!!!!!! :D
        ui->smileyFace->setIcon(QIcon(":/images/normal_face.png"));
    }


    //If we haven't started yet, let's start the counter
    if ( !hasStarted )
    {
        //game->safeClick(row,column);
        timer->start(1000);
        hasStarted = true;

    }
}

/**
  * changeIcon(MineSweeperButton, int, int)
  * Changes the icon of a given button to the icon of the number that it contains / mine
  * @param MineSweeperButton - The button to change
  * @param int - The row to inspect
  * @param int - The column to inspect
  */
void MainWindow::changeIcon(MineSweeperButton *buttonPushed, int row, int column)
{
    //Set the image according to the value of the cell
    if ( game->getValue (row, column) == 0)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/flat_button.png")));
    } else if ( game->getValue (row,column) == 1)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/one_flat_button.png")));
    } else if (game->getValue (row,column) == 2)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/two_flat_button.png")));
    } else if (game->getValue (row,column) == 3)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/three_flat_button.png")));
    } else if (game->getValue (row,column) == 4)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/four_flat_button.png")));
    } else if (game->getValue (row,column) == 5)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/five_flat_button.png")));
    } else if (game->getValue (row,column) == 6)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/six_flat_button.png")));
    } else if (game->getValue (row,column) == 7)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/seven_flat_button.png")));
    } else if (game->getValue (row,column) == 8)
    {
        buttonPushed->setIcon (QIcon(QString(":/images/eight_flat_button.png")));
    } else if (game->getValue(row, column) == MINE) {
        buttonPushed->setIcon (QIcon(QString(":/images/mine_exploded_flat_button.png")));
    }
}

/**
  * clear(int row, int column, bool allowedClear)
  * clear recursively reveals all zeros and reveals its neighbors that are not mines
  * @param int - The row to inspect
  * @param int - The column to inspect
  * @param bool - If we are allowed to clear the particular cell. This is used when recursively clearing something not a zero -- it is a neighbor of a zero so it is not allowed to clear others
  */
void MainWindow::clear(int row, int column, bool allowedClear)
{
    //Retrieve the button
    QString coordinates = QString::number(row) + "," + QString::number(column);

    MineSweeperButton *buttonPushed = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

    //Ensure that the button is not flat, that the value is not a mine, that it is allowed to clear (not something that isn't a zero) and it isn't flagged
    if ( buttonPushed->isFlat () == false && game->getValue (row, column) != MINE && allowedClear == true && mineStatus[row][column] != FLAGGED_CELL)
    {
        buttonPushed->setFlat (true); //We're now flat

        //Set the image according to the value of the cell
        changeIcon(buttonPushed, row, column);

        cellsRevealed++;

        //If we have 90 cells revealed (10 mines, 90 not mines), we win the game!
        if (cellsRevealed == (gridHeight*gridWidth - numberOfMines) && game->getValue(row, column) != MINE)
        {
            changeIcon(buttonPushed, row, column);
            won();
            return;
        }   // in case clearing occurs in the end of the game.

        //Make sure that we are allowed to clear
        if ( game->getValue (row, column) == 0)
            allowedClear = true;
        else
            allowedClear = false;

        //Recrusively call clear on all adjacent cells
        //Top left
        if ( (row-1) != -1 && (column -1) != -1)
            clear(row-1, column-1, allowedClear);
        //Top center
        if ( (row-1) != -1)
            clear(row-1, column, allowedClear);
        //Top right
        if ( (row-1) != -1 && (column + 1) != gridWidth)
            clear(row-1, column+1, allowedClear);
        //Left
        if ( (column -1) != -1)
            clear(row, column-1, allowedClear);
        //Right
        if ( (column + 1) != gridWidth)
            clear(row, column+1, allowedClear);
        //Bottom left
        if ( (row+1) != gridHeight && (column -1) != -1)
            clear(row+1, column-1, allowedClear);
        //Bottom center
        if ( (row+1) != gridHeight)
            clear(row+1, column, allowedClear);
        //Bottom right
        if ( (row+1) != gridHeight && (column+1) != gridWidth)
            clear(row+1, column+1, allowedClear);
    }
}

/**
  * lost()
  * Handles losing the game
  */
void MainWindow::lost() {
    ui->smileyFace->setIcon(QIcon(":/images/sad_face.png")); //I HAS THE SADS :(
    timer->stop();
    hasFinished = true;

    //Go through all the cells and reveal all the mines
    for ( int i = 0; i < gridHeight; i++ )
    {
        for ( int j = 0; j < gridWidth; j++ )
        {
            //Get the coordinates and obtain the button
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

            //Check if it's a mine, if it is.. show it!
            if (! button->isFlat () && game->getValue (i,j) == MINE )
            {
                button->setFlat (true);

                //Are we flagged? Good job! you find a mine and flagged it :)
                if ( mineStatus[i][j] == FLAGGED_CELL ) {
                    button->setIcon (QIcon(QString(":/images/mine_disarmed_flat_button.png")));
                } else {
                    //BOO!! You didn't find this mine!
                    button->setIcon (QIcon(QString(":/images/mine_flat_button.png")));
                }
            }

        }
    }

    //We're done with this!
    hasFinished = true;
}

/**
  * reset()
  * Resets the game board
  */
void MainWindow::reset() {
    //Reset global variables
    hasFinished = false;
    cellsRevealed = 0;
    flagsFlagged = 0;
    minesFlagged = 0;
    hasStarted = false;
    currentTime = 0;

    //Stop the timer
    this->timer->stop();

    ui->lcdFlagCount->display( numberOfMines - flagsFlagged );
    ui->lcdTimer->display(currentTime); //Set time to zero

    //Reset smiley to :)
    ui->smileyFace->setIcon(QIcon(":/images/normal_face.png"));

    //Create a new game object
    game = new Minesweeper(gridWidth,gridHeight,numberOfMines);

    //Go through all the cells and reset the icons
    for( int i = 0; i < gridHeight; i++)
    {
        for( int j = 0; j < gridWidth; j++ )
        {
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

            button->setIcon (QIcon(":/images/not_flat_button.png"));
            button->setIconSize (QSize(30,30));
            button->setFlat(false);
            mineStatus[i][j] = BLANK_CELL;
        }
    }
}

/**
  * won()
  * Handles winning the game
  */
void MainWindow::won()
{
    //Happy smiley face!!! :D
    ui->smileyFace->setIcon(QIcon(":/images/happy_face.png"));

    //Stop the timer, we're done
    timer->stop();
    hasFinished = true;

    //Set all the mines to disarmed
    for ( int i = 0; i < gridHeight; i++ )
    {
        for ( int j = 0; j < gridWidth; j++ )
        {
            QString coordinates = QString::number(i)+","+QString::number(j);
            MineSweeperButton *button = qobject_cast<MineSweeperButton *>(signalMapper->mapping(coordinates));

            if (! button->isFlat () && game->getValue(i, j) == MINE )
            {
                button->setFlat (true);
                button->setIcon (QIcon(QString(":/images/mine_disarmed_flat_button.png")));
            }

        }
    }

    //Show the save the score screen
    SaveScore* scoreScreen = new SaveScore(this->currentTime);
    scoreScreen->show();
}

/**
  * handleSmileyFace()
  * Handles the smiley face when clicked which is reset the board
  */
void MainWindow::handleSmileyFace()
{
    this->reset();
}

/**
  * handleButtonPressed()
  * Handles when a button is pressed
  */
void MainWindow::handleButtonPressed()
{
    //If we have finished.. nothing happens
    if (hasFinished)
    {
        return;
    }

    ui->smileyFace->setIcon(QIcon(":/images/scared_face.png")); //Oh my.. the suspense is building! :o

}

/**
  * handleTopTen()
  * Handles the top ten player dialog
  */
void MainWindow::handleTopTen()
{
    TopTen* scores = new TopTen();
    scores->show();
}

/**
  * handleHelpButton()
  * Handles the help button dialog
  */
void MainWindow::handleHelpButton()
{
    HelpWindow* help = new HelpWindow();
    help->show();
}

/**
  * handleAboutButton()
  * Handles the about button dialog
  */
void MainWindow::handleAboutButton()
{
    AboutWindow* aboutWindow = new AboutWindow();
    aboutWindow->show();
}

/**
  * handleButtonReleased()
  * Handles when a cell is released
  */
void MainWindow::handleButtonReleased()
{
    if (hasFinished) {
        return;
    }

    ui->smileyFace->setIcon(QIcon(":/images/normal_face.png")); //Phew didn't click on that!
}

/**
  * Destructor
  */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
  * changeEvent()
  * Changes based on given event
  */
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
