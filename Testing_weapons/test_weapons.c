#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weapons.h"

int main(){
  Weapon **weapons=NULL;
  Resources **res=NULL;
  
  /*Creating the list of weapons*/
  weapons = load_weapons("weapons.txt");
  if(weapons == NULL){
    printf("Error when allocating memory for double weapons´ pointer.\n");
    return ERROR;
  }
  
  /*Loading a resouce file*/

  res = load_resources("resources.txt");
  if(res == NULL){
    printf("Error when allocating memory for double resources´ pointer.\n");
    return -1;
  }
  
  /*Showing the stats of all the weapons and the gunpowder*/
  printf("\nWeapon 1.\n");
  printf("%s ", weapon_getName(weapons[0]));
  printf("%d ", weapon_getPowderWaste(weapons[0]));
  printf("%d ", weapon_getSpeed(weapons[0]));
  printf("%d ", weapon_getDamage(weapons[0]));
  if(own_weapon(weapons[0])==OWNED) printf("OWNED");
  else printf("NOT OWNED");
  printf("\n\n");
  
  printf("\nWeapon 2.\n");
  printf("%s ", weapon_getName(weapons[1]));
  printf("%d ", weapon_getPowderWaste(weapons[1]));
  printf("%d ", weapon_getSpeed(weapons[1]));
  printf("%d ", weapon_getDamage(weapons[1]));
  if(own_weapon(weapons[1])==OWNED) printf("OWNED");
  else printf("NOT OWNED");
  printf("\n\n");
  
  printf("\nWeapon 3.\n");
  printf("%s ", weapon_getName(weapons[2]));
  printf("%d ", weapon_getPowderWaste(weapons[2]));
  printf("%d ", weapon_getSpeed(weapons[2]));
  printf("%d ", weapon_getDamage(weapons[2]));
  if(own_weapon(weapons[2])==OWNED) printf("OWNED");
  else printf("NOT OWNED");
  printf("\n\n");
  
  printf("\nWeapon 4.\n");
  printf("%s ", weapon_getName(weapons[3]));
  printf("%d ", weapon_getPowderWaste(weapons[3]));
  printf("%d ", weapon_getSpeed(weapons[3]));
  printf("%d ", weapon_getDamage(weapons[3]));
  if(own_weapon(weapons[3])==OWNED) printf("OWNED");
  else printf("NOT OWNED");
  printf("\n\n");
  
  printf("\nResource.\n");
  printf("%d ", resources_getObjectType(res[3]));
  printf("%s ", resources_getName(res[3]));
  printf("%d ", resources_getMax(res[3]));
  printf("%d ", resources_getActualValue(res[3]));
  printf("%d ", resources_getRow(res[3]));
  printf("%d ", resources_getCol(res[3]));
  printf("\n\n");
  
  /*Owning weapon 3*/
  printf("Owning weapon 3\n\n");
  if(change_own(weapons[2]) == FAILED){
      printf("Error when owning a new weapon\n");
      return -1;
  }
  
  /*Shooting with weapon 3*/
  if(shot_weapon(weapons[2], res) == FAILED){
      printf("Error when shooting the weapon\n");
      return -1;
  }

  
  /*Showing new stats*/
  printf("\nWeapon 1.\n");
  printf("%s ", weapon_getName(weapons[0]));
  printf("%d ", weapon_getPowderWaste(weapons[0]));
  printf("%d ", weapon_getSpeed(weapons[0]));
  printf("%d ", weapon_getDamage(weapons[0]));
  if(own_weapon(weapons[0])==OWNED) printf("OWNED");
  else printf("NOT OWNED");
  printf("\n\n");
  
  printf("\nWeapon 2.\n");
  printf("%s ", weapon_getName(weapons[1]));
  printf("%d ", weapon_getPowderWaste(weapons[1]));
  printf("%d ", weapon_getSpeed(weapons[1]));
  printf("%d ", weapon_getDamage(weapons[1]));
  if(own_weapon(weapons[1])==OWNED) printf("OWNED");
  else printf("NOT OWNED");
  printf("\n\n");
  
  printf("\nWeapon 3.\n");
  printf("%s ", weapon_getName(weapons[2]));
  printf("%d ", weapon_getPowderWaste(weapons[2]));
  printf("%d ", weapon_getSpeed(weapons[2]));
  printf("%d ", weapon_getDamage(weapons[2]));
  if(own_weapon(weapons[2])==OWNED) printf("OWNED");
  else printf("NOT OWNED");
  printf("\n\n");
  
  printf("\nWeapon 4.\n");
  printf("%s ", weapon_getName(weapons[3]));
  printf("%d ", weapon_getPowderWaste(weapons[3]));
  printf("%d ", weapon_getSpeed(weapons[3]));
  printf("%d ", weapon_getDamage(weapons[3]));
  if(own_weapon(weapons[3])==OWNED) printf("OWNED");
  else printf("NOT OWNED");
  printf("\n\n");
  
  
  
  /*Finishing the test deleting the memory allocated*/
  destroy_weapons(weapons);
  destroy_resources(res);
  
  return 0;
}