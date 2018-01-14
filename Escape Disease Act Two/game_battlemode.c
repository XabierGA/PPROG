#include "game_battlemode.h"

Resources **r_battle=NULL;
Object **obj_battle=NULL;
Weapon **wp_battle=NULL;
Enemy **e_battle=NULL;
Interface *intrf_battle=NULL;
Player *pl_battle=NULL;
Strings **s_battle=NULL;
int n_ene_battle=-1;

struct termios initial_battle;


typedef struct {
  time_t initial_battle; /* Initial time at which the function is first called */
  Interface  *intrf;    /* pointer to the interface where the clock is to be displayed */
} clock_data_battle;


void _term_init_battle() {
	struct termios new;	          /*a termios structure contains a set of attributes about 
					  how the terminal scans and outputs data*/
		
	tcgetattr(fileno(stdin), &initial_battle);    /*first we get the current settings of out 
						 terminal (fileno returns the file descriptor 
						 of stdin) and save them in initial. We'd better 
						 restore them later on*/
	new = initial_battle;	                      /*then we copy them into another one, as we aren't going 
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

int read_key_battle(){
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

int get_mid_col_battle(rectangle* r, int len){
    return ((r->last_col-r->ini_col)/2)-(len/2);
}

int get_mid_row_battle(rectangle* r){
    return ((r->last_row-r->ini_row)/2);
}

void game_init_battle(int lang){
    
    intrf_battle = create_intrf("txtfiles/rectangles.txt", "txtfiles/maps.txt");
    
    if(lang==ENGLISH){
        r_battle = load_resources("txtfiles/english/resources_battle.txt");
        wp_battle = load_weapons("txtfiles/english/weapons_battle.txt");
        obj_battle = load_objects("txtfiles/english/objects_battle.txt");
        pl_battle = load_player("txtfiles/english/player_battle.txt");
        e_battle = load_enemies("txtfiles/english/enemies_battle.txt", &n_ene_battle);
    	s_battle = load_strings("txtfiles/english/strings_eng.txt");
    }
    else if(lang==SPANISH){
    	r_battle = load_resources("txtfiles/spanish/resources_battle.txt");
        wp_battle = load_weapons("txtfiles/spanish/weapons_battle.txt");
        obj_battle = load_objects("txtfiles/spanish/objects_battle.txt");
        pl_battle = load_player("txtfiles/spanish/player_battle.txt");
        e_battle = load_enemies("txtfiles/spanish/enemies_battle.txt", &n_ene_battle);
    	s_battle = load_strings("txtfiles/spanish/strings_spa.txt");
    }
    else if(lang==GALICIAN){
    	r_battle = load_resources("txtfiles/galician/resources_battle.txt");
        wp_battle = load_weapons("txtfiles/galician/weapons_battle.txt");
        obj_battle = load_objects("txtfiles/galician/objects_battle.txt");
        pl_battle = load_player("txtfiles/galician/player_battle.txt");
        e_battle = load_enemies("txtfiles/galician/enemies_battle.txt", &n_ene_battle);
    	s_battle = load_strings("txtfiles/galician/strings_gal.txt");
    }
    else{
    	printf("Error. Battle 7.\n");
    	exit(ERROR);
    }
    
    if(intrf_battle==NULL || r_battle==NULL || wp_battle==NULL || obj_battle==NULL || pl_battle==NULL || e_battle==NULL || s_battle==NULL ||n_ene_battle==-1){
        printf("Error. Battle 8.\n");
        exit(ERROR);
    }
    
}


int dir_conv_battle(int d) {
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

void read_space_battle(){
    char c = 0;
    while(1){
        c = read_key_battle();
        if(c != ' '){
            continue;
        }
        return;
    }
}


int battlemode_battle(int *ene_array, int map_id){
    shoot_stuff *stst=NULL;
    moveEne_stuff *mest=NULL;
    pthread_t pth_shoot, pth_moveEne;
    Enemy **ene=NULL, **econt;
    int c;
    rectangle *battle=NULL;
    Maps *copymap=NULL;
    
    
    if(ene_array == NULL){
        printf("Error. Main-F1-1.\n");
        exit(ERROR);
    }
    if(n_ene_battle == -1){
        printf("Error. Main-F1-2.\n");
        exit(ERROR);  
    }
    
    copymap = map_getCopy(intrf_battle->maps_array, map_id);
    if(copymap==NULL){
        printf("Error. Main-F1-4.\n");
        exit(ERROR);
    }
    
    battle = win_find_rectangle(RECT_BATTLE, intrf_battle->rect_array);
    if(battle==NULL){
        printf("Error. Main-F1-5.\n");
        exit(ERROR);
    }
    
    ene = generate_arrayEnemies(e_battle, ene_array, n_ene_battle);
    if(ene==NULL) exit(12345);
    
    generate_EnePosRand(ene, copymap);
    
    print_enemies(intrf_battle, ene);
    
    copymap->field[player_getRow(pl_battle) - 2][player_getCol(pl_battle) - 2] = player_getDisplay(pl_battle); /* Inserting the player display into the map*/
    

    for(econt = ene; *(econt)!=NULL; econt++){
        mest = (moveEne_stuff *) malloc(sizeof(moveEne_stuff)); /* When are we freeing this ??? */
        mest->intrf = intrf_battle;
        mest->pl = pl_battle;
        mest->r = r_battle;
        mest->copymap = copymap;
        mest->ene = *(econt);
        pthread_create(&pth_moveEne, NULL, move_enemies, (void *) mest);
    }

    while(1){
        c = read_key_battle();
        if(c == 'o'){
            tcsetattr(fileno(stdin), TCSANOW, &initial_battle);
            break;
        }
        
        
        else if(c < 0){ /* MOVING */
            if(move(intrf_battle, copymap, pl_battle, -c)==DOOR && enemy_checkPhyStat(ene)==ALL_KILLED){
                destroy_enemies(ene);
                return DOOR;
            }
        }
        
        else if(c == 'q'){ /* Swaping weapon */
            change_equipped(wp_battle, UPW);
            print_weapons(intrf_battle, wp_battle);
        }
        else if(c == 'e'){ /* Swaping weapon */
            change_equipped(wp_battle, DOWNW);
            print_weapons(intrf_battle, wp_battle);
        }
        /* Using objects */
        else if(c == '1'){
            use_object(r_battle, obj_battle[0]);
            print_objects(intrf_battle, obj_battle);
            print_resources(intrf_battle, r_battle);
        }
        else if(c == '2'){
            use_object(r_battle, obj_battle[1]);
            print_objects(intrf_battle, obj_battle);
            print_resources(intrf_battle, r_battle);
        }
        else if(c == '3'){
            use_object(r_battle, obj_battle[2]);
            print_objects(intrf_battle, obj_battle);
            print_resources(intrf_battle, r_battle);
        }
        else if(c == '4'){
            use_object(r_battle, obj_battle[3]);
            print_objects(intrf_battle, obj_battle);
            print_resources(intrf_battle, r_battle);
        }
        else if(c == '5'){
            use_object(r_battle, obj_battle[4]);
            print_objects(intrf_battle, obj_battle);
            print_resources(intrf_battle, r_battle);
        }
        else if(c == '6'){
            use_object(r_battle, obj_battle[5]);
            print_objects(intrf_battle, obj_battle);
            print_resources(intrf_battle, r_battle);
        }
        /* Shooting */
        else if (c == 'w' || c == 'a' || c == 's' || c == 'd') {
            stst = (shoot_stuff *) malloc(sizeof(shoot_stuff));
            stst->intrf = intrf_battle;
            stst->wp = wp_battle;
            stst->pl = pl_battle;
            stst->r = r_battle;
            stst->copymap = copymap;
            stst->dir = dir_conv_battle(c);
            stst->ene = ene;
            pthread_create(&pth_shoot, NULL, shoot, (void *) stst);
        }
    }
    destroy_enemies(ene);
    return 0;
}


