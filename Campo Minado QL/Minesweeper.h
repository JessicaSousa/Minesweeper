#ifndef MINESWEEPER_H
#define MINESWEEPER_H
#include <vector>
#include <iostream> //cout

#define MINE 9

class Minesweeper
{
public:
    Minesweeper(int gridWidth, int gridHeight, int numberOfMines);
    Minesweeper();
    ~Minesweeper();
    bool isMine(int, int);
    int getValue(int, int);

private:
    std::vector<std::vector<int> > mineBoard;
    void fillSurroundings(int,int);
    void generateBoard();
    int gridWidth;
    int gridHeight;
    int numberOfMines;
};

#endif // MINESWEEPER_H
