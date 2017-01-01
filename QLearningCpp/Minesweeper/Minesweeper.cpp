/*
 * Minesweeper.cpp
 *
 *  Created on: 31 de dez de 2016
 *      Author: jcardoso
 */

#include "Minesweeper.h"


MineSweeper::MineSweeper(int row, int column,int difficulty, bool verbose){

        this->row_size = row;
        this->column_size = column;

        this->verbose = verbose;

        board.resize(row);
        for(int r = 0; r < row; r++ ){
            for(int c = 0; c < column; c++ ){
                Square s({r,c});
                this->board[r].push_back(s);
            }
        }

        switch(difficulty){
            case 1:
                if (row * column < 10)
                    this->bomb_number = 2;
                else if (row*column < 20)
                    this->bomb_number = 3;
                else if (row * column < 30)
                    this->bomb_number = 5;
                else if (row * column < 100)
                    this->bomb_number = 10;
                else
                    this->bomb_number = 15;
                break;
            case 2:
                if (row * column < 30)
                    this->bomb_number = 10;
                else if (row * column < 100)
                    this->bomb_number = 15;
                else
                    this->bomb_number = 20;
                break;
            case 3:
                if (row * column < 30)
                    this->bomb_number = 15;
                else if (row * column < 100)
                    this->bomb_number = 20;
                else
                    this->bomb_number = 30;
                break;
            default:
                std::cout << "Your level input is wrong!\n";
                return;
                break;
        }
        this->insert_mines();

        if (verbose)
            std::cout << "Playing on " << row<<" x "<< column <<" board with difficulty " << difficulty << "\n";
    }

//Insere as minas no campo e incrementa as suas adjacências
void MineSweeper::insert_mines(){

	std::vector<int> bombs = Random::RandomSample(this->bomb_number,(this->row_size)*(this->column_size));
    //std::copy(bombs.begin(), bombs.end(), std::ostream_iterator<int>(std::cout, " "));
    std::vector<Coord> bomb_positions;

    //Mapeiam os índices obtidos em RandomSample nos índices 2D na matriz do campo
    for(auto bomb: bombs){
         bomb_positions.push_back({bomb%this->row_size, bomb / this->row_size});
    }
    //Atribuição das bombas no campo
    for(auto bomb: bomb_positions){
        this->board[bomb.x][bomb.y].value = this->bomb_value;
    }
    //Incremento da vizinhança da bomba
    for(auto bomb_position: bomb_positions){
        Square bomb = this->board[bomb_position.x][bomb_position.y];
        std::map<int, Square*> neigbourlist = this->get_neighbors(bomb);

        typedef std::map<int, Square*>::iterator it_type;
        for(it_type neigbour = neigbourlist.begin(); neigbour != neigbourlist.end(); neigbour++) {
        	if(neigbour->second->value != this->bomb_value){
        		neigbour->second->value++;
            }
        }
    }

}


//Obtém os vizinhos de um quadrado e armazena em uma hash
std::map<int, Square*> MineSweeper::get_neighbors(Square square){
	Coord location = square.location;
    std::map<int, Square*> neighbors;

    int i = 0;
    for(int row = location.x - 1; row <  location.x + 2; row++){
    	for(int col = location.y - 1; col < location.y + 2; col++){

    		if(row == location.x && col == location.y) continue;

            if(row >= 0 && row < this->row_size && col >= 0 && col < this->column_size){
            	neighbors.insert(std::make_pair(i,&this->board[row][col]));
             }
             i = i + 1;
        }
    }

    return neighbors;

}


std::vector<int> MineSweeper::get_state(){
	std::vector<int> state;

	for(int row = 0; row < this->row_size; row++){

		for(int col = 0; col < this->column_size; col++){

    	   Square square = this->board[row][col];

    	   if(square.isUncovered == 0) state.push_back(this->covered_value);
           else state.push_back(square.value);
       }

     }

	return state;
}

