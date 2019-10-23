#include <stdio.h>
#include <stdlib.h>

#define E 1 // ERVA
#define A 2
#define T 3
#define D 4 // DESCONHECIDO
#define max(a, b) a>b ? a : b
#define min(a, b) a>b ? a : b

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
int solveB(char **camp, int L, int C, int l0, int c0, int l0_t, int c0_t, int ha_tendas) {
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

int main() {
    printf("Hello world!\n");
    return 0;
}
