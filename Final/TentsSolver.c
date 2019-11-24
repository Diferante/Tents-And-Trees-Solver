#include "generalStack.h"
#include <stdio.h>
typedef struct {
    int x, y;
    } Point;


char **Matriz;
unsigned int L;
unsigned int C;
FILE *fp;

void _free_matriz() {
    int i;

    if(Matriz == NULL) return;
    for(i=0; i<L; i++) {
        if(Matriz[i] == NULL) break;
        free(Matriz[i]);
    }
    free(Matriz);
}

void InitSolver(FILE *fpointer, int Linhas, int Colunas) {
    fp = fpointer;
    L = Linhas;
    C = Colunas;
}

void Guesser( Point* opens){
    int i;
    initStack(L+C, sizeof(Point));


}

char Solver(){
    char res= 0;
    Point* opens;
    // Read matriz
    //Check Admissibilidade
    //Create opens
    while(1){
        //Check 100% certain open changes to '.' or to 'T'
        if(res == 'W') // if managed to win
            break;
        Guesser(opens);
    }
    return res;
}
