#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h> // Incluído para operações de arquivo
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

struct dados{
    char nome[50];
    int pontos;
    struct dados *next;
};

struct dados ordem;

int BAR_MIN_X = 35;
int BAR_MAX_X = 46;
int BAR_Y = MAXY - 1;

int x = 40, y = 21;
int incX = 1, incY = -1;

int cont_score = 0;

bool game_over = false;

int blocos[6][9];

char nome[100]; // Nome do jogador armazenado aqui

void inicializarSemente() {
    srand(time(NULL));
}

// Função para gerar um número aleatório entre 1 e 6
int gerarNumeroAleatorio() {
    return (rand() % 3) + 1;
}

// Função para preencher a matriz com números aleatórios de 1 a 6
void preencherMatriz() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 9; j++) {
            blocos[i][j] = gerarNumeroAleatorio();
        }
    }
}

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
    char ch;
    int i = 0;

    while ((ch = getchar()) != '\n' && i < sizeof(nome) - 1) {
        nome[i++] = ch;
        printf("%c", ch);
        screenUpdate();
    }
    nome[i] = '\0';
    screenGotoxy(30, 16);
    printf("                                                    ");
    
    screenGotoxy(27, 11);
    printf("Pressione ESPACO para iniciar!");
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

     if (BAR_MAX_X-2 < 82) {
        if (ch == 100) {

            screenGotoxy(BAR_MIN_X, 23);
            printf("         ");
            

            if (BAR_MAX_X+4 > 82){
                BAR_MIN_X += 3;
                BAR_MAX_X += 3;
            }
            else{
                BAR_MIN_X += 4;
                BAR_MAX_X += 4;
            }
            
            for (int i = BAR_MIN_X; i <= BAR_MAX_X; i++) {
                screenGotoxy(i, BAR_Y);
                printf("T");
            }
        }
    }

    if (BAR_MIN_X > 4) {
        if (ch == 97) {

            screenGotoxy(BAR_MAX_X - 3, 23);
            printf("         ");

            if (BAR_MAX_X == 85){
                BAR_MIN_X += (-3);
                BAR_MAX_X += (-3);
            }
            else{
                BAR_MIN_X += (-4);
                BAR_MAX_X += (-4);
            }
            for (int i = BAR_MIN_X; i <= BAR_MAX_X; i++) {
                screenGotoxy(i, BAR_Y);
                printf("T");
            }
        }
    }
}

void printBlocos() {
    screenSetColor(BLUE, DARKGRAY);

    screenGotoxy(4, 4);
    int y = 5;

    for (int l = 0; l < 6; l++) {
        for (int c = 0; c < 9; c++) {
            if (c + 1 != 9 && blocos[l][c] == 3) {
                printf("🟥🟥🟥🟥 ");
            } else if (blocos[l][c] == 3) {
                printf("🟥🟥🟥🟥");
            } else if (c + 1 != 9 && blocos[l][c] == 2) {
                printf("🟧🟧🟧🟧 ");
            } else if (blocos[l][c] == 2) {
                printf("🟧🟧🟧🟧");
            } else if (c + 1 != 9 && blocos[l][c] == 1) {
                printf("🟨🟨🟨🟨 ");
            } else if (blocos[l][c] == 1) {
                printf("🟨🟨🟨🟨");
            } else {
                printf("         ");
            }
        }
        screenGotoxy(4, y);
        y++;
    }
}

void ColisaoBloco(int ballX, int ballY, int x, int y) {
    if (y > ballY ){
         if (ballY >= 5 && ballY <= 10) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY - 5; // Ajuste para a nova posição dos blocos
            int blockCol = (ballX - 3) / 9;
            if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] != 0) {
                blocos[blockRow][blockCol] -= 1; // Remover bloco
                incY = -incY; // Inverter direção da bola
                printBlocos(); // Redesenhar blocos
            }
        }
    
    }
    else{
         if (ballY >= 3 && ballY <= 10) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY - 3; // Ajuste para a nova posição dos blocos
            int blockCol = (ballX - 3) / 9;
            if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] != 0) {
                blocos[blockRow][blockCol] -= 1; // Remover bloco
                incY = -incY; // Inverter direção da bola
                printBlocos(); // Redesenhar blocos
            }
        }
    }
}


void printScore() {
    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(3, 2);
    int cont_blocosq = 0;
    cont_score = 0;
    for (int l = 0; l < 6; l++) {
        for (int c = 0; c < 9; c++) {
            if (blocos[l][c] == 0) {
                cont_score = cont_score + 50;
            }
        }
    }
    printf("Score: %d", cont_score);
}

void salvarScoreNoArquivo(const char* nome, int score) {
    FILE* arquivo = fopen("scores.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "%s %d\n", nome, score);
        fclose(arquivo);
    } 
}


void LerDados(){

    FILE* arquivo = fopen("scores.txt", "r");
    char name[50];
    int pont;
    while(fscanf(arquivo, "%s %d", name, &pont) != EOF){


    }

}


void inserir(){

    
}

int main() 
{
    static int ch = 0;
    inicializarSemente();
    preencherMatriz();
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



    while (ch != 112) // Loop infinito
    {
        if (!game_over) 
        {
            if (timerTimeOver() == 1){
                printScore();
                screenUpdate();
                int newX = x + incX;
                int newY = y + incY;

                ColisaoBloco(newX, newY, x, y);

                if (newX >= (MAXX -strlen("⚪")-2) || newX <= MINX+2) incX = -incX;
                
                if (newY >= MAXY-1 || newY <= MINY+1) incY = -incY;

                if (newY == BAR_Y-1 && (newX >= BAR_MIN_X-2 && newX <= BAR_MAX_X+2)) {
                    incY = -incY;
                    if (newX < BAR_MIN_X + 4) {
                        incX = -1; // Mudar para a esquerda
                    } 
                    else if(newX > BAR_MIN_X +5){
                        incX = 1; // Mudar para a direita
                    } else {
                        incX = 0;
                        incY = -1;
                    }
                }

                
                printBall(newX, newY);

                if (keyhit()) {
                    ch = readch();
                    printBarra(ch);
                    screenUpdate();
                }

                if (y >= 23) {
                    screenGotoxy(37, 10);
                    screenSetColor(RED, DARKGRAY);
                    printf("FIM DE JOGO");
                    game_over = true;
                    salvarScoreNoArquivo(nome, cont_score); // Salvar o score no arquivo quando o jogo termina
                }
                
                screenUpdate();
            }
        }
        else // Se game_over for verdadeiro
        {
            // Mantenha o loop rodando para manter a tela congelada
            // mas não execute a lógica de atualização do jogo
            ch = readch();
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
