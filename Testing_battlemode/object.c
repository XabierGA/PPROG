#include "object.h"

struct _object{
      char *name;
      int object_type;
      int value;
      int amount;
      int row;
      int col;
};


/*Function that loads all the objects from a file*/
Object** load_objects(char *filename){
    Object **o=NULL;
    FILE *in;
    char buff[BUFFER_SIZE], name[NAME_SIZE];
    int i, j, n_ob, type, value, amount, r, c;
    
    
    /*Checking*/
    if(filename == NULL){
        printf("Error. Objects-F1-1.\n");
        exit(ERROR);
    }
    /*-----------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Objects-F1-2.\n");
        exit(ERROR);
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_ob);
    
    
    o = (Object **) malloc((n_ob+1) * sizeof(Object*));
    if(o==NULL){
        printf("Error. Objects-F1-3.\n");
        fclose(in);
        exit(ERROR);
    }
    
    
    for(i=0; i<n_ob; i++){
        fgets(buff, BUFFER_SIZE, in);
		sscanf(buff, "%s %d %d %d %d %d", name, &type, &value, &amount, &r, &c);
		
		o[i] = create_object(name, type, value, amount, r, c);
		if(o[i]==NULL){
		    printf("Error. Objects-F1-4.\n");
		    for(j=i-1; j>=0; j--){
		        delete_object(o[j]);
		    }
		    free(o);
		    fclose(in);
		    exit(ERROR);
		}
	}
	o[n_ob] = NULL;
	
	fclose(in);
	
	return o;
}



/*Function that creates an object*/
Object* create_object(char *name, int type, int value, int amount, int r, int c){
      Object *obj=NULL;
      
      if(name==NULL||value<0||amount<0||r<0||c<0){
            printf("Error. Object-F2-1.\n");
            exit(ERROR);/*Exit the game*/
      }
      
      if(type != AMMO && type != MEDICINE && type != DRINK && type != FOOD){
            printf("Error. Object-F2-2.\n");
            exit(ERROR);
      }
      
      obj = (Object *) malloc(sizeof(Object));
      if(obj==NULL){
            printf("Error. Object-F2-3.\n");
            exit(ERROR);
      }
     
      obj->name = strdup(name);
      obj->object_type = type;
      obj->value = value;
      obj->amount = amount;
      obj->row = r;
      obj->col = c;
     
      return obj;
}


void destroy_objects(Object **ob){
    Object **aux=NULL;
    
    if(ob == NULL) return;
    
    for(aux = ob; *aux!=NULL; aux++){
        delete_object(*aux);
    }
    free(ob);
}


/*Function of liberating memory*/
void delete_object(Object* ob){
    if(ob == NULL) return;
        
    if (ob->name != NULL){
        free(ob->name);
    }
    
    free(ob);
}



int object_getType(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F5-1.\n");
        exit(ERROR);
    }
    
    return ob->object_type;
}


int object_getAmount(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F6-1.\n");
        exit(ERROR);
    }
    
    return ob->amount;
}


int object_getValue(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F7-1.\n");
        exit(ERROR);
    }
    
    return ob->value;
}

char* object_getName(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F8-1.\n");
        exit(ERROR);
    }
    
    return ob->name;
}


int object_getRow(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F9-1.\n");
        exit(ERROR);
    }
    
    return ob->row;
}

int object_getColumn(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F10-1\n");
        exit(ERROR);
    }
    
    return ob->col;
}

Status object_changeAmount(Object *ob, int inc){
    if (ob == NULL){
        printf("Error. Objects-F11-1.\n");
        exit(ERROR);
    }
    
    ob->amount += inc;
    return OK;
}

Status use_object(Resources **r, Object *o){
    int i;
    Resources **aux=NULL;
    
    if(r==NULL || o==NULL){
        printf("Error. Objects-F12-1.\n");
        exit(ERROR);
    }
    if((o->amount) < 1){
        printf("Error. Objects-F12-2.\n");
        exit(ERROR);
    }
    
    for(i=0, aux = r; *aux != NULL; aux++, i++){
        if(o->object_type == resources_getObjectType(r[i])){
            if(modify_resource(r[i], o->value) == ERROR){
                printf("Error. Objects-F12-3.\n");
                exit(ERROR);
            }
            object_changeAmount(o, -1);
            return OK;
        }
    }
    
    printf("Error. Objects-F12-3.\n");
    exit(ERROR);
}


Object* object_getObject(Object **o, int type){
    Object **aux=NULL;
    
    if(o == NULL){
        printf("Error. Objects-F13-1.\n");
        exit(ERROR);
    }
    if(type != MEDICINE && type != AMMO && type != FOOD && type != DRINK){
        printf("Error. Objects-F13-2.\n");
        exit(ERROR);
    }
    
    for(aux = o; (*aux)!=NULL; aux++){
        if(object_getType(*aux) == type){
            return (*aux);
        }
    }
    printf("Error. Objects-F13-3.\n");
    exit(ERROR);
}