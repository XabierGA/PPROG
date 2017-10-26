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


/* Function that display a resource and its ammount. In the interface, the resources should be updated
every time they are changed */
Status display_resource(?????, char *resource_name);

/* Function that gets the object type of a given resource*/
int resources_get_ObjectType(Resources *r);

/* Function that gets the actual value of a given resource*/
int resources_get_ActualValue(Resources *r);
#endif /* RESOURCES_H */



