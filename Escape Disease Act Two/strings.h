#ifndef STRINGS_H
#define STRINGS_H

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"

struct _strings{
    int type;
    char* str;
};


typedef struct _strings Strings;

/*Loads all the strings of a given file into an array of strings*/
Strings** load_strings(char* filename);

/*Auxiliar function for load_strings. It creates a String (new datatype) from a given string (C datatype)*/
Strings* create_string(int type, char* str);

/*Returns a certain string from an array of Strings that has the type we are looking for*/
char* strings_get_string_by_type(int type, Strings** strs);

/*Returns the type of a certain String*/
int strings_get_type(Strings* str);

/*Frees the alocated memory for an array of Strings*/
void strings_destroy(Strings** strs);

/*Auxiliar function for strings_destroy, frees the alocated memory for the given String*/
void delete_string(Strings* str);

#endif