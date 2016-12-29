#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>
#include<ctime>
#include <map>

template <typename type>
bool in(type elm, std::vector<type> vec){
    for( auto x: vec ){
        if(elm == x )
            return true;
    }
    return false;
}
 

std::vector<int> RandomSample(std::size_t size, std::size_t max)
{
    std::vector<int> myvector;
    for (int i=0; i<max; ++i) myvector.push_back(i);
    std::random_shuffle ( myvector.begin(), myvector.end() );
    
    std::vector<int> out;
    for(int i = 0; i < size; i++){
        out.push_back(myvector[i]);
    }
    return out;
}

struct Tuple{
    int x,y;
    
    
    Tuple& operator=(const Tuple& t)
    {
        x = t.x;
        y = t.y;
        return *this;
    }

    bool operator==(Tuple t) const{
        return(t.x==x && t.y == y);
    }
    
    friend std::ostream &operator<<( std::ostream &output, const Tuple &t ){
        output << "(" << t.x << "," << t.y << ")";
        return output;
    }
    
};
class Square{
  public:
  Tuple location = {0, 0};
  bool isUncovered = false;
  int value = 0;
  
  Square(Tuple location){
      this->location = location;
  }
  
  friend std::ostream &operator<<( std::ostream &output, 
         const Square &S ) { 
         output << "location: " << S.location << ", isUncovered: " 
                                 << S.isUncovered << ", value: " << S.value;
         return output;            
   }  
  
  bool operator==(const Square &s) {
    return(this->isUncovered == s.isUncovered && this->value == s.value && location == s.location);
  }
 
};
class MineSweeper{
    public:
    std::vector<std::vector<Square>> board;  
    std::vector<Square> frontier;

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
    
