#include "player.h"


struct _player{
    char display;       /*Char with which the player is going to be printed*/
    int orientation;    /*Indicates where the player is looking to (NORTH, SOUTH, WEST, EAST)*/
    int row;            /*Position of the player in X axis*/
    int col;            /*Position of the player in Y axis*/
};


Player* load_player(char *filename){
    FILE *in=NULL;
    Player *player=NULL;
    char buff[BUFFER_SIZE], display;
    int orientation, r, c;
    
    
    if(filename == NULL){
        printf("Error. Player-F1-1.\n");
        return NULL;
    }
    
    in = (FILE *) fopen(filename, "r");
    if(in==NULL){
        printf("Error. Player-F1-2.\n");
        return NULL;
    }
    
    fgets(buff, BUFFER_SIZE, in);
    sscanf(buff, "%c %d %d %d", &display, &orientation, &r, &c);
    
    player = (Player *) malloc(sizeof(Player));
    if(player==NULL){
        printf("Error. Player-F1-3.\n");
        fclose(in);
        return NULL;
    }
    
    player->display = display;
    player->orientation = orientation;
    player->row = r;
    player->col = c;
    
    fclose(in);
    
    return player;
}



void player_delete(Player *pl){
    if(pl==NULL) return;
    
    free(pl);
}



char player_getDisplay(Player *pl){
    if(pl==NULL){
        printf("Error. Player-F2-1.\n");
        return 0;
    }
    
    return pl->display;
}



int player_getOrientation(Player *pl){
    if(pl==NULL){
        printf("Error. Player-F3-2.\n");
        return ERROR;
    }
    
    return pl->orientation;
}



Status player_setOrientation(Player *pl, int orient){
    if(pl==NULL){
        printf("Error. Player-F4-1.\n");
        return FAILED;
    }
    if(orient!=UP && orient!=DOWN && orient!=LEFT && orient!=RIGHT){
        printf("Error. Player-F4-2.\n");
        return FAILED;
    }
    
    pl->orientation = orient;
    
    return OK;
}



int player_getRow(Player *pl){
    if(pl==NULL){
        printf("Error. Player-F5-2.\n");
        return ERROR;
    }
    
    return pl->row;
}



int player_getCol(Player *pl){
    if(pl==NULL){
        printf("Error. Player-F6-2.\n");
        return ERROR;
    }
    
    return pl->col;
}



Status player_setLocation(Player *pl, int r, int c){
    if(pl==NULL||r<=0||c<=0){
        printf("Error. Player-F7-2.\n");
        return FAILED;
    }
    
    pl->row = r;
    pl->col = c;
    
    return OK;
}

