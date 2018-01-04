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


void* delete_bar(void* stuff){
    int i;
    delete_stuff *del;
    
    del = (delete_stuff*) stuff;
    
    while(superflag == 1){
        sleep(1);
        
        if(percent > 2){
            percent = percent-3;
        }
        for(i=percent; i<91; i++){
            if(i>2){
                win_write_char_at(del->intrf->rect_array[0], 14, i, ' ');
            }
        }
    } 
    return (void*) del;
}


int main(){
    Interface *intrf=NULL;
    int flag=2, c;
    struct timespec time1, time2, time3;
    pthread_t db;
    delete_stuff *delst=NULL;
    
    _term_init();
    
    intrf = create_intrf("rectangles.txt", "map.txt");
    if(intrf == NULL) exit(1);
    
    print_map(intrf, 1);
    
    sleep(4);
    print_map(intrf, 2);
    clock_gettime(CLOCK_REALTIME, &time1);
    clock_gettime(CLOCK_REALTIME, &time3);
    
    
    delst = (delete_stuff *) malloc(sizeof(delete_stuff));
    delst->intrf = intrf;
    pthread_create(&db, NULL, delete_bar, (void*) delst);
    
    while(1){
        clock_gettime(CLOCK_REALTIME, &time2);
        if((time2.tv_sec-time1.tv_sec) > 30){
            break;
        }
        
        c=read_key();
        
        if(c == 'o'){
            break;
        }
        
        if(flag != 0 && c == -RIGHT){
            flag = 0;
            percent += 1;
        }
        if(flag != 1 && c == -LEFT){
            flag = 1;
            percent += 1;
        }
        
        print_bar(intrf);
        
        if(percent>=91){
            superflag=0;
            break;
        }
    }
    
    if(percent<91){
        win_write_line_at(intrf->rect_array[0], 14, 30, "YOU LOST...");
    }
    else{
        win_write_line_at(intrf->rect_array[0], 14, 30, "YOU WON!!!!");
    }
    sleep(2);
    tcsetattr(fileno(stdin), TCSANOW, &initial);	/*We now restore the settings we back-up'd 
							  so that the termial behaves normally when 
							  the program ends */
							  
    destroy_intrf(intrf);
    
    return 0;
}