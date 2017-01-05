/*
 * Game.cpp
 *
 *  Created on: 31 de dez de 2016
 *      Author: jcardoso
 */

#include "Game.h"


Game::Game(int nrows, int ncols, int difficulty, bool verbose){

	this->verbose = verbose;
	this->ncols = ncols;
	this->nrows = nrows;

	switch(difficulty){
		case 1:
			if (nrows * ncols < 10)
				this->numberOfMines = 2;
			else if (nrows*ncols < 20)
				this->numberOfMines = 3;
			else if (nrows * ncols < 30)
				this->numberOfMines = 5;
			else if (nrows * ncols < 100)
				this->numberOfMines = 10;
			else
				this->numberOfMines = 15;
			break;
		case 2:
			if (nrows * ncols < 30)
				this->numberOfMines = 10;
			else if (nrows * ncols < 100)
				this->numberOfMines = 15;
			else
				this->numberOfMines = 20;
			break;
		case 3:
			if (nrows * ncols < 30)
				this->numberOfMines = 15;
			else if (nrows * ncols < 100)
				this->numberOfMines = 20;
			else
				this->numberOfMines = 30;
			break;
		default:
			std::cout << "Your level input is wrong!\n";
			return;
			break;
	 }

	this->game = Minesweeper(ncols, nrows, this->numberOfMines);

	//Inicializa o campo com as dimensões informadas
	this->mineBoard.resize(nrows);
	for (int i = 0; i < nrows; ++i)
		this->mineBoard[i].resize(ncols);

	//popula o campo
	for(int i = 0; i < nrows; i++)
		for(int j = 0; j < ncols; j++)
			this->mineBoard[i][j] = COVERED;

	if (verbose)
	    std::cout << "Playing on " << nrows<<" x "<< ncols <<" board with difficulty " << difficulty << "\n";

}

void Game::openSafeCells(int row, int col){

	if(this->mineBoard[row][col] != COVERED){
		return;
	}

	//Remove square que se encontra dentro de frontier
	this->frontier.erase(std::remove(this->frontier.begin(),
	                                 this->frontier.end(), std::make_pair(row,col)),
	                                 this->frontier.end());

	this->mineBoard[row][col] = this->game.getValue(row,col);
	this->cellsRevealed++;

	//Obtém vizinhança do quadrado
	std::vector<std::pair<int,int>> neigbourlist = this->getNeighbors(row,col);

	if(this->mineBoard[row][col] == 0){

		for(std::pair<int,int> coord: neigbourlist ){
			openSafeCells(coord.first, coord.second);
		}

	}else{

		for(std::pair<int,int> coord: neigbourlist ){

			//Procura elemento em frontier
			bool found = ( std::find(frontier.begin(), frontier.end(), coord) != frontier.end());
			//Obtém valor do elemento
			int value =  this->mineBoard[coord.first][coord.second];
			if(value == COVERED && !found ){
				this->frontier.push_back(coord);
			}
		}
	}

}

void Game::lost(){

	for(int i = 0; i < nrows; i ++){
		for(int j = 0; j < ncols; j++){
			if(this->mineBoard[i][j] == COVERED){
				this->mineBoard[i][j] = (this->isMine(i,j)) ? MINE : COVERED;
			}
		}
	}
}

void Game::makeMove(int row, int col){

	if(row < 0 && row >= nrows)
		return;
	if(col <0 && col >= ncols)
		return;

	//Se célula ainda não foi revelada
	if(this->mineBoard[row][col] == COVERED){
		if(this->game.isMine(row,col)){
			lost();
			this->gameEnd = true;
		}
		else{
			openSafeCells(row,col);
		}
	}

	//Se o campo foi limpo, então venceu
	if(this->cellsRevealed == ((this->nrows*this->ncols) - this->numberOfMines)){
		this->gameEnd = true;
		this->gameWon = true;
	}

	if (this->verbose){
			std::vector<std::vector<int>> s = this->getMineBoard();

			std::cout << "Move: ";
			std::cout << "(" << row << "," << col <<")";

			std::cout << "\nState: ";
			for(int i = 0; i < nrows; i++){
				for(int j = 0; j < ncols; j++){
					std::cout << s[i][j] << " ";
				}
			}
			std::cout << "\n";

			std::cout << "Frontier: ";
			std::vector<std::pair<int,int>> f = this->getFrontier();
			for(std::pair<int,int> i: f){
				std::cout <<"("<< i.first << "," << i.second << ") ";
			}
			std::cout << "\n";


		}

}

//Obtém coordenadas dos vizinhos de uma célula
std::vector<std::pair<int,int>> Game::getNeighbors(int row, int col){
	std::vector<std::pair<int,int>> neighbors;

	for(int r = row - 1; r <  row + 2; r++){
		for(int c = col - 1; c < col + 2; c++){
			if(r == row && c == col){
				continue;
			}
			if(r >= 0 && r < this->nrows && c >= 0 && c < this->ncols){
				neighbors.push_back(std::make_pair(r,c));
			}
		}
	}
	return neighbors;
}

//Retorna o campo do jogo (pode contar isso como estado)
std::vector<std::vector<int> > Game::getMineBoard(){
	return this-> mineBoard;
}

bool Game::isGameEnd(){
	return this->gameEnd;
}

bool Game::isWon(){
	return this->gameWon;
}

std::vector<std::pair<int,int>> Game::getFrontier(){
	return this->frontier;
}

bool Game::isMine(int row, int col){
	return this->game.isMine(row,col);
}
