#include <stdio.h>
#include "resources.h"


struct _resources{
    int object_type;
    char *name;
    int max;
    int actual;
    int row;
    int col;
};


Resources** load_resources(char *filename){
    Resources **r=NULL;
    FILE *in;
    char buff[BUFFER_SIZE], name[NAME_SIZE];
    int i, j, n_res, type, max, actual, row, col;


    /*Checking*/
    if(filename == NULL){
        printf("Error. Resources F1-1.");
        exit(ERROR);
    }
    /*-----------------------------------*/

    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Resources-F1-2.\n");
        exit(ERROR);
    }

    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_res);


    r = (Resources **) malloc((n_res+1) * sizeof(Resources*));
    if(r==NULL){
        printf("Error. Resources-F1-3.\n");
        fclose(in);
        exit(ERROR);
    }


    for(i=0; i<n_res; i++){
        fgets(buff, BUFFER_SIZE, in);
	    sscanf(buff, "%d %s %d %d %d %d", &type, name, &max, &actual, &row, &col);

	    r[i] = create_resource(type, name, max, actual, row, col);
	    if(r[i]==NULL){
		    printf("Error. Resources-F1-4.\n");
	      for(j=i-1; j>=0; j--){
	         delete_resource(r[j]);
	      }
	      free(r);
	      fclose(in);
	      exit(ERROR);
	    }
    }
	  r[n_res] = NULL;

	  fclose(in);

	  return r;
}



Resources* create_resource(int type, char *name, int max, int actual, int row, int col){
    Resources *r;
    /*Checking*/
    if(type != MEDICINE && type != FOOD && type != DRINK && type != AMMO){
        printf("Error. Resources-F2-1.\n");
        exit(ERROR);
    }
    if(max <= 0 || actual>max || actual<0){
        printf("Error. Resources-F2-2.\n");
        exit(ERROR);
    }
    if(name == NULL){
        printf("Error. Resources-F2-3.\n");
        exit(ERROR);
    }
    /*-----------------------------------------------*/

    r = (Resources *) malloc(sizeof(Resources));
    if(r == NULL){
        printf("Error. Resources-F2-4.\n");
        exit(ERROR);
    }

    r->object_type = type;
    r->name = strdup(name);
    r->max = max;
    r->actual = actual;
    r->row = row;
    r->col = col;

    return r;
}



void destroy_resources(Resources **r){
    Resources **aux=NULL;
    
    if(r == NULL){
        printf("Error. Resources-F3-1.\n");
        exit(ERROR);
    } 
    
    for(aux = r; *aux!=NULL; aux++){
        delete_resource(*aux);
    }
    free(r);
}



void delete_resource(Resources *r){
    if(r == NULL){
        printf("Error. Resources-F4-1.\n");
        exit(ERROR);
    } 

    if (r->name != NULL){
        free(r->name);
    }

    free(r);
}



int modify_resource(Resources *r, int value){
    if(r==NULL){
        printf("Error. Resources-F5-1.\n");
        return ERROR;
    }
    
    if(value < 0){
        if(-(value) > r->actual){
            return r->actual;
        }
    }

    r->actual += value;
    
    if(r->actual > r->max){
        r->actual = r->max;
    }

    return r->actual;
}



int resources_getObjectType(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F6-1.\n");
        exit(ERROR);
    }

    return r->object_type;
}



int resources_getActualValue(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F7-1.\n");
        exit(ERROR);
    }

    return r->actual;
}



int resources_getRow(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F8-1.\n");
        exit(ERROR);
    }

    return r->row;
}



int resources_getCol(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F9-1.\n");
        exit(ERROR);
    }

    return r->col;
}



char* resources_getName(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F10-1.\n");
        exit(ERROR);
    }
    
    return r->name;
}



int resources_getMax(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F11-1.\n");
        exit(ERROR);
    }
    
    return r->max;
}



Resources* resources_getResource(Resources **r, int type){
    Resources **aux=NULL;
    
    if(r == NULL){
        printf("Error. Resources-F12-1.\n");
        exit(ERROR);
    }
    if(type != MEDICINE && type != AMMO && type != FOOD && type != DRINK){
        printf("Error. Resources-F12-2.\n");
        exit(ERROR);
    }
    
    for(aux = r; (*aux)!=NULL; aux++){
        if(resources_getObjectType(*aux) == type){
            return (*aux);
        }
    }
    printf("Error. Resources-F12-3.\n");
    exit(ERROR);
}