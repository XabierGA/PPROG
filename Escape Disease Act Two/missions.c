#include "missions.h"
#include <time.h>


#define HM_TAM 100
#define TENTOTHENINE 1000000000


int read_key_mission(){
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


/* ----------------------------------------- H A N G M A N ------------------------------------------------ */
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


int hangman(Interface *intrf, Strings **s, int lang){
    FILE *f=NULL;
    rectangle *rectStory=NULL, *rectBattle=NULL;
    char buff[HM_TAM], word[HM_TAM];
    char letter;
    int i, num_words, random_n, flag, correct, deaths, col; /* <--- DEATHS HAS TO BE INITIALIZED */
    char found[8] = "00000000";
    
    /* Error Checking */
    if(intrf==NULL){
        printf("Error. Hangman 1.\n");
        exit(ERROR);
    }
    if(s==NULL){
        printf("Error. Hangman 2.\n");
        exit(ERROR);
    }
    if(lang!=ENGLISH && lang!=SPANISH && lang!=GALICIAN){
        printf("Error. Hangman 3.\n");
        exit(ERROR);
    }/*-----------------------------------------------*/
    
    /* Opening the correct file where the word is going to be taken */
    if(lang==ENGLISH){
        f = (FILE *) fopen("txtfiles/hangman_eng.txt", "r");
    }
    else if(lang==SPANISH){
        f = (FILE *) fopen("txtfiles/hangman_spa.txt", "r");
    }
    else if(lang==GALICIAN){
        f = (FILE *) fopen("txtfiles/hangman_gal.txt", "r");
    }
    else{
        printf("Error. Hangman 4.\n");
        exit(ERROR);
    }
    if(f==NULL){
        printf("Error. Hangman 5.\n");
        exit(ERROR);
    }
    /*--------------------------------------------------------------*/
    
    /* Getting the number of words in the file */
    fgets(buff, HM_MAX, f);
    sscanf(buff, "%d", &num_words);
    
    random_n = rand_num(1, num_words);
    
    /*We got the random number that will choose the random word, and we search for it*/
    for(i=1; i<random_n; i++){
        fgets(buff, HM_TAM, f_in);
        sscanf(buff, "%s", word);
    }
    fgets(buff, HM_TAM, f);
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
    
    /*---------------Once we´ve got the word, the game STARTS ------------------*/
    
    win_write_line_at(rectStory, 5, 5, strings_get_string_by_type(40, strs)); /* Press a key to try to guess */
    
    
    while(1){
        flag=0;
        letter = read_key_mission();
        
        for(i=0; i<strlen(word); i++){
            if(letter == word[i]){
                flag=1;
                correct++;
                col = conv(i);
                win_write_char_at(rectBattle, 17, col, word[i]);
                found[i] = word[i];
                win_write_line_at(rectStory, 6, 5, strings_get_string_by_type(41, strs)); /* Nice! This letter appears! */
            }
        }
        
        if(flag==0){
            deaths++;
            change_map(found, deaths, intrf, rectBattle);
            if(deaths==7){
                win_clear(rectStory);
                win_write_line_at(rectStory, 5, 5, strings_get_string_by_type(42, strs)); /* You have consumed all your lives*/
                break;
            }
        }
        else if(correct==8){
            win_clear(rectStory);
            win_write_line_at(rectStory, 5, 5, strings_get_string_by_type(43, strs)); /* You won! Congratulations*/
            break;
        }
    }
    
    fclose(f);
    return deaths;
}
/* --------------------------------------------------------------------------------------------------------------------------- */



/* ------------------------------------------------- N O T     N O T --------------------------------------------------------- */

int puntos_NotNot =0 ;

int aleat_num(int inf, int sup) {
  
  int dif;
  
  if (inf > sup) return inf - 1;
  
  dif = sup - inf;
  
  return inf + (int)((dif + 1) * (rand() / (RAND_MAX + 1.0)));

}

int Not_Not(Interface *intrf){
    int c, i, score;
    int aleat = 0;
    struct timespec t_inicio, t_fin, aux;
    double total;
    
    srand(time(NULL));

    if(intrf == NULL) exit(12345);
    
    print_map (intrf, 13);
    sleep(2);
    
    
    sleep(2);
    clock_gettime(CLOCK_REALTIME, &t_inicio);
    
    for (i = 0; i<10 ;i++){
        
        
        aleat = aleat_num(1, 11);
        
        print_map(intrf, aleat+100);
        
        if (aleat == 1){
            c = read_key_mission();
            if (c==-RIGHT){
                puntos_NotNot++;
            }else{
                break;
            } 
        }else if (aleat == 2){
            c = read_key_mission();
            if (c==-LEFT){
                puntos_NotNot++;
            }else{
                break;
            } 
        }else if (aleat == 3){
            c = read_key_mission();
            if (c==-RIGHT||c==-UP||c==-DOWN){
                puntos_NotNot++;
            }else{
                break;
            } 
        }else if (aleat == 4){
            c = read_key_mission();
            if (c==-LEFT||c==-UP||c==-DOWN){
                puntos_NotNot++;
            }else{
                break;
            } 
        }else if (aleat == 5){
            c = read_key_mission();
            if (c==-RIGHT){
                puntos_NotNot ++;
            }else{
                break;
            }
        }else if (aleat == 6){
            c = read_key_mission();
            if (c==-LEFT){
                puntos_NotNot ++;
            }else{
                break;
            }
        }else if (aleat == 7){
           c = read_key_mission();
           if (c==-RIGHT||c==-UP){
                puntos_NotNot ++;
            }else{
                break;
            }
        }else if (aleat == 8){
           c = read_key_mission();
           if (c==-UP){
                puntos_NotNot ++;
            }else{
                break;
            }
        }else if (aleat == 9){
           c = read_key_mission();
           if (c==-DOWN){
                puntos_NotNot ++;
            }else{
                break;
            }
        }else if (aleat == 10){
           c = read_key_mission();
           if (c==-RIGHT||c==-DOWN||c==-LEFT){
                puntos_NotNot ++;
            }else{
                break;
            }
        }else if (aleat == 11){
           c = read_key_mission();
           if (c==-RIGHT||c==-UP||c==-LEFT){
                puntos_NotNot ++;
            }else{
                break;
            }
        }    
    }
    
    clock_gettime(CLOCK_REALTIME, &t_fin);
    aux.tv_sec = t_fin.tv_sec - t_inicio.tv_sec; 
    total = aux.tv_sec;
    score = (int)((puntos_NotNot*100)/(int)total);
    
    return score;
}
/* ------------------------------------------------------------------------------------------ */

/* ----------------------------------------- 2 1 -------------------------------------------- */
int g_21(Interface *intrf){
    int c, score=0;
    int aleat = 0;
    
    
    srand(time(NULL));
    if(intrf == NULL) exit(12345);
    
    
    while(1){
        
        
        aleat = aleat_num(1, 7);
        
        score += aleat;
        
        if (score > 21){
            break;
        }
        
        print_map (intrf, score+200);
        
        if (score == 1){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 2){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 3){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 4){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 5){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 6){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 7){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 8){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 9){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 10){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 11){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 12){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 13){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 14){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 15){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 16){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 17){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 18){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 19){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 20){
            c = read_key_mission();
            if (c==-UP)
                continue;
            else
                break;
        }else if (score == 21){
                break;
        }    
    }
    if (score>21){
        score = 0;
    }else if(score == 21){
        score = 100;   
    }else{
        score = score *3;
    }
    
    return score;
}

/* -------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------Increasing Bar--------------------------------------------------------*/

int percent_ib=0, superflag_ib=1; 

typedef struct{
    Interface* intrf;
}delete_stuff;

void print_bar(Interface *intrf){
    int i;
    
    for(i=3; i<percent_ib && i<91; i++){
        win_write_char_at(intrf->rect_array[0], 14, i, '#');
    }
}

void* delete_bar(void* stuff){
    int i;
    delete_stuff *del;
    
    del = (delete_stuff*) stuff;
    
    while(superflag_ib == 1){
        sleep(1);
        
        if(percent_ib > 2){
            percent_ib = percent_ib-3;
        }
        for(i=percent_ib; i<91; i++){
            if(i>2){
                win_write_char_at(del->intrf->rect_array[0], 14, i, ' ');
            }
        }
    } 
    return (void*) del;
}

int Increasing_Bar(Interface *intrf, Strings **strs, int lang){
    int flag=2, c;
    struct timespec time1, time2, time3;
    pthread_t db;
    delete_stuff *delst=NULL;
    
    _term_init();
    
    if(intrf == NULL){
        printf("Increasing_Bar Error 1\n");
        return -1;
    } 
    if(lang != ENGLISH && lang != SPANISH && lang != GALICIAN){
        printf("Increasing_Bar Error 2\n");
        return -1;
    }
    
    print_map(intrf, 120);
    
    
    win_write_line_at(intrf->rect_array[0], 14, 30, strings_get_string_by_type(30, strs));
    
    
    sleep(4);
    print_map(intrf, 121);
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
        
        c=read_key_mission();
        
        if(c == 'o'){
            break;
        }
        
        if(flag != 0 && c == -RIGHT){
            flag = 0;
            percent_ib += 1;
        }
        if(flag != 1 && c == -LEFT){
            flag = 1;
            percent_ib += 1;
        }
        
        print_bar(intrf);
        
        if(percent_ib>=91){
            superflag_ib=0;
            break;
        }
    }
    
    if(percent_ib<91){
        win_write_line_at(intrf->rect_array[0], 14, 30, strings_get_string_by_type(31, strs););
    }
    else{
        win_write_line_at(intrf->rect_array[0], 14, 30, strings_get_string_by_type(32, strs););
    }
    sleep(2);
    tcsetattr(fileno(stdin), TCSANOW, &initial);	/*We now restore the settings we back-up'd 
							  so that the termial behaves normally when 
							  the program ends */
							  
    destroy_intrf(intrf);
    
    return 0;
}


