#ifndef DEFINES_H
#define DEFINES_H

/*Game Mode*/
#define EASY 1000
#define MEDIUM 1001
#define HARD 1002
#define LEGENDARY 1003

/*Languages*/
#define ENGLISH 1010
#define SPANISH 1011
#define GALICIAN 1012

/*Object types*/
#define WEAPON 1020
#define AMMO 1021
#define FOOD 1022
#define MONSTER 1023
#define DRINK 1024
#define MEDICINES 1025

/*Resources types*/
#define HP 1030
#define HYDRATION 1031
#define GUNPOWDER 1032
#define HUNGER 1033

/*Status*/
typedef enum{
    ERROR=2000, OK=2001
}Status;

#endif /* DEFINES_H */
