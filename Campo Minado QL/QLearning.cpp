/*
 * QLearning.cpp
 *
 *  Created on: 2 de jan de 2017
 *      Author: jcardoso
 */
#include "QLearning.h"


Tuple Get2DCoordFromIndex(int index,int ncols){
	int i = index/ncols;
	int j = index - (i*ncols);
	return std::make_pair(i,j);
}

int GetIndexFrom2DCoord(Tuple coord,int ncols){
	int i = (coord.first)*ncols + coord.second;
	return i;
}

Tuple RandomMove(std::vector<std::vector<int>> board, std::vector<Tuple> frontier){

	int row = board.size();
	int col = board.front().size();

	while(true){
		int x = Random::RandInt(0,row-1);
	    	int y = Random::RandInt(0,col-1);
		Tuple move(x,y);
		
		bool found = (std::find(frontier.begin(), frontier.end(), move)) != frontier.end();

		if(!found)
			return move;
		if(board[x][y] != COVERED)
			return move;
	}

}



QTable SimpleQL::TrainQValues(int num_total_simulations, int row, int col, int difficulty, int rewardValue){

    QTable qValues;

    for(int iterationNo = 0; iterationNo < num_total_simulations; iterationNo++){

        if (iterationNo % 1000 == 0)
            std::cout << "Playing " << iterationNo <<"th training game.\n";

        Game game(row, col, difficulty);

        int x = Random::RandInt(0,row-1);
        int y = Random::RandInt(0,col-1);

        std::vector<std::vector<int>> currentState = game.getMineBoard();
        int reward = 0;


        while(true){

            reward = (!game.isMine(x,y)) ? rewardValue : -1*rewardValue;
            QPair stateAndAction(currentState, std::make_pair(x,y));
            qValues[stateAndAction] += reward;
            game.makeMove(x,y);
            currentState = game.getMineBoard();

            if(game.isGameEnd()) break;

            std::vector<std::pair<int,int>> frontier = game.getFrontier();

            Tuple nextMove = Random::RandomChoice(frontier);
            //Tuple nextMove = GetNextMove(qValues,game);
            x = nextMove.first;
            y = nextMove.second;
        }

    }
    
    return qValues;
}


Tuple SimpleQL::GetNextMove(QTable qMap, Game game){

    Tuple bestMoveLocation(-1,-1);

    float maxQValue = -std::numeric_limits<float>::infinity();

    std::vector<Tuple> possibleMoves = game.getFrontier();
    std::vector<std::vector<int>> currentState = game.getMineBoard();

    bool shouldPickRandomMove = true;

    for(Tuple move: possibleMoves){
        QPair stateAndAction(currentState, move);
        int q = qMap[stateAndAction];
        if (q > maxQValue){
            bestMoveLocation = move;
            maxQValue =  q;
        }
    }

    if (maxQValue > 0)
        shouldPickRandomMove = false;

    if (shouldPickRandomMove){
        return RandomMove(currentState, possibleMoves);
    }

   return bestMoveLocation;
}


QTable SimpleQL::ReadQTable(std::string filepath,int nRows, int nCols){

	QTable qTable;

	std::ifstream infile(filepath);

	std::string line;
	while (std::getline(infile, line))
	{
		std::vector<std::vector<int>> state(nRows, std::vector< int >(nCols));
		Tuple action;

		int reward;

		std::stringstream ss;
		std::stringstream   linestream(line);
		std::string value;

		int num;

		//Obter Estado
		std::getline(linestream, value, ';');
		ss.str(value);

		int index = 0;
		while ( ss >> num ){
			std::pair<int,int> coord = Get2DCoordFromIndex(index,nCols);
			state[coord.first][coord.second] = num;
			index++;
		}
		ss.clear();

		//Obter movimento
		std::getline(linestream, value, ';');
		ss.str(value);
		ss >> action.first >> action.second;
		ss.clear();

		//Obter Recompensa
		std::getline(linestream, value, ';');
		ss.str(value);
		ss>> reward;
		ss.clear();

		QPair pair(state,action);
		qTable[pair] = reward;

	}

	return qTable;
}

void SimpleQL::WriteQTable(std::string filename, QTable table){

	if(table.empty())
		return;

	std::ofstream out(filename.c_str(),std::ofstream::out);

	QTable::iterator it;

	for(it = table.begin(); it != table.end(); it++){

		std::vector<std::vector<int>> state = (*it).first.first;
		Tuple action = (*it).first.second;

		for(unsigned int i = 0; i < state.front().size(); i++){
			for(unsigned int j = 0; j < state.size(); j++){
				out << state[i][j] << " ";
			}
		}

		out << ";" << action.first << " " << action.second <<";" <<(*it).second << "\n";
	}

}


