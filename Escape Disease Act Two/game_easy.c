#include "game_easy.h"

Resources **r=NULL;
Object **obj=NULL;
Weapon **wp=NULL;
Enemy **e=NULL;
Interface *intrf=NULL;
Player *pl=NULL;
Strings **s=NULL;
int n_ene=-1;

int hydration=0;
int hunger=0;


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

int get_mid_col(rectangle* r, int len){
    return ((r->last_col-r->ini_col)/2)-(len/2);
}

int get_mid_row(rectangle* r){
    return ((r->last_row-r->ini_row)/2);
}

void game_init(int lang){
    
    intrf = create_intrf("txtfiles/rectangles.txt", "txtfiles/maps.txt");
    
    if(lang==ENGLISH){
        r = load_resources("txtfiles/english/resources_easy.txt");
        wp = load_weapons("txtfiles/english/weapons_easy.txt");
        obj = load_objects("txtfiles/english/objects_easy.txt");
        pl = load_player("txtfiles/english/player_easy.txt");
        e = load_enemies("txtfiles/english/enemies_easy.txt", &n_ene);
    	s = load_strings("txtfiles/english/strings_eng.txt");
    }
    else if(lang==SPANISH){
    	r = load_resources("txtfiles/spanish/resources_easy.txt");
        wp = load_weapons("txtfiles/spanish/weapons_easy.txt");
        obj = load_objects("txtfiles/spanish/objects_easy.txt");
        pl = load_player("txtfiles/spanish/player_easy.txt");
        e = load_enemies("txtfiles/spanish/enemies_easy.txt", &n_ene);
    	s = load_strings("txtfiles/spanish/strings_spa.txt");
    }
    else if(lang==GALICIAN){
    	r = load_resources("txtfiles/galician/resources_easy.txt");
        wp = load_weapons("txtfiles/galician/weapons_easy.txt");
        obj = load_objects("txtfiles/galician/objects_easy.txt");
        pl = load_player("txtfiles/galician/player_easy.txt");
        e = load_enemies("txtfiles/galician/enemies_easy.txt", &n_ene);
    	s = load_strings("txtfiles/galician/strings_gal.txt");
    }
    else{
    	printf("Error. Easy 7.\n");
    	exit(ERROR);
    }
    
    if(intrf==NULL || r==NULL || wp==NULL || obj==NULL || pl==NULL || e==NULL || s==NULL ||n_ene==-1){
        printf("Error. Easy 8.\n");
        exit(ERROR);
    }
    
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

void read_space(){
    char c = 0;
    while(1){
        c = read_key_easy();
        if(c != ' '){
            continue;
        }
        return;
    }
}

int battlemode_easy(int *ene_array, Maps *copymap){
    shoot_stuff *stst=NULL;
    moveEne_stuff *mest=NULL;
    pthread_t pth_shoot, pth_moveEne;
    Enemy **ene=NULL, **econt;
    int c;
    Resources *hung=NULL, *hydra=NULL;
    rectangle *battle=NULL;
    
    
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
    
    hung = resources_getResource(r, FOOD);
    hydra = resources_getResource(r, DRINK);
    if(hung==NULL || hydra==NULL){
        printf("Error. Main-F1-4.\n");
        exit(ERROR);
    }
    battle = win_find_rectangle(RECT_BATTLE, intrf->rect_array);
    if(battle==NULL){
        printf("Error. Main-F1-5.\n");
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
            tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
            break;
        }
        
        
        else if(c < 0){ /* MOVING */
            hydration++;
            hunger++;
            if(hydration == 20){
                hydration = 0;
                if(modify_resource(hydra, -1)<=0){
                    print_resources(intrf, r);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s)); /* You are dead becuase of no hydration */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf, r);
            }
            if(hunger == 40){
                hunger = 0;
                if(modify_resource(hung, -1)<=0){
                    print_resources(intrf, r);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s)); /* You are dead becuase of hunger */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf, r);
            }
            if(move(intrf, copymap, pl, -c)==DOOR && enemy_checkPhyStat(ene)==ALL_KILLED){
                destroy_enemies(ene);
                return DOOR;
            }
        }
        
        else if(c == 'q'){ /* Swaping weapon */
            change_equipped(wp, UPW);
            print_weapons(intrf, wp);
        }
        else if(c == 'e'){ /* Swaping weapon */
            change_equipped(wp, DOWNW);
            print_weapons(intrf, wp);
        }
        /* Using objects */
        else if(c == '1'){
            use_object(r, obj[0]);
            print_objects(intrf, obj);
            print_resources(intrf, r);
        }
        else if(c == '2'){
            use_object(r, obj[1]);
            print_objects(intrf, obj);
            print_resources(intrf, r);
        }
        else if(c == '3'){
            use_object(r, obj[2]);
            print_objects(intrf, obj);
            print_resources(intrf, r);
        }
        else if(c == '4'){
            use_object(r, obj[3]);
            print_objects(intrf, obj);
            print_resources(intrf, r);
        }
        else if(c == '5'){
            use_object(r, obj[4]);
            print_objects(intrf, obj);
            print_resources(intrf, r);
        }
        else if(c == '6'){
            use_object(r, obj[5]);
            print_objects(intrf, obj);
            print_resources(intrf, r);
        }
        else if(c == '7'){
            use_object(r, obj[6]);
            print_objects(intrf, obj);
            print_resources(intrf, r);
        }
        else if(c == '8'){
            use_object(r, obj[7]);
            print_objects(intrf, obj);
            print_resources(intrf, r);
        }
        /* Shooting */
        else if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
            stst = (shoot_stuff *) malloc(sizeof(shoot_stuff));
            stst->intrf = intrf;
            stst->wp = wp;
            stst->pl = pl;
            stst->r = r;
            stst->copymap = copymap;
            stst->dir = dir_conv_easy(c);
            stst->ene = ene;
            pthread_create(&pth_shoot, NULL, shoot, (void *) stst);
            
            hydration++;
            hunger++;
            if(hydration == 20){
                hydration = 0;
                if(modify_resource(hydra, -1)<=0){
                    print_resources(intrf, r);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s)); /* You are dead becuase of no hydration */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf, r);
            }
            if(hunger == 40){
                hunger = 0;
                if(modify_resource(hung, -1)<=0){
                    print_resources(intrf, r);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s)); /* You are dead becuase of hunger */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf, r);
            }
        }
    }
    destroy_enemies(ene);
    return 0;
}