void game_battlemode(int lang){
    srand(time(NULL));
    
    game_init_battle(lang);
    
    _term_init_battle();
    
    rectangle* story = NULL;
    rectangle* info = NULL;
    rectangle* battle = NULL;
    story = win_find_rectangle(RECT_STORY, intrf_battle->rect_array);
    info = win_find_rectangle(RECT_INFO, intrf_battle->rect_array);
    battle = win_find_rectangle(RECT_BATTLE, intrf_battle->rect_array);
    
    if(initialize_intrf(intrf_battle, 120, r_battle, wp_battle, obj_battle, s_battle) == FAILED){
        printf("Error. Battle 1.\n");
        exit(ERROR);
    }
    win_write_line_at(battle, 18, get_mid_col_battle(battle, strlen(strings_get_string_by_type(8, s_battle))), strings_get_string_by_type(8, s_battle));
    sleep(1);
    
    char* cont = NULL;
    cont = strings_get_string_by_type(9991, s_battle);
    
    /* PRINTEAR EL MAPA CON TODAS LAS ESTADÍSTIVCAS DE LOS BICHOS MIENTRAS SE CUENTA LA MIERDA EN EL OTRO RECTANGULO */
    
    win_write_line_at(story, 2, get_mid_col_battle(story, strlen(strings_get_string_by_type(500, s_battle))), strings_get_string_by_type(500, s_battle));
    win_write_line_at(story, 3, 3, strings_get_string_by_type(501, s_battle));
    win_write_line_at(story, 4, 3, strings_get_string_by_type(502, s_battle));
    win_write_line_at(story, 5, 3, strings_get_string_by_type(503, s_battle));
    win_write_line_at(story, 6, 3, strings_get_string_by_type(504, s_battle));
    win_write_line_at(story, 7, 3, strings_get_string_by_type(505, s_battle));
    win_write_line_slow_at(info, 2, 3, strings_get_string_by_type(9992, s_battle));
    win_write_line_slow_at(info, 3, 3, strings_get_string_by_type(9993, s_battle));
    win_write_line_slow_at(info, 4, 3, strings_get_string_by_type(9994, s_battle));
    win_write_line_slow_at(info, 5, 3, strings_get_string_by_type(9996, s_battle));
    win_write_line_at(story, 8, 3, strings_get_string_by_type(506, s_battle));
    win_write_line_slow_at(story, 11, get_mid_col_battle(story, strlen(cont)), cont);
    read_space_battle();
    
    win_clear(battle);
    win_clear(story);
    
    Resources *hp=NULL;
    char *sentence=NULL;
    int *enemies=NULL;
    enemies = (int *) malloc(n_ene_battle * sizeof(int));
    if(enemies==NULL){
        printf("Error. Battle 2.\n");
        exit(ERROR);
    }
    
    hp = resources_getResource(r_battle, MEDICINE);
    
    sentence = strings_get_string_by_type(507, s_battle);
    int k;
    for(k=1; 1; k++){
        enemies[0] = k;
        enemies[1] = ceil(k/3);
        enemies[2] = ceil(k/3);
        enemies[3] = ceil(k/4);
        enemies[4] = floor(k/5);
        enemies[5] = floor(k/6);
        enemies[6] = floor(k/8);
        
        char *n_round = (char *) malloc(sizeof(int));
        char *auxiliar = (char *)malloc(strlen(sentence) * sizeof(char));
        strcpy(auxiliar, sentence);
        sprintf(n_round, " %d", k);
        strcat(auxiliar, n_round);
        win_write_line_at(story, get_mid_row_battle(story), get_mid_col_battle(story, strlen(auxiliar)), auxiliar);
        
        print_map(intrf_battle, 300);
        player_setLocation(pl_battle, 35, 92);
        print_player(intrf_battle, pl_battle);
        if(battlemode_battle(enemies, 300) != DOOR){
            free(n_round);
            free(auxiliar);
            break;
        }
        modify_resource(hp, 100000);
        print_resources(intrf_battle, r_battle);
        win_clear(story);
        free(auxiliar);
        free(n_round);
    }
    
    free(enemies);
    
    tcsetattr(fileno(stdin), TCSANOW, &initial_battle);
    return;
}