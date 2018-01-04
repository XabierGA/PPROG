#include "rectangle.h"


/*
  Prints on the screen the style for writing on this window,
  foreground colour, background colour, and font style.
 */
static void prepare_font(rectangle *rec){
    printf("%c[%d;%dm", 27, rec->bg_color, rec->fg_color);
    return;
}



/*
  Moves the cursor into a window position, translating it into
  absolute screen positions. Note the "+1" used to translate from the
  0-based library coordinates to the 1-based escape codes.
 */
void move_to(rectangle *rec, int row, int col){
    /*Sort of checkings*/
    if(row < 0){
        row = 0;
    }
    
    if(col < 0){
        col = 0;   
    }
    
    if(row > rec->n_rows){
        row = rec->last_row;
    }
    
    if(col > rec->n_cols){
        col = rec->last_col;
    }
    /*-----------------*/
    
    row += (rec->ini_row - 1);
    col += (rec->ini_col - 1);
    printf("%c[%d;%dH", 27, row, col);
    
    return;
}



/*
  Returns true if a window is visible, that is, if the structure is
  non-null, has valid values, and has at least one row and one column.
 */
static int is_visible(rectangle *rec){
    if(rec==NULL){
      printf("Error. Rectangles-F3-1.\n");
      return INVISIBLE;
    }
    
    if (rec->ini_row < 0 || rec->ini_col < 0 || rec->n_rows <= 0 || rec->n_cols <= 0){
      printf("Error. Rectangles-F3-2.\n");
      return INVISIBLE;
    } 
    
    return VISIBLE;
}



/*
  Creates a window on the screen. 
  Parameters:
     ini_row, ini_col: row and column where the window begins;
     n_rows, n_cols: number of rows and columns of the window.
  
  Returns:
    a pointer to a newly allocated sc_rectangle structure with the 
    information for the window.
  
  Note that this function will do absolutely nothing on the screen
  itself. In particular, it will NOT clear the window. This function
  will simply create the structure, initialize its fields, and return
  it.
 */
rectangle* win_new(int ini_row, int ini_col, int last_row, int last_col, int bg, int fg, int type){
    rectangle *rec = (rectangle *) malloc(sizeof(rectangle));
    if(rec==NULL){
      printf("Error. Rectangles-F4-1.\n");
      return NULL;
    }
    
    rec->ini_row = ini_row;
    rec->ini_col = ini_col;
    rec->last_row = last_row,
    rec->last_col = last_col;
    
    rec->n_rows = last_row - ini_row + 1;
    rec->n_cols = last_col - ini_col + 1;
  
    rec->bg_color = bg;
    rec->fg_color = fg;
    
    rec->rect_type = type;
     
    return rec;
}



/*
  Clears a window, leaving it all in the current background color
  
  Parameters:
      rec: the window structure on which the function operates.
  
  Returns:
      nothing.
 */
void win_clear(rectangle *rec){
    char *buf;
    int i, row_to_move;
  
    rec->last_line = 0;
    
    if (!is_visible(rec)){
      printf("Error. Rectangles-F5-1.\n");
      return;
    } 
    
    buf = (char *) malloc((rec->n_cols -1) * sizeof(char));
    if(buf==NULL){
      printf("Error. Rectangles-F5-2.\n");
      return;
    } 
    
    memset(buf, ' ', rec->n_cols - 2);
    buf[rec->n_cols - 2] = 0;
    prepare_font(rec);
    
    for(i = rec->ini_row + 1 ; i < rec->last_row; i++){
        row_to_move = i - rec->ini_row + 1;
        move_to(rec, row_to_move, 2);
        printf("%s", buf);
    }
    fflush(stdout);
    free(buf);
    return;
}



/*
  Releases the structure allocated for a window. After calling this
  function it is no longer possible to use the window.
  
  This function doesn't erase the window contents from the screen.
  
  Parameters:
      rec: the window structure on which the function operates.
  
  Returns:
      nothing.
 */
void win_delete(rectangle *rec){
    if(rec) free(rec);
    
    return;
}


/*
  Sets the background color for the window
  
  Parameters:
      rec:  the window structure on which the function operates.
      color: the new background color;
  
  Returns:
      OK: regular execution
      FAILED: incorrect color parameter
 */
Status win_bgcolor(rectangle *rec, int color){
    if (!is_visible(rec)){
      printf("Error. Rectangles-F7-1.\n");
      return FAILED;
    } 
    
    if (color >= 30 && color <= 39){
      rec->bg_color = color;
      return OK;
    } 
    
    printf("Error. Rectangles-F7-2.\n");
    return FAILED;
}



/*
  Sets the foreground colour for the window
  
  Parameters:
      rec:  the window structure on which the function operates.
      color: the new foreground colour;
  
  Returns:
      OK: regular execution
      FAILED: incorrect colour parameter
 */
Status win_fgcolor(rectangle *rec, int color){
    if (!is_visible(rec)){
      printf("Error. Rectangles-F8-1.\n");
      return FAILED;
    } 
    
    if (color >= 30 && color <= 39){
      rec->fg_color = color;
      return OK;
    } 
    
    printf("Error. Rectangles-F8-2.\n");
    return FAILED;
}



