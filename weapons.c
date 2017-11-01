#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "weapons.h"

struct _weapon{
    char *name;
    int powder_waste;
    int speed;
    int damage;
    int owned;/*This value is equal to OWNED (1040) or NOTOWNED (1041)*/
};

Weapon** load_weapons(char *filename){
    Weapon **w=NULL;º
    FILE *in=NULL;
    char buff[BUFFER_SIZE], name[NAME_SIZE];
    int i, n_weapons, powder_waste, speed, damage, owned;
    
    /* Checking*/
    if(filename == NULL){
        printf("Error. Weapons-F1-1.\n");
    }
    /*--------------------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Weapons-F1-2.\n");
        return NULL;
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_weapons);
    
    
    w = (Weapon **) malloc((n_weapons+1) * sizeof(Weapon*);
    if(w==NULL){
        printf("Error. Weapons-F1-3.\n");
        fclose(in);
        return NULL;
    }
    
    
    for(i=0; i<n_weapons; i++){
        fgets(buff, BUFFER_SIZE, in);
		sscanf(buff, "%s %d %d %d %d", name, &powder_waste, &speed, &damage, &owned);
		
		w[i] = create_weapon(name, powder_waste, speed, damage, owned);
		if(w[i]==NULL){
		    printf("Error. EWeapons-F1-4.\n");
		    for(j=i-1; j>=0; j--){
		        delete_enemy(w[j]);
		    }
		    free(w);
		    fclose(in);
		    return NULL;
		}
	}
	w[n_weapons] = NULL;
	
	fclose(in);
	
	return w;
}



Weapon* create_weapon(char *name, int powder_waste, int speed, int damage, int owned){
    Weapon *weap;
     
    if(damage<0 || name==NULL || powder_waste<0 || speed<0){
        printf("Error. Weapons-F2-1.\n");
        return NULL;
    }
    
    if(owned != OWNED || owned != NOT_OWNED)
     
    weap = (Weapon *) malloc(sizeof(Weapon));
    if(weap==NULL){
        printf("Error. Weapons-F2-2.\n");
        return NULL;
    } 
     
    weap->name = strdup(name);
    weap->powder_waste = powder_waste;
    weap->speed = speed;
    weap->damage = damage;
    weap->owned = owned;
     
    return weap;
}


void destroy_weapon(Weapon *wp){
    if (wp==NULL) return;
    if (wp->name != NULL){
        free(wp->name);
    }
    free (wp);
}



Boolean own_weapon(Weapon *wp){
    if (wp == NULL) return F;
    if (wp->owned==OWNED)return T;
    return F;
}



int weapon_getPowderWaste(Weapon *wp){
    if(wp == NULL){
        return ERROR;
    }
    
    return wp->powder_waste;
}


int weapom_getSpeed(Weapon *wp){
    if(wp == NULL){
        return ERROR;
    }
    
    return wp->speed;
}


int weapon_getDamage(Weapon *wp){
    if(wp == NULL){
        return ERROR;
    }
    
    return wp->damage;
}


