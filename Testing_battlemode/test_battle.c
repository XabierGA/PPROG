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

Resources **r=NULL;
Object **obj=NULL;
Weapon **wp=NULL;
Enemy **e=NULL;
Interface *intrf=NULL;
Player *pl=NULL;
int n_ene=-1;



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


int dir_conv(int d) {
    if (d == 'w')
        return UP;
    if (d == 'a')
        return LEFT;
    if (d == 's')
        return DOWN;
    if (d == 'd')
        return RIGHT;
    return HERE;
}


int battlemode(int *ene_array, Maps *copymap){
    shoot_stuff *stst=NULL;
    moveEne_stuff *mest=NULL;
    pthread_t pth_shoot, pth_moveEne;
    Enemy **ene=NULL, **econt;
    int c;
    
    if(ene_array == NULL){
        printf("Error. Main-F1-1.\n");
        exit(ERROR);
    }
    if(n_ene == -1){
        printf("Error. Main-F1-2.\n");
        exit(ERROR);  
    }
    if(copymap == NULL){
        printf("Error. Main-F1-3.\n");
        exit(ERROR);
    }
    
    ene = generate_arrayEnemies(e, ene_array, n_ene);
    if(ene==NULL) exit(12345);
    
    generate_EnePosRand(ene, copymap);
    
    print_enemies(intrf, ene);
    
    copymap->field[player_getRow(pl) - 2][player_getCol(pl) - 2] = player_getDisplay(pl); /* Inserting the player display into the map*/
    

    for(econt = ene; *(econt)!=NULL; econt++){
        mest = (moveEne_stuff *) malloc(sizeof(moveEne_stuff)); /* When are we freeing this ??? */
        mest->intrf = intrf;
        mest->pl = pl;
        mest->r = r;
        mest->copymap = copymap;
        mest->ene = *(econt);
        pthread_create(&pth_moveEne, NULL, move_enemies, (void *) mest);
    }

    while(1){
        c = read_key();
        if(c == 'o'){
            break;
        }
        
        
        else if(c < 0){
            if(move(intrf, copymap, pl, -c)==DOOR && enemy_checkPhyStat(ene)==ALL_KILLED){
                destroy_enemies(ene);
                return DOOR;
            }
        }
        
        else if(c == 'q'){
            change_equipped(wp, UPW);
            print_weapons(intrf, wp);
        }
        else if(c == 'e'){
            change_equipped(wp, DOWNW);
            print_weapons(intrf, wp);
        }
        
        else if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
            stst = (shoot_stuff *) malloc(sizeof(shoot_stuff)); /* WHen are we freeing this ??? */
            stst->intrf = intrf;
            stst->wp = wp;
            stst->pl = pl;
            stst->r = r;
            stst->copymap = copymap;
            stst->dir = dir_conv(c);
            stst->ene = ene;
            pthread_create(&pth_shoot, NULL, shoot, (void *) stst);
        }
        /*if(stst != NULL) free(stst);*/ /*Here we cant free it or it will explote */
    }
    /*if(stst != NULL) free(stst);*/ /*Here the memory problems aren't fixed */
    destroy_enemies(ene);
    return 0;
}



int main(){
    int c;
    int n[3] = {1,0,0};
    Maps *copymap=NULL, *copymap2=NULL, *copymap3=NULL;
    
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
    
    e = load_enemies("enemies.txt", &n_ene);
    if(e==NULL || n_ene==-1)exit(12345);
    
    _term_init();
    
    if(initialize_intrf(intrf, 1, r, wp, obj, pl) == FAILED) exit(12345);
    
    /*----------------------------------------------------------------------------------------*/
    copymap = map_getCopy(intrf->maps_array, 1); /*We get a copy of the map where we are gonna play*/
    if(copymap == NULL){
        printf("Error. Main 1.\n");
        exit(ERROR);
    } 
    
    if(battlemode(n, copymap) != DOOR) goto FINAL_LABEL;
    
    /*-----------------------------------------------------------------------------------------*/

    win_clear(intrf->rect_array[0]);
    player_setLocation(pl, 30, 50);
    if(initialize_intrf(intrf, 1, r, wp, obj, pl) == FAILED){
        printf("Error. Main 2.\n");
        exit(ERROR);
    }
    
    
    copymap2 = map_getCopy(intrf->maps_array, 1);
    while(1){
        c = read_key();
        
        if(c < 0){
            if(move(intrf, copymap2, pl, -c) == DOOR){
                break;
            }
        }
        else if(c == 'o') goto FINAL_LABEL;
    }
    /*----------------------------------------------------------------------------------*/

    win_clear(intrf->rect_array[0]);
    int n2[3] = {1,1,1};
    
    copymap3 = map_getCopy(intrf->maps_array, 2);
    player_setLocation(pl, 25, 15);
    if(initialize_intrf(intrf, 1, r, wp, obj, pl) == FAILED){
        printf("Error. Main 3.\n");
        exit(ERROR);
    }
    
    if(battlemode(n2, copymap3) != DOOR) goto FINAL_LABEL;
    
    printf("CONGRATULATIONS!\n");
    
    /*sleep(10);*/
    FINAL_LABEL:
    /*delete_map(copymap);*/
    
    destroy_resources(r);
    
    destroy_weapons(wp);
    
    destroy_objects(obj);
    
    player_delete(pl);
    
    destroy_intrf(intrf);
    
    destroy_enemies(e);
    
    tcsetattr(fileno(stdin), TCSANOW, &initial);	/*We now restore the settings we back-up'd 
							  so that the termial behaves normally when 
							  the program ends */
    
    return 0;
}