#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int BAR_MIN_X = 36;
int BAR_MAX_X = 46;
int BAR_Y = MAXY - 1;

int x = 40, y = 21;
int incX = 1, incY = -1;

bool game_over = false;

int blocos[6][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};

void telaInicial() {
    screenGotoxy(15, 3);
    screenSetColor(WHITE, DARKGRAY);
    printf(" ____   _____  ____   ___    ____ _    _  _____  _____");
    screenGotoxy(15, 4);
    printf("|  _ \\ |_   _|/ ___| / _ \\  / ___| |  | || ____||_   _|");
    screenGotoxy(15, 5);
    printf("| |_) |  | | | |    | | | || |   | |__| || |_     | |");
    screenGotoxy(15, 6);
    printf("|  _ <   | | | |    | | | || |   |  __  ||  _|    | |");
    screenGotoxy(15, 7);
    printf("| | | | _| |_| |___ | |_| || |___| |  | || |___   | |");
    screenGotoxy(15, 8);
    printf("|_| |_||_____ \\____| \\___/  \\____|_|  |_||_____|  |_|");

    screenGotoxy(27, 11);
    printf("Pressione ESPACO para inicar!");
    screenUpdate();

    while (1) {
        if (keyhit()) {
            int ch = readch();
            if (ch == ' ') {
                break;
            }
        }
    }
}

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
    int y = 4;

    for (int l=0; l<6; l++){
        for (int c=0; c<8; c++){
            if(c+1 != 8 && blocos[l][c] == 1){
            printf("[#######] ");
            }
            else if(blocos[l][c] == 1){
                printf("[#######]");
            }
            else{
                printf("          ");
            }
        }
        screenGotoxy(3, y);
        y++;
    }

}


void checkBallCollisionWithBlocks(int ballX, int ballY) {
    if (ballY >= 3 && ballY <= 8) { // Limites verticais dos blocos
        int blockRow = ballY - 3;
        int blockCol = (ballX - 3) / 10;
        if (blockCol >= 0 && blockCol < 8 && blocos[blockRow][blockCol] == 1) {
            blocos[blockRow][blockCol] = 0; // Remover bloco
            incY = -incY; // Inverter direção da bola
            printBlocos(); // Redesenhar blocos
        }
    }
}

int main() 
{
    static int ch = 0;

    screenInit(1);
    keyboardInit();
    timerInit(50);
    screenGotoxy(x, 22);
    printf("🔴");
    printBarra(ch);
    telaInicial();
    

    screenInit(1);
    keyboardInit();
    timerInit(50);

    printBall(x, y);
    printBlocos();
    screenUpdate();
    printBarra(ch);

    while (true) // Loop infinito
    {
        if (!game_over) 
        {
            if (timerTimeOver() == 1){
                int newX = x + incX;
                int newY = y + incY;
                checkBallCollisionWithBlocks(newX, newY);

                if (newX >= (MAXX -strlen("🔴")-1) || newX <= MINX+1) incX = -incX;
                
                if (newY >= MAXY-1 || newY <= MINY+1) incY = -incY;

                if (newY == BAR_Y-1 && (newX >= BAR_MIN_X-2 && newX <= BAR_MAX_X+2)) {
                    incY = -incY;
                    
                }
                
                printBall(newX, newY);

                if (keyhit()) {
                    ch = readch();
                    printBarra(ch);
                    screenUpdate();
                }

                if(y >= 23){
                    screenGotoxy(35, 10);
                    printf("FIM DE JOGO");
                    game_over = true;
                }
                
                screenUpdate();
            }
        }
        else // Se game_over for verdadeiro
        {
            // Mantenha o loop rodando para manter a tela congelada
            // mas não execute a lógica de atualização do jogo
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}