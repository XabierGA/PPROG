#ifndef WEAPONS_H
#define WEAPONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defines.h"

typedef struct _weapons weapons;

Weapons** load_weapons();

Weapons* create_weapons();

void destroy_weapons(Weapons*);

#endif /* WEAPONS.H */