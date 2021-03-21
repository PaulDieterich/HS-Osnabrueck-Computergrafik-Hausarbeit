//
//  Ship.h
//  Stellt ein Schiff im Spiel dar. Abgeleitet von Model, hält noch ein paar Extras
//  und lässt sich einfach ohne Parameter erzeugen
//
//  @author Justin Falkenstein
//
#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <map>
#include "ShipWeapon.h"
#include "Model.h"
#include "Resource.h"
#include "Utility.h"

using namespace std;

class Ship: public Model
{
private:
	float maxHealth;			// Maximales Leben
	float health;				// Wenn 0 -> Tot
	float maxShield;			// Maximaler Schild
	float shield;				// Schild -> Dämpft Schaden ab
	float speed;				// Basisgeschwindigkeit
	float basePrice;			// Der Preis für dieses Schiff im Originalzustand
	float cargoCapacity;		// Wie viel kg das Schiff an Ressourcen tragen kann
	float cargoWeight;			// Wie viel kg das Schiff momentan trägt, zu Optimierungszwecken in eigener Variable
	float fuel;					// Wie viel Liter Treibstoff im Tank ist
	float fuelUsage;			// Wie viel Liter Treibstoff pro Minute bei normalem Schub benötigt wird
	float fuelMax;				// Wie viel Treibstoff maximal im Tank sein kann
	ShipWeapon weapon;			// Die Waffen des Schiffes
	map<Resource*, int> cargo;  // Die Ressourcen des Schiffes und die Anzahl an Einheiten
public:
	Ship(const char* modelFile, ShipWeapon weapon, float baseHealth, float baseShield, float baseSpeed, float basePrice, float baseCap, float fuel, float fuelUsage, float fuelMax, int munition) :
		Model(modelFile, false), weapon(weapon), health(baseHealth), maxHealth(baseHealth), maxShield(baseShield), shield(baseShield), speed(baseSpeed), basePrice(basePrice), cargoCapacity(baseCap), cargoWeight(0), fuel(fuel), fuelUsage(fuelUsage), fuelMax(fuelMax) {}

	/**
	 * Zieht dem Schiff Leben ab
	 * @param rawDamage Der Schaden, der dem Schiff ohne Beachtung des Schildes zugefügt wird
	 */
	void receiveDamage(float rawDamage);

	/**
	 * Sammelt Ressourcen ein
	 * @param resource Die Ressource, die eingesammelt werden soll
	 * @param amount Die Anzahl an Einheiten, die eingesammelt werden
	 */
	bool collectResource(Resource* resource, int amount);

	/**
	 * Fliegt das Schiff und reduziert Treibstoff
	 */
	void drive(bool boost = false);

	/**
	 * Feuert die Waffe ab
	 */
	void shoot();

	/**
	 * Füllt das Schiff auf
	 */
	void refill();

	//
	// Getter
	//
	const float getSpeed() { if (isEmpty()) return 0; return speed;  }
	const float getHealth() { return health; }
	const float getShield() { return shield; }
	const float getAmmo() { return weapon.getAmmo();  }
	const float getMaxAmmo() { return weapon.getAmmoCapacity(); }
	const float getCargoWeight() { return cargoWeight; }
	const float getFuel() { return fuel; }
	const float getBasePreis() { return basePrice; }
	const float getWeaponDamage() { return weapon.getDamage(); }
	const int getWeaponRPM() { return weapon.getRPM(); }
	const map<Resource*, int> getCargo() { return cargo; }
	const bool isDestroyed() { return health <= 0; }
	const bool isEmpty() { return fuel <= 0; }
	const bool hasCargo() { return !cargo.empty(); }
	const bool isAmmoEmpty() { return weapon.getAmmo() == 0; }
	const bool isShooting() { return weapon.isShooting(); }

};

class Viper : public Ship {
public:
	Viper(): Ship(ASSET_DIRECTORY "/models/ships/Viper_MK4/Viper-mk-IV-fighter.dae", ShipWeapon(100.0, 1000, 500, 1000, 100), 50.0, 200.0, 10.0, 15000.0, 500.0, 50.0, 1.0, 400.0, 80) {}
};

class Fighter : public Ship {
public:
	Fighter(): Ship(ASSET_DIRECTORY "/models/ships/SF_Fighter/SciFi_Fighter.obj", ShipWeapon(100.0, 1000, 500, 2000, 200), 150.0, 200.0, 5.0, 25000.0, 1000.0, 200.0, 50.0, 600.0, 100) {}
};

class Blaster : public Ship {
public:
	Blaster(): Ship(ASSET_DIRECTORY "/models/ships/Blaster/Spaceship.obj", ShipWeapon(100.0, 1000, 500, 3000, 400), 200.0, 400.0, 2.5, 45000.0, 2500.0, 500.0, 20.0, 1000.0, 50) {}
};