//Usado no Q-Learning Modificado
std::vector<int> MineSweeper::get_label(){
	std::vector<int> label;
	for(int row = 0; row < this->row_size; row++){
		for(int col = 0; col < this->column_size; col++){

           Square square = this->board[row][col];

           bool found = in(&square,this->frontier);

            if(found){
            	if(square.value == this->bomb_number) label.push_back(1);
                else label.push_back(0);
             }else{
                  label.push_back(0);
              }

         }
     }
     return label;
}


void MineSweeper::update_board(Square * square){

	if(square->isUncovered){
		return;
	}

	//Remove square que se encontra dentro de frontier
	this->frontier.erase(std::remove(this->frontier.begin(),
			this->frontier.end(), square),
			this->frontier.end());

	//descobrir quadrado
	square->isUncovered = true;
	this->num_uncovered += 1;
	/*
        std::cout << "Current Square: \n";
        std::cout << *square << std::endl;
	 */
	//Caso o valor seja igual a zero, descobre recursivamente
	if(square->value == 0){
		std::map<int, Square*> neigbourlist = this->get_neighbors(*square);
		for(auto const &neigbour : neigbourlist){
			Square * value = (neigbour.second);
			//std::cout << "Vizinho: " << * value << std::endl;
			this->update_board(value);

		}
	}
	else{
		std::map<int, Square*> neigbourlist = this->get_neighbors(*square);
		for(auto const &neigbour : neigbourlist){
			//verifica se o quadrado está em um vetor de quadrados (frontier)
			Square * value = (neigbour.second);
			bool found = in(value,this->frontier);
			if(value->isUncovered == false && !found){
				this->frontier.push_back(value);
				//std::cout << "Vizinho: " << * value << std::endl;
			}
		}

	}

}


std::vector<Square*> MineSweeper::get_frontier(){
	return this->frontier;
}

std::vector<int> MineSweeper::get_init_state(){
	std::vector<int>state;

	for(int i  = 0; i < (this->row_size)*(this->column_size); i++){
		state.push_back(this->covered_value);
	}

	return state;
}

std::vector<int> MineSweeper::get_next_state(Square * square){
	if(!square->isUncovered){
		if(square->value == this->bomb_value){
			this->gameEnd = true;
		}
		else{
			this->score +=5;
			this->update_board(square);
		}
	}
	if(this->num_uncovered == ((this->row_size*this->column_size) - this->bomb_number)){
		this->gameEnd = true;
		this->gameWon = true;
	}
	if (this->verbose){
		std::vector<int> s = this->get_state();
		std::cout << "\nState: ";
		for(auto i: s){
			std::cout << i << " ";
		}
		std::cout << "\n";
		std::cout << "Label: ";
		std::vector<int> l = this->get_label();
		for(auto i: l){
			std::cout << i << " ";
		}
		std::cout << "\n";

		std::cout << "Frontier: ";
		std::vector<Square *> f = this->get_frontier();
		for(auto i: f){
			std::cout << *i << " ";
		}
		std::cout << "\n";


	}
	return this->get_state();
}

bool MineSweeper::inFrontier(Coord t){
	for(auto f: this->frontier)
		if(t == f->location)
			return true;
	return false;
}

Square * MineSweeper::get_random_move_off_frontier(){
	//std::srand ( unsigned ( std::time(0) ) );

	Coord randomLocation = {rand() % (int) this->row_size, rand() % (int) this->column_size};

	while(inFrontier(randomLocation) || this->get_square(randomLocation)->isUncovered){
		randomLocation = {rand() % (int) this->row_size, rand() % (int) this->column_size};
	}


	return this->get_square(randomLocation);
}


int MineSweeper::get_state_index_from_location(int row, int col){
	return (row*this->row_size + col);
}
Coord MineSweeper::get_location_from_state_index(int index){
	return {index / this->row_size, index % this->row_size};
}

Square* MineSweeper::get_square(Coord location){
	return &(this->board[location.x][location.y]);
}

bool MineSweeper::is_bomb(Square square){
	return (square.value == this->bomb_value);
}
