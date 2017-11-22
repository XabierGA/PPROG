/*-----------------------------------------------------------------
    File made by Alejandro Santorum based on Simone Santini´s project.
    Date: Nov. 3, 2017
    Version 1
------------------------------------------------------------------
   Project:  General C utilities
   File:     rectangle.h
   Rev.      2.0
   Date:     Sept. 22, 2011


   Revisions:
   1.0       Sept. 22, 2011
   2.0       Oct. 1, 2014
             Removed some redundant and complex functions. Streamlined
	     the code to make it a better example.
	     
    
   (C) Simone Santini, 2011-2014 & modified by Alejandro Santorum

   This file defines the functions that form a library to manage 
   vitual windows on a character screen. The window is not marked in any
   way on the screen (marking it out, if necessary, is a purpose of the 
   calling program), but allows the user to position the cursor and print
   inside a window (almost) as if it were an independent screen. 

   In the examples that preceed the functions, we assume that we have
   a screen with 10 rows and 40 columns and in it we open a "window"
   of 5 rows and 15 column beginning at position 2, 19. The situation
   is therefore this:

   0                                        39
    +----------------------------------------+
  0 |                                        |
    |                   +---------------+    |
    |                   |               |    |
    |                   |               |    |
    |                   |               |    |
    |                   |               |    |
    |                   |               |    |
    |                   +---------------+    |
    |                                        |
   9|                                        |
    +----------------------------------------+

    Of course, the library doesn't mark the window on the screen, and
    in order to remark this fact, we shall write down the screen as:

                       19             33
     0                  v             v    39
    +----------------------------------------+
  0 |                                        |
    |                                        |
    |                                        | < 2
    |                                        | 
    |                                        |
    |                                        |
    |                                        | < 6
    |                                        | 
    |                                        |
   9|                                        |
    +----------------------------------------+


    NOTE:
    These functions are implemented using ASCII ESC codes for the control
    of the cursor. This might produce some incompatibility problem with 
    Windows and MS/DOS systems.

    Note also that the ESC code consider that the cursor coordinates
    begin with 1 (1, 1 is the position of the top left character of
    the screen). However, following the standard computing convention,
    this library will consider that the coordinates begin with 0. So,
    (0,0) are the coordinates of the top-left-most character in the
    screen and for a screen of size, say 40x80, the bottom-right
    character has coordinates (39,79).

  -------------------------------------------------------------------
*/

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>

#include "defines.h"


/*
  
  This is the structure used to store the information about a window:
  this is the structure returned by the function that creates a new
  window, and must be passed as a parameter to all functions that
  write in the window.
*/
typedef struct{
  int  ini_row;     /* row on the screen where the window begins */
  int  ini_col;     /* column on the screen where the window begins */
  int last_row;     /* row on the screen where the window finishes */
  int last_col;     /* column on the screen where the window finishes */
  int  n_rows;     /* number of rows in the window  */
  int  n_cols;     /* number of column in the window  */
  
  int  last_line;
  
  int  bg_color; /* the background colour for this window */
  int  fg_color; /* the foreground colour for this window */
  
  int rect_type; /* differenciates each rectangle if you need to print plenty of them at the same time */
}rectangle;


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
rectangle* win_new(int ini_row, int ini_col, int last_row, int last_col, int bg, int fg, int type);


/*
  Clears a window, leaving it all in the current background color
  
  Parameters:
      rec: the window structure on which the function operates.
  
  Returns:
      nothing.
 */
void win_clear(rectangle *rec);


/*
  Releases the structure allocated for a window. After calling this
  function it is no longer possible to use the window.
  
  This function doesn't erase the window contents from the screen.
  
  Parameters:
      rec: the window structure on which the function operates.
  
  Returns:
      nothing.
 */
void win_delete(rectangle *rec);


/*
  Sets the background color for the window
  
  Parameters:
      rec:  the window structure on which the function operates.
      color: the new background color;
  
  Returns:
      OK: regular execution
      FAILED: incorrect color parameter
 */
Status win_bgcolor(rectangle *rec, int color);


/*
  Sets the foreground colour for the window
  
  Parameters:
      rec:  the window structure on which the function operates.
      color: the new foreground colour;
  
  Returns:
      OK: regular execution
      FAILED: incorrect colour parameter
 */
Status win_fgcolor(rectangle *rec, int color);


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
int win_write_line_at(rectangle *rec, int row, int col, char *str);


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
Status win_write_char_at(rectangle *rec, int row, int col, char ch);


int rectangle_getIniRow(rectangle *rec);


int rectangle_getIniCol(rectangle *rec);


int rectangle_getLastRow(rectangle *rec);


int rectangle_getLastCol(rectangle *rec);



/* Returns the type of the rectangle. It allows you to differenciate each rectangle*/
int rectangle_getType(rectangle *rec);


int rectangle_getNRows(rectangle *rec);


/* Returns the number of columns of a given rectangle */
int rectangle_getNCols(rectangle *rec);


/* Draws in the screen a given rectangle */
Status rectangle_draw(rectangle *rec);


#endif /* RECTANGLE_H */