/*
  This function writes a string in a single line starting at a given
  position of the window, and without changing line. Each new line
  character contained in the string will be considered as a string
  termination, and if the string is too long to fit in the screen, it
  will be truncated. 
  
  So, if on the screen of the example we execute the functions:
  
  win_write_line_at(rec, 3, 26, "en un lugar de la mancha");
  win_write_line_at(rec, 5, 24, "oh!\nI didn't know");
   
  
  we get:
  
                       19             33
     0                  v             v    39
    +----------------------------------------+
  0 |                                        |
    |                                        |
    |                          en un lu      | < 2
    |                                        | 
    |                        oh!             |
    |                                        |
    |                                        | < 6
    |                                        | 
    |                                        |
   9|                                        |
    +----------------------------------------+
  
    Parameters:
      rec:  the window structure on which the function operates.
      row:   the row of the window at which the writing begins
      col:   the column of the window at which the writing begins
      str: the screen that we must write
  
    Returns:
      the number of characters actually written on the screen.
  
 */
int win_write_line_at(rectangle *rec, int row, int col, char *str){
    char *nl_p;
    char save, av_space, ret;

    if (!is_visible(rec)){
      printf("Error. Rectangles-F9-1.\n");
      return ERROR;
    } 
    
    if (row >= rec->n_rows || col >= rec->n_cols){
      printf("Error. Rectangles-F9-2.\n");
      return ERROR;
    } 
    
    nl_p = strchr(str, '\n');
    if (nl_p) *nl_p = 0;
  
    av_space = rec->n_cols - col;
    save = -1;
    if (strlen(str) > av_space) {
        save = str[av_space - 1];
        str[av_space - 1] = 0;
    }
  
    prepare_font(rec);
    move_to(rec, row, col);
    printf("%s", str);
    fflush(stdout);
    if (save > 0) {
        str[av_space - 1] = save;
        ret = av_space;
    }
    else
        ret = strlen(str);
        
    if (nl_p) *nl_p = '\n';
    rec->last_line = row;
    
    return ret;
}



/*
  Writes a char at a given position of the window.

    Parameters:
      rec:  the window structure on which the function operates.
      row:   the row of the window at which the writing begins
      col:   the column of the window at which the writing begins
      ch: the character that we must write
  
    Returns:
      OK: Everything succeed
      FAILED: Some error happenned
  
*/
Status win_write_char_at(rectangle *rec, int row, int col, char ch){
  
    if (!is_visible(rec)){
      printf("Error. Rectangles-F10-1.\n");
      return FAILED;
    } 
    if (row > rec->n_rows || col > rec->n_cols){
      printf("Error. Rectangles-F10-2.\n");
      return FAILED;
    } 
    if(row < 0 || col <0){
      printf("Error. Rectangles-F10-3.\n");
      return FAILED;
    } 
  
    move_to(rec, row, col);
    printf("%c", ch);
    fflush(stdout);
    
    move_to(rec, 1, 1);
    fflush(stdout);
    
    return OK;
}



int rectangle_getIniRow(rectangle *rec){
  if(rec == NULL){
    printf("Error. Rectangles-F11-1.\n");
    return ERROR;
  }
  
  return rec->ini_row;
}



int rectangle_getIniCol(rectangle *rec){
  if(rec == NULL){
    printf("Error. Rectangles-F12-1.\n");
    return ERROR;
  }
  
  return rec->ini_col;
}



int rectangle_getLastRow(rectangle *rec){
  if(rec == NULL){
    printf("Error. Rectangles-F12-1.\n");
    return ERROR;
  }
  
  return rec->last_row;
}



int rectangle_getLastCol(rectangle *rec){
  if(rec == NULL){
    printf("Error. Rectangles-F13-1.\n");
    return ERROR;
  }
  
  return rec->last_col;
}



int rectangle_getType(rectangle *rec){
  if(rec == NULL){
    printf("Error. Rectangles-F14-1.\n");
    return ERROR;
  }
  
  return rec->rect_type;
}



int rectangle_getNRows(rectangle *rec){
  if(rec == NULL){
    printf("Error. Rectangles-F15-1.\n");
    return ERROR;
  }
  
  return rec->n_rows;
}



int rectangle_getNCols(rectangle *rec){
  if(rec == NULL){
    printf("Error. Rectangles F16-1.\n");
    return ERROR;
  }
  
  return rec->n_cols;
}



Status rectangle_draw(rectangle *rec){
  int i, r;
  
  if(rec == NULL){
    printf("Error. Rectangles rectangle_draw-1.\n");
    return FAILED;
  }
  
  /* Draw the top row */
  fprintf(stdout, "%c[%d;%dH", 27, rec->ini_row, rec->ini_col); /* Move to the top/left corner of the rectangle */
  fprintf(stdout, "+");
  
  for(i=0; i < rec->n_cols-2; i++){
    fprintf(stdout, "-");
  }
  fprintf(stdout, "+");
  
  /* Draw the bottom row */
  fprintf(stdout, "%c[%d;%dH", 27, rec->last_row, rec->ini_col); /* Move to the bottom/left corner of the rectangle */
  fprintf(stdout, "+");
  
  for(i=0; i < rec->n_cols-2; i++){
    fprintf(stdout, "-");
  }
  fprintf(stdout, "+");
  
  /* Draw the vertical lines */
  for(r = rec->ini_row+1; r < rec->last_row; r++){
    fprintf(stdout, "%c[%d;%dH", 27, r, rec->ini_col); /* Move just 1 unit under the top/left corner */
    fprintf(stdout, "|");
    fprintf(stdout, "%c[%d;%dH", 27, r, rec->last_col);
    fprintf(stdout, "|");
  }
  
  return OK;
}