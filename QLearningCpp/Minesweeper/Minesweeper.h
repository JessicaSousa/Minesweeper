/*
 * Minesweeper.h
 *
 *  Created on: 31 de dez de 2016
 *      Author: jcardoso
 */

#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

#include <iterator>
#include <map>

#include "Random.h"
#include "Square.h"

template <typename type>
bool in(type elm, std::vector<type> vec){
    for( auto x: vec ){
        if(elm == x )
            return true;
    }
    return false;
}

class MineSweeper{
    public:
    std::vector<std::vector<Square>> board;
    std::vector<Square*> frontier;

    int row_size = 0;
    int column_size = 0;

    int bomb_number = 0;

    int bomb_value = 9;
    int covered_value = 10;

    int num_uncovered = 0;
    bool gameEnd = false;

    int score = 0;
    bool gameWon = false;

    bool verbose = false;

    MineSweeper(int row=4, int column=4,int difficulty=1, bool verbose=false);

    //Insere as minas no campo e incrementa as suas adjacências
    void insert_mines();

    //Obtém os vizinhos de um quadrado e armazena em uma hash
    std::map<int, Square*> get_neighbors(Square square);

    //obtem o estado do campo ( valores cobertos(10) e/ou valores dos quadrados)
    std::vector<int> get_state();

    int get_state_index_from_location(int row, int col);

    Coord get_location_from_state_index(int index);

    Square* get_square(Coord location);

    bool is_bomb(Square square);

    std::vector<int> get_label();

    //Atualiza o campo dado um quadrado indicando a jogada
   void update_board(Square * square);

   std::vector<Square*> get_frontier();

   std::vector<int> get_init_state();

   std::vector<int> get_next_state(Square * square);

   bool inFrontier(Coord t);

   Square * get_random_move_off_frontier();


};

#endif /* MINESWEEPER_H_ */
