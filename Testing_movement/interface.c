#include "interface.h"


Interface* create_intrf(char* rect_fname, char* maps_fname){
    int n_rect, i, j, ini_row, ini_col, last_row, last_col, bg, fg, rect_type;
    FILE *in=NULL;
    char buff[BUFFER_SIZE];
    Interface *intrf=NULL;
    Maps **aux;
    
    /*-----------Checkings-------------------------*/
    if(rect_fname==NULL){
        printf("Error. Interface-F1-1.\n");
        return NULL;
    }
    
    if(maps_fname==NULL){
        printf("Error. Interface-F1-2.\n");
        return NULL;
    }
    
    in = (FILE *) fopen(rect_fname, "r");
    if(in == NULL){
        printf("Error. Interface-F1-3.\n");
        return NULL;
    }
    /*-------------------------------------------------*/
    
    intrf = (Interface *) malloc(sizeof(Interface));
    if(intrf == NULL){
        printf("Error. Interface-F1-4.\n");
        return NULL;
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_rect);
    
    intrf->rect_array = (rectangle **) malloc(n_rect * sizeof(rectangle *));
    for(i=0; i<n_rect; i++){
        fgets(buff, BUFFER_SIZE, in);
	    sscanf(buff, "%d %d %d %d %d %d %d", &ini_row, &ini_col, &last_row, &last_col, &bg, &fg, &rect_type);

	    intrf->rect_array[i] = win_new(ini_row, ini_col, last_row, last_col, bg, fg, rect_type);
	    if(intrf->rect_array[i]==NULL){
		    printf("Error. Interface-F1-5.\n");
	        for(j=i-1; j>=0; j--){
	         win_delete(intrf->rect_array[j]);
	        }
	        free(intrf->rect_array);
	        free(intrf);
	        fclose(in);
	        return NULL;
	    }
    }
    
    intrf->n_rectangles = n_rect;
    
    intrf->maps_array = load_maps(maps_fname);
    if(intrf->maps_array == NULL){
        printf("Error. Interface-F1-6.\n");
        for(j=0; j<n_rect; j++){
            win_delete(intrf->rect_array[j]);
        }
        free(intrf->rect_array);
        free(intrf);
        fclose(in);
    }
    for(aux = intrf->maps_array, i=1; *aux != NULL; aux++, i++);
    
    intrf->n_maps = i;
    
    
    fclose(in);
    
    return intrf;
}



void destroy_intrf(Interface *intrf){
    int i;
    
    if(intrf == NULL) return;
    
    for(i=0; i<intrf->n_rectangles; i++){
        win_delete(intrf->rect_array[i]);
    }
    free(intrf->rect_array);
    
    if(intrf->maps_array != NULL){
        destroy_maps(intrf->maps_array);
    }
    free(intrf);
}



Status print_resources(Interface *intrf, Resources **r){
    int i, num;
    Resources **aux=NULL;
    char *buff, *str;
    
    if(intrf == NULL || r == NULL){
        printf("Error. Interface-F3-1.\n");
        return FAILED;
    }
    
    for(i=0; i<intrf->n_rectangles; i++){ /*Searching for the rectangle which is going to print the resources*/
        if(rectangle_getType(intrf->rect_array[i]) == RECT_RES){ /*Found it*/
            num = rectangle_getNCols(intrf->rect_array[i]);
            for(aux = r; *aux != NULL; aux++){  /*Printing all the resources information*/
                buff = (char *) malloc(num * sizeof(char));
                if(buff == NULL){
                    printf("Error. Interface-F3-2.\n");
                    return FAILED;
                }
                str = (char *) malloc(num * sizeof(char));
                if(buff == NULL){
                    printf("Error. Interface-F3-2.\n");
                    return FAILED;
                }
                
                strcpy(buff, resources_getName(*aux));
                sprintf(str, ": %d / %d", resources_getActualValue(*aux), resources_getMax(*aux));
                strcat(buff, str);
                if(win_write_line_at(intrf->rect_array[i], resources_getRow(*aux), resources_getCol(*aux), buff) == FAILED){
                    printf("Error. Interface-F3-3.\n");
                    return FAILED;
                }
                
                free(buff);
                free(str);
            }
            return OK;
        }
    }
    printf("Error. Interface-F3-4.\n");
    return FAILED;
}



