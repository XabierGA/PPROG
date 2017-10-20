#ifndef WEAPONS_H
#define WEAPONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

typedef struct _weapon Weapon;

/*This function loads the weapons from a file, where all the specifications are*/
Weapon** load_weapons(char* filename);


/*This function creates a weapon, reserves memory and fill it with the data specified*/
Weapon* create_weapon(char *name, int powder_waste, int speed, int damage);


/*This function uses a weapon, it wastes the amount of gunpowder needed to shoot and deals damage in the line it is shot, a Weapon
only uses the resource gunpowder to shoot, the object ammo gives you gunpowder, but we supose we have infinite bullets*/
Weapon* shot_weapon(Weapon* wp, /*here we need to pass the resource gunpowder*/);

/*This function receives a weapon and it frees all the memory allocated for it*/
void destroy_weapon(Weapon* wp);

#endif /* WEAPONS_H */ 