    MineSweeper(int row=4, int column=4,int difficulty=1, bool verbose=false){
        
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
    void insert_mines(){
        std::srand ( unsigned ( std::time(0) ) );
        std::vector<int> bombs = RandomSample(this->bomb_number,(this->row_size)*(this->column_size));
        //std::copy(bombs.begin(), bombs.end(), std::ostream_iterator<int>(std::cout, " "));
        std::vector<Tuple> bomb_positions;
        
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
    std::map<int, Square*> get_neighbors(Square square){
        Tuple location = square.location;
        std::map<int, Square*> neighbors;
        int i = 0;
        for(int row = location.x - 1; row <  location.x + 2; row++){
            for(int col = location.y - 1; col < location.y + 2; col++){
                if(row == location.x && col == location.y){
                    continue;
                }
                if(row >= 0 && row < this->row_size && col >= 0 && col < this->column_size){
                   neighbors.insert(std::make_pair(i,&this->board[row][col]));
                }
                i = i + 1;
            }
        }
        return neighbors;
    }
    
    //obtem o estado do campo ( valores cobertos(10) e/ou valores dos quadrados)
    std::vector<int> get_state(){
        std::vector<int> state;
        for(int row = 0; row < this->row_size; row++){
            for(int col = 0; col < this->column_size; col++){
                Square square = this->board[row][col];
                if(square.isUncovered == 0){
                    state.push_back(this->covered_value);
                }
                else{
                    state.push_back(square.value);
                }
            }
        }
        return state;
    }
    
    int get_location_from_state_index(int index){
        return index / this->row_size, index % this->row_size;
    }
    
    Square* get_square(Tuple location){
        return &(this->board[location.x][location.y]);
    }
    
    bool is_bomb(Square square){
        return (square.value == this->bomb_value);
    }
    
    std::vector<int> get_label(){
        std::vector<int> label;
        for(int row = 0; row < this->row_size; row++){
            for(int col = 0; col < this->column_size; col++){
                
                Square square = this->board[row][col];
                bool found = in(square,this->frontier);
                
                if(found){
                    if(square.value == this->bomb_number){
                        label.push_back(1);
                    }else{
    
                        label.push_back(0);
                    }
                }else{
                  
                  label.push_back(0);  
                
                }
               
            }
        }
        return label;
    }
    
    //Atualiza o campo dado um quadrado indicando a jogada
    void update_board(Square * square){
        
        if(square->isUncovered){
            return;
        }

        //Remove square que se encontra dentro de frontier
        this->frontier.erase(std::remove(this->frontier.begin(),
                                         this->frontier.end(), *square), 
                                         this->frontier.end());

        //descobrir quadrado
        square->isUncovered = true;
        this->num_uncovered += 1;
        
        //Caso o valor seja igual a zero, descobre recursivamente
        if(square->value == 0){
            std::map<int, Square*> neigbourlist = this->get_neighbors(*square);
            for(auto const &neigbour : neigbourlist){
                Square * value = (neigbour.second);
                this->update_board(value);
            }
        }
        else{
            std::map<int, Square*> neigbourlist = this->get_neighbors(*square);
            for(auto const &neigbour : neigbourlist){
                //verifica se o quadrado está em um vetor de quadrados (frontier)
                Square value = *(neigbour.second);
                bool found = in(value,this->frontier);
                if(value.isUncovered == false && !found){
                    this->frontier.push_back(value);
                }
            }
        }
        
    }
    
   std::vector<Square> get_frontier(){
        return this->frontier;
   }
   
   std::vector<int> get_init_state(){
        std::vector<int>state;
 
        for(int i  = 0; i < (this->row_size)*(this->column_size); i++){
            state.push_back(this->covered_value);
        }

        return state;
   }
    
    std::vector<int> get_next_state(Square * square){
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
        }
        return this->get_state();
    }
    
    bool inFrontier(Tuple t){
        for(auto f: this->frontier)
            if(t == f.location)
                return true;
        return false;
    }
    
    Square * get_random_move_off_frontier(){
        Tuple randomLocation = {rand() % (int) this->row_size, rand() % (int) this->column_size};
        
        while(inFrontier(randomLocation) || this->get_square(randomLocation)->isUncovered){
            randomLocation = {rand() % (int) this->row_size, rand() % (int) this->column_size};
        }
        
        return this->get_square(randomLocation);
    }
    
    
};



/*
# Generates a map containing estimated q values for each (state, action) pair,
# where the action is the location of the next move. Generates this by picking
# a random move from the frontier, then by inputting all correct moves into the 
# map. Picks a random correct move and repeats. Performs better than the alternative
# using random playing because it gathers more data.
def generate_state_map_using_label(num_total_simulations=100, row=4, col=4, difficulty=1, reward=1):
    qMap = collections.Counter()

    for iterationNo in xrange(num_total_simulations):
        if iterationNo % 1000 == 0:
            print "Playing %dth training game." % iterationNo
        game = MineSweeper(row, col, difficulty)

        topLeftCorner = (0, 0)
        nextMove = game.get_square(topLeftCorner)
        currentState = tuple(game.get_next_state(nextMove))

        while not game.gameEnd:
            label = game.get_label()    # get list of correct moves
            listOfCorrectMoveIndices = []
            qMap[currentState] += 1     # to indicate that the algorithm has visited this state once 
            for j in range(len(label)):
                stateAndAction = (currentState, game.get_location_from_state_index(j))
                if label[j] == 1:
                    qMap[stateAndAction] += reward
                    listOfCorrectMoveIndices.append(j)

            nextMove = None

            if not listOfCorrectMoveIndices:
                nextMove = game.get_random_move_off_frontier()
            else:
                index = random.choice(listOfCorrectMoveIndices)
                randomCorrectLocation = game.get_location_from_state_index(index)
                nextMove = game.get_square(randomCorrectLocation)
            
            currentState = tuple(game.get_next_state(nextMove))

    return qMap

# Returns the square of the best move
def getNextMove(qMap, game):
    bestMoveLocation = (-1, -1)
    maxQValue = float("-inf")
    possibleMoves = map(lambda x: x.location, game.get_frontier())
    currentState = tuple(game.get_state())
    shouldPickRandomMove = True
    
    for move in possibleMoves:
        q = qMap[ (currentState, move) ]
        if q > maxQValue:
            bestMoveLocation, maxQValue = move, q

    if maxQValue > 0 or qMap[currentState] == 0:
        shouldPickRandomMove = False

    if shouldPickRandomMove:
        return game.get_random_move_off_frontier()

    else:
        return game.get_square(bestMoveLocation)

*/
using namespace std;

int main(){
    int r = 4, c = 4; //3 minas
    MineSweeper jogo(r,c,1,true);
    
    std::cout << "------- Minado -------\n";
    for(int i = 0; i < r; i++){
        for(int j = 0; j < c; j++){
            cout << jogo.board[i][j].value << " ";
        }
        cout << endl;
    }
    Square * s = jogo.get_square({0,0});
    std::cout<<*s<<endl;
    jogo.get_next_state(s);
    std::cout<<*s<<endl;
    //jogo.get_next_state(jogo.get_square({0,1}));
    //jogo.get_next_state(jogo.get_square({0,2}));
    
    /*
    std::cout << "Current State" << std::endl;
    
    for(int i = 0; i < r; i++){
       for(int j = 0; j < c; j++){
            cout << currentState[i + r*j] << " ";
        }
        cout << endl; 
    }*/
    //m.get_next_state(move)
    
    
   
    //m.update_board(m.get_square({0,2}));
   
    return 0;
}