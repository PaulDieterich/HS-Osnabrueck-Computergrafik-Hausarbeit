#include "Asteroid.h"

void Asteroid::receiveDamage(float rawDamage)
{
	if (health <= 0) return;
	health -= rawDamage / size * 2;

	if (health < 0) health = 0;
	if (health == 0) load(ASSET_DIRECTORY "models/asteroids/destroyed.obj", true);
}
