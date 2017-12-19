#ifndef INTERFACE_H
#define INTERFACE_H

#include "resources.h"
#include "object.h"
#include "weapon.h"
#include "rectangle.h"
#include "maps.h"
#include "player.h"

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
    int map_id;
    int dir;
} shoot_stuff;


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


/*Prints the resources, weapons, objects, player and initial map at the beggining of the program*/
Status initialize_intrf(Interface *intrf, int initial_map, Resources **r, Weapon **wp, Object **obj, Player *pl);


/*Function that allows the player to move*/
void move(Interface *intrf, int map_id, Player *pl, int dir);


/*Function that allows the player to shoot*/
void *shoot(void *);

#endif /*INTERFACE_H*/