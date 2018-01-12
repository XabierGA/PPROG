#define MAX 100

#include "interface.h"
#include "resources.h"
#include "object.h"
#include "player.h"
#include "weapon.h"
#include "maps.h"
#include "rectangle.h"
#include <pthread.h>
#include <termios.h>

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


int conv(int i){
    switch(i){
        case 0:
            return 10;
        case 1:
            return 14;
        case 2:
            return 18;
        case 3:
            return 22;
        case 4:
            return 26;
        case 5:
            return 30;
        case 6:
            return 34;
        case 7:
            return 38;
        default:
            printf("----------> %d <-----------", i);
            return 2;
    }
}

void change_map(char *found, int deaths, Interface *intrf, rectangle *rec){
    int i;
    int col;
    
    print_map(intrf, deaths);
    
    for(i=0; i<8; i++){
        if(found[i] != '0'){
            col = conv(i);
            win_write_char_at(rec, 17, col, found[i]);
        }
    }
    return;
}

int main(){
    Resources **r=NULL;
    Weapon **wp=NULL;
    Object **obj=NULL;
    Player *pl=NULL;
    Interface *intrf=NULL;
    rectangle *rectStory=NULL, *rectBattle=NULL;
    /*All the variables for the game*/
    char letter;
    int i, col, flag, num_words, random_n, deaths=0, nice=0;
    FILE *f_in=NULL;
    char buff[MAX], word[MAX];
    char found[8] = "00000000";
    /*--------------------------------------*/
    /*--------------------------------------*/
    srand(time(NULL));
    
    r = load_resources("resources.txt");
    if(r == NULL) exit(12345);
    
    wp = load_weapons("weapons.txt");
    if(wp == NULL) exit(12345);
    
    obj = load_objects("objects.txt");
    if(obj == NULL) exit(12345);
    
    pl = load_player("player.txt");
    if(pl == NULL) exit(12345);
    
    intrf = create_intrf("rectangles.txt", "map.txt");
    if(intrf == NULL) exit(12345);
    
    for(i=0; i < intrf->n_rectangles; i++){
        rectangle_draw(intrf->rect_array[i]);
    }
    
    /* Printing the resources */
    print_resources(intrf, r);
    
    /* Printing the armory */
    print_weapons(intrf, wp);
    
    /* Printing the inventory */
    print_objects(intrf, obj);
    
    /* Printing the initial map */
    print_map(intrf, 0);
    /*-------------------------------------*/
    _term_init();
    
    /*Opening file*/
    f_in = (FILE *) fopen("hangman_eng.txt", "r");
    if(f_in == NULL){ /* Memory error */
        printf("ERROR trying to open file at hangedman game.\n");
        return ERROR;
    }
    
   /* Getting the number of words in the file */
    fgets(buff, MAX, f_in);
    sscanf(buff, "%d", &num_words);
    
    random_n = rand_num(1, num_words);
    
    /*We got the random number that will choose the random word, and we search for it*/
    for(i=1; i<random_n; i++){
        fgets(buff, MAX, f_in);
        sscanf(buff, "%s", word);
    }
    fgets(buff, MAX, f_in);
    sscanf(buff, "%s", word);
    
    for(i=0; i<intrf->n_rectangles; i++){
        if(rectangle_getType(intrf->rect_array[i]) == RECT_STORY){
            rectStory = intrf->rect_array[i];
            break;
        }
    }
    for(i=0; i<intrf->n_rectangles; i++){
        if(rectangle_getType(intrf->rect_array[i]) == RECT_BATTLE){
            rectBattle = intrf->rect_array[i];
            break;
        }
    }
    
    /*-------------------------------Once we´ve got the word, the game STARTS ------------------------------*/
    
    win_write_line_at(rectStory, 5, 5, "PRESS A KEY TO GUESS A LETTER (THE WORD IS CAPITALIZED)");
    
    
    while(1){
        flag=0;
        letter = read_key();
        
        for(i=0; i<strlen(word); i++){
            if(letter == word[i]){
                flag=1;
                nice++;
                col = conv(i);
                win_write_char_at(rectBattle, 17, col, word[i]);
                found[i] = word[i];
                win_write_line_at(rectStory, 6, 5, "NICEEE! THIS LETTER APPEARS!");
            }
        }
        
        if(flag==0){
            deaths++;
            change_map(found, deaths, intrf, rectBattle);
            if(deaths==7){
                win_clear(rectStory);
                win_write_line_at(rectStory, 5, 5, "YOU ARE DEAD, YOU LOST!");
                break;
            }
        }
        else if(nice==8){
            win_clear(rectStory);
            win_write_line_at(rectStory, 5, 5, "YOU WON, CONGRATULATIONS!");
            break;
        }
    }
    tcsetattr(fileno(stdin), TCSANOW, &initial);	/*We now restore the settings we back-up'd 
							  so that the termial behaves normally when 
							  the program ends */
							  
    destroy_resources(r);
    
    destroy_weapons(wp);
    
    destroy_objects(obj);
    
    player_delete(pl);
    
    destroy_intrf(intrf);
    
    return 0;
}