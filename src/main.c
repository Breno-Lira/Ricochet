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

void LerDados();
void inserir(struct dados **head, char *nome, int pontos);
void PrintDados(struct dados *head);

struct dados ordem;

int BAR_MIN_X = 35;
int BAR_MAX_X = 46;
int BAR_Y = MAXY - 1;

int poderAleatorio = 0;

int x = 40, y = 21;
int incX = 1, incY = -1;

int x2 = 40, y2 = 20;
int incX2 = 1, incY2 = -1;

int xpoder = 20, ypoder = 12, tempoder;
int incXpoder = 0, incYpoder = 1;

int cont_score = 0;
int contPoder = 0;
int contPoderV = 0;
int contPoderV2 = 0;

bool game_over = false;
bool b2 = false;
bool overdrive = false;
bool frost = false;
bool poderEmqueda = false;

int blocos[6][9];

char nome[100]; // Nome do jogador armazenado aqui

void inicializarSemente() {
    srand(time(NULL));
}

// Função para gerar um número aleatório entre 1 e 3
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
    screenGotoxy(40, 13);
    printf("PÓDIO 🏆");
    LerDados();
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

void printBall(int nextX, int nextY){
    screenSetColor(WHITE, DARKGRAY);
    screenGotoxy(x, y);
    printf("   ");
    x = nextX;
    y = nextY;
    screenGotoxy(x, y);
    if (overdrive == false && frost == false){
        printf("⚪");
    }
    else if (overdrive == true){
        printf("🔴");
    }
    else if (frost == true){
        printf("🔵");
    } 
}

void printBall2(int nextX2, int nextY2){
    if ( b2 == true){
        screenSetColor(WHITE, DARKGRAY);
        screenGotoxy(x2, y2);
        printf("   ");
        x2 = nextX2;
        y2 = nextY2;
        screenGotoxy(x2, y2);
        printf("🟢");
    }
    if(b2 == false){
        screenGotoxy(x2, y2);
        printf("   ");
        x2 = 40;
        y2 = 20;
    }
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
            }
            else if (c + 1 != 9 && blocos[l][c] == 4) {
                printf("🟦🟦🟦🟦 ");
            }
            else if (blocos[l][c] == 4) {
                printf("🟦🟦🟦🟦");
            }
            else {
                printf("         ");
            }
        }
        screenGotoxy(4, y);
        y++;
    }
}

void gerarPoder(){
        int powerup = rand() % 10 + 1;
        screenGotoxy(40,2);
        printf("  ");
        printf("%d", powerup);
        if (powerup <= 5){
            poderAleatorio = rand() % 3 + 1;
            poderEmqueda = true;
        }

}


void pritandoPoder1(int nextXP, int nextYP){
        if (b2 == false){
            screenSetColor(WHITE, DARKGRAY);
            screenGotoxy(xpoder, ypoder);
            printf("   ");
            contPoderV++;
            if (contPoderV >= contPoderV2){
                ypoder = nextYP;
                contPoderV2 += 5;
            }
            screenGotoxy(xpoder, ypoder);
            printf("🧪");
        }
        else{
            poderEmqueda = false;
        }

}

void pritandoPoder2(int nextXP, int nextYP){
        if (overdrive == false){
        screenSetColor(WHITE, DARKGRAY);
        screenGotoxy(xpoder, ypoder);
        printf("   ");
        contPoderV++;
        if (contPoderV >= contPoderV2){
        ypoder = nextYP;
        contPoderV2 += 5;
        }
        screenGotoxy(xpoder, ypoder);
        printf("💥");
        }
        else{
            poderEmqueda = false;
        }

}

void pritandoPoder3(int nextXP, int nextYP){
        if (overdrive == false){
        screenSetColor(WHITE, DARKGRAY);
        screenGotoxy(xpoder, ypoder);
        printf("   ");
        contPoderV++;
        if (contPoderV >= contPoderV2){
        ypoder = nextYP;
        contPoderV2 += 5;
        }
        screenGotoxy(xpoder, ypoder);
        printf("❄️");
        }
        else{
            poderEmqueda = false;
        }

}

