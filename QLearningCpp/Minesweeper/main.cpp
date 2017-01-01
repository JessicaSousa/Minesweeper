#include <cassert>
#include <limits>
#include <tuple>
#include "Minesweeper.h"

typedef std::pair<std::vector<int>,Coord> QPair;
typedef std::map<QPair, int> QTable;

QTable training(int num_total_simulations = 100, int row = 4, int col = 4, int difficulty = 1, int rewardValue = 1){

	//chamar antes de um loop
	Random::seed();

    std::map<QPair, int> qMap;

    for(int iterationNo = 0; iterationNo < num_total_simulations; iterationNo++){

        if (iterationNo % 1000 == 0)
            std::cout << "Playing " << iterationNo <<"th training game.\n";

        MineSweeper game(row, col, difficulty);

        Coord location = {Random::RandInt(0,game.row_size-1), Random::RandInt(0,game.column_size-1)};//{rand() % game.row_size, rand() % game.column_size};
        Square * nextMove = game.get_square(location);
        std::vector<int> currentState = game.get_init_state();
        int reward = 0;


        while(true){

            reward = (!game.is_bomb(*nextMove)) ? rewardValue : -1*rewardValue;

            QPair stateAndAction(currentState, nextMove->location);
            qMap[stateAndAction] += reward;
            currentState = game.get_next_state(nextMove);

            if(game.gameEnd) break;

            std::vector<Square*> frontier = game.get_frontier();
            nextMove = Random::RandomChoice(game.frontier);
        }
    }

    return qMap;
}


Square * getNextMove(QTable qMap, MineSweeper * game){

    Coord bestMoveLocation = {-1,-1};

    float maxQValue = -std::numeric_limits<float>::infinity();
    std::vector<Square*> possibleMoves = game->get_frontier();
    std::vector<int> currentState = game->get_state();
    bool shouldPickRandomMove = true;

    for(auto move: possibleMoves){
        QPair stateAndAction(currentState, move->location);
        int q = qMap[stateAndAction];
        if (q > maxQValue){
            bestMoveLocation = move->location;
            maxQValue =  q;
        }

    }

    if (maxQValue > 0)
        shouldPickRandomMove = false;

    if (shouldPickRandomMove){
        return game->get_random_move_off_frontier();
    }
    else{
        return game->get_square(bestMoveLocation);
    }


}

using namespace std;

int main(){

    int r = 4, c = 4; //3 minas
    MineSweeper game(r,c);


    game.board[0][0].value = 0;
    game.board[0][1].value = 2;
    game.board[0][2].value = 9;
    game.board[0][3].value = 2;

    game.board[1][0].value = 0;
    game.board[1][1].value = 2;
    game.board[1][2].value = 9;
    game.board[1][3].value = 2;

    game.board[2][0].value = 0;
    game.board[2][1].value = 2;
    game.board[2][2].value = 2;
    game.board[2][3].value = 2;

    game.board[3][0].value = 0;
    game.board[3][1].value = 1;
    game.board[3][2].value = 9;
    game.board[3][3].value = 1;


    /*
    std::cout << "------- Minado -------\n";
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            cout << game.board[i][j].value << " ";
        }
        cout << endl;
    }

    Coord coord = {0,1};
    Square * move = game.get_square(coord);
    std::vector<int> currentState = game.get_next_state(move);

    do{

        std::cout << "Current State" << std::endl;

        for(int i = 0; i < r; i++){
           for(int j = 0; j < c; j++){

                cout << currentState[game.get_state_index_from_location(i,j)] << " ";
            }
            cout << endl;
        }

        std::cout << "coord: ";
        cin >> coord.x >> coord.y;

        move = game.get_square(coord);
        currentState = game.get_next_state(move);
    }while(!game.gameEnd);
    */
    //Gera a tabela Q
    QTable qMap = training(10, 4, 4, 1, 1);

    Square * nextMove = getNextMove(qMap, &game);

    std::cout << *nextMove << std::endl;

    std::vector<int> currentState = game.get_next_state(nextMove);

    std::cout << *nextMove << std::endl;

    std::cout << "Current State" << std::endl;

    for(int i = 0; i < r; i++){
       for(int j = 0; j < c; j++){
            cout << currentState[game.get_state_index_from_location(i,j)] << " ";
        }
        cout << endl;
    }

    /*QTable qMap;
    Square * move = getNextMove(qMap, &game);
    std::cout << *move << std::endl;
    std::vector<int> currentState = game.get_next_state(move);

    //Primeiro clique é sempre seguro =)
    while (game.is_bomb(game.get_square(currentState))){
        MineSweeper j(r,c,1,false);
        game = j; //resetar o game
    }


    std::cout << "Current State" << std::endl;

    for(int i = 0; i < r; i++){
       for(int j = 0; j < c; j++){
            cout << currentState[i + r*j] << " ";
        }
        cout << endl;
    }*/

    return 0;
}
