#ifndef INTERFACE_H
#define INTERFACE_H

#include "resources.h"
#include "objects.h"
#include "weapons.h"
#include "rectangles.h"
#include "maps.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct _interface{
    rectangles **rect_array;
    int n_rectangles;
    Maps **maps_array;
    int n_maps;
};

typedef struct _interface Interface;

Interface* create_intrf(char* filename);

void destroy_intrf(Interface *intrf);

#endif /*INTERFACE_H*/