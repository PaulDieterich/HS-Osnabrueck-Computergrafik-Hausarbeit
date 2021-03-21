#include "ShipWeapon.h"

void ShipWeapon::shoot()
{
	if (shooting) return;

	// Munition reduzieren und prüfen, ob sie leer ist
	if (ammo-- < 0) ammo = 0;
	if (ammo == 0) return;
	shooting = true;

	thread weaponShoot(&ShipWeapon::startTimer, this);
	weaponShoot.detach();
}

void ShipWeapon::startTimer() {
	this_thread::sleep_for(chrono::milliseconds(60000/rpm));
	shooting = false;
}