#include <stdio.h>
#include <stdlib.h>

#define E '.' // ERVA
#define A 'A'
#define T 'T'
#define D '.' // DESCONHECIDO
#define max(a, b) a>b ? a : b
#define min(a, b) a>b ? a : b
#define CHECK_CANTO fscanf(fp, " %c", &c); if(c == 'T') return 1
#define CHECK_LADO fscanf(fp, " %c", &c); if(c == 'T') return 1; if(c == 'A') sem_arvores = 0
#define CHECK_CENTRO fscanf(fp, " %c", &c); if(c == 'A') return 1

//Macros para C recurrentes:
#define DIR 0
#define BAIXO 1
#define ESQ 2
#define CIMA 3

char **Matriz;
int L;
int C;
FILE *fp;

void InitSolver(FILE *fpointer, int Linhas, int Colunas) {
    fp = fpointer;
    L = Linhas;
    C = Colunas;
}


// Descrição:
// Argumentos:
// Retorno: 1 caso seja detetada a impossibilidade de existir uma tenda, 0 caso contrário.
int SolveBfromFile(int l0, int c0) {
    int i, j;
    int l0_tents, c0_tents;
    int somaC = 0, somaL = 0;
    char sem_arvores = 1;
    char c;

    if(l0 < 0 || l0 >= L || c0 < 0 || c0 >= C) {
        // Limpar os números antes de saír
        do {
            c = fgetc(fp);
        } while(c != 'A' && c != 'T' && c != '.');
        return -1;
    }

    // Ler vetor tendas por linha
    for(i=0; i<l0; i++) fscanf(fp, " %*d");
    fscanf(fp, " %d", &l0_tents);
    for(i=l0+1; i<L; i++) fscanf(fp, " %*d");

    // Ler vetor tendas por coluna
    for(i=0; i<c0; i++) fscanf(fp, " %*d");
    fscanf(fp, " %d", &c0_tents);
    for(i=c0+1; i<C; i++) fscanf(fp, " %*d");

    if(l0_tents == 0 || c0_tents == 0) return 1;

    // Ler matriz

    // Ler linhas antes dos adjacentes de (l0, c0)
    for(i=0; i < l0 - 1; i++) {
        for(j=0; j<c0; j++) fscanf(fp, " %*c");
        fscanf(fp, " %c", &c);
        if(c == 'T')
            if(++somaC > c0_tents) return 1;
        for(j=c0+1; j<C; j++) fscanf(fp, " %*c");
    }
    if(l0 != 0) {
        // Ler l0 - 1
        if(c0 != 0) {
            for(j = 0; j < c0 - 1; j++) fscanf(fp, " %*c");
            CHECK_CANTO;
        }
        CHECK_LADO;
        if(c0 != C - 1)  {
            CHECK_CANTO;
            for(j =c0 + 2; j < C; j++) fscanf(fp, " %*c");
        }
    }

    // Ler l0
    if(c0 != 0) {
        for(j = 0; j < c0 - 1; j++) {
            fscanf(fp, " %c", &c);
            if(c == 'T')
                if(++somaL > l0_tents) return 1;
        }
        CHECK_LADO;
    }
    CHECK_CENTRO;
    if(c0 != C - 1)  {
        CHECK_LADO;
        for(j =c0 + 2; j < C; j++) {
            fscanf(fp, " %c", &c);
            if(c == 'T')
                if(++somaL > l0_tents) return 1;
        }
    }

    if(l0 != L - 1) {
        // Ler l0 + 1
        if(c0 != 0) {
            for(j = 0; j < c0 - 1; j++) fscanf(fp, " %*c");
            CHECK_CANTO;
        }
        CHECK_LADO;

        if(sem_arvores == 1) return 1;
        if(c0 != C - 1)  {
            CHECK_CANTO;
            for(j =c0 + 2; j < C; j++) fscanf(fp, " %*c");
        }
    }

    if(sem_arvores == 1) return 1;

    // Ler linhas depois dos adjacentes de (l0, c0)
    for(i=l0+2; i < L; i++) {
        for(j=0; j<c0; j++) fscanf(fp, " %*c");
        fscanf(fp, " %c", &c);
        if(c == 'T')
            if(++somaC > c0_tents) return 1;
        for(j=c0+1; j<C; j++) fscanf(fp, " %*c");
    }
    return 0;
}

// Descrição: Determina se o problema é admissível, segundo os critérios de A, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de 'A', número de linhas e número de colunas.
// Retorno: 0 caso seja detetada a inadmissibilidade do problema, 1 caso contrário.
int SolveAfromFile() {
    int i, j;
    int somaL = 0, somaC = 0;
    int arvores = 0;
    char c;

    for(i=0; i<L; i++) {
        fscanf(fp, " %d", &j);
        somaL += j;
    }

    for(i=0; i<C; i++) {
        fscanf(fp, "%d", &j);
        somaC += j;
    }
    /* Verifica admissiblidade do jogo */
    if(somaC != somaL) return 0;

    for ( i = 0; i < L; i++) {
        for ( j = 0; j < C; j++) {
            fscanf(fp, " %c", &c);
            if(c=='A') arvores++;
        }
    }

    /* Verifica admissiblidade do jogo */
    if(somaL > arvores) return 0;

    return 1;

}