Status print_weapons(Interface *intrf, Weapon **wp){
    int i, num;
    Weapon **aux=NULL;
    char *buff, *str;
    
    if(intrf == NULL || wp == NULL){
        printf("Error. Interface-F4-1.\n");
        return FAILED;
    }
    
    for(i=0; i<intrf->n_rectangles; i++){ /*Searching for the rectangle which is going to print the 0*/
        if(rectangle_getType(intrf->rect_array[i]) == RECT_WEAP){ /*Found it*/
            num = rectangle_getNCols(intrf->rect_array[i]);
            for(aux = wp; *aux != NULL; aux++){  /*Printing all the weapons information*/
                if(own_weapon(*aux) == OWNED){ /*BUT we only print those who are OWNED*/
                    buff = (char *) malloc(num * sizeof(char));
                    if(buff == NULL){
                        printf("Error. Interface-F4-2.\n");
                        return FAILED;
                    }
                    str = (char *) malloc(num * sizeof(char));
                    if(buff == NULL){
                        printf("Error. Interface-F4-2.\n");
                        return FAILED;
                    }
                
                    strcpy(buff, weapon_getName(*aux));
                    sprintf(str, ": %d", weapon_getPowderWaste(*aux));
                    strcat(buff, str);
                    if(win_write_line_at(intrf->rect_array[i], weapon_getRow(*aux), weapon_getCol(*aux), buff) == FAILED){
                        printf("Error. Interface-F4-3.\n");
                        return FAILED;
                    }
                
                    free(buff);
                    free(str);
                }
            }
            return OK;
        }
    }
    
    printf("Error. Interface-F4-4.\n");
    return FAILED;
}



Status print_objects(Interface *intrf, Object **obj){
    int i, num;
    Object **aux=NULL;
    char *buff, *str;
    
    if(intrf == NULL || obj == NULL){
        printf("Error. Interface-F5-1.\n");
        return FAILED;
    }
    
    for(i=0; i<intrf->n_rectangles; i++){ /*Searching for the rectangle which is going to print the resources*/
        if(rectangle_getType(intrf->rect_array[i]) == RECT_INVENT){ /*Found it*/
            num = rectangle_getNCols(intrf->rect_array[i]);
            for(aux = obj; *aux != NULL; aux++){  /*Printing all the resources information*/
                buff = (char *) malloc(num * sizeof(char));
                if(buff == NULL){
                    printf("Error. Interface-F5-2.\n");
                    return FAILED;
                }
                str = (char *) malloc(num * sizeof(char));
                if(buff == NULL){
                    printf("Error. Interface-F5-2.\n");
                    return FAILED;
                }
                
                strcpy(buff, object_getName(*aux));
                sprintf(str, ": %d  %d", object_getAmount(*aux), object_getValue(*aux));
                strcat(buff, str);
                if(win_write_line_at(intrf->rect_array[i], object_getRow(*aux), object_getColumn(*aux), buff) == FAILED){
                    printf("Error. Interface-F5-3.\n");
                    return FAILED;
                }
                
                free(buff);
                free(str);
            }
            return OK;
        }
    }
    printf("Error. Interface-F5-4.\n");
    return FAILED;
}



Status print_map(Interface *intrf, int map_id){
    int i, j, r, flag=0;
    rectangle *aux=NULL;
    
    if(intrf == NULL || map_id < 0){
        printf("Error. Interface-F6-1.\n");
        return FAILED;
    }
    
    r = intrf->maps_array[map_id]->n_rows;
    
    for(j=0; j < intrf->n_rectangles; j++){
        if(rectangle_getType(intrf->rect_array[j]) == RECT_BATTLE){
            aux = intrf->rect_array[j];
            flag = 1;
            break;
        }
    }
    
    if(flag != 1){
        printf("Error. Interface-F6-2.\n");
        return FAILED;
    }
    
    for(i=0; i<r; i++){
        if(win_write_line_at(aux, i, 0, intrf->maps_array[map_id]->field[i]) == FAILED){
            printf("Error. Interface-F6-3.\n");
            return FAILED;
        }
    }
    
    return OK;
}


Status initialize_intrf(Interface *intrf, int initial_map, Resources **r, Weapon **wp, Object **obj, Player *pl){
    int i, j;
    rectangle *aux=NULL;
    
    /* Error checking */
    if(intrf == NULL || initial_map < 0){
        printf("Error. Interface-F7-1.\n");
        return FAILED;
    }
    
    /* Drawing all the rectangle of the interface */
    for(i=0; i < intrf->n_rectangles; i++){
        if(rectangle_draw(intrf->rect_array[i]) == FAILED){
            printf("Error. Interface-F7-2.\n");
            return FAILED;
        }
    }
    
    /* Printing the resources */
    if(print_resources(intrf, r) == FAILED){
        printf("Error. Interface-F7-3.\n");
        return FAILED;
    }
    
    /* Printing the armory */
    if(print_weapons(intrf, wp) == FAILED){
        printf("Error. Interface-F7-4.\n");
        return FAILED;
    }
    
    /* Printing the inventory */
    if(print_objects(intrf, obj) == FAILED){
        printf("Error. Interface-F7-5.\n");
        return FAILED;
    }
    
    /* Printing the initial map */
    if(print_map(intrf, initial_map) == FAILED){
        printf("Error. Interface-F7-6.\n");
        return FAILED;
    }
    
    /* Looking for the Battlefield rectangle in order to print the player sign there later */
    for(j=0; j < intrf->n_rectangles; j++){
        if(rectangle_getType(intrf->rect_array[j]) == RECT_BATTLE){
            aux = intrf->rect_array[j];
            break;
        }
    }
    /* Printing the player at the initial position */
    if(win_write_char_at(aux, player_getRow(pl), player_getCol(pl), player_getDisplay(pl)) == FAILED){
        printf("Error. Interface-F7-7.\n");
        return FAILED;
    }
    
    return OK;
}
