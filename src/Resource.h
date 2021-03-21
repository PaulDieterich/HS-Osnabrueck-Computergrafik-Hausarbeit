//
//  Resource.h
//  Stellt eine einsammelbare, interaktive Ressource in der Welt dar
//
//  @author Justin Falkenstein
//
#pragma once
#include <stdio.h>
#include <iostream>
#include "color.h"
using namespace std;

enum ResourceRarity {
	COMMON = 80,
	UNCOMMON = 40,
	RARE = 30,
	LEGENDARY = 10
} typedef ResourceRarity;

class Resource
{
private:
	ResourceRarity rarity; // Chance zwischen 1 und 100% in der Welt zu spawnen
	Color color;		   // Eine Farbe, die der Ressource für UI und anderen Grafikstuff zugeordnet ist
	string name;		   // Der Name des Elements / Der Resource
	float value;		   // Wert der Resource einer einzelnen Einheit
	float weight;		   // Gewicht einer Einheit in Kg
	bool fuel;			   // Bestimmt, ob das Material als Brennstoff verwendet werden kann
public:
	Resource(string name, float value, ResourceRarity rarity, float weight, bool fuel, Color color) : 
		name(name), value(value), rarity(rarity), weight(weight), fuel(fuel), color(color) {};

	//
	// Getter
	//
	const string getName() const { return name; }
	const float getRarity() const { return rarity; }
	const float getWeight() const { return weight; }
	const Color& getColor() const { return color; }
	const float getValue() const { return value; }
	const bool isFuel() const { return fuel; }
};

