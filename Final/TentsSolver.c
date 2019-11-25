#include <stdio.h>
#include <stdlib.h>
#include "generalStack.h"

#define BUFFER_SIZE 1024
typedef struct {
    int x, y;
} Point;

void _free_matriz(char **Matriz, unsigned int L) {
    int i;

    if(Matriz == NULL) return;
    for(i=0; i<L; i++) {
        if(Matriz[i] == NULL) break;
        free(Matriz[i]);
    }
    free(Matriz);
}

// Descrição: Lê os vetores iniciais e verifica se as somas são iguais.
// Retorno: Número de tendas caso a soma seja igual, -2 se não for igual ou -1 se houver erro na leitura.
int Read_Hints(FILE* fp, unsigned int L, unsigned int C, int *Ltents, int *Ctents) {
    int i, j=0, b, nbytes, offset, res;
    int somaL = 0, somaC = 0;
    char *end_ptr, buffer[BUFFER_SIZE + 1];

    nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
    b = -1;
    j=0;
    offset = 0;
    buffer[BUFFER_SIZE] = '\n';
    while(1) {
        // Skip whitespaces and such;
        for(b++; b - offset< nbytes; b++) {
            if(buffer[b] >= '0' && buffer[b] <= '9')
                break;
        }

        if(nbytes==b-offset) {
            nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
            if(nbytes == 0)
                return -1;
            offset = 0;
            b = -1;
            continue;
        }
        res = strtol(buffer+b, &end_ptr, 10);
        if(end_ptr == &buffer[BUFFER_SIZE]) {
            offset = end_ptr - buffer - b;
            for(i=0; i<offset; i++)
                buffer[i] = buffer[b+i];
            nbytes = fread(buffer + offset, 1, BUFFER_SIZE-offset, fp);
            res = strtol(buffer, &end_ptr, 10);
        }
        b = end_ptr - buffer - 1;

        if(j < L){
            somaL += res;
            Ltents[j] = res;
        } else if(j-L < C) {
            somaC += res;
            Ctents[j-L] = res;
        }
        if(++j == L + C) {
            fseek(fp, 1 +b - offset -nbytes , SEEK_CUR);
            break;
        }

    }
    if(somaL != somaC)
        return -2;
    return somaL;
}


// Descrição: Lê a matriz e determina se o problema é admissível.
// Argumentos:
// Retorno: 0 caso seja detetada a inadmissibilidade do problema, 1 caso contrário.
int Read_Matriz (FILE* fp, char **Matriz, unsigned int L, unsigned int C, int soma_tendas){
    int i, j, b, nbytes;
    int arvores = 0, res, offset;
    char buffer[BUFFER_SIZE+1], *end_ptr;

    Matriz[0] = (char*) malloc(C*sizeof(char));
    if(Matriz[0] == NULL) return -1;

    nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
    b = -1;
    i=0;
    j=0;
    while(1) {
        // Skip whitespaces and such;
        for(b++; b < nbytes; b++) {
            if(buffer[b] == 'A' || buffer[b] == 'T' || buffer[b] == '.')
                break;
        }

        if(nbytes==b) {
            nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
            if(nbytes == 0)
                return -1;
            b = -1;
            continue;
        }
        Matriz[i][j] == buffer[b];
        if(buffer[b]=='A') arvores++;
        if(j == C - 1) {
            j=0;
            if(++i == L) {
                fseek(fp, 1 +b -nbytes , SEEK_CUR);
                break;
            }
            Matriz[i] = (char*) malloc(C*sizeof(char));
            if(Matriz[i] == NULL) return -1;
        } else {
            j++;
        }
    }
    return 1;

}

void Guesser(char **Matriz, unsigned int L, unsigned int C, Point* opens) {
    int i;
    initStack(L+C, sizeof(Point));


}

int Solver(FILE *fpointer, unsigned int L, unsigned int C, FILE *fp) {
    int res;
    char **Matriz;
    int *Ltents, *Ctents;
    Point *opens;

    Ltents = (int*) malloc(L*sizeof(int));
    if(Ltents == NULL) exit(0);
    Ctents = (int*) malloc(C*sizeof(int));
    if(Ctents == NULL) {
        free(Ltents);
        exit(0);
    }
    // Read Linhas, Colunas
    res = Fill_Hints_fromFile(fp, L, C, Ltents, Ctents);
    if(res != 0){
        free(Ltents);
        free(Ctents);
        return res;
    }

    Matriz = (char**) calloc(L, sizeof(char*));
    if(Matriz == NULL) return -1;

    /*Read matriz and check Admissibilidade*/
    res = SolveAfromFile(fp, Matriz, L, C, Ltents, Ctents);
    if(res != 0) {
        free(Ltents);
        free(Ctents);
        _free_matriz(Matriz, L);
        return res;
    }
    /*cria opens na matriz e verifica opens obvios */
    Matriz=teste();
    while(1){
        //Check 100% certain open changes to '.' or to 'T'
        if(res == 'W') // if managed to win
            break;
        Guesser(opens);
    }
    return res;
}
