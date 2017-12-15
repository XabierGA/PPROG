                                                                         #ifndef MAPS_H
#define MAPS_H

#include "defines.h"

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


/*Frees the memory of a given map*/
void delete_map(Maps *map);


/*Frees the memory used for an array of maps*/
void destroy_maps(Maps **maps);

/*Gets the id of a given map*/
int map_getId(Maps *map);

#endif /*MAPS_H*/