#include "game_hard.h"

Resources **r_h=NULL;
Object **obj_h=NULL;
Weapon **wp_h=NULL;
Enemy **e_h=NULL;
Interface *intrf_h=NULL;
Player *pl_h=NULL;
Strings **s_h=NULL;
int n_ene_h=-1;

int hydration_h=0;
int hunger_h=0;


struct termios initial_hard;


typedef struct {
  time_t initial_hard; /* Initial time at which the function is first called */
  Interface  *intrf_h;    /* pointer to the interface where the clock is to be displayed */
} clock_data_hard;


void _term_init_hard() {
	struct termios new;	          /*a termios structure contains a set of attributes about 
					  how the terminal scans and outputs data*/
		
	tcgetattr(fileno(stdin), &initial_hard);    /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
	new = initial_hard;	                      /*then we copy them into another one, as we aren't going 
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

int read_key_hard(){
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

int get_mid_col_hard(rectangle* r_h, int len){
    return (((r_h->last_col-r_h->ini_col)/2)-(len/2)+1);
}

int get_mid_row_hard(rectangle* r_h){
    return ((r_h->last_row-r_h->ini_row)/2);
}

void game_init_hard(int lang){
    
    intrf_h = create_intrf("txtfiles/rectangles.txt", "txtfiles/maps.txt");
    
    if(lang==ENGLISH){
        r_h = load_resources("txtfiles/english/resources_hard.txt");
        wp_h = load_weapons("txtfiles/english/weapons_hard.txt");
        obj_h = load_objects("txtfiles/english/objects_hard.txt");
        pl_h = load_player("txtfiles/english/player_hard.txt");
        e_h = load_enemies("txtfiles/english/enemies_hard.txt", &n_ene_h);
    	s_h = load_strings("txtfiles/english/strings_eng.txt");
    }
    else if(lang==SPANISH){
    	r_h = load_resources("txtfiles/spanish/resources_hard.txt");
        wp_h = load_weapons("txtfiles/spanish/weapons_hard.txt");
        obj_h = load_objects("txtfiles/spanish/objects_hard.txt");
        pl_h = load_player("txtfiles/spanish/player_hard.txt");
        e_h = load_enemies("txtfiles/spanish/enemies_hard.txt", &n_ene_h);
    	s_h = load_strings("txtfiles/spanish/strings_spa.txt");
    }
    else if(lang==GALICIAN){
    	r_h = load_resources("txtfiles/galician/resources_hard.txt");
        wp_h = load_weapons("txtfiles/galician/weapons_hard.txt");
        obj_h = load_objects("txtfiles/galician/objects_hard.txt");
        pl_h = load_player("txtfiles/galician/player_hard.txt");
        e_h = load_enemies("txtfiles/galician/enemies_hard.txt", &n_ene_h);
    	s_h = load_strings("txtfiles/galician/strings_gal.txt");
    }
    else{
    	printf("Error. Hard 7.\n");
    	exit(ERROR);
    }
    
    if(intrf_h==NULL || r_h==NULL || wp_h==NULL || obj_h==NULL || pl_h==NULL || e_h==NULL || s_h==NULL ||n_ene_h==-1){
        printf("Error. Hard 8.\n");
        exit(ERROR);
    }
    
}

int walking_simulator_hard(int map_id){
    int c = 0;
    Maps *copymap=NULL;
    Resources *hung=NULL, *hydra=NULL;
    rectangle *battle=NULL;
    
    copymap = map_getCopy(intrf_h->maps_array, map_id);
    if(copymap==NULL){
        printf("Error. Walking_simulator_hard 1.\n");
        exit(ERROR);
    }
    
    hung = resources_getResource(r_h, FOOD);
    hydra = resources_getResource(r_h, DRINK);
    if(hung==NULL || hydra==NULL){
        printf("Error. Main-F1-4.\n");
        exit(ERROR);
    }
    battle = win_find_rectangle(RECT_BATTLE, intrf_h->rect_array);
    if(battle==NULL){
        printf("Error. Main-F1-5.\n");
    }
    
    while(1){
        c = read_key_hard();
        
        if(c==-UP || c==-DOWN || c==-RIGHT || c==-LEFT){
            hydration_h++;
            hunger_h++;
            if(hydration_h == 20){
                hydration_h = 0;
                if(modify_resource(hydra, -1)<=0){
                    print_resources(intrf_h, r_h);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s_h)); /* You are dead becuase of no hydration_h */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf_h, r_h);
            }
            if(hunger_h == 40){
                hunger_h = 0;
                if(modify_resource(hung, -1)<=0){
                    print_resources(intrf_h, r_h);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s_h)); /* You are dead becuase of hunger_h */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf_h, r_h);
            }
            if(move(intrf_h, copymap, pl_h, -c) == DOOR){
                return DOOR;
            }
        }
        else if(c == 'o') return 0;
        else if(c == 'p') return DOOR;
        else if(c == '1'){
            use_object(r_h, obj_h[0]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '2'){
            use_object(r_h, obj_h[1]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '3'){
            use_object(r_h, obj_h[2]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '4'){
            use_object(r_h, obj_h[3]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '5'){
            use_object(r_h, obj_h[4]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '6'){
            use_object(r_h, obj_h[5]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '7'){
            use_object(r_h, obj_h[6]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '8'){
            use_object(r_h, obj_h[7]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
    }
}

int dir_conv_hard(int d) {
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

void read_space_hard(){
    char c = 0;
    while(1){
        c = read_key_hard();
        if(c != ' '){
            continue;
        }
        return;
    }
}

void battle_info_hard(rectangle *info){
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s_h));
    win_write_line_slow_at(info, 3, 3, strings_get_string_by_type(9993, s_h));
    win_write_line_slow_at(info, 4, 3, strings_get_string_by_type(9994, s_h));
    win_write_line_slow_at(info, 5, 3, strings_get_string_by_type(9995, s_h));
    return;
}

void town_info_hard(rectangle *info){
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s_h));
    win_write_line_slow_at(info, 3, 3, strings_get_string_by_type(9995, s_h));
}

int battlemode_hard(int *ene_array, Maps *copymap){
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
    if(n_ene_h == -1){
        printf("Error. Main-F1-2.\n");
        exit(ERROR);  
    }
    if(copymap == NULL){
        printf("Error. Main-F1-3.\n");
        exit(ERROR);
    }
    
    hung = resources_getResource(r_h, FOOD);
    hydra = resources_getResource(r_h, DRINK);
    if(hung==NULL || hydra==NULL){
        printf("Error. Main-F1-4.\n");
        exit(ERROR);
    }
    battle = win_find_rectangle(RECT_BATTLE, intrf_h->rect_array);
    if(battle==NULL){
        printf("Error. Main-F1-5.\n");
    }
    
    ene = generate_arrayEnemies(e_h, ene_array, n_ene_h);
    if(ene==NULL) exit(12345);
    
    generate_EnePosRand(ene, copymap);
    
    print_enemies(intrf_h, ene);
    
    copymap->field[player_getRow(pl_h) - 2][player_getCol(pl_h) - 2] = player_getDisplay(pl_h); /* Inserting the player display into the map*/
    

    for(econt = ene; *(econt)!=NULL; econt++){
        mest = (moveEne_stuff *) malloc(sizeof(moveEne_stuff)); /* When are we freeing this ??? */
        mest->intrf = intrf_h;
        mest->pl = pl_h;
        mest->r = r_h;
        mest->copymap = copymap;
        mest->ene = *(econt);
        pthread_create(&pth_moveEne, NULL, move_enemies, (void *) mest);
    }

    while(1){
        c = read_key_hard();
        if(c == 'o'){
            tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
            break;
        }
        if(c == 'p'){
            kill_all_enemies(ene);
            destroy_enemies(ene);
            return DOOR;
        }
        
        
        else if(c < 0){ /* MOVING */
            hydration_h++;
            hunger_h++;
            if(hydration_h == 20){
                hydration_h = 0;
                if(modify_resource(hydra, -1)<=0){
                    print_resources(intrf_h, r_h);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s_h)); /* You are dead becuase of no hydration_h */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf_h, r_h);
            }
            if(hunger_h == 40){
                hunger_h = 0;
                if(modify_resource(hung, -1)<=0){
                    print_resources(intrf_h, r_h);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s_h)); /* You are dead becuase of hunger_h */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf_h, r_h);
            }
            if(move(intrf_h, copymap, pl_h, -c)==DOOR && enemy_checkPhyStat(ene)==ALL_KILLED){
                destroy_enemies(ene);
                return DOOR;
            }
        }
        
        else if(c == 'q'){ /* Swaping weapon */
            change_equipped(wp_h, UPW);
            print_weapons(intrf_h, wp_h);
        }
        else if(c == 'e'){ /* Swaping weapon */
            change_equipped(wp_h, DOWNW);
            print_weapons(intrf_h, wp_h);
        }
        /* Using objects */
        else if(c == '1'){
            use_object(r_h, obj_h[0]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '2'){
            use_object(r_h, obj_h[1]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '3'){
            use_object(r_h, obj_h[2]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '4'){
            use_object(r_h, obj_h[3]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '5'){
            use_object(r_h, obj_h[4]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '6'){
            use_object(r_h, obj_h[5]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '7'){
            use_object(r_h, obj_h[6]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        else if(c == '8'){
            use_object(r_h, obj_h[7]);
            print_objects(intrf_h, obj_h);
            print_resources(intrf_h, r_h);
        }
        /* Shooting */
        else if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
            stst = (shoot_stuff *) malloc(sizeof(shoot_stuff));
            stst->intrf = intrf_h;
            stst->wp = wp_h;
            stst->pl = pl_h;
            stst->r = r_h;
            stst->copymap = copymap;
            stst->dir = dir_conv_hard(c);
            stst->ene = ene;
            pthread_create(&pth_shoot, NULL, shoot, (void *) stst);
            
            hydration_h++;
            hunger_h++;
            if(hydration_h == 20){
                hydration_h = 0;
                if(modify_resource(hydra, -1)<=0){
                    print_resources(intrf_h, r_h);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s_h)); /* You are dead becuase of no hydration_h */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf_h, r_h);
            }
            if(hunger_h == 40){
                hunger_h = 0;
                if(modify_resource(hung, -1)<=0){
                    print_resources(intrf_h, r_h);
                    win_clear(battle);
                    win_write_line_at(battle, copymap->n_rows / 2, copymap->n_cols / 2, strings_get_string_by_type(700, s_h)); /* You are dead becuase of hunger_h */
                    tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
                    exit(EXIT_SUCCESS);
                }
                print_resources(intrf_h, r_h);
            }
        }
    }
    destroy_enemies(ene);
    return 0;
}


void game_hard(int lang){
    srand(time(NULL));
    
    game_init_hard(lang);
    
    _term_init_hard();
    
    rectangle* story = NULL;
    rectangle* info = NULL;
    rectangle* battle = NULL;
    story = win_find_rectangle(RECT_STORY, intrf_h->rect_array);
    info = win_find_rectangle(RECT_INFO, intrf_h->rect_array);
    battle = win_find_rectangle(RECT_BATTLE, intrf_h->rect_array);
    
    
    if(initialize_intrf(intrf_h, 120, r_h, wp_h, obj_h, s_h) == FAILED){
        printf("Error. Hard 8.\n");
        exit(ERROR);
    }
    win_write_line_at(battle, 18, get_mid_col_hard(battle, strlen(strings_get_string_by_type(10, s_h))), strings_get_string_by_type(10, s_h));
    
    
    /* TUTORIAL */
    int enemies[7] = {2,1,1,1,0,0,0};
    char* cont = NULL;
    
    story = win_find_rectangle(RECT_STORY, intrf_h->rect_array);
    info = win_find_rectangle(RECT_INFO, intrf_h->rect_array);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(11, s_h));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(12, s_h));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(13, s_h));
    win_write_line_slow_at(story, 5, 3, strings_get_string_by_type(14, s_h));
    win_write_line_slow_at(story, 6, 3, strings_get_string_by_type(15, s_h));
    win_write_line_slow_at(story, 7, 3, strings_get_string_by_type(16, s_h));
    win_write_line_slow_at(story, 8, 3, strings_get_string_by_type(17, s_h));
    win_write_line_slow_at(story, 9, 3, strings_get_string_by_type(18, s_h));
    win_write_line_slow_at(story, 10, 3, strings_get_string_by_type(19, s_h));
    cont = strings_get_string_by_type(9991, s_h);
    win_write_line_slow_at(story, 11, get_mid_col_hard(story, strlen(cont)), cont);
    battle_info_hard(info);
    read_space_hard();
    print_map(intrf_h, 300);
    print_player(intrf_h, pl_h);
    
    Maps *copymap=NULL;
    copymap = map_getCopy(intrf_h->maps_array, 300);
    if(battlemode_hard(enemies, copymap) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    print_map(intrf_h, 0);
    
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(20, s_h));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(21, s_h));
    win_write_line_slow_at(story, 4, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(22, s_h));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(23, s_h));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(24, s_h));
    win_write_line_slow_at(story, 5, get_mid_col_hard(story, strlen(cont)), cont);
    town_info_hard(info);
    read_space_hard();
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(999, s_h));
    win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    win_clear(story);
    win_clear(info);
    /* STORY STARTS */
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1000, s_h));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1001, s_h));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1002, s_h));
    win_write_line_slow_at(story, 5, 3, strings_get_string_by_type(1003, s_h));
    win_write_line_slow_at(story, 6, 3, strings_get_string_by_type(1004, s_h));
    win_write_line_slow_at(story, 7, 3, strings_get_string_by_type(1005, s_h));
    win_write_line_slow_at(story, 8, 3, strings_get_string_by_type(1006, s_h));
    win_write_line_slow_at(story, 9, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    win_clear(story);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1007, s_h));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1008, s_h));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1009, s_h));
    win_write_line_slow_at(story, 5, 3, strings_get_string_by_type(1010, s_h));
    win_write_line_slow_at(story, 6, 3, strings_get_string_by_type(1011, s_h));
    win_write_line_slow_at(story, 7, 3, strings_get_string_by_type(1012, s_h));
    win_write_line_slow_at(story, 8, 3, strings_get_string_by_type(1013, s_h));
    win_write_line_slow_at(story, 9, 3, strings_get_string_by_type(1014, s_h));
    win_write_line_slow_at(story, 10, 3, strings_get_string_by_type(1015, s_h));
    win_write_line_slow_at(story, 11, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    win_clear(battle);
    win_clear(story);
    /* FIRST BATTLE */
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1016, s_h));
    
    battle_info_hard(info);
    player_setLocation(pl_h, 31, 2);
    print_map(intrf_h, 31);
    print_player(intrf_h, pl_h);
    
    Maps *copymap2=NULL;
    int enemies2[7] = {3,2,2,2,0,0,0};
    copymap2 = map_getCopy(intrf_h->maps_array, 31);
    if(battlemode_hard(enemies2, copymap2) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    change_own(wp_h[1]); /* Shitty_gun AVAILABLE */
    print_weapons(intrf_h, wp_h);
    
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1017, s_h));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1018, s_h));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1019, s_h));
    win_write_line_slow_at(story, 5, get_mid_col_hard(story, strlen(cont)), cont);
    town_info_hard(info);
    read_space_hard();
    print_map(intrf_h, 11);
    player_setLocation(pl_h, 31, 2);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(11) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1020, s_h));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1021, s_h));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1022, s_h));
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s_h));
    
    int score;
    
    score = Not_Not(intrf_h, s_h, lang);
    win_clear(story);
    win_write_line_slow_at(story, 2, get_mid_col_hard(story, strlen(strings_get_string_by_type(2999, s_h))), strings_get_string_by_type(2999, s_h));
    if(score < 1){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[2], 3); /* +Three water */
        object_changeAmount(obj_h[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3000, s_h))), strings_get_string_by_type(3000, s_h));
    }else if(score <3){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[2], 4); /* +Four water */
        object_changeAmount(obj_h[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3001, s_h))), strings_get_string_by_type(3001, s_h));
    }else if (score < 5){
        object_changeAmount(obj_h[0], 4); /* +Four bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[3], 6); /* +Six bullets */
        object_changeAmount(obj_h[2], 6); /* +Six water */ 
        object_changeAmount(obj_h[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3002, s_h))), strings_get_string_by_type(3002, s_h));
    }else if (score < 8){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3003, s_h))), strings_get_string_by_type(3003, s_h));
    }else if(score < 10){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 3); /* +Three caviar */
        object_changeAmount(obj_h[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3004, s_h))), strings_get_string_by_type(3004, s_h));
    }else if (score >= 10){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 8); /* +Eight caviar */
        object_changeAmount(obj_h[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3005, s_h))), strings_get_string_by_type(3005, s_h));
    }
    win_write_line_slow_at(story, 4, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    win_clear(story);
    print_objects(intrf_h, obj_h);
    
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1023, s_h));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1024, s_h));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1025, s_h));
    
    win_clear(battle);
    
    town_info_hard(info);
    print_map(intrf_h, 1);
    player_setLocation(pl_h, 20, 2);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(1) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    battle_info_hard(info);
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1026, s_h))), strings_get_string_by_type(1026, s_h));
    player_setLocation(pl_h, 35, 44);
    print_map(intrf_h, 32);
    print_player(intrf_h, pl_h);
    
    Maps *copymap3=NULL;
    int enemies3[7] = {4,2,2,2,1,0,0};
    copymap3 = map_getCopy(intrf_h->maps_array, 32);
    if(battlemode_hard(enemies3, copymap3) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1027, s_h))), strings_get_string_by_type(1027, s_h));
    
    print_map(intrf_h, 21); /* MAZE 1 */
    town_info_hard(info);
    player_setLocation(pl_h, 3, 2);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(21) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1028, s_h))), strings_get_string_by_type(1028, s_h));
    
    print_map(intrf_h, 6);
    player_setLocation(pl_h, 3, 37);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(6) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_hard(story)-2, get_mid_col_hard(story, strlen(strings_get_string_by_type(1029, s_h)))+2, strings_get_string_by_type(1029, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story)-1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1030, s_h))), strings_get_string_by_type(1030, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1031, s_h))), strings_get_string_by_type(1031, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story)+1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1032, s_h))), strings_get_string_by_type(1032, s_h));
    
    print_map(intrf_h, 4);
    player_setLocation(pl_h, 16, 43);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(4) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1033, s_h))), strings_get_string_by_type(1033, s_h));
    
    print_map(intrf_h, 2);
    player_setLocation(pl_h, 31, 39);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(2) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1047, s_h))), strings_get_string_by_type(1047, s_h));
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s_h));
    
    score = pong(intrf_h, s_h, lang);
    win_clear(story);
    win_clear(info);
    
    char *aux=NULL;
    char *aux2=NULL;
    
    aux = (char *) malloc(sizeof(int)+2);
    if(aux==NULL){
        printf("Error. Hard 9.\n");
        exit(ERROR);
    }
    sprintf(aux, " %d", score);
    aux2 = (char *) malloc(strlen(aux)+strlen(strings_get_string_by_type(3006, s_h)+2));
    if(aux2==NULL){
        printf("Error. Hard 10.\n");
        exit(ERROR);
    }
    strcpy(aux2, strings_get_string_by_type(3006, s_h));
    strcat(aux2, aux);
    
    win_write_line_slow_at(story, 2, get_mid_col_hard(story, strlen(strings_get_string_by_type(2999, s_h))), strings_get_string_by_type(2999, s_h));
    if(score < 5){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[2], 3); /* +Three water */
        object_changeAmount(obj_h[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3000, s_h))), strings_get_string_by_type(3000, s_h));
    }else if(score < 15){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[2], 4); /* +Four water */
        object_changeAmount(obj_h[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3001, s_h))), strings_get_string_by_type(3001, s_h));
    }else if (score < 25){
        object_changeAmount(obj_h[0], 4); /* +Four bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[3], 6); /* +Six bullets */
        object_changeAmount(obj_h[2], 6); /* +Six water */ 
        object_changeAmount(obj_h[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3002, s_h))), strings_get_string_by_type(3002, s_h));
    }else if (score < 35){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3003, s_h))), strings_get_string_by_type(3003, s_h));
    }else if(score < 45){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 3); /* +Three caviar */
        object_changeAmount(obj_h[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3004, s_h))), strings_get_string_by_type(3004, s_h));
    }else if (score >= 55){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 8); /* +Eight caviar */
        object_changeAmount(obj_h[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3005, s_h))), strings_get_string_by_type(3005, s_h));
    }
    win_write_line_slow_at(story, 4, get_mid_col_hard(story, strlen(cont)), cont);
    print_objects(intrf_h, obj_h);
    sleep(1);
    win_clear(battle);
    
    win_write_line_slow_at(battle, get_mid_row_hard(battle), get_mid_col_hard(battle, strlen(aux2)), aux2);
    free(aux);
    free(aux2);
    
    read_space_hard();
    win_clear(battle);
    win_clear(story);
    
    change_own(wp_h[2]);
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1037, s_h))), strings_get_string_by_type(1037, s_h));
    print_weapons(intrf_h, wp_h);
    town_info_hard(info);
    
    print_map(intrf_h, 12);
    player_setLocation(pl_h, 18, 2);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(12) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1027, s_h))), strings_get_string_by_type(1027, s_h));
    
    print_map(intrf_h, 22); /* MAZE 2 */
    player_setLocation(pl_h, 3, 2);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(22) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    battle_info_hard(info);
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1038, s_h))), strings_get_string_by_type(1038, s_h));
    player_setLocation(pl_h, 18, 40);
    print_map(intrf_h, 34);
    print_player(intrf_h, pl_h);
    
    Maps *copymap4=NULL;
    int enemies4[7] = {4,2,2,2,2,0,0};
    copymap4 = map_getCopy(intrf_h->maps_array, 34);
    if(battlemode_hard(enemies4, copymap4) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1027, s_h))), strings_get_string_by_type(1027, s_h));
    town_info_hard(info);
    
    print_map(intrf_h, 23); /* MAZE 3 */
    player_setLocation(pl_h, 3, 2);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(23) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    win_write_line_slow_at(info, get_mid_row_hard(info), get_mid_col_hard(info, strlen(strings_get_string_by_type(2020, s_h))), strings_get_string_by_type(2020, s_h));
    
    score = hangman(intrf_h, s_h, lang);
    win_clear(story);
    win_write_line_slow_at(story, 2, get_mid_col_hard(story, strlen(strings_get_string_by_type(2999, s_h))), strings_get_string_by_type(2999, s_h));
    if(score <= -2){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[2], 3); /* +Three water */
        object_changeAmount(obj_h[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3000, s_h))), strings_get_string_by_type(3000, s_h));
    }else if(score <= 0){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[2], 4); /* +Four water */
        object_changeAmount(obj_h[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3001, s_h))), strings_get_string_by_type(3001, s_h));
    }else if (score <= 2){
        object_changeAmount(obj_h[0], 4); /* +Four bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[3], 6); /* +Six bullets */
        object_changeAmount(obj_h[2], 6); /* +Six water */ 
        object_changeAmount(obj_h[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3002, s_h))), strings_get_string_by_type(3002, s_h));
    }else if (score <= 6){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3003, s_h))), strings_get_string_by_type(3003, s_h));
    }else if(score <12){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 3); /* +Three caviar */
        object_changeAmount(obj_h[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3004, s_h))), strings_get_string_by_type(3004, s_h));
    }else if (score >= 12){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 8); /* +Eight caviar */
        object_changeAmount(obj_h[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3005, s_h))), strings_get_string_by_type(3005, s_h));
    }
    win_write_line_slow_at(story, 4, get_mid_col_hard(story, strlen(cont)), cont);
    print_objects(intrf_h, obj_h);
    read_space_hard();
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1040, s_h))), strings_get_string_by_type(1040, s_h));
    town_info_hard(info);
    
    print_map(intrf_h, 6);
    player_setLocation(pl_h, 3, 37);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(6) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_hard(story)-2, get_mid_col_hard(story, strlen(strings_get_string_by_type(1041, s_h)))+2, strings_get_string_by_type(1041, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story)-1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1042, s_h))), strings_get_string_by_type(1042, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1043, s_h))), strings_get_string_by_type(1043, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story)+1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1044, s_h))), strings_get_string_by_type(1044, s_h));
    change_own(wp_h[3]);
    print_weapons(intrf_h, wp_h);
    
    print_map(intrf_h, 4);
    player_setLocation(pl_h, 16, 43);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(4) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(10445, s_h))), strings_get_string_by_type(10445, s_h));
    
    print_map(intrf_h, 3);
    player_setLocation(pl_h, 31, 38);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(3) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1045, s_h))), strings_get_string_by_type(1045, s_h));
    
    print_map(intrf_h, 11);
    player_setLocation(pl_h, 31, 2);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(11) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    battle_info_hard(info);
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1026, s_h))), strings_get_string_by_type(1026, s_h));
    player_setLocation(pl_h, 4, 4);
    print_map(intrf_h, 35);
    print_player(intrf_h, pl_h);
    
    Maps *copymap5=NULL;
    int enemies5[7] = {4,3,3,3,2,0,0};
    copymap5 = map_getCopy(intrf_h->maps_array, 35);
    if(battlemode_hard(enemies5, copymap5) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    win_write_line_slow_at(info, get_mid_row_hard(info), get_mid_col_hard(info, strlen(strings_get_string_by_type(1046, s_h))), strings_get_string_by_type(1046, s_h));
    
    score = Increasing_Bar(intrf_h, s_h, lang);
    win_clear(story);
    win_write_line_slow_at(story, 2, get_mid_col_hard(story, strlen(strings_get_string_by_type(2999, s_h))), strings_get_string_by_type(2999, s_h));
    win_clear(battle);
    if(score <= 15){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[2], 3); /* +Three water */
        object_changeAmount(obj_h[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3000, s_h))), strings_get_string_by_type(3000, s_h));
    }else if(score <= 30){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[2], 4); /* +Four water */
        object_changeAmount(obj_h[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3001, s_h))), strings_get_string_by_type(3001, s_h));
    }else if (score <= 45){
        object_changeAmount(obj_h[0], 4); /* +Four bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[3], 6); /* +Six bullets */
        object_changeAmount(obj_h[2], 6); /* +Six water */ 
        object_changeAmount(obj_h[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3002, s_h))), strings_get_string_by_type(3002, s_h));
    }else if (score <= 60){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3003, s_h))), strings_get_string_by_type(3003, s_h));
    }else if(score <= 75){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 3); /* +Three caviar */
        object_changeAmount(obj_h[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3004, s_h))), strings_get_string_by_type(3004, s_h));
    }else if (score >= 75){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 8); /* +Eight caviar */
        object_changeAmount(obj_h[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3005, s_h))), strings_get_string_by_type(3005, s_h));
    }
    
    win_write_line_slow_at(story, 4, get_mid_col_hard(story, strlen(strings_get_string_by_type(3007, s_h))), strings_get_string_by_type(3007, s_h));
    change_own(wp_h[4]); /* Giving the machine Gun */
    print_weapons(intrf_h, wp_h);
    
    win_write_line_slow_at(story, 6, get_mid_col_hard(story, strlen(cont)), cont);
    print_objects(intrf_h, obj_h);
    read_space_hard();
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1027, s_h))), strings_get_string_by_type(1027, s_h));
    town_info_hard(info);
    
    print_map(intrf_h, 24); /* MAZE 4 */
    player_setLocation(pl_h, 3, 2);
    print_player(intrf_h, pl_h);
    if(walking_simulator_hard(24) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    
    battle_info_hard(info);
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1026, s_h))), strings_get_string_by_type(1026, s_h));
    player_setLocation(pl_h, 4, 4);
    print_map(intrf_h, 36);
    print_player(intrf_h, pl_h);
    
    Maps *copymap6=NULL;
    int enemies6[7] = {4,3,3,3,2,1,0};
    copymap6 = map_getCopy(intrf_h->maps_array, 36);
    if(battlemode_hard(enemies6, copymap6) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_hard(story)-1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1034, s_h))), strings_get_string_by_type(1034, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1035, s_h))), strings_get_string_by_type(1035, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story)+1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1036, s_h))), strings_get_string_by_type(1036, s_h));
    
    win_write_line_slow_at(info, get_mid_row_hard(info), get_mid_col_hard(info, strlen(strings_get_string_by_type(2021, s_h))), strings_get_string_by_type(2021, s_h));
    win_write_line_slow_at(info, get_mid_row_hard(info)+1, get_mid_col_hard(info, strlen(strings_get_string_by_type(2022, s_h))), strings_get_string_by_type(2022, s_h));
    
    score = g_21(intrf_h);
    win_clear(story);
    win_write_line_slow_at(story, 2, get_mid_col_hard(story, strlen(strings_get_string_by_type(2999, s_h))), strings_get_string_by_type(2999, s_h));
    win_clear(battle);
    if(score <= 6){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[2], 3); /* +Three water */
        object_changeAmount(obj_h[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3000, s_h))), strings_get_string_by_type(3000, s_h));
    }else if(score <= 9){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[2], 4); /* +Four water */
        object_changeAmount(obj_h[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3001, s_h))), strings_get_string_by_type(3001, s_h));
    }else if (score <= 12){
        object_changeAmount(obj_h[0], 4); /* +Four bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[3], 6); /* +Six bullets */
        object_changeAmount(obj_h[2], 6); /* +Six water */ 
        object_changeAmount(obj_h[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3002, s_h))), strings_get_string_by_type(3002, s_h));
    }else if (score <= 15){
        object_changeAmount(obj_h[0], 1); /* +One bandage */
        object_changeAmount(obj_h[1], 2); /* +Two fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3003, s_h))), strings_get_string_by_type(3003, s_h));
    }else if(score <= 18){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 3); /* +Three caviar */
        object_changeAmount(obj_h[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3004, s_h))), strings_get_string_by_type(3004, s_h));
    }else if (score >= 18){
        object_changeAmount(obj_h[0], 2); /* +Two bandages */
        object_changeAmount(obj_h[1], 3); /* +Three fruits */
        object_changeAmount(obj_h[4], 4); /* +Four medical kits */
        object_changeAmount(obj_h[3], 8); /* +Eight bullets */
        object_changeAmount(obj_h[2], 8); /* +Eight water */
        object_changeAmount(obj_h[6], 8); /* +Eight alcohol */
        object_changeAmount(obj_h[5], 8); /* +Eight caviar */
        object_changeAmount(obj_h[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(3005, s_h))), strings_get_string_by_type(3005, s_h));
    }
    win_write_line_slow_at(story, 4, get_mid_col_hard(story, strlen(cont)), cont);
    print_objects(intrf_h, obj_h);
    read_space_hard();
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_hard(story)-1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1067, s_h))), strings_get_string_by_type(1067, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1068, s_h))), strings_get_string_by_type(1068, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story)+1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1069, s_h))), strings_get_string_by_type(1069, s_h));
    
    print_map(intrf_h, 12);
    player_setLocation(pl_h, 18, 3);
    print_player(intrf_h, pl_h);
    win_write_line_slow_at(story, 7, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    change_own(wp_h[5]); /* Giving the Bazooka */
    print_weapons(intrf_h, wp_h);
    
    town_info_hard(info);
    if(walking_simulator_hard(12) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    usleep(300000);
    
    win_write_line_slow_at(story, get_mid_row_hard(story)-1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1050, s_h))), strings_get_string_by_type(1050, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1051, s_h))), strings_get_string_by_type(1051, s_h));
    
    print_map(intrf_h, 500);
    
    win_write_line_slow_at(story, 7, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    battle_info_hard(info);
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1026, s_h))), strings_get_string_by_type(1026, s_h));
    player_setLocation(pl_h, 18, 45);
    print_map(intrf_h, 501);
    print_player(intrf_h, pl_h);
    
    Maps *copymap7=NULL;
    int enemies7[7] = {4,3,3,3,3,2,1};
    copymap7 = map_getCopy(intrf_h->maps_array, 501);
    if(battlemode_hard(enemies7, copymap7) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    print_map(intrf_h, 502);
    
    win_write_line_slow_at(story, get_mid_row_hard(story)-1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1053, s_h))), strings_get_string_by_type(1053, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1054, s_h))), strings_get_string_by_type(1054, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story)+1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1055, s_h))), strings_get_string_by_type(1055, s_h));
    win_write_line_slow_at(story, 7, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    
    win_clear(story);
    win_write_line_slow_at(story, get_mid_row_hard(story)-1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1056, s_h))), strings_get_string_by_type(1056, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1057, s_h))), strings_get_string_by_type(1057, s_h));
    win_write_line_slow_at(story, get_mid_row_hard(story)+1, get_mid_col_hard(story, strlen(strings_get_string_by_type(1058, s_h))), strings_get_string_by_type(1058, s_h));
    win_write_line_slow_at(story, 7, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_hard(story), get_mid_col_hard(story, strlen(strings_get_string_by_type(1059, s_h))), strings_get_string_by_type(1059, s_h));
    win_write_line_slow_at(story, 6, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    print_map(intrf_h, 99);
    win_write_line_slow_at(story, 2, get_mid_col_hard(story, strlen(strings_get_string_by_type(1060, s_h))), strings_get_string_by_type(1060, s_h));
    win_write_line_slow_at(story, 3, get_mid_col_hard(story, strlen(strings_get_string_by_type(1061, s_h))), strings_get_string_by_type(1061, s_h));
    win_write_line_slow_at(story, 4, get_mid_col_hard(story, strlen(strings_get_string_by_type(1062, s_h))), strings_get_string_by_type(1062, s_h));
    win_write_line_slow_at(story, 5, get_mid_col_hard(story, strlen(strings_get_string_by_type(1063, s_h))), strings_get_string_by_type(1063, s_h));
    win_write_line_slow_at(story, 6, get_mid_col_hard(story, strlen(strings_get_string_by_type(1064, s_h))), strings_get_string_by_type(1064, s_h));
    win_write_line_slow_at(story, 7, get_mid_col_hard(story, strlen(strings_get_string_by_type(1065, s_h))), strings_get_string_by_type(1065, s_h));
    win_write_line_slow_at(story, 8, get_mid_col_hard(story, strlen(strings_get_string_by_type(1066, s_h))), strings_get_string_by_type(1066, s_h));
    win_write_line_slow_at(story, 9, get_mid_col_hard(story, strlen(cont)), cont);
    read_space_hard();
    
    
    tcsetattr(fileno(stdin), TCSANOW, &initial_hard);
    return;
}