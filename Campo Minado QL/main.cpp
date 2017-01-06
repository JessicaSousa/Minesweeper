/*
 * main.cpp
 *
 *  Created on: 31 de dez de 2016
 *      Author: jcardoso
 */

#include <iostream>
#include <limits>
#include <random>
#include <map>
#include <string>  //std::to_string
#include "Game.h"
#include "Random.h"
#include "QLearning.h"


using namespace std;

double sumUncovered(std::vector<std::vector<int>> vec){
	
	double sum = 0.0;
	
	for(int i = 0; i < vec.size(); i++){
		for(int j = 0; j < vec[0].size(); j++){
			if(vec[i][j] != COVERED)
				sum++;
		}
	}
	return sum;
}

int main(){

	Random::seed();

	int numLearningIterations = 1000;
	int numPlayingIterations = 1;
	int numRows = 4;
	int numCols = 4;
	int difficulty = 1;

	//Gerar Q-Values Table
	QPairMapState qTable = ModifiedQL::TrainQValues(numLearningIterations, numRows, numCols,difficulty);
	
	for(int i = 0; i < numPlayingIterations; i++){
		
		if (i % 10 == 0)
			std::cout << "Playing " << i <<"th testing game.\n";
		
		Game game(numRows, numCols,difficulty);
		Tuple firstMove(0, 0);
	
		//Garantir sempre o primeiro clique seguro
		while(game.isMine(firstMove.first, firstMove.second)){
			game = Game(numRows, numCols,difficulty);
		}
	
		game.makeMove(firstMove.first, firstMove.second);
		
		//Obter Estado atual do jogo
		std::vector<std::vector<int>> currentState = game.getMineBoard();
	
		//Enquanto o jogo não chegar ao fim
		while(!game.isGameEnd()){
			//Obter ação para realizar próximo movimento
			Tuple nextMove = ModifiedQL::GetNextMove(qTable.first, qTable.second, game);
			//Realiza jogada
			game.makeMove(nextMove.first, nextMove.second);
			//Obtém estado atual do jogo
			currentState = game.getMineBoard();
		}			
	    
	}

	return 0;
}