// Descrição: Determina se esta tenda possui árvores adjacentes disponíveis.
// Argumentos: Linha e coluna da tenda.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
char T_sozinha(unsigned char l0, unsigned char c0) {
    Matriz[l0][c0] = '.';
    // Ver a direita
    if(c0!=C-1) {
        if(Matriz[l0][c0+1] == 'A') {
            Matriz[l0][c0+1] = '.';
            if(c0==C-2)
                return 'N';
            if(Matriz[l0][c0+2] != 'T')
                return 'N';
            if(T_sozinha(l0, c0+2) == 'N')
                return 'N';
        }
    }
    // Ver em baixo
    if(l0!=L-1) {
        if(Matriz[l0+1][c0] == 'A') {
            Matriz[l0+1][c0] = '.';
            if(l0==L-2)
                return 'N';
            if(Matriz[l0+2][c0] != 'T')
                return 'N';
            if(T_sozinha(l0+2, c0) == 'N')
                return 'N';
        }
    }
    // Ver a esquerda
    if(c0!=0) {
        if(Matriz[l0][c0-1] == 'A') {
            Matriz[l0][c0-1] = '.';
            if(c0==1)
                return 'N';
            if(Matriz[l0][c0-2] != 'T')
                return 'N';
            if(T_sozinha(l0, c0-2) == 'N')
                return 'N';
        }
    }
    // Ver em cima
    if(l0!=0) {
        if(Matriz[l0-1][c0] == 'A') {
            Matriz[l0-1][c0] = '.';
            if(l0==1)
                return 'N';
            if(Matriz[l0-2][c0] != 'T')
                return 'N';
            if(T_sozinha(l0-2, c0) == 'N')
                return 'N';
        }
    }

    return 'S';
}


// Descrição: Cria uma matriz para o mapa e determina se há tendas ilegais, no que toca a somatórios por
//            linhas/colunas e adjacência entre tendas, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para o início da matriz, número de linhas, número de colunas
//             vetores de tendas, por linhas e por colunas, e endereço onde escrever o endereço da matriz criada.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
int VerSomasAdjacentes_fromFile(int *Ltents, int *Ctents) {
    int i, j;

    Matriz = (char**) malloc(L*sizeof(char*));
    if(Matriz == NULL) return -1;

    // linha 0
    Matriz[0] = (char*) malloc(C*sizeof(char));
    if(Matriz[0] == NULL) return -1;
    // célula (0, 0)
    fscanf(fp, " %c", &Matriz[0][0]);
    if(Matriz[0][0] == 'T') {
        if(--Ltents[0] < 0) return 1;
        if(--Ctents[0] < 0) return 1;
    }
    // Resto da linha 0
    for ( j = 1; j < C; j++) {
        fscanf(fp, " %c", &Matriz[0][j]);
        if(Matriz[0][j] == 'T') {
            // Ver tendas adjacentes na parte já lida da matriz.
            if(Matriz[0][j-1] == 'T')
                return 1;
            if(--Ltents[0] < 0) return 1;
            if(--Ctents[j] < 0) return 1;
        }
    }

    // Resto das linhas
    for ( i = 1; i < L; i++) {
        Matriz[i] = (char*) malloc(C*sizeof(char));
        if(Matriz[i] == NULL) return -1;
        // célula (i, 0)
        fscanf(fp, " %c", &Matriz[i][0]);
        if(Matriz[i][0] == 'T') {
            // Ver tendas adjacentes na parte já lida da matriz.
            if( Matriz[i-1][0] == 'T' || Matriz[i-1][1] == 'T')
                return 1;
            if(--Ltents[i] < 0) return 1;
            if(--Ctents[0] < 0) return 1;
        }
        // Resto da linha i excepto última
        for ( j = 1; j < C - 1; j++) {
            fscanf(fp, " %c", &Matriz[i][j]);

            if(Matriz[i][j] == 'T') {
                // Ver tendas adjacentes na parte já lida da matriz.
                if(Matriz[i-1][j-1] == 'T' || Matriz[i-1][j] == 'T' ||
                        Matriz[i-1][j+1] == 'T' || Matriz[i][j-1] == 'T')
                    return 1;
                if(--Ltents[i] < 0) return 1;
                if(--Ctents[j] < 0) return 1;
            }
        }
        // célula (i, C-1)
        fscanf(fp, " %c", &Matriz[i][C-1]);
        if(Matriz[i][C-1] == 'T') {
            // Ver tendas adjacentes na parte já lida da matriz.
            if(Matriz[i-1][C-2] == 'T' || Matriz[i-1][C-1] == 'T' ||
                    Matriz[i][C-2] == 'T')
                return 1;
            if(--Ltents[i] < 0) return 1;
            if(--Ctents[C-1] < 0) return 1;
        }
    }
    return 0;
}

// Descrição: Determina se há tendas ilegais, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de 'C', número de linhas e número de colunas.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
int SolveCfromFile() {
    int i, j;
    int * Ltents, *Ctents;
    int res;



    Ltents = (int*) malloc(L*sizeof(int));
    if(Ltents == NULL) return -1;
    Ctents = (int*) malloc(C*sizeof(int));
    if(Ctents == NULL) return -1;

    for(i=0; i<L; i++) {
        fscanf(fp, " %d", &Ltents[i]);
    }
    for(i=0; i<C; i++) {
        fscanf(fp, " %d", &Ctents[i]);
    }

    /* Carregar mapa e avaliar somas e tendas adjacentes */
    res = VerSomasAdjacentes_fromFile(Ltents, Ctents);
    if(res != 0) return res;

    for(i=0; i<L; i++) {
        for(j=0; j<C; j++) {
            if(Matriz[i][j] == 'T') {
                if(T_sozinha(i, j) == 'S') return 1;
            }
        }
    }
    return 0;
}


