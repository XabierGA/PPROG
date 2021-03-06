#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "weapons.h"

struct _weapon{
    char *name;
    int powder_waste;
    int speed;
    int damage;
    int owned; /*This value is equal to OWNED (1040) or NOT_OWNED (1041)*/
};

Weapon** load_weapons(char *filename){
    Weapon **w=NULL;
    FILE *in=NULL;
    char buff[BUFFER_SIZE], name[NAME_SIZE];
    int i, j, n_weapons, powder_waste, speed, damage, owned;
    
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
    
    
    w = (Weapon **) malloc((n_weapons+1) * sizeof(Weapon*));
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
		    printf("Error. Weapons-F1-4.\n");
		    for(j=i-1; j>=0; j--){
		        delete_weapon(w[j]);
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


void delete_weapon(Weapon *wp){
    if (wp==NULL) return;
    if (wp->name != NULL){
        free(wp->name);
    }
    free (wp);
}

void destroy_weapons(Weapon **wp){
    Weapon **aux=NULL;
    
    if(wp == NULL) return;
    
    for(aux = wp; *aux!=NULL; aux++){
        delete_weapon(*aux);
    }
    free(wp);
}

char *weapon_getName(Weapon* wp){
    if(wp==NULL){
        return NULL;
    }
    return wp->name;
}

int own_weapon(Weapon *wp){
    if (wp == NULL) return NOT_OWNED;
    if (wp->owned==OWNED)return OWNED;
    return NOT_OWNED;
}



int weapon_getPowderWaste(Weapon *wp){
    if(wp == NULL){
        return ERROR;
    }
    
    return wp->powder_waste;
}


int weapon_getSpeed(Weapon *wp){
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

Status change_own(Weapon *wp){
    if(wp == NULL){
        return FAILED;
    }
    wp->owned = OWNED;
    
    return OK;
}

Status shot_weapon(Weapon *wp, Resources **r){
    Resources **aux=NULL;
    Resources *r2=NULL;
    
    if(wp==NULL||r==NULL||wp->owned==NOT_OWNED) return FAILED;
    
    for(aux = r; aux!=NULL; aux++){
        if(resources_getType(*aux)==GUNPOWDER){
            r2 = *aux;
        }
    }
    if(r2==NULL){
        printf("Error. Weapons-F10-1.\n");
        return FAILED;
    } 
    
    if(wp->powder_waste > resources_getActualValue(r2)) return FAILED;
    
    if(modify_resource(r2, -(wp->powder_waste)) == ERROR){
        printf("Ni puta idea de lo que ha pasado.\n");
        return FAILED;
    }
    
    return OK;
}
