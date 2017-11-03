#ifndef WEAPONS_H
#define WEAPONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "resources.h"

typedef struct _weapon Weapon;

/*This function loads the weapons from a file, where all the specifications are*/
Weapon** load_weapons(char* filename);


/*This function creates a weapon, reserves memory and fill it with the data specified*/
Weapon* create_weapon(char *name, int powder_waste, int speed, int damage, int owned);


/*This function receives a weapon and it frees all the memory allocated for it*/
void delete_weapon(Weapon* wp);

/*This function receives the array created in load_weapons and it frees all its memory*/
void destroy_weapons(Weapon** wp);

/*Function that returns the name of a certain weapon*/
char *weapon_getName(Weapon* wp);

/*Function that says that if a weapon is owned or not*/
int own_weapon(Weapon *wp);

/*Function that returns the powder waste of a certain weapon*/
int weapon_getPowderWaste(Weapon *wp);


/*Function that returns the shooting speed of a weapon*/
int weapon_getSpeed(Weapon *wp);


/*Function that returns the damage of a specific weapon*/
int weapon_getDamage(Weapon *wp);

Status change_own(Weapon *wp);

/*This function uses a weapon, it wastes the amount of gunpowder needed to shoot and deals damage in the line it is shot, a Weapon
only uses the resource gunpowder to shoot, the object ammo gives you gunpowder, but we supose we have infinite bullets*/
Status shot_weapon(Weapon* wp, Resources** r);

#endif /* WEAPONS_H */ 