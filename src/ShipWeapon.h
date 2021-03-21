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
	float damage;		// Der Schaden, den die Waffe zuf�gt
	int distance;		// Die Distanz, die ein Schuss zur�cklegt
	int ammoCapacity;	// Die maximale Menge an Munition
	int ammo;			// Die momentane Menge an Munition
	int rpm;			// Rounds per minute -> Anzahl an Sch�ssen pro Minute
	bool shooting;		// Gibt an, ob die Waffe gerade abgefeuert wird (inkl. Kadenz)
public:
	ShipWeapon(): ShipWeapon(10.0, 1000, 500, 100, 100) {}
	ShipWeapon(float damage, int ammoCapacity, int ammo, int distance, int rpm) : damage(damage), ammoCapacity(ammoCapacity), ammo(ammo), distance(distance), rpm(rpm), shooting(false){}
	
	/**
	 * Reduziert die Munition der Waffe und startet den Timer bis zum n�chsten Schusss
	 */
	void shoot();

	/**
	 * Started den Timer f�r die Waffe und setzt damit den shooting boolean, um zu bestimmen,
	 * wann das n�chte Mal geschossen werden darf.
	 * Wird in eigenem Thread von der Shoot Methode aufgerufen
	 */
	void startTimer();

	/**
	 * F�llt Muni auf
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



