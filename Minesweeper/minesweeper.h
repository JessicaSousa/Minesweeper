/**
 * Minesweeper header files
 * @author Stephen Liang
 * @author Aisha Halim
 * Created for CS 340 Fall 2010 - Professor Troy
 */

#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#include <vector>

#define MINE 9

class Minesweeper
{
public:
    Minesweeper(int gridWidth, int gridHeight, int numberOfMines);
    ~Minesweeper();
    bool isMine(int, int);
    int getValue(int, int);
    void safeClick(int,int);
private:
    std::vector<std::vector<int> > mineBoard;
    void fillSurroundings(int,int,bool);
    void generateBoard();
    int gridWidth;
    int gridHeight;
    int numberOfMines;
};

#endif // MINESWEEPER_H
