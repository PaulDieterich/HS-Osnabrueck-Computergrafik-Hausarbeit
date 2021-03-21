//
//  Ateroid.h
//  Stellt einen Asteroiden im Spiel dar. Abgeleitet von Model, hält noch ein paar Extras
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
	Resource* resource; // Jeder Asteroid kann einen bestimmten Rohstoff halten, der beim zerstören freigesetzt wird
	bool collected;		// Bestimmt, ob die Ressource in them Asteroiden schon mitgenommen wurde
	float size;		    // Die Größe des Asteroiden, wichtig für den Schaden bei Kollision und Anzahl Einheiten von Ressourcen bei Zerstörung
	float health;	    // Die Lebenspunkte, bis der Asteroid als zerstört gilt

public:
	Asteroid(const char* modelFile, float size, Resource* resource) : Model(modelFile, false), size(size), resource(resource), collected(false), health(2 * size) {}

	/**
	 * Zieht dem Asteroiden Leben ab
	 * @param rawDamage Der Schaden, der dem Asteroiden zugefügt wird
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

