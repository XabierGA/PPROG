#ifndef WEAPON_H
#define WEAPON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"
#include "resources.h"

typedef struct _weapon Weapon;

/*This function loads the weapons from a file, where all the specifications are*/
Weapon** load_weapons(char* filename);


/*This function creates a weapon, reserves memory and fill it with the data specified*/
Weapon* create_weapon(char *name, char display, int powder_waste, int speed, int damage, int owned, int equipped, int row, int col);


/*This function receives the array created in load_weapons and it frees all its memory*/
void destroy_weapons(Weapon** wp);


/*This function receives a weapon and it frees all the memory allocated for it*/
void delete_weapon(Weapon* wp);


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


/*It changes the status of a given weapon when it´s available*/
Status change_own(Weapon *wp);


/*This function uses a weapon, it wastes the amount of gunpowder needed to shoot and deals damage in the line it is shot, a Weapon
only uses the resource gunpowder to shoot, the object ammo gives you gunpowder, but we supose we have infinite bullets*/
Status shot_weapon(Weapon** wp, Resources** r);


/*Returns the row where the weapon is going to be shown at the interface*/
int weapon_getRow(Weapon *wp);


/*Returns the column where the weapon is going to be shown at the interface*/
int weapon_getCol(Weapon *wp);


/*Function that changes the state EQUIPPED to NOT_EQUIPPED or viceversa*/
/*the int dir argument is to change the equipped weapon for the above weapon or the under one the actual*/
Status change_equipped(Weapon **wp, int dir);


/*Function that returns EQUIPPED if the weapon is equipped at the moment, or NOT_EQUIPPED in the another case*/
int weapon_equipped(Weapon *wp);


/*Function that returns the pointer to the equipped weapon at the moment of the call*/
Weapon* weapon_getEquippedWeapon(Weapon **wp);

/*Function that gets the display character of the bullets of a given weapon*/
char weapon_getDisplay(Weapon *wp);

#endif /* WEAPON_H */