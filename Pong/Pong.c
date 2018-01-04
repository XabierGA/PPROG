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

int superflag = 0;
int row_bar;/*Max:28 Initial: 14 Min: 6*/
    

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
}bar_stuff;

typedef struct{
    int row;
    int col;
}Ball;


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

void print_ball(Interface* intrf, Ball* ball, int dir){
    if(dir == RIGHT){
        ball->col ++;
    }
    else if(dir == LEFT){
        ball->col --;
    }
    else if(dir == UP){
        ball->row --;
    }
    else if(dir == DOWN){
        ball->row++;
    }
    
    
    win_write_char_at(intrf->rect_array[0], ball->row, ball->col, '*');
}


int generate_random_way(int inf, int sup){
    int dif;
  
    if (inf > sup) return inf - 1;
  
    dif = sup - inf;
  
    return inf + (int)((dif + 1) * (rand() / (RAND_MAX + 1.0)));
}

void move_ball(Interface* intrf, Ball* ball, int dir1, int dir2, int* cicle, int way){
    win_write_char_at(intrf->rect_array[0], ball->row, ball->col, ' ');
    if(way == 1){
        print_ball(intrf, ball, dir1);
        *cicle = 0;
        return;
    }
    
    if(way == 2){
        if(*cicle == 0){
            print_ball(intrf, ball, dir1);
            *cicle = 1;
            return;
        }
        if(*cicle == 1){
            print_ball(intrf, ball, dir2);
            *cicle = 0;
            return;
        }
    }
    
    if(way == 3){
        if(*cicle == 0){
            print_ball(intrf, ball, dir1);
            *cicle = 1;
            return;
        }
        if(*cicle == 1){
            print_ball(intrf, ball, dir1);
            *cicle = 2;
            return;
        }
        if(*cicle == 2){
            print_ball(intrf, ball, dir2);
            *cicle = 0;
            return;
        }
    }
    
    if(way == 4){
        if(*cicle == 0){
            print_ball(intrf, ball, dir1);
            *cicle = 1;
            return;
        }
        if(*cicle == 1){
            print_ball(intrf, ball, dir2);
            *cicle = 2;
            return;
        }
        if(*cicle == 2){
            print_ball(intrf, ball, dir2);
            *cicle = 0;
            return;
        }
    }
    
}

void print_bars(Interface* intrf, int c){
    int i;
    
    if(row_bar <= 6 || row_bar + 3 >= 34){
        return;
    }
    
    if(c == -UP){
        win_write_char_at(intrf->rect_array[0], row_bar + 5, 1,' ');
        win_write_char_at(intrf->rect_array[0], row_bar + 5, 91,' ');
    }
    else if(c == -DOWN){
        win_write_char_at(intrf->rect_array[0], row_bar - 1, 1,' ');
        win_write_char_at(intrf->rect_array[0], row_bar - 1, 91,' ');
    }
    else{
        return;
    }
    
    for(i = row_bar; i< (row_bar+5); i++){
        win_write_char_at(intrf->rect_array[0], i, 1,'|');
        win_write_char_at(intrf->rect_array[0], i, 91,'|');
    }
}

void* move_bars(void* stuff){
    int c;
    bar_stuff *bar;
    
    bar = (bar_stuff*) stuff;
    
    row_bar = 14;
    print_bars(bar->intrf, -UP);
    
    while(superflag == 0){
        c = read_key();
        if((row_bar == 6 && c == -UP )|| (row_bar + 3 == 34 && c == -DOWN)){
            continue;
        }
        
        if(c == -DOWN){
            row_bar ++;
        }
        else if(c == -UP){
            row_bar --;
        }
        else{
            continue;
        }
        
        print_bars(bar->intrf, c);
    }
    
    return (void*) bar;
}

int change_dir(int dir){
    if (dir == UP){
        return DOWN;
    }
    if (dir == DOWN){
        return UP;
    }
    if (dir == LEFT){
        return RIGHT;
    }
    if (dir == RIGHT){
        return LEFT;
    }
    return 0;
}


int main(){
    Interface *intrf=NULL;
    int way, dir1, dir2, cicle=0; 
    Ball *ball;
    struct timespec time1, time2;
    pthread_t db;
    bar_stuff *stuff=NULL;
    
    _term_init();
    
    srand(time(NULL)); /* ----------> CABORNERO, YOU HAVE TO CHANGE THE SEED !!!! <------------------ */
    
    intrf = create_intrf("rectangles.txt", "map.txt");
    if(intrf == NULL) exit(1);
    
    print_map(intrf, 1);
    
    ball = (Ball *)malloc(sizeof(Ball));
    
    ball->row = 14;
    ball->col = 45;
    win_write_char_at(intrf->rect_array[0], ball->row, ball->col,'*');
    
    clock_gettime(CLOCK_REALTIME, &time1);
    
    
    stuff = (bar_stuff *) malloc(sizeof(bar_stuff));
    stuff->intrf = intrf;
    pthread_create(&db, NULL, move_bars, (void*) stuff);
    
    way = generate_random_way(1, 4);
    dir1 = LEFT;
    dir2 = UP;
    
    while(1){
        clock_gettime(CLOCK_REALTIME, &time2);
        if((time2.tv_sec-time1.tv_sec) > 60){
            superflag = 1;
            break;
        }
        
        if((ball->col == 2 && dir1 == LEFT )|| (ball->col == 90 && dir1 == RIGHT)){
            if(ball->row >= row_bar && ball->row <= row_bar + 5){
                way = generate_random_way(1, 4);
            }
            else{
                superflag = 2;
                break;
            }
            cicle = 0;
            dir1 = change_dir(dir1);
        }
        
        if((ball->row == 7 && dir2 == UP )|| (ball->row == 34 && dir2 == DOWN)){
            dir2 = change_dir(dir2);
        }
        
        move_ball(intrf, ball, dir1, dir2, &cicle, way);
        usleep(60000);
    }
    
    if(superflag == 1){
        win_write_line_at(intrf->rect_array[0], 14, 30, "YOU WON!!!!");
    }
    if(superflag == 2){
        win_write_line_at(intrf->rect_array[0], 14, 30, "YOU LOST...");
    }
    sleep(2);
    
    tcsetattr(fileno(stdin), TCSANOW, &initial);	/*We now restore the settings we back-up'd 
							  so that the termial behaves normally when 
							  the program ends */
							  
    destroy_intrf(intrf);
    
    return 0;
}