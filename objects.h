#ifndef OBJECTS_H
#define OBJECTS_H
#include "defines.h"
#define MAXNAME 64
/* In the following structure:
        object_type: depending on the number received it is one type or another; resource(ammo as well), enemy, weapons, usables.
        name: name as the objects is know in the game.
        
        

*/


typedef struct _object Object;
  
  /*Function that receives the name, the serial number, the type and creates an object*/
  Object *create_object (int *serial, int *type, char *name);
  
  /*Function that receives an object and destroys it*/
  void destroy_object (Object *ob);
  
  
  
  
  
  
  
  
  

#endif /* OBJECTS_H */