#include <stdio.h>
#include "defines.h"
#include "player.h"
#include <stdlib.h>

int main (){
    Player *pl=NULL;
    char Display;
    int Col, Row, Orientation;
    
    pl = load_player("player.txt");
    if(pl == NULL){
    printf("Error when allocating memory for player pointer.\n");
    return -1;
  }
  printf("Comprobación de valores: \n");
  Col = player_getCol(pl);
  if (Col == ERROR){
      printf("Error en getCol\n");
      player_delete(pl);
      return -1;
    }
  Row = player_getRow(pl);
  if (Row == ERROR){
      printf("Error en getRow\n");
      player_delete(pl);
      return -1;
    }
  Display = player_getDisplay(pl);
  if (Display == 0){
      printf("Error en getDisplay\n");
      player_delete(pl);
      return -1;
    }
  Orientation = player_getOrientation(pl);
  if (Orientation == ERROR){
      printf("Error en getOrientation\n");
      player_delete(pl);
      return -1;
    }
  printf("El jugador está en la Col = %d, Row = %d con la Orientación %d y el Display: %c\n", Col, Row, Orientation, Display);
  printf("\n");
 
  printf("Cambiando valores a col = 56, row = 32, orientation = 1052\n");
  player_setLocation(pl, 32, 56);
  player_setOrientation(pl, RIGHT);
  printf("\n");
  
  printf("Comprobación de valores: \n");
  Col = player_getCol(pl);
  Row = player_getRow(pl);
  Display = player_getDisplay(pl);
  Orientation = player_getOrientation(pl);
  printf("El jugador está en la Col = %d, Row = %d con la Orientación %d y el Display: %c\n", Col, Row, Orientation, Display);
  
  player_delete(pl);
  
  return 0;
}