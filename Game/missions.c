#include <stdio.h>
#include <dos.h>
#include <limits.h>
#include "missions.h"
#define N_IT 5


int genera_iteracionNotNot();


int NotNot (){
    int puntos=0, i=0, flag=0;
    printf("Welcome to NotNot a game for fast and smart people, you will need the arrow keys to play. Good Luck.\n");
    sleep (2000);
    for (i=0;i<N_IT;i++){
        flag=genera_iteracionNotNot();
        if (flag==1){
            puntos++;
        }else{
            break;
        }
    }
    printf("Congratulations you got %d points out of %d.\n", puntos, N_IT);
    return puntos;
}

int genera_iteracionNotNot(){
    int caso=0, flag=0;
    caso = 1 + (int)(((20) * (rand() / (RAND_MAX + 1.0)));
    /*Cuando se cumpla la condición que se pide pone la flag a 1*/
    switch (caso){
        case 1:
            printf("Not Not Right\n");
            /*Código que escaneee la flecha derecha*/
            break;
        case 2:
            printf("Not Not Not Left\n");
            /*Código que escanee la flecha dercha, arriba o abajo*/
            break;
        case 3:
            printf("Nothing\n");
            /*Si se pulsa una tecla falla*/
            break;
        case 4:
            printf("Not Not Nothing");
            /*Si se pulsa una tecla falla*/
            break;
        case 5:
            printf("Not Nothing\n");
            /*Debe escanear al menos una flecha*/
            break;
        case 6:
            printf("Not Not Up\n");
            /*Debe escanar la flecha arriba*/
            break;
        case 7 
            printf("Not Not Not Not Down\n");
            /*Debe escanar la flecha abajo*/
            break;
        case 8:
            printf("Not Not Nothing");
            /*Si se pulsa una tecla Falla*/
            break;
        case 9:
            printf("Right\n");
            /*Debe escanear la flecha derecha*/
            break;
        case 10:
            printf("Left\n");
            /*Debe escanear la flecha Izquierda*/
            break;
        case 11:
            printf("Down\n");
            /*Debe escanear la flecha abajo*/
            break;
        case 12:
            printf("Left\n");
            /*Debe escanear la flecha Arriba*/
            break;
        case 13:
            printf("Not Right\n");
            /*Debe no escanear la flecha derecha*/
            break;
        case 14:
            printf("Not Left\n");
            /*Debe no escanear la flecha Izquierda*/
            break;
        case 15:
            printf("Not Down\n");
            /*Debe no escanear la flecha abajo*/
            break;
        case 16:
            printf("Not Left\n");
            /*Debe no escanear la flecha Arriba*/
            break;
        case 17:
            printf("Not Not Not Down\n");
            /*Debe no escanear la flecha abajo*/
            break;
        case 18:
            printf("Not Not Not Nothing\n");
            /*Debe escanear algo*/
            break;
        case 19:
            printf("Not Not Not Not Not Not Up\n");
            /*Debe escanear Arriba*/
            break;
        case 20:
            printf("Not Not Not Not Not Not Not Not Not Not Right\n");
            /*Debe escanear Derecha*/
            break;
        default:
            break;
    }
        
    }
}

int genera_iteracionTrivial_EPS();

int Trivial_EPS (){
    int puntos=0, i=0, flag=0, N_IT=10;
    printf("Welcome to the EPS Trivial. I hope you Know all the answers. Good Luck\n");
    sleep (2000);
    for (i=0;i<N_IT;i++){
        flag=genera_iteracionTrivial_EPS();
        if (flag==1){
            puntos++;
        }else{
            break;
        }
    }
    printf("Congratulations you got %d points out of %d.\n", puntos, N_IT);
    return puntos;
}

int genera_iteracionTrivial_EPS(){
    int caso=0, flag=0;
    caso = 1 + (int)(((20) * (rand() / (RAND_MAX + 1.0)));
    /*Cuando se cumpla la condición que se pide pone la flag a 1*/
    switch (caso){
        case 1:
            printf("How Much Does it cost a full fat Waffle in the cafe?\na) 1,80€\nb) 2,00€\nc) 1,90€\nd) 500€(Totally worth it)\n");
            /*Código que escaneee la letra a*/
            break;
        case 2:
            printf("How many toilets are there in the EPS)\na) 1\nb) 3\nc) Any\nd) None of the rest are correct\n");
            /*Código que escanee la letra d*/
            break;
        case 3:
            printf("Which is the biggest classroom?\na) Aula 1\nb) Aula 2\nc) Aula 7\nd) The toilet\n");
            /*Codigo que escanee la letra a*/
            break;
        case 4:
            printf("How is the waiter of the cafeteria named?\na) Antonio José\nb) Fernando Manuel\nc) Alejandro Antonio\nd) Diego\n");
            /*Codigo que escanee la letra d*/
            break;
        case 5:
            printf("Wich is the favourite expression of Roberto Marabini?\na) So far so Good\nb) Just for Fun\nc) Come on Guys\nd) All are correct\n");
            /*Debe escanear la letra d*/
            break;
        case 6:
            printf("When was the EPS built?\na) 2010\nb) 1995\nc) 2001\nd) 3698\n");
            /*Debe escanear la letra c*/
            break;
        case 7 
            printf("Why does the cafe does not sell Gin-Tonics?\na) Because they are boring\nb) Because the Students are alredy Drunk\nc) Because the Teachers are alredy Drunk\nd) Because they are not allowed\n");
            /*Debe escanear la letra d*/
            break;
        case 8:
            printf("Is Alvaro Del Val a good teacher?\na) Yes\nb) Yes\nc) Yes\nd) None are right\n");
            /*Debe escanear la d*/
            break;
        case 9:
            printf("Which teacher is secretly Santa Claus?\na) Santini\nb) Torrea\nc) Dudu(He sometimes brings muffins)\nd) Marabini\n");
            /*Debe escanear la b*/
            break;
        case 10:
            printf("How many space do we have available in our lab account?\na) 1 GB\nb) 1 TB\nc) 2GB\nd) 3MB\n");
            /*Debe escanear la a*/
            break;
        case 11:
            printf("\n");
            /**/
            break;
        case 12:
            printf("\n");
            /**/
            break;
        case 13:
            printf("\n");
            /**/
            break;
        case 14:
            printf("\n");
            /**/
            break;
        case 15:
            printf("\n");
            /**/
            break;
        case 16:
            printf("\n");
            /**/
            break;
        case 17:
            printf("\n");
            /**/
            break;
        case 18:
            printf("\n");
            /**/
            break;
        case 19:
            printf("\n");
            /**/
            break;
        case 20:
            printf("\n");
            /**/
            break;
        default:
            break;
    }
        
    }
}