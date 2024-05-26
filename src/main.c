#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h> // Incluído para operações de arquivo
#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <termios.h>
#include <unistd.h>

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

int modo;
int rodada = 1;
int contBC = 0;

int poderAleatorio = 0;

int x = 40, y = 21;
int incX = 1, incY = -1;

int x2 = 41, y2 = 20;
int incX2 = 1, incY2 = -1;

int x3 = 39, y3 = 20;
int incX3 = -1, incY3 = -1;

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

void inicializarSemente(); //Inicia semente para gerar numero aleatorio
int gerarNumeroAleatorio(); // Função para gerar um número aleatório entre 1 e 3
void preencherMatriz(); // Função para preencher a matriz com números aleatórios de 1 a 3
void telaInicial(); //Função que inicia a tela inicial
void printBall(int nextX, int nextY); // Função que printa a bola principal
void printBall2(int nextX2, int nextY2); // Função que printa a bola clone 1
void printBall3(int nextX3, int nextY3); // Função que printa a bola colne 2
void printBarra(int ch); // Função que printa a barra
void printBlocos(); // Função que printa  os Blocos
void gerarPoder(); // Função usanda para a chance de um poder aparecer
void pritandoPoder1(int nextXP, int nextYP); // Função para printar o icone do clone
void pritandoPoder2(int nextXP, int nextYP); // Função para printar o icone do overdrive
void pritandoPoder3(int nextXP, int nextYP); // Função para printar o icone do freeze
void ColisaoBloco(int ballX, int ballY, int x, int y); // Função para definir a colisão da bola principal com os blocos
void ColisaoBloco2(int ballX2, int ballY2, int x2, int y2); // Função para definir a colisão da bola clone 1 com os blocos
void ColisaoBloco3(int ballX3, int ballY3, int x3, int y3); // Função para definir a colisão da bola clone 2 com os blocos
void printScore(); // Função para calcular e printar o score na tela
void salvarScoreNoArquivo(const char* nome, int score); // Função para salvar o nome e score no arquivo txt
void LerDados();// Função que lê os dados dos arquivos txt
void inserir(struct dados **head, char *nome, int pontos); // Função que salva os dados no arquivo
void PrintDados(struct dados *head); //Função para printar na tela as informações presentes no arquivo txt
void contadorPoderes(); //Função de timer de cada poder
void set_input_mode(void); //Função usada para escolha de modo se automatizado