void game_easy(int lang){
    srand(time(NULL));
    
    game_init(lang);
    
    _term_init_easy();
    
    rectangle* story = NULL;
    rectangle* info = NULL;
    rectangle* battle = NULL;
    story = win_find_rectangle(RECT_STORY, intrf->rect_array);
    info = win_find_rectangle(RECT_INFO, intrf->rect_array);
    battle = win_find_rectangle(RECT_BATTLE, intrf->rect_array);
    
    
    if(initialize_intrf(intrf, 120, r, wp, obj, s) == FAILED){
        printf("Error. Easy 8.\n");
        exit(ERROR);
    }
    win_write_line_at(battle, 18, 26, strings_get_string_by_type(7, s));
    
    
    /* TUTORIAL */
    int enemies[3] = {1,1,1};
    char* cont = NULL;
    
    story = win_find_rectangle(RECT_STORY, intrf->rect_array);
    info = win_find_rectangle(RECT_INFO, intrf->rect_array);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(11, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(12, s));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(13, s));
    win_write_line_slow_at(story, 5, 3, strings_get_string_by_type(14, s));
    win_write_line_slow_at(story, 6, 3, strings_get_string_by_type(15, s));
    win_write_line_slow_at(story, 7, 3, strings_get_string_by_type(16, s));
    win_write_line_slow_at(story, 8, 3, strings_get_string_by_type(17, s));
    win_write_line_slow_at(story, 9, 3, strings_get_string_by_type(18, s));
    win_write_line_slow_at(story, 10, 3, strings_get_string_by_type(19, s));
    cont = strings_get_string_by_type(9991, s);
    win_write_line_slow_at(story, 11, get_mid_col(story, strlen(cont)), cont);
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s));
    win_write_line_slow_at(info, 3, 3, strings_get_string_by_type(9993, s));
    win_write_line_slow_at(info, 4, 3, strings_get_string_by_type(9994, s));
    win_write_line_slow_at(info, 5, 3, strings_get_string_by_type(9995, s));
    read_space();
    print_map(intrf, 300);
    print_player(intrf, pl);
    
    Maps *copymap=NULL;
    copymap = map_getCopy(intrf->maps_array, 300);
    if(battlemode_easy(enemies, copymap) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    print_map(intrf, 0);
    
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(20, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(21, s));
    win_write_line_slow_at(story, 4, get_mid_col(story, strlen(cont)), cont);
    read_space();
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(22, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(23, s));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(24, s));
    win_write_line_slow_at(story, 5, get_mid_col(story, strlen(cont)), cont);
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s));
    win_write_line_slow_at(info, 3, 3, strings_get_string_by_type(9995, s));
    read_space();
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(999, s));
    win_write_line_slow_at(story, 3, get_mid_col(story, strlen(cont)), cont);
    read_space();
    win_clear(story);
    win_clear(info);
    /* STORY STARTS */
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1000, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1001, s));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1002, s));
    win_write_line_slow_at(story, 5, 3, strings_get_string_by_type(1003, s));
    win_write_line_slow_at(story, 6, 3, strings_get_string_by_type(1004, s));
    win_write_line_slow_at(story, 7, 3, strings_get_string_by_type(1005, s));
    win_write_line_slow_at(story, 8, 3, strings_get_string_by_type(1006, s));
    win_write_line_slow_at(story, 9, get_mid_col(story, strlen(cont)), cont);
    read_space();
    win_clear(story);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1007, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1008, s));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1009, s));
    win_write_line_slow_at(story, 5, 3, strings_get_string_by_type(1010, s));
    win_write_line_slow_at(story, 6, 3, strings_get_string_by_type(1011, s));
    win_write_line_slow_at(story, 7, 3, strings_get_string_by_type(1012, s));
    win_write_line_slow_at(story, 8, 3, strings_get_string_by_type(1013, s));
    win_write_line_slow_at(story, 9, 3, strings_get_string_by_type(1014, s));
    win_write_line_slow_at(story, 10, 3, strings_get_string_by_type(1015, s));
    win_write_line_slow_at(story, 11, get_mid_col(story, strlen(cont)), cont);
    read_space();
    
    tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
    return;
}