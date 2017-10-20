#ifndef DEFINES_H
#define DEFINES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Game Mode*/
#define EASY 1000
#define MEDIUM 1001
#define HARD 1002
#define LEGENDARY 1003
#define BATTLE_MODE 1004

/*Languages*/
#define ENGLISH 1010
#define SPANISH 1011
#define GALICIAN 1012

/*Object types*/
#define AMMO 1021
#define FOOD 1022
#define DRINK 1023
#define MEDICINE 1024

/*Enemies types*/
#define ZOMBIE 1030
#define WALL 1031


/*Status*/
typedef enum{
    FAILED=2000, OK=2001
}Status;

/*Others*/
BUFFER_SIZE 128
ERROR -11111

#endif /* DEFINES_H */
