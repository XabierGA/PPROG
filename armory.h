#ifndef ARMORY_H
#define ARMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

typedef struct _armory Armory;

Armory* create_armory(char* filename);

void destroy_armory(Armory *armory);


 
#endif /* ARMORY.H */