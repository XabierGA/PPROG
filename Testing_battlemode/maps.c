#include "maps.h"


/*THIS IS AN INTERNAL FUNCTION (End of the file)*/
char** map_getField(Maps *m);
/*-----------------------------*/

/*Function that recieves the name of a file where all the maps are drawn. It allocates all the needed memory for each
of them and returns a pointer to the array of maps*/
Maps** load_maps(char *filename){
    Maps **maps;
    FILE *in=NULL;
    char buff[BUFFER_SIZE];
    int n_maps, n_cols, n_rows, map_id, i, j, k, m;
    
    if(filename == NULL){
        printf("Error. Maps-F1-1.\n");
        exit(ERROR);
    }
    
    in = (FILE *) fopen(filename, "r");
    if(in==NULL){
        printf("Error. Maps-F1-2.\n");
        exit(ERROR);
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_maps);
    
    maps = (Maps **) malloc((n_maps+1) * sizeof(Maps*));
    if(maps == NULL){
        printf("Error. Maps-F1-3.\n");
        exit(ERROR);
    }
    
    for(i=0; i<n_maps; i++){
        fgets(buff, BUFFER_SIZE, in);
        sscanf(buff, "%d %d %d", &n_rows, &n_cols, &map_id);
        
        maps[i] = create_map(n_rows, n_cols, map_id);
        if(maps[i]==NULL){
            printf("Error. Maps-F1-4.\n");
            for(j=0; j<i; j++){
                delete_map(maps[j]);
            }
            exit(ERROR);
        }
        
        for(k=0; k < n_rows; k++){
            fgets(buff, BUFFER_SIZE, in);
            for(m=0; m < n_cols; m++){
                maps[i]->field[k][m] = buff[m];
            }
            maps[i]->field[k][m] = 0;
        }
    }
    
    maps[n_maps] = NULL;
    
    fclose(in);
    
    return maps;
}


/*Function that recieves the number of rows and the number of columns of a map and it allocates memory for a
char field (matrix n_rows*n_cols). Returns a pointer to this map*/
Maps* create_map(int rows, int cols, int map_id){
    Maps *map=NULL;
    int i, j;
    
    if(rows < 2 || cols <2){
        printf("Error. Maps-F2-1.\n");
        exit(ERROR);
    }
    
    map = (Maps *) malloc(sizeof(Maps));
    if(map == NULL){
        printf("Error. Maps-F2-2.\n");
        exit(ERROR);
    }
    
    map->n_rows = rows;
    map->n_cols = cols;
    map->map_id = map_id;
    
    map->field = (char **) malloc(rows * sizeof(char *));
    if(map->field == NULL){
        printf("Error. Maps-F2-3.\n");
        exit(ERROR);
    }
    
    for(i=0; i<rows; i++){
        map->field[i] = (char *) malloc((cols+1) * sizeof(char));
        if(map->field[i]==NULL){
            printf("Error. Maps-F2-4.\n");
            for(j=0; j<i; j++){
                free(map->field[j]);
            }
            exit(ERROR);
        }
    }
    
    return map;
}


/*Frees the memory used for an array of maps*/
void destroy_maps(Maps **maps){
    Maps **aux=NULL;
    
    if(maps == NULL) return;
    
    for(aux = maps; *aux != NULL; aux++){
        delete_map(*aux);
    }
    
    free(maps);
} 


/*Frees the memory of a given map*/
void delete_map(Maps *map){
    int i;
    
    if(map == NULL) return;
    
    for(i=0; i < map->n_rows; i++){
        if(map->field[i] != NULL){
           free(map->field[i]);
        } 
    }
    free(map->field);
    free(map);
}


/*Gets the id of a given map*/
int map_getId(Maps *map){
    if(map == NULL){
        printf("Error. Maps-F5-1.\n");
        exit(ERROR);
    }
    
    return map->map_id;
}


/*Gets the number of rows of a given map*/
int map_getRows(Maps *map){
    if(map == NULL){
        printf("Error. Maps-F6-1.\n");
        exit(ERROR);
    }
    return map->n_rows;
}


/*Gets the number of columns of a given map*/
int map_getCols(Maps *map){
    if(map == NULL){
        printf("Error. Maps-F7-1.\n");
        exit(ERROR);
    }
    return map->n_cols;
}


/*Function that creates a copy of a map identified with its id*/
/*It allows us to modificate the map as much as we wanto to in a certain point of the program*/
Maps* map_getCopy(Maps** maps, int id){
    Maps** aux=NULL;
    Maps* m=NULL;
    int i, flag=0;
    char **field=NULL;
    
    if(maps==NULL){
        printf("Error. Maps-F8-1.\n");
        exit(ERROR);
    }
    
    for(aux = maps; *(aux)!=NULL; aux++){
        if( map_getId(*aux) == id){
            flag=1;
            break;
        }
    }
    
    if(flag != 1){
        printf("Error. Maps-F8-2.\n");
        exit(ERROR);
    }
    
    m = create_map(map_getRows(*aux), map_getCols(*aux), map_getId(*aux));
    if(m==NULL){
        printf("Error. Maps-F8-3.\n");
        exit(ERROR);
    }
    
    field = map_getField(*aux);
    if(field==NULL){
        printf("Error. Maps-F9-3.\n");
        exit(ERROR);
    }
    
    for(i=0; i<map_getRows(*aux); i++){
        strcpy(m->field[i], field[i]);
    }
    
    return m;
}



/*--------------------------------------------------------------------------*/
/* --------------- INTERNAL FUNCTION ----------------------------------*/
char** map_getField(Maps *map){
    if(map == NULL){
        return NULL;
    }
    return map->field;
}