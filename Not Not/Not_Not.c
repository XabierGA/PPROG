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

int puntos = 0;

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
    int c, i, score;
    int aleat = 0;
    struct timespec t_inicio, t_fin, aux;
    double total;
    
    srand(time(NULL));
    
    intrf = create_intrf("rectangles.txt", "map.txt");
    if(intrf == NULL) exit(12345);
    
    print_map (intrf, 13);
    sleep(2);
    
  
    _term_init();
    sleep(2);
    clock_gettime(CLOCK_REALTIME, &t_inicio);
    
    for (i = 0; i<10 ;i++){
        
        
        aleat = aleat_num(1, 11);
        
        print_map (intrf, aleat);
        
        if (aleat == 1){
            c = read_key();
            if (c==-RIGHT){
                puntos++;
            }else{
                break;
            } 
        }else if (aleat == 2){
            c = read_key();
            if (c==-LEFT){
                puntos++;
            }else{
                break;
            } 
        }else if (aleat == 3){
            c = read_key();
            if (c==-RIGHT||c==-UP||c==-DOWN){
                puntos++;
            }else{
                break;
            } 
        }else if (aleat == 4){
            c = read_key();
            if (c==-LEFT||c==-UP||c==-DOWN){
                puntos++;
            }else{
                break;
            } 
        }else if (aleat == 5){
            c = read_key();
            if (c==-RIGHT){
                puntos ++;
            }else{
                break;
            }
        }else if (aleat == 6){
            c = read_key();
            if (c==-LEFT){
                puntos ++;
            }else{
                break;
            }
        }else if (aleat == 7){
           c = read_key();
           if (c==-RIGHT||c==-UP){
                puntos ++;
            }else{
                break;
            }
        }else if (aleat == 8){
           c = read_key();
           if (c==-UP){
                puntos ++;
            }else{
                break;
            }
        }else if (aleat == 9){
           c = read_key();
           if (c==-DOWN){
                puntos ++;
            }else{
                break;
            }
        }else if (aleat == 10){
           c = read_key();
           if (c==-RIGHT||c==-DOWN||c==-LEFT){
                puntos ++;
            }else{
                break;
            }
        }else if (aleat == 11){
           c = read_key();
           if (c==-RIGHT||c==-UP||c==-LEFT){
                puntos ++;
            }else{
                break;
            }
        }    
    }
    
    clock_gettime(CLOCK_REALTIME, &t_fin);
    aux.tv_sec = t_fin.tv_sec - t_inicio.tv_sec; 
    total = aux.tv_sec;
    score = (int)((puntos*10)/(int)total);
    printf("\n\nPringao tienes %d aciertos en %d segundos luego tu score es %d.\n\n",(int)puntos, (int)total, score);
    destroy_intrf(intrf);
    tcsetattr(fileno(stdin), TCSANOW, &initial);
    
    return 0;
}