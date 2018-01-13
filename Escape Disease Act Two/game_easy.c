#include "game_easy.h"

Resources **r=NULL;
Object **obj=NULL;
Weapon **wp=NULL;
Enemy **e=NULL;
Interface *intrf=NULL;
Player *pl=NULL;
Strings **s=NULL;
int n_ene=-1;


struct termios initial_easy;


typedef struct {
  time_t initial_easy; /* Initial time at which the function is first called */
  Interface  *intrf;    /* pointer to the interface where the clock is to be displayed */
} clock_data_easy;


void _term_init_easy() {
	struct termios new;	          /*a termios structure contains a set of attributes about 
					  how the terminal scans and outputs data*/
		
	tcgetattr(fileno(stdin), &initial_easy);    /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
	new = initial_easy;	                      /*then we copy them into another one, as we aren't going 
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

int read_key_easy(){
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


int dir_conv_easy(int d) {
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


int battlemode_easy(int *ene_array, Maps *copymap){
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
        c = read_key_easy();
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
            stst->dir = dir_conv_easy(c);
            stst->ene = ene;
            pthread_create(&pth_shoot, NULL, shoot, (void *) stst);
        }
    }
    destroy_enemies(ene);
    return DOOR;
}


void game_easy(int lang){
    srand(time(NULL));
    
    r = load_resources("txtfiles/resources_easy.txt");
    if(r == NULL){
        printf("Error. Easy 1.\n");
        exit(ERROR);
    }
    
    wp = load_weapons("txtfiles/weapons_easy.txt");
    if(wp == NULL){
        printf("Error. Easy 2.\n");
        exit(ERROR);
    }    
    
    obj = load_objects("txtfiles/objects_easy.txt");
    if(obj == NULL){
        printf("Error. Easy 3.\n");
        exit(ERROR);
    }    
    
    pl = load_player("txtfiles/player_easy.txt");
    if(pl == NULL){
        printf("Error. Easy 4.\n");
        exit(ERROR);
    }    
    
    intrf = create_intrf("txtfiles/rectangles.txt", "txtfiles/maps.txt");
    if(intrf == NULL){
        printf("Error. Easy 5.\n");
        exit(ERROR);
    }    
    
    e = load_enemies("txtfiles/enemies_easy.txt", &n_ene);
    if(e==NULL || n_ene==-1){
        printf("Error. Easy 6.\n");
        exit(ERROR);
    }    
    
    if(lang==ENGLISH){
    	s = load_strings("txtfiles/strings_eng.txt");
    }
    else if(lang==SPANISH){
    	s = load_strings("txtfiles/strings_spa.txt");
    }
    else if(lang==GALICIAN){
    	s = load_strings("txtfiles/strings_gal.txt");
    }
    else{
    	printf("Error. Easy 7.\n");
    	exit(ERROR);
    }
    
    _term_init_easy();
    
    rectangle* story = NULL;
    rectangle* info = NULL;
    rectangle* battle = NULL;
    story = win_find_rectangle(RECT_STORY, intrf->rect_array);
    info = win_find_rectangle(RECT_INFO, intrf->rect_array);
    battle = win_find_rectangle(RECT_BATTLE, intrf->rect_array);
    
    
    if(initialize_intrf(intrf, 120, r, wp, obj) == FAILED){
        printf("Error. Easy 8.\n");
        exit(ERROR);
    }
    win_write_line_at(battle, 27, 15, strings_get_string_by_type(7, s));
    
    
    /* TUTORIAL */
    char c = 0;
    int enemies[3] = {1,1,1};
    
    story = win_find_rectangle(RECT_STORY, intrf->rect_array);
    info = win_find_rectangle(RECT_INFO, intrf->rect_array);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(11, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(12, s));
    sleep(3);
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(13, s));
    win_write_line_slow_at(story, 5, 3, strings_get_string_by_type(14, s));
    sleep(3);
    win_write_line_slow_at(story, 6, 3, strings_get_string_by_type(15, s));
    win_write_line_slow_at(story, 7, 3, strings_get_string_by_type(16, s));
    sleep(3);
    win_write_line_slow_at(story, 8, 3, strings_get_string_by_type(17, s));
    win_write_line_slow_at(story, 9, 3, strings_get_string_by_type(18, s));
    sleep(3);
    win_write_line_slow_at(story, 10, 3, strings_get_string_by_type(19, s));
    win_write_line_slow_at(info, 3, 3, strings_get_string_by_type(9991, s));
    win_write_line_slow_at(info, 4, 3, strings_get_string_by_type(9992, s));
    win_write_line_slow_at(info, 5, 3, strings_get_string_by_type(9993, s));
    win_write_line_slow_at(info, 6, 3, strings_get_string_by_type(9994, s));
    while(1){
        c = read_key_easy();
        if(c != ' '){
            continue;
        }
        break;
    }
    print_map(intrf, 300);
    print_player(intrf, pl);
    
    Maps *copymap=NULL;
    copymap = map_getCopy(intrf->maps_array, 300);
    battlemode_easy(enemies, copymap);
    
    win_clear(story);
    win_clear(info);
    
    
    
    tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
}