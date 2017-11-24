 #include "rectangle.h"
#define MAX_BUFF 200
#define MAX_REC 10

int main (){
    rectangle **r;
    char buff[MAX_BUFF];
    int ini_row, ini_col, last_col, last_row, bg_color, fg_color, type, i, n_rect;
    FILE *pf;
    
    pf = (FILE *) fopen ("rectangles.txt", "r");
    
    fgets(buff, MAX_BUFF, pf);
    sscanf(buff, "%d", &n_rect);
    
    r = (rectangle **) malloc(n_rect * sizeof(rectangle *));
    
    for (i=0; i<n_rect; i++){
        fgets (buff, MAX_BUFF, pf);
        sscanf(buff, "%d %d %d %d %d %d %d", &ini_row, &ini_col, &last_row, &last_col, &bg_color, &fg_color, &type);
        r[i] = win_new(ini_row, ini_col, last_row, last_col, bg_color, fg_color, type);
        rectangle_draw(r[i]);
        
    }
    
    for (i=0; i<n_rect; i++){
        win_delete(r[i]);
    }
    free(r);
    
    fclose (pf);
    return 0;
}