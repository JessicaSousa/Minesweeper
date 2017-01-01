/*
 * Random.h
 *
 *  Created on: 31 de dez de 2016
 *      Author: jcardoso
 */

#ifndef RANDOM_H_
#define RANDOM_H_
#include <vector>
#include <ctime> //time()
#include <algorithm>  //std::shuffle
#include <random> // random_device

namespace Random{

	inline void seed(){
		std::srand ( unsigned ( std::time(NULL) ) );
	}

	inline  int RandInt(int min, int max)
	{
	   return rand()%(max-min+1) + min;
	}

	template<class T>
	inline  T RandomChoice(std::vector<T> &elements)
	{

	   return elements[RandInt(0, elements.size()-1)];
	}

	inline std::vector<int> RandomSample(std::size_t size, std::size_t max)
	{
	    std::vector<int> v;
	    for (size_t i=0; i<max; ++i) v.push_back(i);

	    std::random_shuffle(v.begin(), v.end());

	    std::vector<int> out;
	    for(size_t i = 0; i < size; i++){
	        out.push_back(v[i]);
	    }
	    return out;
	}



};


#endif /* RANDOM_H_ */
