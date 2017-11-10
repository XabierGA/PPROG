#ifndef ENEMIES_H
#define ENEMIES_H

#include "defines.h"
#include "resources.h"
#define MAX_HEALTHRANGE 100

struct _enemies{
    char *name;             /*Name of the enemy*/
    char display;           /*Char with which the enemy is printed*/
    int type;               /*Enemy type: ZOMBIRE(deals damage) WALL(doesn´t deal damage)*/   
    int HP;                 /*HP of the enemy*/
    int physical_status;    /*Fields which is is used to know if a given enemy is dead or alive*/
    int speed;              /*Speed with witch the enemy moves*/
    int damage;             /*Damage dealt by the enmy*/
    int row;                /*Row of the enemy´s position*/
    int col;                /*Column of the enemy´s position*/
};

typedef struct _enemies Enemy;


/*This array of arrays of enemies is used to know all the enemies we are going to have in the game.
This function reads all the enemies information that it´s placed in a file.*/
Enemy** load_enemies(char *filename);


/*Function that creates an enemy*/
/*THIS FUNCTION WORKS AS A HELP TO "load_enemies" FUNCTION*/
Enemy* create_enemy(char *name, char display, int type, int HP, int phy_stat, int speed, int damage, int row, int col);


/*Function that destroys an enemy, it receives an enemy and frees all the memory allocated for it*/
/*It works mainly as a help to "destroy_enemies" function*/
void delete_enemy(Enemy *e); 


/*Function that destroys all the enemies of a given array*/
void destroy_enemies(Enemy **e);


/*Function that modifies an enemy's health once is hitten. It returns the final value of its HP or ERROR*/
int modify_enemyhp(Enemy *e, int increase);


/*Function that makes an enemy attack the player, we will need here to make the enemies go to the current
location of the player, if the enemies are in a close range the player will automaticlly loose health, but this
mechanic will be implemented in other TAD*/
Enemy* attack_player(Enemy*e, Resources*r);


/*This function recieves the double pointer which has all the information of the enemies, and it generates
the enemy with the same name as "name", the second input parameter */
Enemy* generate_enemy(Enemy** pe, char* name);


/*Function that gets the display character of a given enemy*/
char enemy_getDisplay(Enemy *ene);


/*Function that returns the type of a given enemy*/
int enemy_getType(Enemy *ene);


/*Function that returns the HP of a given enemy*/
int enemy_getHP(Enemy *ene);


/*Function that returns the moving speed of a given enemy*/
int enemy_getSpeed(Enemy *ene);


/*Function that gets the damage of a given enemy*/
int enemy_getDamage(Enemy *ene);


/*Function that returns the row of a given enemy*/
int enemy_getRow(Enemy *ene);


/*Function that returns the column of a given enemy*/
int enemy_getCol(Enemy *ene);


/*Function that returns the name of a given enemy*/
char* enemy_getName(Enemy *ene);


/*Function that returns if one enemy is ALIVE or DEAD*/
int enemy_getPhyStat(Enemy *ene);


#endif /* ENEMIES_H */