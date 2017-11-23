#include "interface.h"
#include "resources.h"
#include "object.h"
#include "player.h"
#include "weapon.h"
#include "maps.h"
#include "rectangle.h"

int main(){
    Resources **r=NULL;
    Weapon **wp=NULL;
    Object **obj=NULL;
    Player *pl=NULL;
    Interface *intrf=NULL;
    
    r = load_resources("resources.txt");
    if(r == NULL) exit(12345);
    
    wp = load_weapons("weapons.txt");
    if(wp == NULL) exit(12345);
    
    obj = load_objects("objects.txt");
    if(obj == NULL) exit(12345);
    
    pl = load_player("player.txt");
    if(pl == NULL) exit(12345);
    
    intrf = create_intrf("rectangles.txt", "map.txt");
    if(intrf == NULL) exit(12345);
    
    if(initialize_intrf(intrf, 0, r, wp, obj, pl) == FAILED) exit(12345);
    
    destroy_resources(r);
    
    destroy_weapons(wp);
    
    destroy_objects(obj);
    
    player_delete(pl);
    
    destroy_intrf(intrf);
    
    return 0;
}