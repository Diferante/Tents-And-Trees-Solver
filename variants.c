#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "charStack.h"

#define CHECK_CANTO if(fscanf(fp, " %c", &c) != 1) exit(0); if(c == 'T') return 1
#define CHECK_LADO if(fscanf(fp, " %c", &c) != 1) exit(0); if(c == 'T') return 1; if(c == 'A') sem_arvores = 0
#define CHECK_CENTRO if(fscanf(fp, " %c", &c) != 1) exit(0); if(c == 'A') return 1

#define DIR 2 // direita, esquerda, cima e baixo
#define ESQ -2
#define CIMA 1
#define BAIXO -1

#define BUFFER_SIZE 128

char **Matriz;
unsigned int L;
unsigned int C;
FILE *fp;
/*unsigned char* stack;
long int massi= 0;*/

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

// Descrição:
// Argumentos:
// Retorno: 0 se ler bem, -1 se chegar ao fim do ficheiro ou erro de alocação.
int Fill_Matriz_fromFile() {
    int i=0, j=0, b;
    int nbytes;
    char buffer[BUFFER_SIZE];

    Matriz = (char**) calloc(L, sizeof(char*));
    if(Matriz == NULL) return -1;
    Matriz[0] = (char*) malloc(C*sizeof(char));
    if(Matriz[0] == NULL) return -1;

    nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
    b = -1;
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

        Matriz[i][j] = buffer[b];
        if(j == C - 1) {
            j=0;
            if(++i == L) {
                fseek(fp, 1 +b -nbytes , SEEK_CUR);
                return 0;
            }
            Matriz[i] = (char*) malloc(C*sizeof(char));
            if(Matriz[i] == NULL) return -1;
        } else {
            j++;
        }
    }
}

// Descrição: Determina se pode existir uma tenda nas coordenadas fornecidas, de acordo com os critérios de B,
//            lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de "B l0 c0", linha e coluna.
// Retorno: 1 caso seja detetada a impossibilidade de existir uma tenda, 0 caso contrário.
int SolveBfromFile(int l0, int c0) {
    int i, j, b;
    int nbytes;
    int l0_tents, c0_tents;
    int somaC = 1, somaL = 1;
    char sem_arvores = 1;
    char c, buffer[BUFFER_SIZE];

    if(l0 < 0 || l0 >= L || c0 < 0 || c0 >= C ) {
        // Limpar os números antes de saír
        do {
            c = fgetc(fp);
        } while(c != 'A' && c != 'T' && c != '.');
        return -1;
    }

    // Ler vetor tendas por linha
    for(i=0; i<l0; i++) if(fscanf(fp, " %*d") != 0) exit(0);
    if(fscanf(fp, " %d", &l0_tents) != 1) exit(0);
    for(i=l0+1; i<L; i++) if(fscanf(fp, " %*d") != 0) exit(0);

    // Ler vetor tendas por coluna
    for(i=0; i<c0; i++) if(fscanf(fp, " %*d") != 0) exit(0);
    if(fscanf(fp, " %d", &c0_tents) != 1) exit(0);
    for(i=c0+1; i<C; i++) if(fscanf(fp, " %*d") != 0) exit(0);

    if(l0_tents == 0 || c0_tents == 0)
        return 1;

    i =0;
    j=0;
    nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
    b = -1;
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
        // Fora dos adjacentes
        if((i < l0 - 1 || i > l0 + 1) && j == c0 && buffer[b]=='T') {
            somaC++;
        } else if((j < c0 - 1 || j > c0 + 1) && i == l0 && buffer[b]=='T') {
            somaL++;
        }

        // Centro e adjacentes
        if(i == l0 && j == c0) {
            if(buffer[b] == 'A') {
                fseek(fp, 1 +b -nbytes , SEEK_CUR);
                return 1;
            }
        } else if(i >= l0 -1 && i <= l0 + 1 && j >= c0 -1 && j <= c0 + 1) {
            if(buffer[b] == 'T') {
                fseek(fp, 1 +b -nbytes , SEEK_CUR);
                return 1;
            } else if((i == l0 || j == c0) && buffer[b] == 'A')
                sem_arvores = 0;
        }
        if(j == C - 1) {
            j=0;
            if(++i == L) {
                fseek(fp, 1 +b -nbytes , SEEK_CUR);
                break;
            }
        } else {
            j++;
        }
    }

    if(sem_arvores == 1) return 1;
    if(somaC > c0_tents || somaL > l0_tents) return 1;
    return 0;
}

// Descrição: Determina se o problema é admissível, segundo os critérios de A, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de 'A', número de linhas e número de colunas.
// Retorno: 0 caso seja detetada a inadmissibilidade do problema, 1 caso contrário.
int SolveAfromFile() {
    int i, j, b;
    int nbytes, somaL = 0, somaC = 0;
    int arvores = 0;
    char buffer[BUFFER_SIZE];

    for(i=0; i<L; i++) {
        if(fscanf(fp, " %d", &j) != 1) exit(0);
        somaL += j;
    }

    for(i=0; i<C; i++) {
        if(fscanf(fp, "%d", &j) != 1) exit(0);
        somaC += j;
    }
    /* Verifica admissiblidade do jogo */
    if(somaC != somaL) return 0;

    i=0, j=0;

    nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
    b = -1;
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

        if(buffer[b]=='A') arvores++;
        if(j == C - 1) {
            j=0;
            if(++i == L) {
                fseek(fp, 1 +b -nbytes , SEEK_CUR);
                break;
            }
        } else {
            j++;
        }
    }
    if(somaL>arvores) return 0;
    return 1;

}

