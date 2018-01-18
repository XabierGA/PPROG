#ifndef INTERFACE_H
#define INTERFACE_H

#include "resources.h"
#include "object.h"
#include "weapon.h"
#include "rectangle.h"
#include "maps.h"
#include "player.h"
#include "strings.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

struct _interface{
    rectangle **rect_array;  /* Array of rectangle */
    int n_rectangles;       /* Total number of rectangle */
    Maps **maps_array;  /* Array of maps */
    int n_maps;         /* Total number of maps */
};
typedef struct _interface Interface;



typedef struct{ /* Struct where all the parameters to shoot function are saved*/
    Interface *intrf;
    Weapon **wp;
    Player *pl;
    Resources **r;
    Maps *copymap;
    int dir;
    Enemy **ene;
} shoot_stuff;


typedef struct{
    Interface *intrf;
    Maps *copymap;
    Player *pl;
    Resources **r;
    Enemy *ene;
} moveEne_stuff;


/*Creates the interface struct reading all the rectangles information and all the maps from the files*/
Interface* create_intrf(char* rect_fname, char* maps_fname);


/*Frees all the allocated memory for the interface*/
void destroy_intrf(Interface *intrf);


/*Function that prints all the actual resources information of the array*/
Status print_resources(Interface *intrf, Resources **r);


/*Function that prints all the actual weapons information of the array*/
Status print_weapons(Interface *intrf, Weapon **wp);


/*Function that prints all the actual objects information of the array*/
Status print_objects(Interface *intrf, Object **obj);


/*Function that prints the map with map_id as id*/
Status print_map(Interface *intrf, int map_id);


Status print_enemies(Interface *intrf, Enemy **ene);


Status print_player(Interface *intrf, Player *pl);


/*Prints the resources, weapons, objects, player and initial map at the beggining of the program*/
Status initialize_intrf(Interface *intrf, int initial_map, Resources **r, Weapon **wp, Object **obj, Strings **str);


/*Function that allows the player to move*/
int move(Interface *intrf, Maps *copymap, Player *pl, int dir);


/*Function that allows the player to shoot*/
void *shoot(void *);

/*Function that places the enemies of a given array in a random possition of a given map*/
Status generate_EnePosRand(Enemy **ene, Maps *copymap);

/*Function that allows enemies to move. Intended to be used in a thread*/
void* move_enemies(void *y);

/*Funcion that tells if a enemy has the player on its sights. Gets thre row and col of both of them*/
Boolean isOnSight(Maps *map, int rp, int cp, int re, int ce, int* flag);

/*Function that tells if the enemy is in one tile distance from the player, including diagonally*/
Boolean isNextTo(Maps *map, int rp, int cp, int re, int ce, int *next_row, int *next_col);

/*Function that allows us to block all the threads*/
void interface_lock();

/*Function that allows us to unlock all the threads*/
void interface_unlock();

#endif /*INTERFACE_H*/