/*--------------------------------------------------------------------------------------------*/



/*---------------------------PONG-------------------------------------------------------------*/
int superflag = 0;
int row_bar;

typedef struct{
    Interface* intrf;
}bar_stuff;

typedef struct{
    int row;
    int col;
}Ball;

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


int main(Interface *intrf, Strings **strs, int lang){
    int way, dir1, dir2, cicle=0; 
    Ball *ball;
    struct timespec time1, time2;
    pthread_t db;
    bar_stuff *stuff=NULL;
    char *buffer;
    
    _term_init();
    
    srand(time(NULL));
    
    if(intrf == NULL){
        printf("Increasing_Bar Error 1\n");
        return -1;
    } 
    if(lang != ENGLISH && lang != SPANISH && lang != GALICIAN){
        printf("Increasing_Bar Error 2\n");
        return -1;
    }
    
    buffer = (char *)malloc(5*sizeof(char));
    print_map(intrf, 120);
    
    win_write_line_at(intrf->rect_array[0], 14, 30, strings_get_string_by_type(33, strs));
    
    sleep(4);
    
    print_map(intrf, 122);
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
        
        itoa(time2.tv_sec-time1.tv_sec,buffer,10);
        win_write_line_at(intrf->rect_array[0], 5, 30, strings_get_string_by_type(34, strs));
        win_write_line_at(intrf->rect_array[0], 5, 40, buffer);
        
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
    
    print_map(intrf, 120);
    if(superflag == 1){
        win_write_line_at(intrf->rect_array[0], 14, 30, strings_get_string_by_type(31, strs));
    }
    if(superflag == 2){
        win_write_line_at(intrf->rect_array[0], 14, 30, strings_get_string_by_type(32, strs));
    }
    sleep(2);
    
    tcsetattr(fileno(stdin), TCSANOW, &initial);	/*We now restore the settings we back-up'd 
							  so that the termial behaves normally when 
							  the program ends */
							  
    destroy_intrf(intrf);
    
    return 0;
}