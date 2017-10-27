#define "inventory.h"

typedef struct _inventory{
    Object *objects;
    int n_objects;
    int last;
};


Inventory* create_inventory(char* filename){
    Inventory *inv=NULL;
    FILE *in=NULL;
    int n_ob;
    
    
    /*Checking*/
    if(filename == NULL){
        printf("Error. Inventory-F1-1.\n")
    }
    /*-----------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Inventory-F1-2.\n");
        return NULL;
    }
    
    fgets(buff, MAX, in);
    sscanf(buff, "%d", &n_ob);
    
    inv = (Inventory *) malloc(n_ob * sizeof(Inventory));
    if(inv == NULL){
        printf("Error. Inventory-F1-3.\n");
        fclose(in);
    }

    inv->n_objects = n_ob;
    inv->last = 0;
    
    return inv;
}



void delete_inventory(Inventory *inv){
    if(inv == NULL){
        return;
    }
    
    free(inv);
}



Status add_inventory_object(Inventory *i, Object *obj){
    if (i == NULL||obj ==NULL){
        printf("Error. Inventory-F3-1.\n");
        return FAILED;
    }
    
    if(i->last == (i->n_objects - 1)){
        printf("Error. Inventory-F3-2.\n");
        return FAILED;
    }
    
    i->objects[i->last] = obj;
    i->last++;
    
    return OK;
}