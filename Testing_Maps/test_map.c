#include "maps.h"

void main(){
    Maps **m;
    m = load_maps("map.txt");
    destroy_maps(m);
    return;
}