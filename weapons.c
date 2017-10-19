#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "weapons.h"

typedef struct _weapons{
    char* name;
    int powder_waste;
    int speed;
    int damage;
};

Weapons** load_weapons(char *filename){
    
}

Weapons* create_weapon(char *name, int powder_waste, int speed, int damage){
    
}

Weapons* shot_weapon(Weapons *wp, /*Gunpowder*/){
    
}

void destroy_weapon(Weapons *wp){
    
}