/*
 * Square.h
 *
 *  Created on: 31 de dez de 2016
 *      Author: jcardoso
 */

#ifndef SQUARE_H_
#define SQUARE_H_
//#include <tuple>
#include <iostream> //ostream


struct Coord{

    int x,y;


    Coord& operator=(const Coord& t)
    {
        x = t.x;
        y = t.y;
        return *this;
    }

    bool operator==(Coord c) const{
        return(c.x==x && c.y == y);
    }

    bool operator>=(Coord c) const{
    	if(c.x == x && c.y == y) return true;
    	if(x > c.x) return true;
    	else if(x == c.x) return y > c.y;
    	return false;
    }

    bool operator<(Coord c) const{
    	if( x < c.x) return true;
    	else if(x == c.x) return y < c.y;
    	return false;
    }

    bool operator!=(Coord t) const{
        return(t.x!=x && t.y != y);
    }

    friend std::ostream &operator<<( std::ostream &output, const Coord &t ){
        output << "(" << t.x << "," << t.y << ")";
        return output;
    }

};
class Square {
public:
	  Coord location = {0, 0};
	  bool isUncovered = false;
	  int value = 0;

	  Square(Coord location);

	  friend std::ostream &operator<<( std::ostream &output, const Square &S );

	  bool operator==(const Square &s);
};

#endif /* SQUARE_H_ */
