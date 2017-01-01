/*
 * Square.cpp
 *
 *  Created on: 31 de dez de 2016
 *      Author: jcardoso
 */

#include "Square.h"

Square::Square(Coord location){
	      this->location = location;
}

std::ostream &operator<<( std::ostream &output,
	         const Square &S ) {
	         output << "location: " << S.location << ", isUncovered: "
	                                 << S.isUncovered << ", value: " << S.value;
	         return output;
}


bool Square::operator==(const Square &s) {
	   return(this->isUncovered == s.isUncovered && this->value == s.value
			   	   	   	   	   	   	   	   	     && this->location == s.location);
}
