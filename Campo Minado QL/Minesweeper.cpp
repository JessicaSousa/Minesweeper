#include "Minesweeper.h"
#include <ctime>
#include <cstdlib>

/**
  * Constructor
  * Inicializa o campo
  */
Minesweeper::Minesweeper(int gridWidth, int gridHeight, int numberOfMines)
{
    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;

    this->numberOfMines = numberOfMines;
    //Inicializa o campo com as dimens�es informadas
    mineBoard.resize(gridHeight);
    for (int i = 0; i < gridHeight; ++i)
        mineBoard[i].resize(gridWidth);
    //Gera o campo com suas minas e popula o número de minas ao redor
    generateBoard();
}


/**
  * Constructor
  */
Minesweeper::Minesweeper()
{
	this->gridWidth = 4;
	this->gridHeight = 4;
	this->numberOfMines = 3;
}
/**
  * Destructor
  */
Minesweeper::~Minesweeper()
{
}


/**
  * fillSurroundings(int, int,bool)
  * Incrementa as adjac�ncias da mina encontrada na posi��o (row,column)
  * @param int - Row
  * @param int - Column
  */
void Minesweeper::fillSurroundings(int row, int column)
{
    //canto superior esquerdo
    if ( (row-1) != -1 && (column -1) != -1 && mineBoard.at(row-1).at(column-1)!= MINE)
    {

        mineBoard.at(row-1).at(column-1)++;

    }
    //centro superior
    if ( (row-1) != -1 && mineBoard.at(row-1).at(column) != MINE)
    {

        mineBoard.at(row-1).at(column)++;

    }
    //canto superior direito
    if ( (row-1) != -1 && (column + 1) != gridWidth && mineBoard.at(row-1).at(column+1) != MINE)
    {

        mineBoard.at(row-1).at(column+1)++;

    }
    //esquerda
    if ( (column -1) != -1 && mineBoard.at(row).at(column-1) != MINE)
    {

         mineBoard.at(row).at(column-1)++;

    }
    //direita
    if ( (column + 1) != gridWidth && mineBoard.at(row).at(column+1) != MINE)
    {

          mineBoard.at(row).at(column+1)++;

    }
    //canto inferior esquerdo
    if ( (row+1) != gridHeight && (column -1) != -1 && mineBoard.at(row+1).at(column-1) != MINE)
    {
            mineBoard.at(row+1).at(column-1)++;

    }
    //centro inferior
    if ( (row+1) != gridHeight && mineBoard.at(row+1).at(column) != MINE)
    {

            mineBoard.at(row+1).at(column)++;

    }
    //canto inferior direito
    if ( (row+1) != gridHeight && (column+1) != gridWidth && mineBoard.at(row+1).at(column+1) != MINE)
    {

            mineBoard.at(row+1).at(column+1)++;

    }
}

/**
  * generateBoard()
  * Encontra posições de minas aleatoriamente e insere no campo
  */
void Minesweeper::generateBoard()
{
    for( int i = 0; i < numberOfMines; i++)
    {
        int row, column;
        //Evitar gerar uma mina sobre a outra
        do
        {
            row    = rand() % gridHeight;
            column = rand() % gridWidth;

        }while(mineBoard.at(row).at(column) == MINE);

        mineBoard.at(row).at(column) = MINE;

        //Após a criação da mina, suas adjacêcias são incrementadas;
        fillSurroundings(row,column);
    }
}


/**
  * isMine(int, int)
  * Deternina se há uma mina nessa localização
  * @param int - Row
  * @param int - Column
  * @return bool - Retorno é verdadeiro se há mina, caso contrário falso
  */
bool Minesweeper::isMine(int row, int column)
{

        if ( row < 0 || row > (gridHeight - 1))
        	std::cout << "Error in input" << std::endl;
        if ( column < 0 || column > (gridWidth - 1))
        	std::cout << "Error in input" << std::endl;
        return mineBoard.at(row).at(column) == MINE;
}

/**
  * getValue(int, int)
  * Retorna o valor de uma célula na posição(row, column)
  * @param int - Row
  * @param int - Column
  * @return int - Representa o valor da célula
  */
int Minesweeper::getValue(int row, int column)
{

    if ( row < 0 || row > (gridHeight - 1))
        std::cout << "Error in input" << std::endl;
    if ( column < 0 || column > (gridWidth - 1))
    	std::cout << "Error in input" << std::endl;

    return mineBoard.at(row).at(column);
}