void move_dir(int* l_out, int* c_out, char dir) {
    if(dir== CIMA) {
        *l_out += -2;
    } else if( dir == BAIXO) {
        *l_out += 2;
    } else if( dir == ESQ) {
        *c_out += -2;
    } else
        *c_out += 2;
}


// Descrição: Determina se esta tenda possui árvores adjacentes disponíveis.
// Argumentos: Linha e coluna da tenda.
// Retorno: 0 caso a tenda tenha árvore disponível, 1 caso contrário.
char isT_alone_iter(int l0, int c0) {
    char from = 0;
    unsigned int initial_size = L + C;
    if(initial_size > 1024) {
        initial_size = 1024;
    }
    initStack(initial_size);
    while(1) {
        Matriz[l0][c0] = '.';
        // Ver a direita
        if(c0!=C-1) {
            if(Matriz[l0][c0+1] == 'A') {
                Matriz[l0][c0+1] = '.';
                if(c0==C-2) {
                    freeStack();
                    return 0;
                }
                if(Matriz[l0][c0+2] != 'T') {
                    freeStack();
                    return 0;
                }
                push(from);
                from = ESQ;
                move_dir(&l0, &c0, DIR);
                continue;
            }
        }
        // Ver a baixo
        if(l0!=L-1) {
            if(Matriz[l0+1][c0] == 'A') {
                Matriz[l0+1][c0] = '.';
                if(l0==L-2) {
                    freeStack();
                    return 0;
                }
                if(Matriz[l0+2][c0] != 'T')  {
                    freeStack();
                    return 0;
                }
                push(from);
                from = CIMA;
                move_dir(&l0, &c0, BAIXO);
                continue;
            }
        }
        // Ver a esquerda
        if(c0!=0) {
            if(Matriz[l0][c0-1] == 'A') {
                Matriz[l0][c0-1] = '.';
                if(c0==1) {
                    freeStack();
                    return 0;
                }
                if(Matriz[l0][c0-2] != 'T') {
                    freeStack();
                    return 0;
                }
                push(from);
                from = DIR;
                move_dir(&l0, &c0, ESQ);
                continue;
            }
        }
        // Ver a cima
        if(l0!=0) {
            if(Matriz[l0-1][c0] == 'A') {
                Matriz[l0-1][c0] = '.';
                if(l0==1) {
                    freeStack();
                    return 0;
                }
                if(Matriz[l0-2][c0] != 'T') {
                    freeStack();
                    return 0;
                }

                push(from);
                from = BAIXO;
                move_dir(&l0, &c0, CIMA);
                continue;
            }
        }
        if(from == 0) {
            freeStack();
            return 1;
        }
        move_dir(&l0, &c0, from);
        from = pop();
    }
}


// Descrição: Determina se há tendas ilegais, no que toca a somatórios por
//            linhas/colunas e adjacência entre tendas.
// Argumentos: Ficheiro com o problema apontando para o início da matriz, número de linhas, número de colunas
//             vetores de tendas, por linhas e por colunas, e endereço onde escrever o endereço da matriz criada.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
int VerSomasAdjacentes_fromMatrix( int *Ltents, int *Ctents) {
    int i, j;

    if(Matriz[0][0] == 'T') {
        if(--Ltents[0] < 0) return 1;
        if(--Ctents[0] < 0) return 1;
    }
    // Resto da linha 0
    for ( j = 1; j < C; j++) {
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
        // célula (i, 0)
        if(Matriz[i][0] == 'T') {
            // Ver tendas adjacentes na parte já lida da matriz.
            if( Matriz[i-1][0] == 'T' || Matriz[i-1][1] == 'T')
                return 1;
            if(--Ltents[i] < 0) return 1;
            if(--Ctents[0] < 0) return 1;
        }
        // Resto da linha i excepto última
        for ( j = 1; j < C - 1; j++) {
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
    if(Ctents == NULL) {
        free(Ltents);
        return -1;
    }

    for(i=0; i<L; i++) {
        res = fscanf(fp, " %d", &Ltents[i]);
    }
    for(i=0; i<C; i++) {
        res = fscanf(fp, " %d", &Ctents[i]);
    }
    /* Carregar mapa e avaliar somas e tendas adjacentes */
    res = Fill_Matriz_fromFile();

    if(res != 0) {
        _free_matriz();
        free(Ltents);
        free(Ctents);
        return res;
    }
    res = VerSomasAdjacentes_fromMatrix(Ltents, Ctents);
    free(Ltents);
    free(Ctents);
    if(res != 0) {
        _free_matriz();
        return res;
    }

    for(i=0; i<L; i++) {
        for(j=0; j<C; j++) {
            if(Matriz[i][j] == 'T') {
                //stack = (unsigned char*) &res;
                if(isT_alone_iter(i, j) == 1) {
                    _free_matriz();
                    return 1;
                }
            }
        }
    }
    _free_matriz();
    return 0;
}


