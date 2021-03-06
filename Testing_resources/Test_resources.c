#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resources.h"

int main(){
  Resources **res=NULL;

  res = load_resources("resources.txt");
  if(res == NULL){
    printf("Error when allocating memory for double resources´ pointer.\n");
    return -1;
  }
  
  /*Checking all the information*/
  printf("Res 1.\n");
  printf("%d ", resources_getObjectType(res[0]));
  printf("%s ", resources_getName(res[0]));
  printf("%d ", resources_getMax(res[0]));
  printf("%d ", resources_getActualValue(res[0]));
  printf("%d ", resources_getRow(res[0]));
  printf("%d ", resources_getCol(res[0]));

  
  printf("\nRes 2.\n");
  printf("%d ", resources_getObjectType(res[1]));
  printf("%s ", resources_getName(res[1]));
  printf("%d ", resources_getMax(res[1]));
  printf("%d ", resources_getActualValue(res[1]));
  printf("%d ", resources_getRow(res[1]));
  printf("%d ", resources_getCol(res[1]));

  
  printf("\nRes 3.\n");
  printf("%d ", resources_getObjectType(res[2]));
  printf("%s ", resources_getName(res[2]));
  printf("%d ", resources_getMax(res[2]));
  printf("%d ", resources_getActualValue(res[2]));
  printf("%d ", resources_getRow(res[2]));
  printf("%d ", resources_getCol(res[2]));

  
  printf("\nRes 4.\n");
  printf("%d ", resources_getObjectType(res[3]));
  printf("%s ", resources_getName(res[3]));
  printf("%d ", resources_getMax(res[3]));
  printf("%d ", resources_getActualValue(res[3]));
  printf("%d ", resources_getRow(res[3]));
  printf("%d ", resources_getCol(res[3]));
  printf("\n\n");
  
  /*Decreasing the actual value of HP*/
  if(modify_resource(res[0], -50) == ERROR){
      printf("Fail at modify res 0.\n");
      return -1;
  }
  
  /*Decreasing the actual value of HUNGER*/
  if(modify_resource(res[1], -40) == ERROR){
      printf("Fail at modify res 1.\n");
      return -1;
  }
  
  /*Decreasing the actual value of HYDRATION*/
  if(modify_resource(res[2], -30) == ERROR){
      printf("Fail at modify res 2.\n");
      return -1;
  }
  
  /*Decreasing the actual value of GUNPOWDER*/
  if(modify_resource(res[3], -20) == ERROR){
      printf("Fail at modify res 3.\n");
      return -1;
  }
  
  /*Increasing the actual value of GUNPOWDER*/
  if(modify_resource(res[3], 100) == ERROR){
      printf("Fail at modify res 0.\n");
      return -1;
  }
  
  /*printing the changes*/
  printf("Res name: %s\n", resources_getName(res[0]));
  printf("Res max value: %d\n", resources_getMax(res[0]));
  printf("Res actual value: %d\n\n", resources_getActualValue(res[0]));
  
  printf("Res name: %s\n", resources_getName(res[1]));
  printf("Res max value: %d\n", resources_getMax(res[1]));
  printf("Res actual value: %d\n\n", resources_getActualValue(res[1]));
  
  printf("Res name: %s\n", resources_getName(res[2]));
  printf("Res max value: %d\n", resources_getMax(res[2]));
  printf("Res actual value: %d\n\n", resources_getActualValue(res[2]));
  
  printf("Res name: %s\n", resources_getName(res[3]));
  printf("Res max value: %d\n", resources_getMax(res[3]));
  printf("Res actual value: %d\n\n", resources_getActualValue(res[3]));
  
  /*Finishing the test*/
  destroy_resources(res);

  return 0;
}
