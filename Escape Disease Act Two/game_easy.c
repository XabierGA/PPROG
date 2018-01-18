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

int get_mid_col_easy(rectangle* r, int len){
    return (((r->last_col-r->ini_col)/2)-(len/2)+1);
}

int get_mid_row_easy(rectangle* r){
    return ((r->last_row-r->ini_row)/2);
}

void game_init_easy(int lang){
    
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

int walking_simulator_easy(int map_id){
    int c = 0;
    Maps *copymap=NULL;
    Resources *hung=NULL, *hydra=NULL;
    rectangle *battle=NULL;
    
    copymap = map_getCopy(intrf->maps_array, map_id);
    if(copymap==NULL){
        printf("Error. Walking_simulator_easy 1.\n");
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
    
    while(1){
        c = read_key_easy();
        
        if(c==-UP || c==-DOWN || c==-RIGHT || c==-LEFT){
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
            if(move(intrf, copymap, pl, -c) == DOOR){
                return DOOR;
            }
        }
        else if(c == 'o') return 0;
        else if(c == 'p') return DOOR;
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

void read_space_easy(){
    char c = 0;
    while(1){
        c = read_key_easy();
        if(c != ' '){
            continue;
        }
        return;
    }
}

void battle_info_easy(rectangle *info){
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s));
    win_write_line_slow_at(info, 3, 3, strings_get_string_by_type(9993, s));
    win_write_line_slow_at(info, 4, 3, strings_get_string_by_type(9994, s));
    win_write_line_slow_at(info, 5, 3, strings_get_string_by_type(9995, s));
    return;
}

void town_info_easy(rectangle *info){
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s));
    win_write_line_slow_at(info, 3, 3, strings_get_string_by_type(9995, s));
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
        if(c == 'p'){
            kill_all_enemies(ene);
            destroy_enemies(ene);
            return DOOR;
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
    
    game_init_easy(lang);
    
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
    win_write_line_at(battle, 18, get_mid_col_easy(battle, strlen(strings_get_string_by_type(7, s))), strings_get_string_by_type(7, s));
    
    
    /* TUTORIAL */
    int enemies[7] = {2,0,0,0,0,0,0};
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
    win_write_line_slow_at(story, 11, get_mid_col_easy(story, strlen(cont)), cont);
    battle_info_easy(info);
    read_space_easy();
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
    win_write_line_slow_at(story, 4, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(22, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(23, s));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(24, s));
    win_write_line_slow_at(story, 5, get_mid_col_easy(story, strlen(cont)), cont);
    town_info_easy(info);
    read_space_easy();
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(999, s));
    win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
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
    win_write_line_slow_at(story, 9, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
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
    win_write_line_slow_at(story, 11, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    win_clear(battle);
    win_clear(story);
    /* FIRST BATTLE */
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1016, s));
    
    battle_info_easy(info);
    player_setLocation(pl, 31, 2);
    print_map(intrf, 31);
    print_player(intrf, pl);
    
    Maps *copymap2=NULL;
    int enemies2[7] = {1,1,1,1,0,0,0};
    copymap2 = map_getCopy(intrf->maps_array, 31);
    if(battlemode_easy(enemies2, copymap2) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    change_own(wp[1]); /* Shitty_gun AVAILABLE */
    print_weapons(intrf, wp);
    
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1017, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1018, s));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1019, s));
    win_write_line_slow_at(story, 5, get_mid_col_easy(story, strlen(cont)), cont);
    town_info_easy(info);
    read_space_easy();
    print_map(intrf, 11);
    player_setLocation(pl, 31, 2);
    print_player(intrf, pl);
    if(walking_simulator_easy(11) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1020, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1021, s));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1022, s));
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s));
    
    int score;
    
    score = Not_Not(intrf, s, lang);
    win_clear(story);
    win_write_line_slow_at(story, 2, get_mid_col_easy(story, strlen(strings_get_string_by_type(2999, s))), strings_get_string_by_type(2999, s));
    if(score < 1){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[2], 3); /* +Three water */
        object_changeAmount(obj[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3000, s))), strings_get_string_by_type(3000, s));
    }else if(score <3){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[2], 4); /* +Four water */
        object_changeAmount(obj[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3001, s))), strings_get_string_by_type(3001, s));
    }else if (score < 5){
        object_changeAmount(obj[0], 4); /* +Four bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[3], 6); /* +Six bullets */
        object_changeAmount(obj[2], 6); /* +Six water */ 
        object_changeAmount(obj[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3002, s))), strings_get_string_by_type(3002, s));
    }else if (score < 8){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3003, s))), strings_get_string_by_type(3003, s));
    }else if(score < 10){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 3); /* +Three caviar */
        object_changeAmount(obj[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3004, s))), strings_get_string_by_type(3004, s));
    }else if (score >= 10){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 8); /* +Eight caviar */
        object_changeAmount(obj[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3005, s))), strings_get_string_by_type(3005, s));
    }
    win_write_line_slow_at(story, 4, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    win_clear(story);
    print_objects(intrf, obj);
    
    win_write_line_slow_at(story, 2, 3, strings_get_string_by_type(1023, s));
    win_write_line_slow_at(story, 3, 3, strings_get_string_by_type(1024, s));
    win_write_line_slow_at(story, 4, 3, strings_get_string_by_type(1025, s));
    
    win_clear(battle);
    
    town_info_easy(info);
    print_map(intrf, 1);
    player_setLocation(pl, 20, 2);
    print_player(intrf, pl);
    if(walking_simulator_easy(1) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    battle_info_easy(info);
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1026, s))), strings_get_string_by_type(1026, s));
    player_setLocation(pl, 35, 44);
    print_map(intrf, 32);
    print_player(intrf, pl);
    
    Maps *copymap3=NULL;
    int enemies3[7] = {2,1,1,1,0,0,0};
    copymap3 = map_getCopy(intrf->maps_array, 32);
    if(battlemode_easy(enemies3, copymap3) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1027, s))), strings_get_string_by_type(1027, s));
    
    print_map(intrf, 21); /* MAZE 1 */
    town_info_easy(info);
    player_setLocation(pl, 3, 2);
    print_player(intrf, pl);
    if(walking_simulator_easy(21) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1028, s))), strings_get_string_by_type(1028, s));
    
    print_map(intrf, 6);
    player_setLocation(pl, 3, 37);
    print_player(intrf, pl);
    if(walking_simulator_easy(6) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_easy(story)-2, get_mid_col_easy(story, strlen(strings_get_string_by_type(1029, s)))+2, strings_get_string_by_type(1029, s));
    win_write_line_slow_at(story, get_mid_row_easy(story)-1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1030, s))), strings_get_string_by_type(1030, s));
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1031, s))), strings_get_string_by_type(1031, s));
    win_write_line_slow_at(story, get_mid_row_easy(story)+1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1032, s))), strings_get_string_by_type(1032, s));
    
    print_map(intrf, 4);
    player_setLocation(pl, 16, 43);
    print_player(intrf, pl);
    if(walking_simulator_easy(4) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1033, s))), strings_get_string_by_type(1033, s));
    
    print_map(intrf, 2);
    player_setLocation(pl, 31, 39);
    print_player(intrf, pl);
    if(walking_simulator_easy(2) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1047, s))), strings_get_string_by_type(1047, s));
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s));
    
    score = pong(intrf, s, lang);
    win_clear(story);
    win_clear(info);
    
    char *aux=NULL;
    char *aux2=NULL;
    
    aux = (char *) malloc(sizeof(int)+2);
    if(aux==NULL){
        printf("Error. Easy 9.\n");
        exit(ERROR);
    }
    sprintf(aux, " %d", score);
    aux2 = (char *) malloc(strlen(aux)+strlen(strings_get_string_by_type(3006, s)+2));
    if(aux2==NULL){
        printf("Error. Easy 10.\n");
        exit(ERROR);
    }
    strcpy(aux2, strings_get_string_by_type(3006, s));
    strcat(aux2, aux);
    
    win_write_line_slow_at(story, 2, get_mid_col_easy(story, strlen(strings_get_string_by_type(2999, s))), strings_get_string_by_type(2999, s));
    if(score < 5){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[2], 3); /* +Three water */
        object_changeAmount(obj[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3000, s))), strings_get_string_by_type(3000, s));
    }else if(score < 15){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[2], 4); /* +Four water */
        object_changeAmount(obj[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3001, s))), strings_get_string_by_type(3001, s));
    }else if (score < 25){
        object_changeAmount(obj[0], 4); /* +Four bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[3], 6); /* +Six bullets */
        object_changeAmount(obj[2], 6); /* +Six water */ 
        object_changeAmount(obj[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3002, s))), strings_get_string_by_type(3002, s));
    }else if (score < 35){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3003, s))), strings_get_string_by_type(3003, s));
    }else if(score < 45){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 3); /* +Three caviar */
        object_changeAmount(obj[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3004, s))), strings_get_string_by_type(3004, s));
    }else if (score >= 55){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 8); /* +Eight caviar */
        object_changeAmount(obj[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3005, s))), strings_get_string_by_type(3005, s));
    }
    win_write_line_slow_at(story, 4, get_mid_col_easy(story, strlen(cont)), cont);
    print_objects(intrf, obj);
    sleep(1);
    win_clear(battle);
    
    win_write_line_slow_at(battle, get_mid_row_easy(battle), get_mid_col_easy(battle, strlen(aux2)), aux2);
    free(aux);
    free(aux2);
    
    read_space_easy();
    win_clear(battle);
    win_clear(story);
    
    change_own(wp[2]);
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1037, s))), strings_get_string_by_type(1037, s));
    print_weapons(intrf, wp);
    town_info_easy(info);
    
    print_map(intrf, 12);
    player_setLocation(pl, 18, 2);
    print_player(intrf, pl);
    if(walking_simulator_easy(12) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1027, s))), strings_get_string_by_type(1027, s));
    
    print_map(intrf, 22); /* MAZE 2 */
    player_setLocation(pl, 3, 2);
    print_player(intrf, pl);
    if(walking_simulator_easy(22) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    battle_info_easy(info);
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1038, s))), strings_get_string_by_type(1038, s));
    player_setLocation(pl, 18, 40);
    print_map(intrf, 34);
    print_player(intrf, pl);
    
    Maps *copymap4=NULL;
    int enemies4[7] = {2,1,1,1,1,0,0};
    copymap4 = map_getCopy(intrf->maps_array, 34);
    if(battlemode_easy(enemies4, copymap4) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1027, s))), strings_get_string_by_type(1027, s));
    town_info_easy(info);
    
    print_map(intrf, 23); /* MAZE 3 */
    player_setLocation(pl, 3, 2);
    print_player(intrf, pl);
    if(walking_simulator_easy(23) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    win_write_line_slow_at(info, get_mid_row_easy(info), get_mid_col_easy(info, strlen(strings_get_string_by_type(2020, s))), strings_get_string_by_type(2020, s));
    
    score = hangman(intrf, s, lang);
    win_clear(story);
    win_write_line_slow_at(story, 2, get_mid_col_easy(story, strlen(strings_get_string_by_type(2999, s))), strings_get_string_by_type(2999, s));
    if(score <= -2){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[2], 3); /* +Three water */
        object_changeAmount(obj[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3000, s))), strings_get_string_by_type(3000, s));
    }else if(score <= 0){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[2], 4); /* +Four water */
        object_changeAmount(obj[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3001, s))), strings_get_string_by_type(3001, s));
    }else if (score <= 2){
        object_changeAmount(obj[0], 4); /* +Four bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[3], 6); /* +Six bullets */
        object_changeAmount(obj[2], 6); /* +Six water */ 
        object_changeAmount(obj[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3002, s))), strings_get_string_by_type(3002, s));
    }else if (score <= 6){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3003, s))), strings_get_string_by_type(3003, s));
    }else if(score <12){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 3); /* +Three caviar */
        object_changeAmount(obj[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3004, s))), strings_get_string_by_type(3004, s));
    }else if (score >= 12){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 8); /* +Eight caviar */
        object_changeAmount(obj[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3005, s))), strings_get_string_by_type(3005, s));
    }
    win_write_line_slow_at(story, 4, get_mid_col_easy(story, strlen(cont)), cont);
    print_objects(intrf, obj);
    read_space_easy();
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1040, s))), strings_get_string_by_type(1040, s));
    town_info_easy(info);
    
    print_map(intrf, 6);
    player_setLocation(pl, 3, 37);
    print_player(intrf, pl);
    if(walking_simulator_easy(6) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_easy(story)-2, get_mid_col_easy(story, strlen(strings_get_string_by_type(1041, s)))+2, strings_get_string_by_type(1041, s));
    win_write_line_slow_at(story, get_mid_row_easy(story)-1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1042, s))), strings_get_string_by_type(1042, s));
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1043, s))), strings_get_string_by_type(1043, s));
    win_write_line_slow_at(story, get_mid_row_easy(story)+1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1044, s))), strings_get_string_by_type(1044, s));
    change_own(wp[3]);
    print_weapons(intrf, wp);
    
    print_map(intrf, 4);
    player_setLocation(pl, 16, 43);
    print_player(intrf, pl);
    if(walking_simulator_easy(4) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(10445, s))), strings_get_string_by_type(10445, s));
    
    print_map(intrf, 3);
    player_setLocation(pl, 31, 38);
    print_player(intrf, pl);
    if(walking_simulator_easy(3) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1045, s))), strings_get_string_by_type(1045, s));
    
    print_map(intrf, 11);
    player_setLocation(pl, 31, 2);
    print_player(intrf, pl);
    if(walking_simulator_easy(11) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    battle_info_easy(info);
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1026, s))), strings_get_string_by_type(1026, s));
    player_setLocation(pl, 4, 4);
    print_map(intrf, 35);
    print_player(intrf, pl);
    
    Maps *copymap5=NULL;
    int enemies5[7] = {2,1,1,1,2,0,0};
    copymap5 = map_getCopy(intrf->maps_array, 35);
    if(battlemode_easy(enemies5, copymap5) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    win_write_line_slow_at(info, get_mid_row_easy(info), get_mid_col_easy(info, strlen(strings_get_string_by_type(1046, s))), strings_get_string_by_type(1046, s));
    
    score = Increasing_Bar(intrf, s, lang);
    win_clear(story);
    win_write_line_slow_at(story, 2, get_mid_col_easy(story, strlen(strings_get_string_by_type(2999, s))), strings_get_string_by_type(2999, s));
    win_clear(battle);
    if(score <= 15){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[2], 3); /* +Three water */
        object_changeAmount(obj[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3000, s))), strings_get_string_by_type(3000, s));
    }else if(score <= 30){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[2], 4); /* +Four water */
        object_changeAmount(obj[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3001, s))), strings_get_string_by_type(3001, s));
    }else if (score <= 45){
        object_changeAmount(obj[0], 4); /* +Four bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[3], 6); /* +Six bullets */
        object_changeAmount(obj[2], 6); /* +Six water */ 
        object_changeAmount(obj[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3002, s))), strings_get_string_by_type(3002, s));
    }else if (score <= 60){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3003, s))), strings_get_string_by_type(3003, s));
    }else if(score <= 75){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 3); /* +Three caviar */
        object_changeAmount(obj[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3004, s))), strings_get_string_by_type(3004, s));
    }else if (score >= 75){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 8); /* +Eight caviar */
        object_changeAmount(obj[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3005, s))), strings_get_string_by_type(3005, s));
    }
    
    win_write_line_slow_at(story, 4, get_mid_col_easy(story, strlen(strings_get_string_by_type(3007, s))), strings_get_string_by_type(3007, s));
    change_own(wp[4]); /* Giving the machine Gun */
    print_weapons(intrf, wp);
    
    win_write_line_slow_at(story, 6, get_mid_col_easy(story, strlen(cont)), cont);
    print_objects(intrf, obj);
    read_space_easy();
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1027, s))), strings_get_string_by_type(1027, s));
    town_info_easy(info);
    
    print_map(intrf, 24); /* MAZE 4 */
    player_setLocation(pl, 3, 2);
    print_player(intrf, pl);
    if(walking_simulator_easy(24) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    
    battle_info_easy(info);
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1026, s))), strings_get_string_by_type(1026, s));
    player_setLocation(pl, 4, 4);
    print_map(intrf, 36);
    print_player(intrf, pl);
    
    Maps *copymap6=NULL;
    int enemies6[7] = {2,1,1,1,2,1,0};
    copymap6 = map_getCopy(intrf->maps_array, 36);
    if(battlemode_easy(enemies6, copymap6) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_easy(story)-1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1034, s))), strings_get_string_by_type(1034, s));
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1035, s))), strings_get_string_by_type(1035, s));
    win_write_line_slow_at(story, get_mid_row_easy(story)+1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1036, s))), strings_get_string_by_type(1036, s));
    
    win_write_line_slow_at(info, get_mid_row_easy(info), get_mid_col_easy(info, strlen(strings_get_string_by_type(2021, s))), strings_get_string_by_type(2021, s));
    win_write_line_slow_at(info, get_mid_row_easy(info)+1, get_mid_col_easy(info, strlen(strings_get_string_by_type(2022, s))), strings_get_string_by_type(2022, s));
    
    score = g_21(intrf);
    win_clear(story);
    win_write_line_slow_at(story, 2, get_mid_col_easy(story, strlen(strings_get_string_by_type(2999, s))), strings_get_string_by_type(2999, s));
    win_clear(battle);
    if(score <= 6){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[2], 3); /* +Three water */
        object_changeAmount(obj[3], 2); /* +Two bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3000, s))), strings_get_string_by_type(3000, s));
    }else if(score <= 9){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[2], 4); /* +Four water */
        object_changeAmount(obj[3], 4); /* +Four bullets */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3001, s))), strings_get_string_by_type(3001, s));
    }else if (score <= 12){
        object_changeAmount(obj[0], 4); /* +Four bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[3], 6); /* +Six bullets */
        object_changeAmount(obj[2], 6); /* +Six water */ 
        object_changeAmount(obj[6], 6); /* +Six alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3002, s))), strings_get_string_by_type(3002, s));
    }else if (score <= 15){
        object_changeAmount(obj[0], 1); /* +One bandage */
        object_changeAmount(obj[1], 2); /* +Two fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 5); /* +Five alcohol */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3003, s))), strings_get_string_by_type(3003, s));
    }else if(score <= 18){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 3); /* +Three caviar */
        object_changeAmount(obj[7], 5); /* +Five missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3004, s))), strings_get_string_by_type(3004, s));
    }else if (score >= 18){
        object_changeAmount(obj[0], 2); /* +Two bandages */
        object_changeAmount(obj[1], 3); /* +Three fruits */
        object_changeAmount(obj[4], 4); /* +Four medical kits */
        object_changeAmount(obj[3], 8); /* +Eight bullets */
        object_changeAmount(obj[2], 8); /* +Eight water */
        object_changeAmount(obj[6], 8); /* +Eight alcohol */
        object_changeAmount(obj[5], 8); /* +Eight caviar */
        object_changeAmount(obj[7], 6); /* +Six missiles */
        win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(3005, s))), strings_get_string_by_type(3005, s));
    }
    win_write_line_slow_at(story, 4, get_mid_col_easy(story, strlen(cont)), cont);
    print_objects(intrf, obj);
    read_space_easy();
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    win_write_line_slow_at(story, get_mid_row_easy(story)-1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1067, s))), strings_get_string_by_type(1067, s));
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1068, s))), strings_get_string_by_type(1068, s));
    win_write_line_slow_at(story, get_mid_row_easy(story)+1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1069, s))), strings_get_string_by_type(1069, s));
    
    print_map(intrf, 12);
    player_setLocation(pl, 18, 3);
    print_player(intrf, pl);
    win_write_line_slow_at(story, 7, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    change_own(wp[5]); /* Giving the Bazooka */
    print_weapons(intrf, wp);
    
    town_info_easy(info);
    if(walking_simulator_easy(12) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    usleep(300000);
    
    win_write_line_slow_at(story, get_mid_row_easy(story)-1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1050, s))), strings_get_string_by_type(1050, s));
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1051, s))), strings_get_string_by_type(1051, s));
    
    print_map(intrf, 500);
    
    win_write_line_slow_at(story, 7, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    battle_info_easy(info);
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1026, s))), strings_get_string_by_type(1026, s));
    player_setLocation(pl, 18, 45);
    print_map(intrf, 501);
    print_player(intrf, pl);
    
    Maps *copymap7=NULL;
    int enemies7[7] = {2,2,2,2,2,1,1};
    copymap7 = map_getCopy(intrf->maps_array, 501);
    if(battlemode_easy(enemies7, copymap7) == 0){
        tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
        return;
    }
    
    win_clear(battle);
    win_clear(story);
    win_clear(info);
    
    print_map(intrf, 502);
    
    win_write_line_slow_at(story, get_mid_row_easy(story)-1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1053, s))), strings_get_string_by_type(1053, s));
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1054, s))), strings_get_string_by_type(1054, s));
    win_write_line_slow_at(story, get_mid_row_easy(story)+1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1055, s))), strings_get_string_by_type(1055, s));
    win_write_line_slow_at(story, 7, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    
    win_clear(story);
    win_write_line_slow_at(story, get_mid_row_easy(story)-1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1056, s))), strings_get_string_by_type(1056, s));
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1057, s))), strings_get_string_by_type(1057, s));
    win_write_line_slow_at(story, get_mid_row_easy(story)+1, get_mid_col_easy(story, strlen(strings_get_string_by_type(1058, s))), strings_get_string_by_type(1058, s));
    win_write_line_slow_at(story, 7, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    
    win_clear(story);
    
    win_write_line_slow_at(story, get_mid_row_easy(story), get_mid_col_easy(story, strlen(strings_get_string_by_type(1059, s))), strings_get_string_by_type(1059, s));
    win_write_line_slow_at(story, 6, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    win_clear(story);
    win_clear(battle);
    win_clear(info);
    
    print_map(intrf, 99);
    win_write_line_slow_at(story, 2, get_mid_col_easy(story, strlen(strings_get_string_by_type(1060, s))), strings_get_string_by_type(1060, s));
    win_write_line_slow_at(story, 3, get_mid_col_easy(story, strlen(strings_get_string_by_type(1061, s))), strings_get_string_by_type(1061, s));
    win_write_line_slow_at(story, 4, get_mid_col_easy(story, strlen(strings_get_string_by_type(1062, s))), strings_get_string_by_type(1062, s));
    win_write_line_slow_at(story, 5, get_mid_col_easy(story, strlen(strings_get_string_by_type(1063, s))), strings_get_string_by_type(1063, s));
    win_write_line_slow_at(story, 6, get_mid_col_easy(story, strlen(strings_get_string_by_type(1064, s))), strings_get_string_by_type(1064, s));
    win_write_line_slow_at(story, 7, get_mid_col_easy(story, strlen(strings_get_string_by_type(1065, s))), strings_get_string_by_type(1065, s));
    win_write_line_slow_at(story, 8, get_mid_col_easy(story, strlen(strings_get_string_by_type(1066, s))), strings_get_string_by_type(1066, s));
    win_write_line_slow_at(story, 9, get_mid_col_easy(story, strlen(cont)), cont);
    read_space_easy();
    
    
    tcsetattr(fileno(stdin), TCSANOW, &initial_easy);
    return;
}