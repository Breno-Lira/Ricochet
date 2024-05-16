/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
*/

#include <string.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int BAR_MIN_X = 35;
int BAR_MAX_X = 45;
int BAR_Y = MAXY - 1;

int x = 34, y = 12;
int incX = 1, incY = 1;

// a = 97 e d = 100

void printBall(int nextX, int nextY)
{
    screenSetColor(RED, DARKGRAY);
    screenGotoxy(x, y);
    printf("    ");
    x = nextX;
    y = nextY;
    screenGotoxy(x, y);
    printf("🔴");
}

void printBarra(int ch) {
    screenSetColor(WHITE, DARKGRAY);
    for (int i = BAR_MIN_X; i <= BAR_MAX_X; i++) {
        screenGotoxy(i, BAR_Y);
        printf("T");
    }

    if(BAR_MAX_X < 81){
        if (ch == 100){

            screenGotoxy(BAR_MIN_X, 23);
            printf("         ");

            BAR_MIN_X +=4;
            BAR_MAX_X +=4;
            
            for (int i = BAR_MIN_X; i <= BAR_MAX_X; i++) {
            screenGotoxy(i, BAR_Y);
            printf("T");
            }
        }
    }

    if (BAR_MIN_X > 4){
        if (ch == 97){

            screenGotoxy(BAR_MAX_X-3, 23);
            printf("         ");

            BAR_MIN_X += (-4);
            BAR_MAX_X += (-4);
            
            for (int i = BAR_MIN_X; i <= BAR_MAX_X; i++) {
            screenGotoxy(i, BAR_Y);
            printf("T");
            }
        }
    }

}

void printBlocos(){

    screenSetColor(BLUE, DARKGRAY);

    screenGotoxy(3, 3);

    char blocos[6][8];
    int y = 4;

    for (int l=0; l<6; l++){
        for (int c=0; c<8; c++){
            if(c+1 != 8){
            printf("[#######] ");
            }
            else{
                printf("[#######]");
            }
        }
        screenGotoxy(3, y);
        y++;
    }



}
/*
 void printKey(int ch)
 {
     screenSetColor(YELLOW, DARKGRAY);
     screenGotoxy(35, 22);
     printf("Key code :");

     screenGotoxy(34, 23);
     printf("            ");

     if (ch == 27) screenGotoxy(36, 23);
     else screenGotoxy(39, 23);

     printf("%d ", ch);
     while (keyhit())
     {
         printf("%d ", readch());
     }
 }
 */

int main() 
{
    static int ch = 0;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    printBall(x, y);
    printBlocos();
    screenUpdate();

    while (ch != 10) //enter
    {
         //Handle user input
         /*if (keyhit()) 
         {
             ch = readch();
             printKey(ch);
             screenUpdate();
         }*/

        // Update game state (move elements, verify collision, etc)

        if (timerTimeOver() == 1){
            
            int newX = x + incX;
            if (newX >= (MAXX -strlen("🔴")-1) || newX <= MINX+1) incX = -incX;
            int newY = y + incY;
            if (newY >= MAXY-1 || newY <= MINY+1) incY = -incY;

        
            if (newY == BAR_Y-1 && (newX >= BAR_MIN_X-2 && newX <= BAR_MAX_X+2)) {
                incY = -incY;  // Inverte a direção vertical da bola
            }

             
            //printKey(ch);
            printBall(newX, newY);

                if (keyhit()) {
                ch = readch();
                printBarra(ch);
                screenUpdate();
            }


            if(y >= 23){
                screenGotoxy(35, 10);
                printf("FIM DE JOGO");
                
            }

            


            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}

 

 