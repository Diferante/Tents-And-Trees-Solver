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
int solveB_fromMatrix(char **camp, int L, int C, int l0, int c0, int l0_t, int c0_t, int ha_tendas) {
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
int solveB_fromFile(FILE *fp, int L, int C, int l0, int c0) {
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


int solveA_fromFile(FILE *fp, int L, int C) {
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
    if(somaC != somaL) return 1;

    for ( i = 0; i < L; i++) {
        for ( j = 0; j < C; j++) {
            fscanf(fp, " %c", &c);
            if(c==A) arvores++;
        }
    }

    /* Verifica admissiblidade do jogo */
    if(somaL > arvores) return 1;

    return 0;

}