int main() 
{
    static int ch = 0; // iniciando a variavel ch 
    inicializarSemente(); //Chamando a função para inicair a seed
    preencherMatriz();//Preenchendo a matriz
    screenInit(1);//Iniciando a tela do jogo
    keyboardInit();//Iniciando as funções do teclado
    timerInit(50);//Iniciando a taxe de atualização da tela
    screenGotoxy(x, 22);
    printf("⚪");
    printBarra(ch);
    telaInicial();//Iniciando a tela inicial
    
    screenInit(1);
    keyboardInit();
    timerInit(50);

    printBall(x, y);
    printBlocos();
    screenUpdate();
    printBarra(ch);



    while (ch != 112) // Loop infinito para ao apertar o 'p'
{
    if (!game_over)//Caso game_over seja false
    {
        if (timerTimeOver() == 1){
            printScore();
            screenUpdate();

            if (modo == 2){
                screenGotoxy(36,2);//Informando o modo na parte de cima daa tela
                printf("Ilimitado rodada:                 ");
                screenGotoxy(36,2);
                printf("Infinito rodada: %d", rodada);
            }
            else{
                screenGotoxy(40,2);//Informando o modo na parte de cima daa tela
                printf("Normal");
            }

            screenGotoxy(86,23);
            printf("│");
           
            int newX = x + incX;//Atualizando a posição da bola principal
            int newY = y + incY;

            int newX2 = x2 + incX2;//Atualizando a posição da bola clone 1
            int newY2 = y2 + incY2;

            int newX3 = x3 + incX3;//Atualizando a posição da bola clone 2
            int newY3 = y3 + incY3;

            int newYpoder = ypoder + incYpoder;//Atualizando a posição dos icones dos poderes

            ColisaoBloco(newX, newY, x, y);
            ColisaoBloco2(newX2, newY2, x2, y2);
            ColisaoBloco3(newX3, newY3, x3, y3);
            
            
            if (newX >= (MAXX -strlen("⚪")-2) || newX <= MINX+2) incX = -incX;//Colisão com os lados da tela
            
            if (newY >= MAXY-1 || newY <= MINY+1) incY = -incY;//Colisão com o teto da tela

            if (newY == BAR_Y-1 && (newX >= BAR_MIN_X-2 && newX <= BAR_MAX_X+2)) {//Colisão com a barra
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

            if (newX3 >= (MAXX -strlen("⚪")-2) || newX3 <= MINX+2) incX3 = -incX3;
            
            if (newY3 >= MAXY-1 || newY3 <= MINY+1) incY3 = -incY3;

            if (newY3 == BAR_Y-1 && (newX3 >= BAR_MIN_X-2 && newX3 <= BAR_MAX_X+2)) {
                incY3 = -incY3;
                if (newX3 < BAR_MIN_X + 4) {
                    incX3 = -1; // Mudar para a esquerda
                } 
                else if(newX3 > BAR_MIN_X +5){
                    incX3 = 1; // Mudar para a direita
                } else {
                    incX3 = 0;
                    incY3 = -1;
                }
            }

            if (poderAleatorio == 1){
                if (newYpoder >= BAR_Y && (xpoder >= BAR_MIN_X-2 && xpoder <= BAR_MAX_X+2)) {//Condição para ativar o poder ao icone encostar na barra 
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

            if (newYpoder >= MAXY){//Fazendo o poder desaparecer caso o jogador não o pegue
                poderAleatorio = 0;
                screenGotoxy(xpoder,ypoder);
                printf("  ");
                poderEmqueda = false;
                ypoder = 2;
            }


            printBlocos();
            printBall(newX, newY);

        
            if (keyhit()) {//Lendo os inputs do jogador
                ch = readch();
                printBarra(ch);
                screenUpdate();
                //Cheats para teste de funcionalidades
                /*
                if (ch == 111){
                    b2 = true; 
                }
                else if (ch == 105){
                    overdrive = true; 
                }
                else if (ch == 117){
                    frost = true; 
                }
                else if (ch == 108){
                    for (int i = 0; i < 6; i++) {
                        for (int j = 0; j < 9; j++) {
                            if (i == 1 && j == 4){
                            blocos[i][j] = 1;
                            }
                            else{
                                blocos[i][j] = 0;
                            }
                        }
                    }
                }
                */
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
            printBall3(newX3, newY3);
            

            if (y >= 23) {//Mensagem de derrota caso a bola bata no chão
                screenGotoxy(37, 12);
                screenSetColor(RED, DARKGRAY);
                printf("FIM DE JOGO!");
                screenGotoxy(35, 14);
                if (rodada == 1){
                    printf("Score final: %d", cont_score);
                    game_over = true;//Acionando game_over como verdadeiro
                    salvarScoreNoArquivo(nome, cont_score); // Salvar o score no arquivo quando o jogo termina
                }
                else if (rodada > 1){
                    int cont_scoreINF = cont_score+((rodada-1)*2700);
                    printf("Score final: %d", cont_scoreINF);
                    game_over = true;
                    salvarScoreNoArquivo(nome, cont_scoreINF); // Salvar o score no arquivo quando o jogo termina
                }
                
            }

            if (modo == 1){//Menssagem de jogo concluido apos destruir todos os blocos
                if (cont_score == 2700){
                    screenGotoxy(37, 12);
                    screenSetColor(YELLOW, DARKGRAY);
                    printf("VOCÊ GANHOU!");
                    screenGotoxy(35, 14);
                    printf("Score final: %d", cont_score);
                    game_over = true;
                    salvarScoreNoArquivo(nome, cont_score); // Salvar o score no arquivo quando o jogo termina
                }
            }

            contBC = 0;
            for (int b=0; b < 6; b++){
                for(int c=0; c<9; c++){
                    contBC += blocos[b][c];//Contando a 'vida' restante dos blocos
                }
            }

            if (rodada > 1 && rodada < 4){//Aumentando a velocidade do jogo
                timerInit(45);
            }
            else if (rodada >= 4){
                timerInit(40);
            }

        
            if (modo == 2 && contBC <= 0){//Preenchendo a matriz novamente no modo infinito
                preencherMatriz();
                contBC = 0;
                rodada++;
                x = 40;
                y = 21;
                screenGotoxy(BAR_MIN_X, BAR_Y);
                printf("             ");
                BAR_MIN_X = 35;
                BAR_MAX_X = 46;
                printBarra(ch);
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

void inicializarSemente() {
    srand(time(NULL));
}

// Função para gerar um número aleatório entre 1 e 3
int gerarNumeroAleatorio() {
    return (rand() % 3) + 1;
}

// Função para preencher a matriz com números aleatórios de 1 a 3
void preencherMatriz() {
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 9; j++) {
            blocos[i][j] = gerarNumeroAleatorio();//Preenchendo matriz com numeros aleatorios
            contBC += blocos[i][j];
        }
    }
}

void telaInicial() {
    screenGotoxy(16, 3);
    screenSetColor(WHITE, DARKGRAY);
    printf(" ____   _____  ____   ___    ____ _    _  _____  _____");
    screenGotoxy(16, 4);
    printf("|  _ \\ |_   _|/ ___| / _ \\  / ___| |  | || ____||_   _|");
    screenGotoxy(16, 5);
    printf("| |_) |  | | | |    | | | || |   | |__| || |_     | |");
    screenGotoxy(16, 6);
    printf("|  _ <   | | | |    | | | || |   |  __  ||  _|    | |");
    screenGotoxy(16, 7);
    printf("| | | | _| |_| |___ | |_| || |___| |  | || |___   | |");
    screenGotoxy(16, 8);
    printf("|_| |_||_____ \\____| \\___/  \\____|_|  |_||_____|  |_|");

    
    screenGotoxy(30, 12);
    printf("Digite seu nome: ");
    char ch;
    int i = 0;
    

    while ((ch = getchar()) != '\n' && i < sizeof(nome) - 1) {//Recebendo o nome do usuario
        nome[i++] = ch;
        printf("%c", ch);
        screenUpdate();
    }
    nome[i] = '\0';
    screenGotoxy(30, 12);
    printf("                                                        ");

    screenGotoxy(28, 12);
    printf("Escolha o modo de jogo: ");

    screenGotoxy(28, 14);
    printf("(1)Modo Normal");
    screenGotoxy(28, 15);
    printf("(2)Modo infinito");
    

    char chModo = getchar();//Escolhendo o modo de jogo
    if (chModo == '2'){
        modo = 2;
    }
    else{
        modo = 1;
    }
    
    screenGotoxy(53, 12);
    printf("               ");

    screenGotoxy(28, 12);
    printf("                        ");
    screenGotoxy(28, 14);
    printf("                 ");
    screenGotoxy(28, 15);
    printf("                  ");

    
    screenGotoxy(29, 20);
    printf("Pressione ESPACO para iniciar!");
    if (modo == 2){
        screenGotoxy(32, 10);
        printf("PLACAR INFINITO🏆");
        LerDados();
        screenUpdate();
    }
    else{
        screenGotoxy(38, 10);
        printf("PLACAR NORMAL🏆");
        LerDados();
        screenUpdate();
    }

    while (1) {//Loop ocorre ate que o usuario aperte a SPACEBAR
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
        x2 = 41;
        y2 = 20;
    }
}

void printBall3(int nextX3, int nextY3){
    if ( b2 == true){
        screenSetColor(WHITE, DARKGRAY);
        screenGotoxy(x3, y3);
        printf("   ");
        x3 = nextX3;
        y3 = nextY3;
        screenGotoxy(x3, y3);
        printf("🟢");
    }
    if(b2 == false){
        screenGotoxy(x3, y3);
        printf("   ");
        x3 = 39;
        y3 = 20;
    }
}

void printBarra(int ch) {
    screenSetColor(WHITE, DARKGRAY);
    for (int i = BAR_MIN_X; i <= BAR_MAX_X; i++) {
        screenGotoxy(i, BAR_Y);
        printf("T");
    }

     if (BAR_MAX_X-2 < 82) {
        if (ch == 100) {//Movimento da barra para direita

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
                screenGotoxy(86,23);
                printf("│");
                screenGotoxy(i, BAR_Y);
                printf("T");
            }
        }
    }

    if (BAR_MIN_X > 4) {
        if (ch == 97) {//Movimento da barra para a esquerda

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
                screenGotoxy(86,23);
                printf("│");
                screenGotoxy(i, BAR_Y);
                printf("T");
            }
        }
    }
}

void printBlocos() {
    screenSetColor(BLUE, DARKGRAY);

    screenGotoxy(4, 4);
    int ybloco = 5;

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
        screenGotoxy(4, ybloco);
        ybloco++;
    }
}

void gerarPoder(){
        int powerup = rand() % 10 + 1;
        if (powerup <= 3){
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
    if(y > ballY){//Logica para colisão por baixo dos blocos
         if (ballY >= 5 && ballY <= 10) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY - 5; 
            int blockCol = (ballX - 3) / 9;
            if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] > 0) {
                if (overdrive == false && frost == false){
                    if (blocos[blockRow][blockCol] == 4){
                        blocos[blockRow][blockCol] = 0; // Remover bloco
                        incY = -incY;
                    }
                    else if (b2 == true){
                        blocos[blockRow][blockCol] -= 1; // Remover 1 vida do bloco
                        incY = -incY; // Inverter direção da bola
                    }
                    else{
                        blocos[blockRow][blockCol] -= 1; // Remover 1 vida do bloco
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
    else{//Logica para colisão por cima dos blocos
        if (ballY >= 3 && ballY <= 8) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY - 3; 
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
            int blockRow = ballY2 - 5; 
            int blockCol = (ballX2 - 3) / 9;
            if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] != 0) {
                if (blocos[blockRow][blockCol] == 4){
                        blocos[blockRow][blockCol] = 0; // Remover bloco
                        incY2 = -incY2;
                }
                else{
                    blocos[blockRow][blockCol] -= 1; // Remover bloco
                    incY2 = -incY2; // Inverter direção da bola
                    printBlocos(); // Redesenhar blocos
                }
            }
        }
    }
    else{
        if (ballY2 >= 3 && ballY2 <= 8) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY2 - 3; 
            int blockCol = (ballX2 - 3) / 9;
            if ((blockCol >= 0 || blockCol < 9) && blocos[blockRow][blockCol] > 0) {
                if (blocos[blockRow][blockCol] == 4){
                        blocos[blockRow][blockCol] = 0; // Remover bloco
                        incY2 = -incY2;
                }
                else{
                blocos[blockRow][blockCol] -= 1; // Remover bloco
                incY2 = -incY2; // Inverter direção da bola
                printBlocos(); // Redesenhar blocos
                }
            }
        }
    }
    
}

void ColisaoBloco3(int ballX3, int ballY3, int x3, int y3) {
    if(y3 > ballY3){
         if (ballY3 >= 5 && ballY3 <= 10) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY3 - 5; 
            int blockCol = (ballX3 - 3) / 9;
            if (blockCol >= 0 && blockCol < 9 && blocos[blockRow][blockCol] != 0) {
                if (blocos[blockRow][blockCol] == 4){
                        blocos[blockRow][blockCol] = 0; // Remover bloco
                        incY3 = -incY3;
                }
                else{
                blocos[blockRow][blockCol] -= 1; // Remover bloco
                incY3 = -incY3; // Inverter direção da bola
                printBlocos(); // Redesenhar blocos
                }
            }
        }
    }
    else{
        if (ballY3 >= 3 && ballY3 <= 8) { // Limites verticais ajustados para o espaço abaixo dos blocos
            int blockRow = ballY3 - 3; 
            int blockCol = (ballX3 - 3) / 9;
            if ((blockCol >= 0 || blockCol < 9) && blocos[blockRow][blockCol] > 0) {
                if (blocos[blockRow][blockCol] == 4){
                        blocos[blockRow][blockCol] = 0; // Remover bloco
                        incY3 = -incY3;
                }
                else{
                blocos[blockRow][blockCol] -= 1; // Remover bloco
                incY3 = -incY3; // Inverter direção da bola
                printBlocos(); // Redesenhar blocos
                }
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
    printf("Score:                ");
    screenGotoxy(3, 2);
    printf("Score: %d", cont_score);
}

void salvarScoreNoArquivo(const char* nome, int score) {
    if (modo == 2){
        FILE* arquivo2 = fopen("scoresInfinito.txt", "a");
        if (arquivo2 != NULL) {
            fprintf(arquivo2, "%s %d\n", nome, score);
            fclose(arquivo2);
        } 
    }
    else{
        FILE* arquivo = fopen("scores.txt", "a");
        if (arquivo != NULL) {
            fprintf(arquivo, "%s %d\n", nome, score);
            fclose(arquivo);
        }
    }
}


void LerDados(){
    if (modo == 2){
        struct dados *head = NULL;
        FILE* arquivo = fopen("scoresInfinito.txt", "r");
        char name[50];
        int pont;
        while(fscanf(arquivo, "%s %d", name, &pont) != EOF){
            inserir(&head, name, pont);
        }
        fclose(arquivo);
        PrintDados(head);
    }
    else{
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
}


void inserir(struct dados **head, char *nome, int pontos){
    struct dados *temp = *head;
    struct dados *novo = (struct dados *)malloc(sizeof(struct dados));
    strcpy(novo->nome, nome);
    novo->pontos = pontos;
    novo->next = NULL;
    if((*head) == NULL || (*head)->pontos <= novo->pontos){//Caso head esteja vazia ou caso seja menor ou igual ao novo valor
        novo->next = *head;
        *head = novo;
    }else{
        while(temp->next != NULL && temp->next->pontos > novo->pontos){//Percorrendo até achar a posição do novo valor
            temp = temp->next;
        }
        novo->next = temp->next;
        temp->next = novo;
    }
}

void PrintDados(struct dados *head){
    struct dados *temp = head;
    int cont = 0, a = 12;
    
    if(head == NULL){
        screenGotoxy(34,12);
        printf("Sem nenhum jogador!");
    }else{
        while(temp != NULL){
            if(cont < 5){
                if(cont == 0){
                    screenGotoxy(29, a);
                    printf("🥇 score: %d | %s\n", temp->pontos, temp->nome);
                }else if(cont == 1){
                    screenGotoxy(29, a);
                    printf("🥈 score: %d | %s\n", temp->pontos, temp->nome);
                }else if (cont == 2){
                    screenGotoxy(29, a);
                    printf("🥉 score: %d | %s\n", temp->pontos, temp->nome); 
                }
                else if (cont == 3){
                    screenGotoxy(29, a);
                    printf("😆 score: %d | %s\n",temp->pontos, temp->nome); 
                }
                else if (cont == 4){
                    screenGotoxy(29, a);
                    printf("😊 score: %d | %s\n",temp->pontos, temp->nome); 
                }
            }
            temp = temp->next;//Indo para o próximo colocado
            cont++;
            a++;
        }
    }
}

void contadorPoderes(){
    if (overdrive == true){
        contPoder++;
        if (contPoder > 70){
            overdrive = false;
            contPoder = 0;
        }
    }
    if (b2 == true){
        contPoder++;
        if (contPoder > 125){
            b2 = false;
            contPoder = 0;
        }
    }
}


void set_input_mode(void) {
    struct termios tattr;

    // Obter atributos do terminal
    tcgetattr(STDIN_FILENO, &tattr);
    // Desativar modo canônico e eco
    tattr.c_lflag &= ~(ICANON | ECHO);
    // Definir atributos do terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &tattr);
}