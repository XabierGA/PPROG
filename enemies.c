#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enemies.h"

typedef struct _enemies{
    char *name;
    char display;
    int type;
    int HP;
    int speed; 
    int damage;
};


/*Function that loads all the enemies from a file*/
Enemy** load_enemies(char *filename){
    Enemy **e=NULL;
    FILE *in=NULL;
    char buff[BUFFER_SIZE], *name;
    char display;
    int i, n_ene, type, HP, speed, damage;
    
    
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
    
    fgets(buff, MAX, in);
    sscanf(buff, "%d", &n_ene);
    
    
    e = (Enemy **) malloc((n_ene+1) * sizeof(Enemy*);
    if(e==NULL){
        printf("Error. Enemies-F1-3.\n");
        fclose(in);
        return NULL;
    }
    
    
    for(i=0; i<n_ene; i++){
        fgets(buff, BUFFER_SIZE, in);
		sscanf(buff, "%s %c %d %d %d %d", name, &display, &type, &HP, &speed, &damage);
		
		e[i] = create_enemy(name, display, type, HP, speed, damage);
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
Enemy* create_enemy(char* name, char display, int type, int HP, int speed, int damage){
    Enemy *ene;
     
    if(damage<0||hp<0||speed<0||type!=WALL||type!=ZOMBIE||name==NULL){
        printf("Error. Enemies-F2-1.\n");
        return NULL;
    }
     
    ene = (Enemy *) malloc(sizeof(Enemy));
    if(ene==NULL){
        printf("Error. Enemies-F2-2.\n");
        return NULL;
    } 
     
    ene->name = strdup(name);
    ene->display = display;
    ene->type = type;
    ene->HP = HP;
    ene->speed = speed;
    ene->damage = damage;
     
    return ene;
}
 
 
/*Function that destroys an enemy, it receives an enemy and frees all the memory allocated for it*/
void destroy_enemy(Enemy *e){
    if (e==NULL) return;
    if (e->name != NULL){
        free(e->name);
    }
    free (e);
}
 

/*Function that modifies an enemy's health once is hitten or healen*/
Enemy* modify_enemyhp(Enemy *e, int increase){
    if (e == NULL) return NULL;
    e->hp = e->hp + increase;
    if (e->hp >= MAX_HEALTHRANGE){
        e->hp = MAX_HEALTHRANGE;
        return e;
    }
    if (e->hp <= 0){
        EnemyDestroy(e);
        return NULL;
    }
}


Enemy* generate_enemy(Enemy **pe, char *name){
    Enemy *e=NULL,
    Enemy **i=NULL;
    
    if(name == NULL||pe=NULL){
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
            e->speed = (*i)->speed;
            e->damage = (*i)->damage;
            
            return e;
        }
        i++;
    }
    printf("Error. Enemies-F5-3.\n");
    return NULL;
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
    
    return ene->Speed;
}


int enemy_getDamage(Enemy *ene){
    if(ene == NULL){
        printf("Error. Enemies-F8-1.\n");
        return ERROR;
    }
    
    return ene->Damage;
}