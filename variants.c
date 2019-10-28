#include <stdio.h>
#include <stdlib.h>

#define E 1 // ERVA
#define A 2
#define T 3
#define D 4 // DESCONHECIDO
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


int ver_adjacentes(char **camp, int L, int C, int l0, int c0, int ha_tendas) {
    char sem_arv = 1;
    if(ha_tendas) {
        // -------------------  Ver lados
        if(l0 > 0) {
            if(camp[l0-1][c0] == T) return 1;
            if(camp[l0-1][c0] == A) sem_arv = 0;
        }
        if(l0 < L-1) {
            if(camp[l0+1][c0] == T) return 1;
            if(camp[l0+1][c0] == A) sem_arv = 0;
        }
        if(c0 > 0) {
            if(camp[l0][c0-1] == T) return 1;
            if(camp[l0][c0-1] == A) sem_arv = 0;
        }
        if(c0 < C-1) {
            if(camp[l0][c0+1] == T) return 1;
            if(camp[l0][c0+1] == A) sem_arv = 0;
        }
        if(sem_arv) return 1;
        // -------------------  Ver diagonais
        if(l0>0 && c0>0) {
            if(camp[l0-1][c0-1] == T)
                return 1;
        }
        if(l0>0 && c0<C-1) {
            if(camp[l0-1][c0+1] == T)
                return 1;
        }
        if(l0<L-1 && c0>0) {
            if(camp[l0+1][c0-1] == T)
                return 1;
        }
        if(l0<L-1 && c0<C-1) {
            if(camp[l0+1][c0+1] == T)
                return 1;
        }
        return 0;
    } else {
        // -------------------  Ver lados
        if(l0 > 0) {
            if(camp[l0-1][c0] == A) sem_arv = 0;
        }
        if(l0 < L-1) {
            if(camp[l0+1][c0] == A) sem_arv = 0;
        }
        if(c0 > 0) {
            if(camp[l0][c0-1] == A) sem_arv = 0;
        }
        if(c0 < C-1) {
            if(camp[l0][c0+1] == A) sem_arv = 0;
        }
        return sem_arv;
    }
}

// Descrição:
// Argumentos:
// Retorno: 1 caso seja detetada a impossibilidade de existir uma tenda, 0 caso contrário.
int SolveBfromMatrix(char **camp, int L, int C, int l0, int c0, int l0_t, int c0_t, int ha_tendas) {
    int i, sum;
    if(camp[l0][c0] == A || l0 == 0 || c0 == 0) return 1;

    if(ver_adjacentes(camp, L, C, l0, c0, ha_tendas) == 1) return 1;
    if(ha_tendas) {
        // -------------------  Ver linha
        sum = 1;
        for(i=0; i < l0; i++) {
            if(camp[i][c0] == T) {
                if(sum == l0_t) return 1;
                sum++;
            }
        }
        for(i=l0+1; i < L; i++) {
            if(camp[i][c0] == T) {
                if(sum == l0_t) return 1;
                sum++;
            }
        }

        // -------------------  Ver coluna
        sum = 1;
        for(i=0; i < c0; i++) {
            if(camp[l0][i] == T) {
                if(sum == c0_t) return 1;
                sum++;
            }
        }
        for(i=c0+1; i < C; i++) {
            if(camp[l0][i] == T) {
                if(sum == c0_t) return 1;
                sum++;
            }
        }
    }
    return 0;
}

// Descrição:
// Argumentos:
// Retorno: 1 caso seja detetada a impossibilidade de existir uma tenda, 0 caso contrário.
int SolveBfromFile(FILE *fp, int L, int C, int l0, int c0) {
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
        return 1;
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
int SolveAfromFile(FILE *fp, int L, int C) {
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
            if(c==A) arvores++;
        }
    }

    /* Verifica admissiblidade do jogo */
    if(somaL > arvores) return 0;

    return 1;

}

char T_sozinha(char** matriz, int L, int C, int l0, int c0);

// Descrição: Determina se esta árvore pode ser utilizada pela tenda no sentido oposto a direcao.
// Argumentos: Matriz com o mapa, número de linhas e número de colunas, .
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
char A_disponivel(char **matriz, int L, int C, unsigned char l0, unsigned char c0, char direcao) {
    matriz[l0][c0] = '.';
    switch(direcao) {
    case DIR:
        if(c0!=C-1) {
            if(matriz[l0][c0+1] == 'T') {
                if(T_sozinha(matriz, L, C, l0, c0+1) == 'S')
                    return 'N';
            }
        }
        break;
    case BAIXO:
        if(l0!=L-1) {
            if(matriz[l0+1][c0] == 'T') {
                if(T_sozinha(matriz, L, C, l0+1, c0) == 'S')
                    return 'N';
            }
        }
        break;
    case ESQ:
        if(c0!=0) {
            if(matriz[l0][c0-1] == 'T') {
                if(T_sozinha(matriz, L, C, l0, c0-1) == 'S')
                    return 'N';
            }
        }
        break;
    case CIMA:
        if(l0!=0) {
            if(matriz[l0-1][c0] == 'T') {
                if(T_sozinha(matriz, L, C, l0-1, c0) == 'S')
                    return 'N';
            }
        }
        break;
    }
    return 'S';
}

