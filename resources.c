#include <stdio.h>
#include "resources.h"


typedef struct _resources{
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
    char buff[BUFFER_SIZE], *name;
    int i, n_res, type, max, actual, row, col;
    
    
    /*Checking*/
    if(filename == NULL){
        printf("Error. Resources F1-1.");
    }
    /*-----------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Resources-F1-2.\n");
        free(r);
        return NULL;
    }
    
    fgets(buff, MAX, in);
    sscanf(buff, "%d", &n_res);
    
    
    r = (Resources **) malloc((n_res+1) * sizeof(Resources*);
    if(r==NULL){
        printf("Error. Resources-F1-3.\n");
        fclose(in);
        return NULL;
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
		    return NULL;
		}
	}
	r[n_res] = NULL;
	
	fclose(in);
	
	return r;
	
}



/*Function that receives all the resources fields and returns a created resource*/
Resource* create_resource(int type, char *name, int max, int actual, int row, int col){
    Resources *r;
    /*Checking*/
    if(type != HP || type != HYDRATION || type != GUNPOWDER || type != HUNGER){
        printf("Error. Resources-F2-1.\n");
        return NULL;
    }
    if(max <= 0 || actual>max || actual<0){
        printf("Error. Resources-F2-2.\n");
        return NULL;
    }
        
    if(name == NULL){
        printf("Error. Resources-F2-3.\n");
        return NULL;
    }
    /*-----------------------------------------------*/
    
    r = (Resources *) malloc(sizeof(Resources);
    if(r == NULL){
        printf("Error. Resources-F2-4.\n");
        return NULL;
    }
    
    r->resource_type = type;
    r->name = strdup(name);
    r->max = max;
    r->actual = actual;
    r->row = row;
    r->col = col;
    
    return r;
}

void delete_resource(Resources *r){
    if(r == NULL) return;
        
    if (r->name != NULL){
        free(r->name);
    }
    
    free(r);
}




int modify_resource(Resources *r, int value){
    if(r==NULL){
        printf("Error. Resources-F3-1.\n");
        return ERROR;
    }
    
    r->actual += value;
    
    return r->actual;
}


int resources_getObjectType(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F4-1.\n");
        return ERROR;
    }
    
    return r->object_type;
}



int resources_getActualValue(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F5-1.\n");
        return ERROR;
    }
    
    return r->actual;
}



int resources_getRow(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F6-1.\n");
        return ERROR;
    }
    
    return r->row;
}



int resources_getCol(Resources *r){
    if(r==NULL){
        printf("Error. Resources-F7-1.\n");
        return ERROR;
    }
    
    return r->col;
}