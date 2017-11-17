#ifndef PLAYER_H
#define PLAYER_H

#include "resoures.h"

typedef struct _player Player;


/*Loads all the starting information of a player provided in a file*/ 
Player* load_player(char *filename);


/*Frees the memory allocated for Player struct*/
void player_delete(Player *pl);


/*Returns the character with which the player is printed*/
char player_getDisplay(Player *pl);


/*Returns where the player is looking to(NORTH, SOUTH, WEST or EAST)*/
int player_getOrientation(Player *pl);


/*Changes the orientation of a given player*/
Status player_setOrientation(Player *pl, int orient);


/*Returns the row of a player (coordinate X)*/
int player_getRow(Player *pl);


/*Returns the column of a player (coordinate Y)*/
int player_getCol(Player *pl);


/*Changes the Location of a given player by receiving its new coordinates*/
Status player_setLocation(Player *pl, int r, int c);

#endif /*PLAYER_H*/