#include "character.h"


typedef struct _character{
    char display;
    int orientation;
    int row;
    int col;
};


Character* create_character(char *filename){
    if(filename == NULL){
        printf("Error. Character-F1-1.\n");
        return NULL;
    }
}

