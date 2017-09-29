#ifndef RESOURCES_H
#define RESOURCES_H

#include "defines.h"

typedef struct _resources Resources;

Resources* create_resource(int type, int max, int actual, char *name, int row, int col);

void delete_resource(Resources *r);


Status modify_hp(Resources *r, int value);

Status modify_gunpowder(Resources *r, int value);

Status modify_hunger(Resources *r, int value);

Status modify_hydration(Resources *r, int value);



#endif /* RESOURCES_H */