void ColisaoBloco(int ballX, int ballY, int x, int y) {
    if(y > ballY){
         if (ballY >= 5 && ballY <= 10) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY - 5; // Ajuste para a nova posição dos blocos
            int blockCol = (ballX - 3) / 9;
            if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] > 0) {
                if (overdrive == false && frost == false){
                    if (blocos[blockRow][blockCol] == 4){
                        blocos[blockRow][blockCol] = 0; // Remover bloco
                        incY = -incY;
                    }
                    else if (b2 == true){
                        blocos[blockRow][blockCol] -= 1; // Remover bloco
                        incY = -incY; // Inverter direção da bola
                    }
                    else{
                        blocos[blockRow][blockCol] -= 1; // Remover bloco
                        incY = -incY; // Inverter direção da bola
                        if (blocos[blockRow][blockCol] == 0){
                            if (poderEmqueda == false){
                            xpoder = x;
                            ypoder = y;
                            gerarPoder();
                            }
                        }
                    }
                printBlocos(); // Redesenhar blocos
                }
                else if (overdrive == true){
                    if (blocos[blockRow][blockCol] == 3){
                        blocos[blockRow][blockCol] = 1;
                        incY = -incY;
                    }
                    else{
                        blocos[blockRow][blockCol] = 0;
                    }
                    printBlocos();
                }
                else if (frost == true){
                    for(int a = 0; a < 9; a++ ){
                        if (!(blocos[blockRow][a] == 0)){
                            blocos[blockRow][a] = 4;
                        }
                        incY = -incY;
                        frost = false;
                    }
                    printBlocos();
                }
                
            }
        }
    }
    else{
        if (ballY >= 3 && ballY <= 8) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY - 3; // Ajuste para a nova posição dos blocos
            int blockCol = (ballX - 3) / 9;
            if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] > 0) {
                if (overdrive == false && frost == false){
                    if (blocos[blockRow][blockCol] == 4){
                        blocos[blockRow][blockCol] = 0; // Remover bloco
                        incY = -incY;
                    }
                    else{
                        blocos[blockRow][blockCol] -= 1; // Remover bloco
                        incY = -incY; // Inverter direção da bola
                    }
                printBlocos(); // Redesenhar blocos
                }
                else if (overdrive == true){
                    if (blocos[blockRow][blockCol] == 3){
                        blocos[blockRow][blockCol] = 1;
                        incY = -incY;
                    }
                    else{
                        blocos[blockRow][blockCol] = 0;
                    }
                    printBlocos();
                }
                else if (frost == true){
                    for(int a = 0; a < 9; a++ ){
                        if (!(blocos[blockRow][a] == 0)){
                            blocos[blockRow][a] = 4;
                        }
                        incY = -incY;
                        frost = false;
                    }
                    printBlocos();
                }
                
            }
        }
    
    }
}   

