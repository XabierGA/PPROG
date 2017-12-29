#ifndef STRINGS_H
#define STRINGS_H

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "defines.h"

struct _strings{
    int type;
    char* str;
}

typedef struct _strings Strings;

Strings** load_strings(char* filename);

Strings* create_string(int type, char* str);

char* strings_get_string_by_type(int type, Strings** strs);

int strings_get_type(Strings* str);

void strings_destroy(Strings** strs);

void delete_string(String* str);

#endif