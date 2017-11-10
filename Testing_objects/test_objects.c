#include <stdio.h>
#include <stdlib.h>
#include "objects.h"

int main (){
    Object** ob;
    Resources** res;
    ob = load_objects("objects.txt");
    if(ob == NULL){
        printf("Error allocating memory for objects");
        return -1;
    }
    res = load_resources("resources.txt");
    if(res == NULL){
        destroy_objects(ob);
        printf("Error allocating memory for resources");
        return -1;
    }
    /*Checking all the information*/
  printf("Object 0\n");
  printf("%d ", object_getType(ob[0]));
  printf("%d ", object_getAmount(ob[0]));
  printf("%d ", object_getValue(ob[0]));
  printf("%s\n ", object_getName(ob[0]));

  
  printf("Object 1.\n");
  printf("%d ", object_getType(ob[1]));
  printf("%d ", object_getAmount(ob[1]));
  printf("%d ", object_getValue(ob[1]));
  printf("%s\n ", object_getName(ob[1]));

  
  printf("Object 2.\n");
  printf("%d ", object_getType(ob[2]));
  printf("%d ", object_getAmount(ob[2]));
  printf("%d ", object_getValue(ob[2]));
  printf("%s\n ", object_getName(ob[2]));

  
  printf("Object 3.\n");
  printf("%d ", object_getType(ob[3]));
  printf("%d ", object_getAmount(ob[3]));
  printf("%d ", object_getValue(ob[3]));
  printf("%s ", object_getName(ob[3]));
  printf("\n\n");
  
  /*Increasing amount of object 0*/
  if(object_changeAmount(ob[0], 3) == FAILED){
      printf("Fail at incrementing amount of object 0.\n");
      return -1;
  }
  
  /*Increasing amount of object 1*/
  if(object_changeAmount(ob[1], 4) == FAILED){
      printf("Fail at incrementing amount of object 1.\n");
      return -1;
  }
  
  /*Increasing amount of object 2*/
  if(object_changeAmount(ob[2], 5) == FAILED){
      printf("Fail at incrementing amount of object 2.\n");
      return -1;
  }
  
  /*Increasing amount of object 3*/
  if(object_changeAmount(ob[3], 6) == FAILED){
      printf("Fail at incrementing amount of object 3.\n");
      return -1;
  }
  
  /*Using object 0*/
  if(use_object(res, ob[0]) == FAILED){
      printf("Fail at using object 0.\n");
      return -1;
  }
  
  /*Using object 1*/
  if(use_object(res, ob[1]) == FAILED){
      printf("Fail at using object 1.\n");
      return -1;
  }
  
  /*Using object 2*/
  if(use_object(res, ob[2]) == FAILED){
      printf("Fail at using object 2.\n");
      return -1;
  }
  
  /*Using object 3*/
  if(use_object(res, ob[3]) == FAILED){
      printf("Fail at using object 3.\n");
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
  
  printf("Ob name: %s\n", object_getName(ob[0]));
  printf("Ob amount: %d\n", object_getAmount(ob[0]));
  
  printf("Ob name: %s\n", object_getName(ob[1]));
  printf("Ob amount: %d\n", object_getAmount(ob[1]));
  
  printf("Ob name: %s\n", object_getName(ob[2]));
  printf("Ob amount: %d\n", object_getAmount(ob[2]));
  
  printf("Ob name: %s\n", object_getName(ob[3]));
  printf("Ob amount: %d\n", object_getAmount(ob[3]));
  
  /*Finishing the test*/
  destroy_resources(res);
  destroy_objects(ob);

  return 0;
}