std::vector<int> ModifiedQL::GetLabel(Game game){

	std::vector<int> label;

	int row_size = game.getRowSize();
	int column_size = game.getColumnSize();
	std::vector<Tuple> frontier = game.getFrontier();


	for(int row = 0; row < row_size; row++){
		for(int col = 0; col < column_size; col++){
			Tuple coord(row,col);

		   	bool found = (std::find(frontier.begin(), frontier.end(), coord)) != frontier.end();

			if(found){
				if(!game.isMine(row,col)) label.push_back(1);
				else label.push_back(0);
			}else{
				label.push_back(0);
			}
		}
	}
	return label;
}

QPairMapState ModifiedQL::TrainQValues(int num_total_simulations, int row, int col, int difficulty, int reward){
	QTable qValues;
	QState qStateCounter;
    
	    for(int iterationNo = 0; iterationNo < num_total_simulations; iterationNo++){

		if (iterationNo % 1000 == 0)
		    std::cout << "Playing " << iterationNo <<"th training game.\n";

		Game game(row, col, difficulty);

		//Canto esquerdo superior
		int x = 0;
		int y = 0;

		//Realizar jogada
		game.makeMove(x,y);

		//Obter o estado atual do campo
		std::vector<std::vector<int>> currentState = game.getMineBoard();

		while(!game.isGameEnd()){
			//Obter a lista dos movimentos corretos
			std::vector<int> label = GetLabel(game);
			std::vector<int> listOfCorrectMoveIndices;
			//Indica que o estado foi visitado uma vez
			qStateCounter[currentState] += 1;

			for( unsigned int i = 0; i < label.size() ; i++ ){
				QPair stateAndAction(currentState, Get2DCoordFromIndex(i,col));

				if( label[i] == 1 ){
					qValues[stateAndAction] += reward;
					listOfCorrectMoveIndices.push_back(i);
				}
			}
			
			if(listOfCorrectMoveIndices.empty()){
				Tuple nextMove = RandomMove(currentState, game.getFrontier());

				x = nextMove.first;
				y = nextMove.second;

			}else{
				int index  = Random::RandomChoice(listOfCorrectMoveIndices);
				Tuple randomCorrectLocation = Get2DCoordFromIndex(index,col);
				x = randomCorrectLocation.first;
				y = randomCorrectLocation.second;
			}

			game.makeMove(x,y);
		    	currentState = game.getMineBoard();
		}
	    }
	return std::make_pair(qValues,qStateCounter);
}


Tuple ModifiedQL::GetNextMove(QTable qMap, QState qStateCounter, Game game){
	Tuple bestMoveLocation(-1,-1);

	float maxQValue = -std::numeric_limits<float>::infinity();

	std::vector<Tuple> possibleMoves = game.getFrontier();
	std::vector<std::vector<int>> currentState = game.getMineBoard();

	bool shouldPickRandomMove = true;

	for(auto move: possibleMoves){
		QPair stateAndAction(currentState, move);
		int q = qMap[stateAndAction];
		if (q > maxQValue){
		    bestMoveLocation = move;
		    maxQValue =  q;
		}
	}
	if (maxQValue > 0 || qStateCounter[currentState] == 0){
		shouldPickRandomMove = false;
	}
	if (shouldPickRandomMove){
		return RandomMove(currentState, possibleMoves);
	}
	return bestMoveLocation;
}


void ModifiedQL::WriteQState(std::string filename, QState table){

	if(table.empty())
		return;

	std::ofstream out(filename.c_str(),std::ofstream::out);

	QState::iterator it;

	for(it = table.begin(); it != table.end(); it++){
        
		std::vector<std::vector<int>> state = (*it).first;

		for(unsigned int i = 0; i < state.front().size(); i++){
			for(unsigned int j = 0; j < state.size(); j++){
				out << state[i][j] << " ";
			}
		}

		out << ";" << (*it).second << "\n";
		
	}

}



QState ModifiedQL::ReadQState(std::string filepath,int nRows, int nCols){

	QState qState;

	std::ifstream infile(filepath);

	std::string line;
	while (std::getline(infile, line))
	{
		
		std::vector<std::vector<int>> state(nRows, std::vector< int >(nCols));


		std::stringstream ss;
		std::stringstream   linestream(line);
		std::string value;

		int num;

		//Obter Estado
		std::getline(linestream, value, ';');
		ss.str(value);

		int index = 0;
		while ( ss >> num ){
			std::pair<int,int> coord = Get2DCoordFromIndex(index,nCols);
			state[coord.first][coord.second] = num;
			index++;
		}
		ss.clear();

		//Obter Numero de visitas
		std::getline(linestream, value, ';');
		ss.str(value);
		ss>> num;
		ss.clear();

		qState[state] = num;
		

	}

	return qState;
}
