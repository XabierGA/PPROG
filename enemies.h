#ifndef ENEMIES_H
#define ENEMIES_H

#include "defines.h"
#define MAX_HEALTHRANGE 100

typedef struct _enemies Enemy;


/*This array of arrays of enemies is used to know all the enemies we are going to have in the game.
This function reads all the enemies information that it´s placed in a file.*/
Enemy** load_enemies(char *filename);

/*Function that creates an enemy (funtion mainly used in "Load_enemies")*/
Enemy* create_enemy(char *name, char display, int type, int HP, int speed, int damage);


/*Function that destroys an enemy, it receives an enemy and frees all the memory allocated for it*/
void destroy_enemy(Enemy *e);


/*Function that modifies an enemy's health once is hitten*/
Enemy* modify_enemyhp(Enemy *e, int increase);


/*Function that makes an enemy attack the player, we will need here to make the enemies go to the current
location of the player, if the enemies are in a close range the player will automaticlly loose health, but this
mechanic will be implemented in other TAD*/
Enemy* attack_player(Enemy*e, Resource*r);


/*This function recieves the double pointer which has all the information of the enemies, and it generates
the enemy with the same name as "name", the second input parameter */
Enemy* generate_enemy(Enemy** pe, char* name);


#endif /* ENEMIES_H */