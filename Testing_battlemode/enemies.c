#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enemies.h"

/*This array of arrays of enemies is used to know all the enemies we are going to have in the game.
This function reads all the enemies information that it´s placed in a file.*/
Enemy** load_enemies(char *filename){
    Enemy **e=NULL;
    FILE *in=NULL;
    char buff[BUFFER_SIZE], name[NAME_SIZE];
    char display;
    int i, j, n_ene, type, HP, physical_status, speed, damage;
    
    
    /*Checking*/
    if(filename == NULL){
        printf("Error. Enemies-F1-1.\n");
        exit(ERROR);
    }
    /*-----------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Enemies-F1-2.\n");
        exit(ERROR);
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_ene);
    
    
    e = (Enemy **) malloc((n_ene+1) * sizeof(Enemy*));
    if(e==NULL){
        printf("Error. Enemies-F1-3.\n");
        fclose(in);
        exit(ERROR);
    }
    
    
    for(i=0; i<n_ene; i++){
        fgets(buff, BUFFER_SIZE, in);
		sscanf(buff, "%s %c %d %d %d %d %d", name, &display, &type, &HP, &physical_status, &speed, &damage);
		
		e[i] = create_enemy(name, display, type, HP, physical_status, speed, damage);
		if(e[i]==NULL){
		    printf("Error. Enemies-F1-4.\n");
		    for(j=i-1; j>=0; j--){
		        delete_enemy(e[j]);
		    }
		    free(e);
		    fclose(in);
		    exit(ERROR);
		}
	}
	e[n_ene] = NULL;
	
	fclose(in);
	
	return e;
	
}


/*Function that creates an enemy*/
/*THIS FUNCTION WORKS AS A HELP TO "load_enemies" FUNCTION*/
Enemy* create_enemy(char* name, char display, int type, int HP, int phy_stat, int speed, int damage){
    Enemy *ene;
     
    if(damage<0||HP<0||speed<0||name==NULL){
        printf("Error. Enemies-F2-1.\n");
        exit(ERROR);
    }
    if(type!=WALL && type!=ZOMBIE){
        printf("Error. Enemies-F2-2.\n");
        exit(ERROR);
    }
    if(phy_stat!=ALIVE && phy_stat!=DEAD){
        printf("Error. Enemies-F2-3.\n");
        exit(ERROR);
    }
     
    ene = (Enemy *) malloc(sizeof(Enemy));
    if(ene==NULL){
        printf("Error. Enemies-F2-4.\n");
        exit(ERROR);
    } 
     
    ene->name = strdup(name);
    ene->display = display;
    ene->type = type;
    ene->HP = HP;
    ene->physical_status = phy_stat;
    ene->speed = speed;
    ene->damage = damage;
     
    return ene;
}


/*Function that destroys all the enemies of a given array*/
void destroy_enemies(Enemy **e){
    Enemy **aux=NULL;
    
    if(e == NULL) return;
    
    for(aux = e; *aux!=NULL; aux++){
        delete_enemy(*aux);
    }
    free(e);
}
 
 
/*Function that destroys an enemy, it receives an enemy and frees all the memory allocated for it*/
/*It works mainly as a help to "destroy_enemies" function*/
void delete_enemy(Enemy *e){
    if (e==NULL) return;
    
    if (e->name != NULL){
        free(e->name);
    }
    
    free(e);
}

 
/*Function that modifies an enemy's health once is hitten. It returns the final value of its HP or ERROR*/
int modify_enemyhp(Enemy *e, int value){
    if (e == NULL){
        printf("Error. Enemies-F5-1.\n");
        exit(ERROR);
    }    
    if(e->physical_status == DEAD) return 0;
    
    e->HP = e->HP + value;
    if (e->HP >= MAX_HEALTHRANGE){
        e->HP = MAX_HEALTHRANGE;
    }
    
    if(e->HP <= 0){
        e->physical_status = DEAD;
    }
    
    return e->HP;
}


/*Function that generates an array of enemies of the given type*/
Enemy** generate_arrayEnemies(Enemy **pe, int *n_ene, int size){
    Enemy **e=NULL, **aux=NULL;
    int i, j, k=0;
    int total=0;
    
    if(pe==NULL || n_ene==NULL){
        printf("Error. Enemies-F6-1.\n");
        exit(ERROR);
    }
    
    for(i=0; i<size; i++){
        total += n_ene[i];
    }
    
    e = (Enemy **) malloc(total * sizeof(Enemy*));
    if(e==NULL){
        printf("Error. Enemies-F6-2.\n");
        exit(ERROR);
    }
    for(i=0; i<total; i++){
        e[i] = (Enemy *) malloc(sizeof(Enemy));
        if(e[i]==NULL){ /*Error*/
            printf("Error. Enemies-F6-3.\n");
            for(j=0; j<i; j++){
                free(e[j]);
            }
            free(e);
            exit(ERROR);
        }
    }
    
    for(aux = pe, i=0; i<size; i++, aux++){
        for(j=0; j<n_ene[i]; j++){
            e[k] = generate_enemy(pe, enemy_getName(*aux));
            k++;
        }
    }
    
    return e;
}


/*This function recieves the double pointer which has all the information of the enemies, and it generates
the enemy with the same name as "name", the second input parameter */
Enemy* generate_enemy(Enemy **pe, char *name){
    Enemy *e=NULL;
    Enemy **i=NULL;
    
    if(name == NULL||pe == NULL){
        printf("Error. Enemies-F7-1.\n");
        exit(ERROR);
    }
    
    i=pe;
    
    while(*i != NULL){
        if(strcmp((*i)->name, name)==0){
            e = (Enemy *) malloc(sizeof(Enemy));
            if(e == NULL){
                printf("Error. Enemies-F7-2.\n");
                exit(ERROR);
            }
            e->name = strdup((*i)->name);
            e->display = (*i)->display;
            e->type = (*i)->type;
            e->HP = (*i)->HP;
            e->physical_status = ALIVE;
            e->speed = (*i)->speed;
            e->damage = (*i)->damage;
            
            return e;
        }
        i++;
    }
    printf("Error. Enemies-F7-3.\n");
    exit(ERROR);
}


/*Function that gets the display character of a given enemy*/
char enemy_getDisplay(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F8-1.\n");
        exit(ERROR);
    }
    
    return ene->display;
}


