#include "interface.h"
#include "maps.h"
#include "rectangle.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <termios.h>

#define TENTOTHENINE 1000000000

int percent=0, superflag=1;
int row_bar=14; /*Max:28 Initial: 14 Min: 6*/
    

struct termios initial;

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


typedef struct{
    Interface* intrf;
}delete_stuff;

typedef struct{
    int row;
    int col;
}ball;


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

void print_bar(Interface *intrf){
    int i;
    for(i=3; i<percent && i<91; i++){
        win_write_char_at(intrf->rect_array[0], 14, i, '#');
    }
    /*
    while(i<91){
        win_write_char_at(intrf->rect_array[0], 14, i, ' ');
        i++;
    }
    */
    
}

int generate_random_way(int inf, int sup){
  int dif;
  
  if (inf > sup) return inf - 1;
  
  dif = sup - inf;
  
  return inf + (int)((dif + 1) * (rand() / (RAND_MAX + 1.0)));
}


void* move_bars(void* stuff){
    int i;
    delete_stuff *del;
    
    del = (delete_stuff*) stuff;
    
    return (void*) del;
}


int main(){
    Interface *intrf=NULL;
    int way, dir, cicle; 
    ball *actual, *after;
    struct timespec time1, time2, time3;
    pthread_t db;
    delete_stuff *delst=NULL;
    
    _term_init();
    
    intrf = create_intrf("rectangles.txt", "map.txt");
    if(intrf == NULL) exit(1);
    
    print_map(intrf, 1);
    
    actual->row = 14;
    actual->col = 45;
    win_write_char_at(intrf->rect_array[0],14,45,"&");
    
    clock_gettime(CLOCK_REALTIME, &time1);
    
    
    delst = (delete_stuff *) malloc(sizeof(delete_stuff));
    delst->intrf = intrf;
    pthread_create(&db, NULL, delete_bar, (void*) delst);
    
    way = generate_random_way(1, 5);
    
    while(1){
        clock_gettime(CLOCK_REALTIME, &time2);
        if((time2.tv_sec-time1.tv_sec) > 30){
            break;
        }
        
        if(actual->col == 2 || actual->col == 92){
            if(actual->row <= row_bar && actual->row >= row_bar - 5){
                way = generate_random_way(1, 5);
            }
            if(actual->col == 2){
                dir = LEFT;
            }
            else{
                dir = RIGHT;
            }
            cicle = 0;
        }
        
    }
    
    
    tcsetattr(fileno(stdin), TCSANOW, &initial);	/*We now restore the settings we back-up'd 
							  so that the termial behaves normally when 
							  the program ends */
							  
    destroy_intrf(intrf);
    
    return 0;
}