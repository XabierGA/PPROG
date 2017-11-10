#ifndef INTERFACE_H
#define INTERFACE_H

#include "rectangles.h"

typedef struct{
    rectangles **rec_array;
    int n_rectangles;
    char **map;
    
    
}Interface;


Interface* intrf_create(char* filename);

#endif /*INTERFACE_H*/