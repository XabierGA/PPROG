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
        free(o);
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
            return NULL;
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
     
      strcpy(object->name, name);
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



