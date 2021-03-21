#include "Station.h"
float Station::sellItems(map< Resource*, int> cargo) {
	float v = 0; 
	
	for (map<Resource*,int>::iterator it = cargo.begin(); it != cargo.end(); ++it)
	{
		v += (*it).first->getValue() * (*it).second;
		(*it).second = 0;
	}
	return v;
}   


