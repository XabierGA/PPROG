#include "interface.h"
#include "resources.h"
#include "object.h"
#include "player.h"
#include "weapon.h"
#include "maps.h"
#include "rectangle.h"
#include "strings.h"
#include "missions.h"
#include <pthread.h>
#include <termios.h>

#define HM_TAM 100

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


/* ---------- H A N G M A N -------------------- */
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
    
    win_write_line_at(rectStory, 5, 5, "PRESS A KEY TO GUESS A LETTER (THE WORD IS CAPITALIZED)"); /* <------ IT HAS TO BE CHANGED */
    
    
    while(1){
        flag=0;
        letter = read_key();
        
        for(i=0; i<strlen(word); i++){
            if(letter == word[i]){
                flag=1;
                correct++;
                col = conv(i);
                win_write_char_at(rectBattle, 17, col, word[i]);
                found[i] = word[i];
                win_write_line_at(rectStory, 6, 5, "NICEEE! THIS LETTER APPEARS!"); /* <-------- IT HAS TO BE CHANGED */
            }
        }
        
        if(flag==0){
            deaths++;
            change_map(found, deaths, intrf, rectBattle);
            if(deaths==7){
                win_clear(rectStory);
                win_write_line_at(rectStory, 5, 5, "YOU ARE DEAD, YOU LOST!"); /* <-------- IT HAS TO BE CHANGED */
                break;
            }
        }
        else if(correct==8){
            win_clear(rectStory);
            win_write_line_at(rectStory, 5, 5, "YOU WON, CONGRATULATIONS!"); /* <-------- IT HAS TO BE CHANGED */
            break;
        }
    }
    
    fclose(f);
    return deaths;
}

/* -------------------------------------------------------------------------------------------------------------*/