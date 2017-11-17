#include "maps.h"


Maps** load_maps(char *filename){
    Maps **maps;
    FILE *in=NULL;
    char buff[BUFFER_SIZE];
    int n_maps, n_cols, n_rows, i, j, k, m;
    
    if(filename == NULL){
        printf("Error. Maps-F1-1.\n");
        return NULL;
    }
    
    in = (FILE *) fopen(filename, "r");
    if(in==NULL){
        printf("Error. Maps-F1-2.\n");
        return NULL;
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_maps);
    
    maps = (Maps **) malloc((n_maps+1) * sizeof(Maps*));
    if(maps == NULL){
        printf("Error. Maps-F1-3.\n");
        return NULL;
    }
    
    for(i=0; i<n_maps; i++){
        fgets(buff, BUFFER_SIZE, in);
        sscanf(buff, "%d %d", &n_rows, &n_cols);
        
        maps[i] = create_map(n_rows, n_cols);
        if(maps[i]==NULL){
            printf("Error. Maps-F1-4.\n");
            for(j=0; j<i; j++){
                delete_map(maps[j]);
            }
        }
        
        for(k=0; k < n_rows; k++){
            fgets(buff, BUFFER_SIZE, in);
            for(m=0; m < n_cols; m++){
                maps[i]->field[k][m] = buff[m];
            }
        }
    }
    
    maps[n_maps] = NULL;
    
    fclose(in);
    
    return maps;
}



Maps* create_map(int rows, int cols){
    Maps *map=NULL;
    int i, j;
    
    if(rows < 2 || cols <2){
        printf("Error. Maps-F2-1.\n");
        return NULL;
    }
    
    map = (Maps *) malloc(sizeof(Maps));
    if(map == NULL){
        printf("Error. Maps-F2-2.\n");
        return NULL;
    }
    
    map->n_rows = rows;
    map->n_cols = cols;
    
    map->field = (char **) malloc(rows * sizeof(char *));
    if(map->field == NULL){
        printf("Error. Maps-F2-3.\n");
        return NULL;
    }
    
    for(i=0; i<rows; i++){
        map->field[i] = (char *) malloc(cols * sizeof(char));
        if(map->field[i]==NULL){
            printf("Error. Maps-F2-4.\n");
            for(j=0; j<i; j++){
                free(map->field[j]);
            }
            return NULL;
        }
    }
    
    return map;
}



void delete_map(Maps *map){
    int i;
    
    if(map == NULL) return;
    
    for(i=0; i < map->n_rows; i++){
        if(map->field[i] != NULL) free(map->field[i]);
    }
    
    free(map);
}



void destroy_maps(Maps **maps){
    Maps **aux=NULL;
    
    if(maps == NULL) return;
    
    for(aux = maps; *aux != NULL; aux++){
        delete_map(*aux);
    }
    
    free(maps);
} 