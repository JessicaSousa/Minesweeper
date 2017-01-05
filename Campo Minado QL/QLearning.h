/*
 * QLearning.h
 *
 *  Created on: 2 de jan de 2017
 *      Author: jcardoso
 */

#ifndef QLEARNING_H_
#define QLEARNING_H_

#include <map>
#include <fstream>      // std::ofstream
#include <sstream>	   //sstream
#include "Random.h"
#include "Game.h"

typedef std::pair<int,int> Tuple;
typedef std::pair<std::vector<std::vector<int>>,Tuple> QPair;
typedef std::map<QPair, int> QTable;


//QLearning modificado

typedef std::map<std::vector<std::vector<int>>,int> QState;
typedef std::pair<QTable,QState> QPairMapState;


Tuple Get2DCoordFromIndex(int index,int ncols);
int GetIndexFrom2DCoord(Tuple coord,int ncols);


Tuple RandomMove(std::vector<std::vector<int>> board, std::vector<Tuple> frontier);

namespace SimpleQL{

	QTable TrainQValues(int num_total_simulations = 100, int row = 4, int col = 4, int difficulty = 1, int reward = 1);
	Tuple GetNextMove(QTable qMap, Game game);

	QTable ReadQTable(std::string filepath,int nRows, int nCols);
	void WriteQTable(std::string filename, QTable table);

}


namespace ModifiedQL{
	QPairMapState TrainQValues(int num_total_simulations = 100, int row = 4, int col = 4, int difficulty = 1, int reward = 1);
	
	Tuple GetNextMove(QTable qMap, QState qStateCounter, Game game);
	std::vector<int> GetLabel(Game game);
	
	QState ReadQState(std::string filepath,int nRows, int nCols);
	void WriteQState(std::string filename, QState table);
}




#endif /* QLEARNING_H_ */
