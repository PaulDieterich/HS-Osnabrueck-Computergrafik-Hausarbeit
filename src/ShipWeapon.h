//
//  ShipWeapon.h
//  Stellt eine simple, austauschbare Schiffswaffe dar
//
//  @author Justin Falkenstein
//

#pragma once
#include <thread>
#include <chrono>
#include <iostream>
using namespace std;

class ShipWeapon
{
private:
	float damage;		// Der Schaden, den die Waffe zufügt
	int distance;		// Die Distanz, die ein Schuss zurücklegt
	int ammoCapacity;	// Die maximale Menge an Munition
	int ammo;			// Die momentane Menge an Munition
	int rpm;			// Rounds per minute -> Anzahl an Schüssen pro Minute
	bool shooting;		// Gibt an, ob die Waffe gerade abgefeuert wird (inkl. Kadenz)
public:
	ShipWeapon(): ShipWeapon(10.0, 1000, 500, 100, 100) {}
	ShipWeapon(float damage, int ammoCapacity, int ammo, int distance, int rpm) : damage(damage), ammoCapacity(ammoCapacity), ammo(ammo), distance(distance), rpm(rpm), shooting(false){}
	
	/**
	 * Reduziert die Munition der Waffe und startet den Timer bis zum nächsten Schusss
	 */
	void shoot();

	/**
	 * Started den Timer für die Waffe und setzt damit den shooting boolean, um zu bestimmen,
	 * wann das nächte Mal geschossen werden darf.
	 * Wird in eigenem Thread von der Shoot Methode aufgerufen
	 */
	void startTimer();

	/**
	 * Füllt Muni auf
	 */
	void refill() { ammo = ammoCapacity; }

	//
	// Getters
	//
	const int getAmmoCapacity() const { return ammoCapacity; }
	const int getAmmo() const { return ammo; }
	const float getDamage() const { return damage; }
	const int getDistance() const { return distance; }
	const int getRPM() const { return rpm; }
	const bool isShooting() const { return shooting; }


};



