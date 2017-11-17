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

struct _interface{
    rectangle **rect_array;  /* Array of rectangle */
    int n_rectangles;       /* Total number of rectangle */
    
    Maps **maps_array;  /* Array of maps */
    int n_maps;         /* Total number of maps */
};

typedef struct _interface Interface;

Interface* create_intrf(char* rect_fname, char* maps_fname);

void destroy_intrf(Interface *intrf);

Status print_resources(Interface *intrf, Resources **r);

Status print_weapons(Interface *intrf, Weapon **wp);

Status print_objects(Interface *intrf, Object **obj);

Status print_map(Interface *intrf, int map_id);

Status initialize_intrf(Interface *intrf, int initial_map, Resources **r, Weapon **wp, Object **obj, Player *pl);

#endif /*INTERFACE_H*/