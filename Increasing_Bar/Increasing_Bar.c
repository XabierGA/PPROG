#include "interface.h"
#include "maps.h"
#include "rectangle.h"
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define TENTOTHENINE 1000000000

int read_key(){
    char choice;
    choice = fgetc(stdin);
    
    if (choice == 27 && fgetc(stdin) == '[') { /* The key is an arrow key */
        choice = fgetc(stdin);

        switch(choice) {
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

void print_bar(int percent, Interface *intrf){
    int i;
    
    for(i=2; i<91; i++){
        win_write_char_at(intrf->rect_array[0], 13, i, '#');
    }
}

int main(){
    Interface *intrf=NULL;
    int percent = 0,flag=2, c;
    struct timespec time1, time2, time3;
    
    intrf = create_intrf("rectangles.txt", "map.txt");
    if(intrf == NULL) exit(1);
    
    print_map(intrf, 1);
    
    sleep(4);
    print_map(intrf, 2);
    clock_gettime(CLOCK_REALTIME, &time1);
    clock_gettime(CLOCK_REALTIME, &time3);
    
    while(1){
        clock_gettime(CLOCK_REALTIME, &time2);
        if((time2.tv_nsec-time1.tv_nsec)/TENTOTHENINE > 10){
            break;
        }
        if((time2.tv_nsec-time3.tv_nsec)/TENTOTHENINE > 1){
            percent += -1;
            time3 = time2;
        }
        
        
        c=read_key();
        if(flag != 0 && c == -RIGHT){
            flag = 0;
            percent += 5;
            
        }
        if(flag != 1 && c == -LEFT){
            flag = 1;
            percent += 5;
        }
        
        print_bar(percent, intrf);
        
        if(percent<=91){
            break;
        }
    }
    
    if(percent<91){
        printf("YOU LOST");
    }
    else{
        printf("YOU WON");
    }
    destroy_intrf(intrf);
    
    return 0;
}