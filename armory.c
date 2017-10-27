#include "armory.h"


typedef struct _armory{
    Weapon *weapons;
    int n_weapons;
    int last;
};


Armory* create_armory(char* filename){
    Armory *arm=NULL;
    FILE *in=NULL;
    int n_wp;
    
    
    /*Checking*/
    if(filename == NULL){
        printf("Error. Armory-F1-1.\n")
    }
    /*-----------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Armory-F1-2.\n");
        return NULL;
    }
    
    fgets(buff, MAX, in);
    sscanf(buff, "%d", &n_wp);
    
    arm = (Armory *) malloc(n_wp * sizeof(Armory));
    if(arm == NULL){
        printf("Error. Armory-F1-3.\n");
        fclose(in);
    }
    
    arm->n_weapons = n_wp;
    arm->last = 0;
    fclose(in);
    
    return arm;
}


void delete_armory(Armory *ar){
    int i;
    if(ar == NULL){
        return;
    }
    for (i = 0; i<= ar->last; i++){
        if (ar->Weapon[i] != NULL){
            destroy_weapon(ar->Weapon[i]);
        }
    }
    
    free(ar);
}