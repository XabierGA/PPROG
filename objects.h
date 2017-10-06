#ifndef OBJECTS_H
#define OBJECTS_H

#include "defines.h"
#include "resources.h"

#define MAXNAME 64
/* In the following structure:
        object_type: depending on the number received it is one type or another; resource(ammo as well), enemy, weapons, usables.
        name: name as the objects is know in the game.
        
        

*/

typedef struct _object Object;
  /*Function that loads objects from a file*/
  Object** load_objects ()
  
  /*Function that receives the name, the serial number, the type and creates an object*/
  Object* create_object (char *name, int type, int value, int amount);
  
  /*Function that receives an object and destroys it*/
  void destroy_object (Object *ob);
  
  /*Function that uses an object*/
  status use_object (Object *ob);
  
  
  /*Function that */
  
  
  
  

#endif /* OBJECTS_H */

