#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "weapon.h"

struct _weapon{
    char *name;
    char display;
    int powder_waste;
    int speed;
    int damage;
    int owned; /*This value is equal to OWNED (1040) or NOT_OWNED (1041)*/
    int equipped;
    int row;
    int col;
};


/*This function loads the weapons from a file, where all the specifications are*/
Weapon** load_weapons(char *filename){
    Weapon **w=NULL;
    FILE *in=NULL;
    char buff[BUFFER_SIZE], name[NAME_SIZE], display;
    int i, j, n_weapons, powder_waste, speed, damage, owned, equipped, row, col;
    
    /* Checking*/
    if(filename == NULL){
        printf("Error. Weapons-F1-1.\n");
        exit(ERROR);
    }
    /*--------------------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Weapons-F1-2.\n");
        exit(ERROR);
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_weapons);
    
    
    w = (Weapon **) malloc((n_weapons+1) * sizeof(Weapon*));
    if(w==NULL){
        printf("Error. Weapons-F1-3.\n");
        fclose(in);
        exit(ERROR);
    }
    
    
    for(i=0; i<n_weapons; i++){
        fgets(buff, BUFFER_SIZE, in);
		sscanf(buff, "%s %c %d %d %d %d %d %d %d", name, &display, &powder_waste, &speed, &damage, &owned, &equipped, &row, &col);
		
		w[i] = create_weapon(name, display, powder_waste, speed, damage, owned, equipped, row, col);
		if(w[i]==NULL){
		    printf("Error. Weapons-F1-4.\n");
		    for(j=i-1; j>=0; j--){
		        delete_weapon(w[j]);
		    }
		    free(w);
		    fclose(in);
		    exit(ERROR);
		}
	}
	w[n_weapons] = NULL;
	
	fclose(in);
	
	return w;
}


/*This function creates a weapon, reserves memory and fill it with the data specified*/
Weapon* create_weapon(char *name, char display, int powder_waste, int speed, int damage, int owned, int equipped, int row, int col){
    Weapon *weap;
    
    /*Checkings*/
    if(damage<0 || name==NULL || powder_waste<0 || speed<0 || row<0 || col<0){
        printf("Error. Weapons-F2-1.\n");
        exit(ERROR);
    }
    if(owned != OWNED && owned != NOT_OWNED){
        printf("Error. Weapons-F2-2.\n");
        exit(ERROR);
    }
    if(equipped != EQUIPPED && equipped != NOT_EQUIPPED){
        printf("Error. Weapons-F2-3.\n");
        exit(ERROR);
    }
    /*-----------------------------------*/
    
     
    weap = (Weapon *) malloc(sizeof(Weapon));
    if(weap==NULL){
        printf("Error. Weapons-F2-4.\n");
        exit(ERROR);
    }
    
     
    weap->name = strdup(name);
    weap->display = display;
    weap->powder_waste = powder_waste;
    weap->speed = speed;
    weap->damage = damage;
    weap->owned = owned;
    weap->equipped = equipped;
    weap->row = row;
    weap->col = col;
    
    return weap;
}


/*This function receives the array created in load_weapons and it frees all its memory*/
void destroy_weapons(Weapon **wp){
    Weapon **aux=NULL;
    
    if(wp == NULL) return;
    
    for(aux = wp; *aux!=NULL; aux++){
        delete_weapon(*aux);
    }
    free(wp);
}


/*This function receives a weapon and it frees all the memory allocated for it*/
void delete_weapon(Weapon *wp){
    if (wp==NULL) return;
    if (wp->name != NULL){
        free(wp->name);
    }
    free (wp);
}


/*Function that returns the name of a certain weapon*/
char *weapon_getName(Weapon* wp){
    if(wp==NULL){
        return NULL;
    }
    return wp->name;
}


/*Function that says that if a weapon is owned or not*/
int own_weapon(Weapon *wp){
    if (wp == NULL) return NOT_OWNED;
    if (wp->owned==OWNED)return OWNED;
    return NOT_OWNED;
}


/*Function that returns the powder waste of a certain weapon*/
int weapon_getPowderWaste(Weapon *wp){
    if(wp == NULL){
        printf("Error. Weapons-F7-1.\n");
        exit(ERROR);
    }
    
    return wp->powder_waste;
}


/*Function that returns the shooting speed of a weapon*/
int weapon_getSpeed(Weapon *wp){
    if(wp == NULL){
        printf("Error. Weapons-F8-1.\n");
        exit(ERROR);
    }
    
    return wp->speed;
}


/*Function that returns the damage of a specific weapon*/
int weapon_getDamage(Weapon *wp){
    if(wp == NULL){
        printf("Error. Weapons-F9-1.\n");
        exit(ERROR);
    }
    
    return wp->damage;
}


/*It changes the status of a given weapon when it´s available*/
Status change_own(Weapon *wp){
    if(wp == NULL){
        printf("Error. Weapons-F10-1.\n");
        exit(ERROR);
    }
    
    wp->owned = OWNED;
    
    return OK;
}


/*This function uses a weapon, it wastes the amount of gunpowder needed to shoot and deals damage in the line it is shot, a Weapon
only uses the resource gunpowder to shoot, the object ammo gives you gunpowder, but we supose we have infinite bullets*/
Status shot_weapon(Weapon **wp, Resources **r){
    Resources *r2=NULL;
    Weapon *w2=NULL;
    
    if(wp==NULL||r==NULL){
        printf("Error. Weapons-F11-1.\n");
        exit(ERROR);
    } 
    
    r2 = resources_getResource(r, AMMO); /*Getting the AMMO resource pointer*/
    if(r2==NULL){
        printf("Error. Weapons-F11-2.\n");
        exit(ERROR);
    }
    
    w2 = weapon_getEquippedWeapon(wp); /*Getting the equipped weapon pointer*/
    if(w2==NULL){
        printf("Error. Weapons-F11-3.\n");
        exit(ERROR);
    }
    
    if(w2->powder_waste > resources_getActualValue(r2)) return FAILED;
    
    if(modify_resource(r2, -(w2->powder_waste)) == ERROR){
        printf("Error. Weapons-F11-4.\n");
        exit(ERROR);
    }
    
    return OK;
}


/*Returns the row where the weapon is going to be shown at the interface*/
int weapon_getRow(Weapon *wp){
    if(wp == NULL){
        printf("Error. Weapons-F12-1\n");
        exit(ERROR);
    }
    
    return wp->row;
}


/*Returns the column where the weapon is going to be shown at the interface*/
int weapon_getCol(Weapon *wp){
    if(wp == NULL){
        printf("Error. Weapons-F13-1.\n");
        exit(ERROR);
    }
    
    return wp->col;
}


/*Function that changes the state EQUIPPED to NOT_EQUIPPED or viceversa*/
/*the int dir argument is to change the equipped weapon for the above weapon or the under one the actual*/
Status change_equipped(Weapon **wp, int dir){
    Weapon **aux=NULL;
    
    /*Checkings*/
    if(wp == NULL){
        printf("Error. Weapons-F14-1.\n");
        exit(ERROR);
    }
    if(dir != UPW && dir != DOWNW){  /*dir indicates in what direction you want to swap the equipped weapon*/
        printf("Error. Weapons-F14-2.\n");
        exit(ERROR);
    }
    /*--------------------------------------*/
    
    
    for(aux = wp; (*aux) != NULL; aux++){
        if((*aux)->equipped == EQUIPPED){
            (*aux)->equipped = NOT_EQUIPPED;
            
            if(dir == DOWNW){
                while(1){
                    aux++;
                    if((*aux) == NULL){ /*If it´s reached the end of the array it has to comeback to the first weapon*/
                        aux = wp;
                    }
                    if((*aux)->owned == OWNED){ /*The weapon must be owned*/
                        (*aux)->equipped = EQUIPPED;
                        return OK;
                    }
                }
            }
            
            if(dir == UPW){
                while(1){
                    if(aux == wp){ /*If it is the first weapon and we want to equipp the last one*/
                        while(*aux != NULL){
                            aux++;
                        }
                    }
                    aux--;
                    if((*aux)->owned == OWNED){ /*The weapon must be owned*/
                        (*aux)->equipped = EQUIPPED;
                        return OK;
                    }
                }
            }
        }
    }
    printf("Error. Weapons-F14-3.\n");
    exit(ERROR);
}


/*Function that returns EQUIPPED if the weapon is equipped at the moment, or NOT_EQUIPPED in the another case*/
int weapon_equipped(Weapon *wp){
    if(wp == NULL){
        printf("Error. Weapons-F15-1.\n");
        exit(ERROR);   
    }
    
    if(wp->equipped == EQUIPPED){
        return EQUIPPED;
    }
    
    else{
        return NOT_EQUIPPED;
    }
}


/*Function that returns the pointer to the equipped weapon at the moment of the call*/
Weapon* weapon_getEquippedWeapon(Weapon **wp){
    Weapon *w2=NULL;
    Weapon **auxw=NULL;
    
    if(wp == NULL){
        printf("Error. Weapons-F16-1.\n");
        exit(ERROR); 
    }
    
    for(auxw = wp; (*auxw) != NULL; auxw++){
        if((*auxw)->equipped == EQUIPPED){
            w2 = *auxw;
        }
    }
    if(w2==NULL){
        printf("Error. Weapons-F16-2.\n");
        exit(ERROR);
    }
    
    return w2;
}


char weapon_getDisplay(Weapon *wp){
    if(wp==NULL){
        printf("Error. Weapons-F17-1.\n");
        exit(ERROR);
    }
    return wp->display;
}