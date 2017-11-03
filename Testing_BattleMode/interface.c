#include "interface.h"

Interface* intrf_create(char* filename){
    int n_rect, i, ini_row, ini_col, n_rows, n_cols, bg, fg;
    FILE *in=NULL;
    char buff[BUFFER_SIZE];
    Interface *intrf=NULL;
    
    if(filename==NULL){
        printf("Error. Interface-F1-1.\n");
        return NULL;
    }
    
    in = (FILE *) fopen(filename, "r");
    if(in == NULL){
        printf("Error. Interface-F1-2.\n");
        return NULL;
    }
    
    intrf = (Interface *) malloc(sizeof(Interface));
    if(intrf == NULL){
        printf("Error. Interface-F1-3.\n");
        return NULL;
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%d", &n_rect);
    
    intrf->rec_array = (rectangle **) malloc(n_rect * sizeof(rectangle *));
    for(i=0; i<n_rect; i++){
        fgets(buff, BUFFER_SIZE, in);
	    sscanf(buff, "%d %d %d %d %d %d", &ini_row, &ini_col, &n_rows, &n_cols, &bg, &fg);

	    intrf->rec_array[i] = win_new(ini_row, ini_col, n_rows, n_cols, bg, fg);
	    if(intrf->rec_array[i]==NULL){
		    printf("Error. Interface-F1-4.\n");
	      for(j=i-1; j>=0; j--){
	         win_delete(intrf->rec_array[j]);
	      }
	      free(intrf->rec_array);
	      free(intrf);
	      fclose(in);
	      return NULL;
	    }
    }
    
    intrf->n_rectangles = n_rect;
    
    
    
}