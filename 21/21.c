#include "interface.h"
#include "maps.h"
#include "rectangle.h"
#include <limits.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <termios.h>
#include <string.h>

#define TENTOTHENINE 1000000000



struct termios initial;

int aleat_num(int inf, int sup) {
  
  int dif;
  
  if (inf > sup) return inf - 1;
  
  dif = sup - inf;
  
  return inf + (int)((dif + 1) * (rand() / (RAND_MAX + 1.0)));

}

/*
  Structure to send the clock parameters to the function that manages
  the closk1
*/

typedef struct {
  time_t initial; /* Initial time at which the function is first called */
  Interface  *intrf;    /* pointer to the interface where the clock is to be displayed */
} clock_data;

void _term_init() {
	struct termios new;	          /*a termios structure contains a set of attributes about 
					  how the terminal scans and outputs data*/
		
	tcgetattr(fileno(stdin), &initial);    /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
	new = initial;	                      /*then we copy them into another one, as we aren't going 
						to change ALL the values. We'll keep the rest the same */
	new.c_lflag &= ~ICANON;	              /*here we are setting up new. This line tells to stop the 
						canonical mode (which means waiting for the user to press 
						enter before sending)*/
	new.c_lflag &= ~ECHO;                 /*by deactivating echo, we tell the terminal NOT TO 
						show the characters the user is pressing*/
	new.c_cc[VMIN] = 1;                  /*this states the minimum number of characters we have 
					       to receive before sending is 1 (it means we won't wait 
					       for the user to press 2,3... letters)*/
	new.c_cc[VTIME] = 0;	              /*I really have no clue what this does, it must be somewhere in the book tho*/
	new.c_lflag &= ~ISIG;                 /*here we discard signals: the program won't end even if we 
						press Ctrl+C or we tell it to finish*/

	tcsetattr(fileno(stdin), TCSANOW, &new);  /*now we SET the attributes stored in new to the 
						    terminal. TCSANOW tells the program not to wait 
						    before making this change*/
}



int read_key(){
    char choice;
    choice = fgetc(stdin);
    
    if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
        choice = fgetc(stdin);

        switch(choice) {
        case('A'):
            return -UP;
        case('B'):
            return -DOWN;
        case('C'):
            return -RIGHT;
        case('D'):
            return -LEFT;
        default:
            return -HERE;
        }
    } 
    
    return choice;
}


int main(){
    Interface *intrf=NULL;
    int c, score=0;
    int aleat = 0;
    
    intrf = create_intrf("rectangles.txt", "map.txt");
    if(intrf == NULL) exit(12345);
    
    print_map (intrf, 23);
    sleep(2);
    print_map(intrf, 22);
    
  
    _term_init();
    sleep(2);
    
    while(1){
        
        
        aleat = aleat_num(1, 7);
        
        score += aleat;
        
        if (score > 21){
            break;
        }
        
        print_map (intrf, score);
        
        if (score == 1){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 2){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 3){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 4){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 5){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 6){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 7){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 8){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 9){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 10){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 11){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 12){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 13){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 14){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 15){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 16){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 17){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 18){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 19){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 20){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 21){
            c = read_key();
            if (c==-UP)
                continue;
            else
                break;
        }    
    }
    if (score>21){
        printf("\n\nYou get 0 points, so greedy!!!\n\n");
    }else{
        printf("\n\nYou get %d coins.\n\n", score); 
    }
    
   destroy_intrf(intrf);
    
    return 0;
}