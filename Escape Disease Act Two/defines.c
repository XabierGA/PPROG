#include "defines.h"

int rand_num(int inf, int sup){
    int res = 0;
    
    if(inf == sup){
        return sup;
    }
    
    else if (inf > sup){
        printf("Error. Defines-F1-1.\n");
        exit(ERROR);
    }

    res = (inf + ((int) ((((double)(sup-inf+1)) * rand())/(RAND_MAX + 1.0))));
    
    return res;
}