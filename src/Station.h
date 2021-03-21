/**
*  Ateroid.h
*  Stellt eine Station im Spiel dar. Abgeleitet von 
*  Model, hält noch ein paar Extras
*
* @author Paul Dieterich
*/
#pragma once

#ifdef WIN32
#define ASSET_DIRECTORY "../../assets/"
#else
#define ASSET_DIRECTORY "../assets/"
#endif
#include <stdio.h>
#include "Model.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "Matrix.h"
#include "vector.h"
#include "Resource.h"
class Station : public Model {

private:
	float size;
	int RessourceValue;
public:
	Station(const char* modelFile, float size) : Model(modelFile, false), size(size) {}

	/**
	 * Verkauft alle Items aus der Cargo
	 */
	float sellItems(map<Resource*, int> cargo);
};