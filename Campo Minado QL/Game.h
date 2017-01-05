/*
 * Game.h
 *
 *  Created on: 31 de dez de 2016
 *      Author: jcardoso
 */

#ifndef GAME_H_
#define GAME_H_

#include <algorithm>
#include "Minesweeper.h"

#define COVERED 10

class Game{

private:

	int score = 0;
	bool gameEnd = false;
	bool gameWon = false;

	int ncols;
	int nrows;
	int cellsRevealed = 0;
    int numberOfMines;


	std::vector<std::vector<int> > mineBoard;
	std::vector<std::pair<int,int>> frontier;

	Minesweeper game;


	void openSafeCells(int row, int col);
	void lost();
	std::vector<std::pair<int,int>> getNeighbors(int row, int col);

public:

	int getRowSize(){return nrows;};
	int getColumnSize(){return ncols;};


	Game(int nrows = 4,int ncols = 4, int difficulty = 1, bool verbose = false);
	void makeMove(int row, int col);

	bool verbose;

	bool isGameEnd();

	bool isWon();

	bool isMine(int row, int col);

	std::vector<std::pair<int,int>> getFrontier();

	std::vector<std::vector<int>> getMineBoard();

};



#endif /* GAME_H_ */
