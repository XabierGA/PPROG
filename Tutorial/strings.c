#include "strings.h"

Strings** load_strings(char* filename){
    FILE* fp;
    Strings** s;
    char* buff[BUFFER_SIZE], sentence[SENTENCE_SIZE];
    int size, i, j;
    
    if(!filename){
        printf("Error. Strings-F1-1");
        exit(ERROR);
    }
    fp = fopen(filename, "r");
    if(!fp){
        printf("Error. Strings-F1-2");
        exit(ERROR);
    }
    fgets(buff, BUFFER_SIZE, fp);
    sscanf(buff, "%d", &size);
    
    s = (Strings**) malloc((size+1) * sizeof(Strings*));
    if(s==NULL){
        printf("Error. Strings-F1-3.\n");
        fclose(in);
        exit(ERROR);
    }
    for(i=0; i<size; i++){
        fgets(buff, BUFFER_SIZE, fp);
		sscanf(buff, "%d %s", &type, sentence);
		
		s[i] = create_string(type, sentence);
		if(s[i]==NULL){
		    printf("Error. Strings-F1-4.\n");
		    for(j=i-1; j>=0; j--){
		        delete_string(s[j]);
		    }
		    free(s);
		    fclose(fp);
		    exit(ERROR);
		}
	}
	s[size] = NULL;
	
	fclose(fp);
	
	return s;
}

Strings* create_string(int type, char* str){
    Strings* straux;
    if(!str){
        printf("Error. Strings-F2-1");
        exit(ERROR);
    }
    straux = (Strings*)malloc(sizeof(Strings));
    if(!straux){
        printf("Error. Strings-F2-2");
        exit(ERROR);
    }
    straux->type = type;
    straux->str = strdup(str);
    if(!straux->str){
        free(straux);
        printf("Error. Strings-F2-3");
        exit(ERROR);
    }
    return(straux);
}

char* strings_get_string_by_type(int type, Strings** strs){
    if(!strs){
        printf("Error. Strings-F3-1");
        exit(ERROR);
    }
    while(strs[i] != NULL){
        if(strs[i]->type == type){
            return strs[i]->str; 
        }
        i++;
    }
    return NULL;
}

int strings_get_type(Strings* str){
    if(!str){
        printf("Error. Strings-F4-1");
        exit(ERROR);
    }
    return str->type;
}

void strings_destroy(Strings** strs){
    int i;
    if(!strs) return;
    while(strs[i] != NULL){
        delete_string(strs[i]);
        i++;
    }
    free(strs);
    return;
}

void delete_string(String* str){
    if(!str) return;
    if(str->str != NULL){
        free(str->str);
    }
    free(str);
    
}
