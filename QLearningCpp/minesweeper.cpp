#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <random>
#include <vector>
#include<ctime>
#include <map>
 

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
typedef struct{
    int x,y;
}Tuple;

class Square{
  public:
  Tuple location = {0, 0};
  bool isUncovered = false;
  int value = 0;
  
  Square(Tuple location){
      this->location = location;
  }
  
};

class MineSweeper{
    public:
    std::vector<std::vector<Square>> board;  
    //frontier = []

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
                Tuple t = {r,c};
                Square s(t);
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
    
    //Insert specified number of mines into the area, increase numbers of its neigbours.
    void insert_mines(){
        std::srand ( unsigned ( std::time(0) ) );
        std::vector<int> bombs = RandomSample(this->bomb_number,(this->row_size)*(this->column_size));
        //std::copy(bombs.begin(), bombs.end(), std::ostream_iterator<int>(std::cout, " "));
        std::vector<Tuple> bomb_positions;
        for(auto bomb: bombs){
            Tuple t = {bomb/this->row_size, bomb % this->row_size};
            bomb_positions.push_back(t);
        }
        
        for(auto bomb: bomb_positions){
            this->board[bomb.x][bomb.y].value = this->bomb_value;
        }
        for(auto bomb_position: bomb_positions){
            Square& bomb = this->board[bomb_position.x][bomb_position.y];
            std::map<int, Square*> neigbourlist = this->get_neighbors(bomb);
            
            typedef std::map<int, Square*>::iterator it_type;
            for(it_type neigbour = neigbourlist.begin(); neigbour != neigbourlist.end(); neigbour++) {
                if(neigbour->second->value != this->bomb_value){
                    neigbour->second->value++;
                }
            }
        }
    }
    
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
    
};

using namespace std;

int main(){
    MineSweeper m(4,4,3,false);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            cout << m.board[i][j].value << " ";
        }
        cout << endl;
    }
    return 0;
}