/*Function that returns the type of a given enemy*/
int enemy_getType(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F9-1.\n");
        exit(ERROR);
    }
    
    return ene->type;
}


/*Function that returns the HP of a given enemy*/
int enemy_getHP(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F10-1.\n");
        exit(ERROR);
    }
    
    return ene->HP;
}


/*Function that returns the moving speed of a given enemy*/
int enemy_getSpeed(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F11-1.\n");
        exit(ERROR);
    }
    
    return ene->speed;
}


/*Function that gets the damage of a given enemy*/
int enemy_getDamage(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F12-1.\n");
        exit(ERROR);
    }
    
    return ene->damage;
}


/*Function that returns the row of a given enemy*/
int enemy_getRow(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F13-1.\n");
        exit(ERROR);
    }
    
    return ene->row;
}


/*Function that returns the column of a given enemy*/
int enemy_getCol(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F14-1.\n");
        exit(ERROR);
    }
    
    return ene->col;
}


/*Function that returns the name of a given enemy*/
char* enemy_getName(Enemy *ene){
    if(ene==NULL){
        printf("Error. Enemies-F15-1.\n");
        exit(ERROR);
    }
    
    return ene->name;
}


/*Function that returns if one enemy is ALIVE or DEAD*/
int enemy_getPhyStat(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F16-1.\n");
        exit(ERROR);
    }
    
    return ene->physical_status;
}


/*Function that sets the row and the col of a given enemy*/
void enemy_setLocation(Enemy *ene, int row, int col){
    if (ene == NULL){
        printf("Error. Enemies-F17-1.\n");
        exit(ERROR);
    }
    ene->row = row;
    ene->col = col;
    return;
}