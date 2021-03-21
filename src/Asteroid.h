//
//  Ateroid.h
//  Stellt einen Asteroiden im Spiel dar. Abgeleitet von Model, h�lt noch ein paar Extras
//
//  @author Justin Falkenstein
//
#pragma once

#include <stdio.h>
#include "basemodel.h"
#include "Model.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "Matrix.h"
#include "vector.h"
#include "Resource.h"
#include "Utility.h"

#define ASTEROID_MODEL_NUM 8
using namespace std;
class Asteroid : public Model
{
private:
	Resource* resource; // Jeder Asteroid kann einen bestimmten Rohstoff halten, der beim zerst�ren freigesetzt wird
	bool collected;		// Bestimmt, ob die Ressource in them Asteroiden schon mitgenommen wurde
	float size;		    // Die Gr��e des Asteroiden, wichtig f�r den Schaden bei Kollision und Anzahl Einheiten von Ressourcen bei Zerst�rung
	float health;	    // Die Lebenspunkte, bis der Asteroid als zerst�rt gilt

public:
	Asteroid(const char* modelFile, float size, Resource* resource) : Model(modelFile, false), size(size), resource(resource), collected(false), health(2 * size) {}

	/**
	 * Zieht dem Asteroiden Leben ab
	 * @param rawDamage Der Schaden, der dem Asteroiden zugef�gt wird
	 */
	void receiveDamage(float rawDamage);

	//
	// Getter
	//
	const float getSize() { return size; }
	const float getHealth() { return health; }
	const bool isCollected() { return collected; }
	const bool isDestroyed() { return health <= 0; }
	Resource* getResource() { return resource; }
	
	//
	// Setter
	//
	void setCollected(bool val) { collected = val; }
};

