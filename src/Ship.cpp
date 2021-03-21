#include "Ship.h"



void Ship::receiveDamage(float rawDamage)
{
    if (health <= 0) return;

    // Schaden hinzuf�gen je nachdem, ob noch Schild vorhanden ist
    if (shield <= 0) {
        health -= rawDamage;
    }
    else {
        health -= rawDamage * (1 / shield);
        shield -= 0.1 * rawDamage;
    }

    // Das Schiff sollte niemals weniger als 0 Leben haben
    if (health < 0) health = 0;
}


bool Ship::collectResource(Resource* resource, int amount)
{
    // Wenn Ressource Treibstoff ist und der Treibstoff etwas leer ist 
    // Dann direkt aufnehmen
    if (resource->isFuel() && fuel < fuelMax) {
        fuel += amount * resource->getWeight() * resource->getRarity();
        if (fuel > fuelMax) fuel = fuelMax;
        return true;
    }

    // Sonst Ressource aufnehmen, wenn m�glich
    if (cargoWeight + (resource->getWeight() * amount) < cargoCapacity) {
        // Da wir eine map haben wird das Pair automatisch hinzugef�gt, wenn noch nicht vorhanden
        cargo[resource] += amount;
        return true;
    }
    return false;
}

void Ship::drive(bool boost)
{
    if (fuel <= 0) return;
    // Da diese Funktion bei Movement jeden Frame aufgerufen wird, muss der Wert entsprechend angepasst werden
    // ~ 60 Frames pro Sekunde -> Treibstoff verbrauch pro Minute 
    // Boost ber�cksichtigen

    float usage = fuelUsage;
    if (boost) usage *= 2;

    fuel -= usage / (60 * 60); // Durch 60 f�r Minute in Sekunden und nochmal f�r Frames
    if (fuel < 0) fuel = 0;
}

void Ship::shoot()
{
    weapon.shoot();
}

void Ship::refill()
{
    weapon.refill();
    health = maxHealth;
    shield = maxShield;
    fuel = fuelMax;
}
