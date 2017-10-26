#ifndef INVENTORY_H
#define INVENTORY_H

#include "defines.h"

typedef struct _inventory Inventory;


/*Function that reads the size of the inventory from a file and creates it*/
Inventory* create_inventory(char* filename);


/*Function that receives a pointer to an inventory and liberates all the memory allocated*/
void delete_inventory(Inventory *inv);


/*Function that inserts or removes one unit of a certain object. If mode is 0, then we check that the item exists on the inventory, 
then we use it and then we delete it. If the mode is 1, we insert the item into the inventory.*/
Status add_inventory_object(Inventory *inv, Object *obj);
#endif /* INVENTORY_H */