#include "interface.h"
#include "resources.h"
#include "object.h"
#include "player.h"
#include "weapon.h"
#include "maps.h"
#include "rectangle.h"
#include "strings.h"
#include "missions.h"
#include "game_easy.h"
#include "game_medium.h"
#include "game_hard.h"
#include "game_legendary.h"
#include "game_battlemode.h"

#include <pthread.h>
#include <termios.h>

#define NLANG 3

struct termios initial;

typedef struct{
  time_t initial; /* Initial time at which the function is first called */
  Interface  *intrf;    /* pointer to the interface where the clock is to be displayed */
} clock_data;


void _term_init(){
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
    Interface *itf=NULL;
    Strings **s=NULL;
    int i, c, id=1, lang, mode;
    
    itf = create_intrf("txtfiles/intro_rectangles.txt", "txtfiles/intro_maps.txt");
    if(itf==NULL){
    	printf("Error. Game 1.\n");
    	exit(ERROR);
    }
    
    _term_init();
    
    for(i=0; i < itf->n_rectangles; i++){
        rectangle_draw(itf->rect_array[i]);
    }
    
    print_map(itf, 0); /* Printing Escape Disease Act Two */
    sleep(3);
    print_map(itf, 1); /* Printing the map where you select the language */
    
    while(1){
    	c = read_key();
    	
    	if(c < 0){ /* If you press an arrow */
    		if(-c == DOWN){
    			id++;
    			if(id > NLANG) id = 1;
    		}
    		if(-c == UP){
    			id--;
    			if(id < 1) id = NLANG;
    		}
    		print_map(itf, id);
    	}
    	
    	else if(c == 'k' || c == 'K') break;
    }
    
    if(id==1){
    	lang = ENGLISH;
    }
    else if(id==2){
    	lang = SPANISH;
    }
    else if(id==3){
    	lang = GALICIAN;
    }
    else{
    	printf("Error. Game 2.\n");
    	exit(ERROR);
    }
    
    win_clear(itf->rect_array[0]);
    
    if(lang==ENGLISH){
    	s = load_strings("txtfiles/english/strings_eng.txt");
    }
    else if(lang==SPANISH){
    	s = load_strings("txtfiles/spanish/strings_spa.txt");
    }
    else if(lang==GALICIAN){
    	s = load_strings("txtfiles/galician/strings_gal.txt");
    }
    else{
    	printf("Error. Game 3.\n");
    	exit(ERROR);
    }
    if(s==NULL){
    	printf("Error. Game 4.\n");
    	exit(ERROR);
    }
    
    win_write_line_at(itf->rect_array[0], 8, 15, strings_get_string_by_type(1, s));
    win_write_line_at(itf->rect_array[0], 15, 15, strings_get_string_by_type(2, s));
    win_write_line_at(itf->rect_array[0], 19, 15, strings_get_string_by_type(3, s));
    win_write_line_at(itf->rect_array[0], 23, 15, strings_get_string_by_type(4, s));
    win_write_line_at(itf->rect_array[0], 27, 15, strings_get_string_by_type(5, s));
    win_write_line_at(itf->rect_array[0], 31, 15, strings_get_string_by_type(6, s));
    
    while(1){
    	c = read_key();
    	
    	if(c=='1'){
    		mode = EASY;
    		break;
    	}
    	else if(c=='2'){
    		mode = MEDIUM;
    		break;
    	}
    	else if(c=='3'){
    		mode = HARD;
    		break;
    	}
    	else if(c=='4'){
    		mode = LEGENDARY;
    		break;
    	}
    	else if(c=='5'){
    		mode = BATTLE_MODE;
    		break;
    	}
    }
    
    tcsetattr(fileno(stdin), TCSANOW, &initial);
    destroy_intrf(itf);
    strings_destroy(s);
    
    
    if(mode==EASY) game_easy(lang);
    else if(mode==MEDIUM) game_medium(lang);
    else if(mode==HARD) game_hard(lang);
    else if(mode==LEGENDARY) game_hard(lang);
    else if(mode==BATTLE_MODE) game_battlemode(lang);
    else{
    	printf("Error. Game 5.\n");
    	exit(ERROR);
    }
    
    return 0;
}
