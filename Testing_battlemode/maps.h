#ifndef MAPS_H
#define MAPS_H

#include "defines.h"
#include "player.h"
#include "enemies.h"

struct _maps{
    char **field;
    int n_rows;
    int n_cols;
    int map_id;
};

typedef struct _maps Maps;


/*Function that recieves the name of a file where all the maps are drawn. It allocates all the needed memory for each
of them and returns a pointer to the array of maps*/
Maps** load_maps(char *filename);


/*Function that recieves the number of rows and the number of columns of a map and it allocates memory for a
char field (matrix n_rows*n_cols). Returns a pointer to this map*/
Maps* create_map(int rows, int cols, int map_id);


/*Frees the memory used for an array of maps*/
void destroy_maps(Maps **maps);


/*Frees the memory of a given map*/
void delete_map(Maps *map);


/*Gets the id of a given map*/
int map_getId(Maps *map);


/*Gets the number of rows of a given map*/
int map_getRows(Maps *map);


/*Gets the number of columns of a given map*/
int map_getCols(Maps *map);


/*Function that creates a copy of a map identified with its id*/
/*It allows us to modificate the map as much as we wanto to in a certain point of the program*/
Maps* map_getCopy(Maps** maps, int id);

#endif /*MAPS_H*/