// Descrição: Determina se esta tenda possui árvores adjacentes disponíveis.
// Argumentos: Matriz com o mapa, número de linhas e número de colunas, .
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
char T_sozinha(char **matriz, int L, int C, int l0, int c0) {
    char dirs[4] = {0,0,0,0}; // Vetor regista se há árvores em cada direção 1-Sim 0-Não


    matriz[l0][c0] = '.';
    if(c0!=C-1) {
        if(matriz[l0][c0+1] == 'A') dirs[DIR] =1;
    }
    if(l0!=L-1) {
        if(matriz[l0+1][c0] == 'A') dirs[BAIXO] =1;
    }
    if(c0!=0) {
        if(matriz[l0][c0-1] == 'A') dirs[ESQ] =1;
    }
    if(l0!=0) {
        if(matriz[l0-1][c0] == 'A') dirs[CIMA] =1;
    }

    switch (dirs[CIMA] + dirs[BAIXO] + dirs[ESQ] + dirs[DIR]) {  // N árvores adjacentes
    case 0:
        return 'S';
    case 1:
        if(dirs[DIR]) {
            matriz[l0][c0+1] = '.';
            return 'N';
        }
        if(dirs[BAIXO]) {
            matriz[l0+1][c0] = '.';
            return 'N';
        }
        if(dirs[ESQ]) {
            matriz[l0][c0-1] = '.';
            return 'N';
        }
        if(dirs[CIMA]) {
            matriz[l0-1][c0] = '.';
            return 'N';
        }
    case 2: // Não têm break de propósito, deve fazer o mesmo nestes casos.
    case 3:
    case 4:
        if(dirs[DIR]) {
            if(A_disponivel(matriz, L, C, l0, c0+1, DIR) == 'S') {
                return 'N';
            }
        }
        if(dirs[BAIXO]) {
            if(A_disponivel(matriz, L, C, l0+1, c0, BAIXO) == 'S') {
                return 'N';
            }
        }
        if(dirs[ESQ]) {
            if(A_disponivel(matriz, L, C, l0, c0-1, ESQ) == 'S') {
                return 'N';
            }
        }
        if(dirs[CIMA]) {
            if(A_disponivel(matriz, L, C, l0-1, c0, CIMA) == 'S') {
                return 'N';
            }
        }
        break;
    }
    return 'S';
}


// Descrição: Cria uma matriz para o mapa e determina se há tendas ilegais, no que toca a somatórios por
//            linhas/colunas e adjacência entre tendas, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para o início da matriz, número de linhas, número de colunas
//             vetores de tendas, por linhas e por colunas, e endereço onde escrever o endereço da matriz criada.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
int VerSomasAdjacentes_fromFile(FILE *fp, int L, int C, int *Ltents, int *Ctents, char ***matriz_out) {
    int i, j;
    char **matriz;

    matriz = (char**) malloc(L*sizeof(char*));
    if(matriz == NULL) return -1;

    // linha 0
    matriz[0] = (char*) malloc(C*sizeof(char));
    if(matriz[0] == NULL) return -1;
    // célula (0, 0)
    fscanf(fp, " %c", &matriz[0][0]);
    if(matriz[0][0] == 'T') {
        if(--Ltents[0] < 0) return 1;
        if(--Ctents[0] < 0) return 1;
    }
    // Resto da linha 0
    for ( j = 1; j < C; j++) {
        fscanf(fp, " %c", &matriz[0][j]);
        if(matriz[0][j] == 'T') {
            // Ver tendas adjacentes na parte já lida da matriz.
            if(matriz[0][j-1] == 'T')
                return 1;
            if(--Ltents[0] < 0) return 1;
            if(--Ctents[j] < 0) return 1;
        }
    }

    // Resto das linhas
    for ( i = 1; i < L; i++) {
        matriz[i] = (char*) malloc(C*sizeof(char));
        if(matriz[i] == NULL) return -1;
        // célula (i, 0)
        fscanf(fp, " %c", &matriz[i][0]);
        if(matriz[i][0] == 'T') {
            // Ver tendas adjacentes na parte já lida da matriz.
            if( matriz[i-1][0] == 'T' || matriz[i-1][1] == 'T')
                return 1;
            if(--Ltents[i] < 0) return 1;
            if(--Ctents[0] < 0) return 1;
        }
        // Resto da linha i excepto última
        for ( j = 1; j < C - 1; j++) {
            fscanf(fp, " %c", &matriz[i][j]);

            if(matriz[i][j] == 'T') {
                // Ver tendas adjacentes na parte já lida da matriz.
                if(matriz[i-1][j-1] == 'T' || matriz[i-1][j] == 'T' ||
                        matriz[i-1][j+1] == 'T' || matriz[i][j-1] == 'T')
                    return 1;
                if(--Ltents[i] < 0) return 1;
                if(--Ctents[j] < 0) return 1;
            }
        }
        // célula (i, C-1)
        fscanf(fp, " %c", &matriz[i][C-1]);
        if(matriz[i][C-1] == 'T') {
            // Ver tendas adjacentes na parte já lida da matriz.
            if(matriz[i-1][C-2] == 'T' || matriz[i-1][C-1] == 'T' ||
                    matriz[i][C-2] == 'T')
                return 1;
            if(--Ltents[i] < 0) return 1;
            if(--Ctents[C-1] < 0) return 1;
        }
    }
    *matriz_out = matriz;
    return 0;
}

// Descrição: Determina se há tendas ilegais, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de 'C', número de linhas e número de colunas.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
int SolveCfromFile(FILE *fp, int L, int C) {
    int i, j;
    int * Ltents, *Ctents;
    char **matriz;
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
    res = VerSomasAdjacentes_fromFile(fp, L, C, Ltents, Ctents, &matriz);
    if(res != 0) return res;

    for(i=0; i<L; i++){
        for(j=0; j<C; j++){
                if(matriz[i][j] == 'T'){
            if(T_sozinha(matriz, L, C, i, j) == 'S') return 1;
                }
        }
    }
    return 0;
}


