#include "objects.h"

typedef struct _objects{
      char *name;
      int object_type;
      int value;
      int amount;
};


/*Function that loads all the objects from a file*/
Object** load_objects(char *filename){
    Enemy **o=NULL;
    FILE *in;
    char buff[BUFFER_SIZE], *name;
    int i, n_ob, type, value, amount;
    
    
    /*Checking*/
    if(filename == NULL){
        printf("Error. Objects-F1-1.\n")
    }
    /*-----------------------------------*/
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Objects-F1-2.\n");
        return NULL;
    }
    
    fgets(buff, MAX, in);
    sscanf(buff, "%d", &n_ob);
    
    
    o = (Object **) malloc((n_ob+1) * sizeof(Object*);
    if(o==NULL){
        printf("Error. Objects-F1-3.\n");
        fclose(in);
        return NULL;
    }
    
    
    for(i=0; i<n_ob; i++){
        fgets(buff, BUFFER_SIZE, in);
		sscanf(buff, "%s %d %d %d", name, &type, &value, &amount);
		
		o[i] = create_object(name, type, value, amount);
		if(o[i]==NULL){
		    printf("Error. Objects-F1-4.\n");
		    for(j=i-1; j>=0; j--){
		        delete_object(o[j]);
		    }
		    free(o);
		    fclose(in);
		    return NULL;
		}
	}
	o[n_ob] = NULL;
	
	fclose(in);
	
	return o;
}



/*Function that creates an object*/
Object* create_object (char *name, int type, int value, int amount){
      Object *obj=NULL;
      
      if(name==NULL||value<0||amount<0){
            printf("Error. Object-F2-1.\n");
            return NULL;/*Exit the game*/
      }
      
      if(type != AMMO || type != MEDICINE || type != DRINK || type != FOOD){
            printf("Error. Object-F2-2.\n");
            return NULL;
      }
      
      obj = (Object *) malloc(sizeof(Object));
      if(obj==NULL){
            printf("Error. Object-F2-3.\n");
            return NULL;
      } 
     
      object->name = strdup(name);
      object->object_type = type;
      object->value = value;
      object->amount = amount;
     
      return obj;
}


/*Function of liberating memory*/
void destroy_object(Object* ob){
    if(ob == NULL) return;
        
    if (ob->name != NULL){
        free(ob->name);
    }
    
    free(ob);
}



Object* generate_object(Object **po, char *name){
    Object *o=NULL,
    Object **i=NULL;
    
    if(name == NULL||po==NULL){
        printf("Error. Objects-F4-1.\n");
        return NULL;
    }
    
    i=po;
    
    while(*i != NULL){
        if(strcmp((*i)->name, name)==0){
            o = (Object *) malloc(sizeof(Object));
            if(o == NULL){
                printf("Error. Objects-F4-2.\n");
                return NULL;
            }
            o->name = strdup((*i)->name);
            o->object_type = (*i)->object_type;
            o->value = (*i)->value;
            o->amount = (*i)->amount;
            
            return o;
        }
        i++;
    }
    
    printf("Error. Objects-F4-3.\n");
    return NULL;
}



Status use_object(Resources **r, Object *o){
    int i;
    Resources **aux=NULL;
    
    if(r==NULL || o==NULL){
        printf("Error. Objects-F5-1.\n");
        return FAILED;
    }
    
    for(i=0, aux = r; *aux != NULL; aux++, i++){
        if(o->object_type == resources_get_ObjectType(r[i])){
            if(modify_resource(r[i], o->value) == ERROR){
                printf("Error. Objects-F5-2.\n");
                return FAILED;
            }
            return OK;
        }
    }
    
    printf("No comments Objects-F5.\n");
    return FAILED;
}



int object_getType(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F6-1.\n");
        return ERROR;
    }
    
    return ob->object_type;
}


int object_getAmount(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F7-1.\n");
        return ERROR;
    }
    
    return ob->amount;
}


int object_getValue(Object *ob){
    if(ob == NULL){
        printf("Error. Objects-F8-1.\n");
        return ERROR;
    }
    
    return ob->value;
}
