#include <stdio.h>

#include "resources.h"

typedef struct _resources{
    int resource_type;
    char *name;
    int maximum;
    int actual;
    int row;
    int column;
};


Resources* create_resource(FILE *f){
    Resources *r=NULL;
    
    /*Checking*/
    if(type != HP || type != HYDRATION || type != GUNPOWDER || type != HUNGER){
        return NULL;
    }
    if(max <= 0 || actual>max || actual<0){
        return NULL;
    }
        
    if(name == NULL){
        return NULL;
    }
    
    
    
    r = (Resources *) malloc(sizeof(Resources);
    if(r==NULL){
        return NULL;
    }
    
    r->typ
}

