#ifndef RESOURCES_H
#define RESOURCES_H

#include "defines.h"

typedef struct _resources Resources;

/*Function that read the text file and makes a resources array*/
Resources** load_resources(char *filename);


/*Function that receives all the resources fields and returns a created resource*/
Resource* create_resource(int type, char *name, int max, int actual, int row, int col);


/*Function that receives a pointer to a resource and liberates all the memory allocated*/
void delete_resource(Resources *r);


/*Function that modifies the value of a given resource*/
int modify_resource(Resources *r, int value);


/* Function that gets the object type of a given resource*/
int resources_getObjectType(Resources *r);


/* Function that gets the actual value of a given resource*/
int resources_getActualValue(Resources *r);


/*Function that returns the row where the resource must be printed in the interface*/
int resources_getRow(Resources *r);


/*Function that returns the col where the resource must be printed in the interface*/
int resources_getCol(Resources *r);

#endif /* RESOURCES_H */