void ColisaoBloco2(int ballX2, int ballY2, int x2, int y2) {
    if(y2 > ballY2){
         if (ballY2 >= 5 && ballY2 <= 10) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY2 - 5; // Ajuste para a nova posição dos blocos
            int blockCol = (ballX2 - 3) / 9;
            if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] != 0) {
                blocos[blockRow][blockCol] -= 1; // Remover bloco
                incY2 = -incY2; // Inverter direção da bola
                printBlocos(); // Redesenhar blocos
            }
        }
    }
    else{
        if (ballY2 >= 3 && ballY2 <= 8) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY2 - 3; // Ajuste para a nova posição dos blocos
            int blockCol = (ballX2 - 3) / 9;
            if ((blockCol >= 0 || blockCol < 9) && blocos[blockRow][blockCol] > 0) {
                blocos[blockRow][blockCol] -= 1; // Remover bloco
                incY2 = -incY2; // Inverter direção da bola
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
    struct dados *head = NULL;
    FILE* arquivo = fopen("scores.txt", "r");
    char name[50];
    int pont;
    while(fscanf(arquivo, "%s %d", name, &pont) != EOF){
        inserir(&head, name, pont);
    }
    fclose(arquivo);
    PrintDados(head);
}


void inserir(struct dados **head, char *nome, int pontos){
    struct dados *temp = *head;
    struct dados *novo = (struct dados *)malloc(sizeof(struct dados));
    strcpy(novo->nome, nome);
    novo->pontos = pontos;
    novo->next = NULL;
    if((*head) == NULL || (*head)->pontos < novo->pontos){
        novo->next = *head;
        *head = novo;
    }else{
        while(temp->next != NULL && temp->next->pontos > novo->pontos){
            temp = temp->next;
        }
        novo->next = temp->next;
        temp->next = novo;
    }
}

void PrintDados(struct dados *head){
    struct dados *temp = head;
    int cont = 0, a = 14;
    if(head == NULL){
        printf("Sem nenhum jogador!");
    }else{
        while(temp != NULL){
            if(cont < 3){
                if(cont == 0){
                    screenGotoxy(29, a);
                    printf("🥇 %s | score: %d\n", temp->nome, temp->pontos);
                }else if(cont == 1){
                    screenGotoxy(29, a);
                    printf("🥈 %s | score: %d\n", temp->nome, temp->pontos);
                }else{
                    screenGotoxy(29, a);
                    printf("🥉 %s | score: %d\n", temp->nome, temp->pontos);
                }
            }
            temp = temp->next;
            cont++;
            a++;
        }
    }
}

void contadorPoderes(){
    if (overdrive == true){
        contPoder++;
        if (contPoder > 45){
            overdrive = false;
            contPoder = 0;
        }
    }
    if (b2 == true){
        contPoder++;
        if (contPoder > 100){
            b2 = false;
            contPoder = 0;
        }
    }


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

            int newX2 = x2 + incX2;
            int newY2 = y2 + incY2;

            int newYpoder = ypoder + incYpoder;

            ColisaoBloco(newX, newY, x, y);
            ColisaoBloco2(newX2, newY2, x2, y2);

            
            
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

            if (newX2 >= (MAXX -strlen("⚪")-2) || newX2 <= MINX+2) incX2 = -incX2;
            
            if (newY2 >= MAXY-1 || newY2 <= MINY+1) incY2 = -incY2;

            if (newY2 == BAR_Y-1 && (newX2 >= BAR_MIN_X-2 && newX2 <= BAR_MAX_X+2)) {
                incY2 = -incY2;
                if (newX2 < BAR_MIN_X + 4) {
                    incX2 = -1; // Mudar para a esquerda
                } 
                else if(newX2 > BAR_MIN_X +5){
                    incX2 = 1; // Mudar para a direita
                } else {
                    incX2 = 0;
                    incY2 = -1;
                }
            }

            if (poderAleatorio == 1){
                if (newYpoder >= BAR_Y && (xpoder >= BAR_MIN_X-2 && xpoder <= BAR_MAX_X+2)) {
                    b2 = true;
                    poderEmqueda = false;
                    screenGotoxy(xpoder,ypoder);
                    printf("  ");
                    ypoder = 1;
                }
            }
            
            else if (poderAleatorio == 2){
                if (newYpoder >= BAR_Y && (xpoder >= BAR_MIN_X-2 && xpoder <= BAR_MAX_X+2)) {
                    overdrive = true;
                    poderEmqueda = false;
                    screenGotoxy(xpoder,ypoder);
                    printf("  ");
                    ypoder = 1;
                }
            }

            else if (poderAleatorio == 3){
                if (newYpoder >= BAR_Y && (xpoder >= BAR_MIN_X-2 && xpoder <= BAR_MAX_X+2)) {
                    frost = true;
                    poderEmqueda = false;
                    screenGotoxy(xpoder,ypoder);
                    printf("  ");
                    ypoder = 1;
                }
            }

            if (newYpoder >= MAXY){
                poderAleatorio = 0;
                screenGotoxy(xpoder,ypoder);
                printf("  ");
                poderEmqueda = false;
                ypoder = 1;
            }

            screenGotoxy(40, 3);
            printf("contador: %d", contPoder);
            

            printBlocos();
            printBall(newX, newY);

        
            if (keyhit()) {
                ch = readch();
                printBarra(ch);
                screenUpdate();
                if (ch == 111){
                    b2 = true; /*ideia para a bola extra ⏺️*/
                }
                else if (ch == 105){
                    overdrive = true; /*ideia para overdrive 🔥/💥  105 ~= 5s */
                }
                else if (ch == 117){
                    frost = true; /*tecla n para der game over*/
                }
            }

            contadorPoderes();
            
            if (poderEmqueda == true && poderAleatorio == 1){
                pritandoPoder1(xpoder, newYpoder);
            }
            if (poderEmqueda == true && poderAleatorio == 2){
                pritandoPoder2(xpoder, newYpoder);
            }
            if (poderEmqueda == true && poderAleatorio == 3){
                pritandoPoder3(xpoder, newYpoder);
            }
        
            printBall2(newX2, newY2);
            

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
