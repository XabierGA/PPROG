 #include "rectangle.h"
#define MAX_BUFF 200
#define MAX_REC 10

int main (){
    rectangle *r[MAX_REC];
    char buff[MAX_BUFF];
    int ini_row, ini_col, last_col, last_row, bg_color, fg_color, type, i, n_rect;
    FILE *pf;
    pf = fopen ("rectangles.txt", "r");
    fscanf (pf, "%d", &n_rect);
    for (i=0; i<n_rect; i++){
        fgets (buff, MAX_BUFF, pf);
        sscanf(buff, "%d %d %d %d %d %d %d", &ini_row, &ini_col, &last_row, &last_col, &bg_color, &fg_color, &type);
        r[i] = win_new(ini_row, ini_col, last_row, last_col, bg_color, fg_color, type);
        recta ngle_draw(r[i]);
        
    }
    for (i=0; i<n_rect; i++){
        win_delete(r[i]);
    }
    fclose (pf);
    return 0;
}