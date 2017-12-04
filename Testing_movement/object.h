#ifndef OBJECT_H
#define OBJECT_H

#include "defines.h"
#include "resources.h"



typedef struct _object Object;


/*Function that load all the objects from a file*/
Object** load_objects(char *filename);
  
  
/*Function that receives the name, the serial number, the type and creates an object (used mainly in "load_objects" function)*/
Object* create_object(char *name, int type, int value, int amount, int r, int c);

/*Function that deletes all the objects in a given array of objects*/
void destroy_objects(Object** ob);
  
  
/*Function that receives an object and destroys it*/
void delete_object(Object *ob);


/*Function that returns the type of a given object*/
int object_getType(Object *ob);

/*Function that returns the amount of an object*/
int object_getAmount(Object *ob);

/*Function that returns the value that one resource can be modified by this object*/
int object_getValue(Object *ob);

/*Function that returns the name of a given object*/
char* object_getName(Object *ob);

/*Function that returns the row of a given object*/
int object_getRow(Object *ob);

/*Function that returns the column of a given object*/
int object_getColumn(Object *ob);

/*Function that increases the amount of a given object*/
Status object_changeAmount(Object *ob, int inc);

/*Function that uses an object*/
Status use_object(Resources **r, Object *o);

/* Function that returns the pointer to the object (which is inside the array of pointers) of the type passed by argument*/
Object* object_getObject(Object **o, int type);


#endif /* OBJECT_H */  