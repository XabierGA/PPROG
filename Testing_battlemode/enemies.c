#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enemies.h"


/*Function that loads all the enemies from a file*/
Enemy** load_enemies(char *filename){
    Enemy **e=NULL;
    FILE *in=NULL;
    char buff[BUFFER_SIZE], name[NAME_SIZE];
    char display;
    int i, j, n_ene, type, HP, physical_status, speed, damage;
    
    
    /*Checking*/
    if(filename == NULL){
        printf("Error. Enemies-F1-1.\n");
        return NULL;
    }
    /*-----------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Enemies-F1-2.\n");
        return NULL;
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_ene);
    
    
    e = (Enemy **) malloc((n_ene+1) * sizeof(Enemy*));
    if(e==NULL){
        printf("Error. Enemies-F1-3.\n");
        fclose(in);
        return NULL;
    }
    
    
    for(i=0; i<n_ene; i++){
        fgets(buff, BUFFER_SIZE, in);
		sscanf(buff, "%s %c %d %d %d %d %d %d %d", name, &display, &type, &HP, &physical_status, &speed, &damage);
		
		e[i] = create_enemy(name, display, type, HP, physical_status, speed, damage);
		if(e[i]==NULL){
		    printf("Error. Enemies-F1-4.\n");
		    for(j=i-1; j>=0; j--){
		        delete_enemy(e[j]);
		    }
		    free(e);
		    fclose(in);
		    return NULL;
		}
	}
	e[n_ene] = NULL;
	
	fclose(in);
	
	return e;
	
}


/*Function that creates an enemy*/
Enemy* create_enemy(char* name, char display, int type, int HP, int phy_stat, int speed, int damage){
    Enemy *ene;
     
    if(damage<0||HP<0||speed<0||name==NULL){
        printf("Error. Enemies-F2-1.\n");
        return NULL;
    }
    if(type!=WALL && type!=ZOMBIE){
        printf("Error. Enemies-F2-2.\n");
        return NULL;
    }
    if(phy_stat!=ALIVE && phy_stat!=DEAD){
        printf("Error. Enemies-F2-3.\n");
        return NULL;
    }
     
    ene = (Enemy *) malloc(sizeof(Enemy));
    if(ene==NULL){
        printf("Error. Enemies-F2-4.\n");
        return NULL;
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
 
 
/*Function that destroys an enemy, it receives an enemy and frees all the memory allocated for it*/
void delete_enemy(Enemy *e){
    if (e==NULL) return;
    
    if (e->name != NULL){
        free(e->name);
    }
    
    free(e);
}
 
 
void destroy_enemies(Enemy **e){
    Enemy **aux=NULL;
    
    if(e == NULL) return;
    
    for(aux = e; *aux!=NULL; aux++){
        delete_enemy(*aux);
    }
    free(e);
}


 

/*Function that modifies an enemy's health once is hitten or healen*/
int modify_enemyhp(Enemy *e, int value){
    if (e == NULL) return ERROR;
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


Enemy* generate_enemy(Enemy **pe, char *name){
    Enemy *e=NULL;
    Enemy **i=NULL;
    
    if(name == NULL||pe == NULL){
        printf("Error. Enemies-F5-1.\n");
        return NULL;
    }
    
    i=pe;
    
    while(*i != NULL){
        if(strcmp((*i)->name, name)==0){
            e = (Enemy *) malloc(sizeof(Enemy));
            if(e == NULL){
                printf("Error. Enemies-F5-2.\n");
                return NULL;
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
    printf("Error. Enemies-F5-3.\n");
    return NULL;
}



Enemy** generate_arrayEnemies(Enemy **pe, int *n_ene, int size){
    Enemy **e=NULL, **aux=NULL;
    int i, j, k=0
    int total=0;
    
    if(pe==NULL || n_ene==NULL){
        printf("Error. Enemies-F111-1.\n");
        exit(1);
    }
    
    for(i=0; i<size; i++){
        total += n_ene[i];
    }
    
    e = (Enemy **) malloc(total * sizeof(Enemy*));
    if(e==NULL){
        printf("Error. Enemies-F111-2.\n");
        exit(1);
    }
    for(i=0; i<size; i++){
        e[i] = (Enemy *) malloc(sizeof(Enemy));
        if(e[i]==NULL){
            printf("Error. Enemies-F111-3.\n");
            for(j=0; j<i; j++){
                free(e[j]);
            }
            free(e);
            exit(1);
        }
    }
    
    for(aux = pe, i=0; i<size; i++, aux++){
        for(j=0; j<n_ene[i]; j++, k++){
            e[k] = generate_enemy(pe, enemy_getName(*aux));
        }
    }
    
    return e;
}


char enemy_getDisplay(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F6-1.\n");
        return 0;
    }
    
    return ene->display;
}


int enemy_getType(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F7-1.\n");
        return ERROR;
    }
    
    return ene->type;
}


int enemy_getHP(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F8-1.\n");
        return ERROR;
    }
    
    return ene->HP;
}


int enemy_getSpeed(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F9-1.\n");
        return ERROR;
    }
    
    return ene->speed;
}


int enemy_getDamage(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F8-1.\n");
        return ERROR;
    }
    
    return ene->damage;
}


int enemy_getRow(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F9-1.\n");
        return ERROR;
    }
    
    return ene->row;
}


int enemy_getCol(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F10-1.\n");
        return ERROR;
    }
    
    return ene->col;
}


char* enemy_getName(Enemy *ene){
    if(ene==NULL){
        printf("Error. Enemies-F11-.\n");
        return NULL;
    }
    
    return ene->name;
}


int enemy_getPhyStat(Enemy *ene){
    if(ene == NULL){
        return ERROR;
    }
    
    return ene->physical_status;
}

void enemy_setLocation(Enemy *ene, int row, int col){
    if (ene == NULL){
        return;
    }
    ene->row = row;
    ene->col = col;
    return;
}