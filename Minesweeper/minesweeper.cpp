/**
 * MineSweeper
 * Handles the minesweeper game, contains the actual board used
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */
#include "minesweeper.h"
#include <ctime>
#include <QtGlobal>

/**
  * Constructor
  * Initializes the board
  */
Minesweeper::Minesweeper(int gridWidth, int gridHeight, int numberOfMines)
{
    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;

    this->numberOfMines = numberOfMines;
    //Initialize board
    mineBoard.resize(gridHeight);
    for (int i = 0; i < gridHeight; ++i)
        mineBoard[i].resize(gridWidth);
    //Generates the board with mines and populates the number of mines around it
    generateBoard();
}

/**
  * Destructor
  */
Minesweeper::~Minesweeper()
{
}


/**
  * fillSurroundings(int, int,bool)
  * Determines if there is a mine
  * @param int - Row to inspect
  * @param int - Column to inspect
  * @param bool - Increment( true : increment the numbers of mines, false: decrement the numbers of mines)
  */
void Minesweeper::fillSurroundings(int row, int column, bool increment)
{
    //Top left
    if ( (row-1) != -1 && (column -1) != -1 && mineBoard.at(row-1).at(column-1)!= MINE)
    {
        if(increment == true)
            mineBoard.at(row-1).at(column-1)++;
        else
            mineBoard.at(row-1).at(column-1)--;
    }
    //Top center
    if ( (row-1) != -1 && mineBoard.at(row-1).at(column) != MINE)
    {
        if(increment == true)
            mineBoard.at(row-1).at(column)++;
        else
            mineBoard.at(row-1).at(column)--;
    }
    //Top right
    if ( (row-1) != -1 && (column + 1) != gridWidth && mineBoard.at(row-1).at(column+1) != MINE)
    {
        if(increment == true)
            mineBoard.at(row-1).at(column+1)++;
        else
            mineBoard.at(row-1).at(column+1)--;
    }
    //Left
    if ( (column -1) != -1 && mineBoard.at(row).at(column-1) != MINE)
    {
        if(increment == true)
            mineBoard.at(row).at(column-1)++;
        else
            mineBoard.at(row).at(column-1)--;
    }
    //Right
    if ( (column + 1) != gridWidth && mineBoard.at(row).at(column+1) != MINE)
    {
        if(increment == true)
            mineBoard.at(row).at(column+1)++;
        else
            mineBoard.at(row).at(column+1)--;
    }
    //Bottom left
    if ( (row+1) != gridHeight && (column -1) != -1 && mineBoard.at(row+1).at(column-1) != MINE)
    {
        if(increment == true)
            mineBoard.at(row+1).at(column-1)++;
        else
            mineBoard.at(row+1).at(column-1)--;
    }
    //Bottom center
    if ( (row+1) != gridHeight && mineBoard.at(row+1).at(column) != MINE)
    {
        if(increment == true)
            mineBoard.at(row+1).at(column)++;
        else
            mineBoard.at(row+1).at(column)--;
    }
    //Bottom right
    if ( (row+1) != gridHeight && (column+1) != gridWidth && mineBoard.at(row+1).at(column+1) != MINE)
    {
        if(increment == true)
            mineBoard.at(row+1).at(column+1)++;
        else
            mineBoard.at(row+1).at(column+1)--;
    }
}

/**
  * generateBoard()
  * Generates the board with mines and populates the number of mines around it
  */
void Minesweeper::generateBoard()
{
    /* initialize random seed: */
    qsrand (time(NULL));
    //Generate the mines and create its adjacent number list
    for( int i = 0; i < numberOfMines; i++)
    {
        int row, column;
        //Prevent generating mine on another mine
        do
        {
            row    = qrand() % gridHeight;
            column = qrand() % gridWidth;

        }while(mineBoard.at(row).at(column) == MINE); //Number 9 indiciates a mine

        mineBoard.at(row).at(column) = MINE; //Number 9 indiciates a mine

        //Now that we have a mine created, increment its surroundings
        //There are eight of them
        fillSurroundings(row,column,true);
    }
}


/**
  * safeClick(int, int)
  * Determines if there is a mine
  * @param int - Row to inspect
  * @param int - Column to inspect
  */
void Minesweeper::safeClick(int row, int column)
{
        //Ensure the input is sanitary
        if ( row < 0 || row > (gridHeight - 1))
            qFatal("Error in input");
        if ( column < 0 || column > (gridWidth - 1))
            qFatal("Error in input");

        int new_row, new_column;

        if(mineBoard.at(row).at(column) == MINE)
        {
            /* initialize random seed: */
            qsrand (time(NULL));

            do
            {
               new_row    = qrand() % gridHeight;
               new_column = qrand() % gridWidth;

            }while(mineBoard.at(new_row).at(new_column) == MINE); //Number 9 indiciates a mine

            mineBoard.at(new_row).at(new_column) = MINE; //Number 9 indiciates a mine

            //Now that we have a mine created, increment its surroundings
            //There are eight of them
            fillSurroundings(new_row,new_column,true);

            mineBoard.at(row).at(column) = 0; //remove mine
            fillSurroundings(row,column,false);
        }


}

/**
  * isMine(int, int)
  * Determines if there is a mine
  * @param int - Row to inspect
  * @param int - Column to inspect
  * @return bool - Whether or not the location is a mine
  */
bool Minesweeper::isMine(int row, int column)
{
        //Ensure the input is sanitary
        if ( row < 0 || row > (gridHeight - 1))
            qFatal("Error in input");
        if ( column < 0 || column > (gridWidth - 1))
            qFatal("Error in input");
        return mineBoard.at(row).at(column) == MINE;
}

/**
  * getValue(int, int)
  * Returns the value of the given cell
  * @param int - Row to inspect
  * @param int - Column to inspect
  * @return int - The value at the row
  */
int Minesweeper::getValue(int row, int column)
{
    //Ensure the input is sanitary
    if ( row < 0 || row > (gridHeight - 1))
        qFatal("Error in input");
    if ( column < 0 || column > (gridWidth - 1))
        qFatal("Error in input");

    return mineBoard.at(row).at(column);
}
