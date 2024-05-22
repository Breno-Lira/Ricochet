#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int BAR_MIN_X = 36;
int BAR_MAX_X = 48;
int BAR_Y = MAXY - 1;

int x = 40, y = 21;
int incX = 1, incY = -1;

bool game_over = false;

int blocos[6][9] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1}
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

    
    screenGotoxy(30, 16);
    printf("Digite seu nome: ");
    char nome[100], ch;
    int i=0;
    while (ch != '\n'){
        if (keyhit()) 
        {
            ch = readch();
            printf("%c", ch);
            screenUpdate();
            i++;
        }
    }
    screenGotoxy(30, 16);
    printf("                                                    ");
    
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
    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(x, y);
    printf("   ");
    x = nextX;
    y = nextY;
    screenGotoxy(x, y);
    printf("⚪");
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

    screenGotoxy(4, 3);
    int y = 4;

    for (int l=0; l<6; l++){
        for (int c=0; c<9; c++){
            if(c+1 != 9 && blocos[l][c] == 1){
            printf("🟦🟦🟦🟦 ");
            }
            else if(blocos[l][c] == 1){
                printf("🟦🟦🟦🟦");
            }
            else{
                printf("         ");
            }
        }
        screenGotoxy(4, y);
        y++;
    }

}


void checkBallCollisionWithBlocks(int ballX, int ballY) {
    if (ballY >= 3 && ballY <= 8) { // Limites verticais dos blocos
        int blockRow = ballY - 3;
        int blockCol = (ballX - 3) / 9;
        if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] == 1) {
            blocos[blockRow][blockCol] = 0; // Remover bloco
            incY = -incY; // Inverter direção da bola
            printBlocos(); // Redesenhar blocos
        }
    }
}

void printScore(){
    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(3, 2);
    int cont_blocosq = 0;
    int cont_score = 0;
    for(int l=0; l<6; l++){
        for(int c=0; c<9; c++){
            if(blocos[l][c] == 0){
                cont_score = cont_score +  50;
            }
        }
    }
    printf("Score: %d", cont_score);
}

int main() 
{
    static int ch = 0;


    screenInit(1);
    keyboardInit();
    timerInit(50);
    screenGotoxy(x, 22);
    printf("⚪");
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
                printScore();
                screenUpdate();
                int newX = x + incX;
                int newY = y + incY;
                checkBallCollisionWithBlocks(newX, newY);

                if (newX >= (MAXX -strlen("⚪")-2) || newX <= MINX+2) incX = -incX;
                
                if (newY >= MAXY-1 || newY <= MINY+1) incY = -incY;

                if (newY == BAR_Y-1 && (newX >= BAR_MIN_X-2 && newX <= BAR_MAX_X+2)) {
                    incY = -incY;
                    if (newX < BAR_MIN_X + 5) {
                        incX = -1; // Mudar para a esquerda
                    } else {
                        incX = 1; // Mudar para a direita
                    }
                    
                }
                
                printBall(newX, newY);

                if (keyhit()) {
                    ch = readch();
                    printBarra(ch);
                    screenUpdate();
                }

                if(y >= 23){
                    screenGotoxy(37, 10);
                    screenSetColor(RED, DARKGRAY);
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