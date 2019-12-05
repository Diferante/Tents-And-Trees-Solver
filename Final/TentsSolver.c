#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreesTents.h"
#include "generalStack.h"

char **Matriz;
unsigned int L;
unsigned int C;
int *Ltents;
int *Ctents;
int tendas_rest;
int arvores;
char estacao_alta;
Stack *points_toAnalyse;

typedef struct {
    int x, y;
} Point;
// Não necessáriamente uma tenda nova
void AnalyseTent(Point tent, int isPaired) {
    Point p, a;
    int arvores_sem_par = 0;
    // Verificar quadrado 3x3
    for (p.x = tent.x - 1; p.x <= tent.x + 1; p.x++) {
        for (p.y = tent.y - 1; p.y <= tent.y + 1; p.y++) {
            if (p.x >= 0 && p.x < L && p.y >= 0 && p.y < C) {
                if (Matriz[p.x][p.y] == '0') {
                    Matriz[p.x][p.y] = '.';
                    push(points_toAnalyse, &p);
                } else if (p.x == tent.x ||
                           p.y == tent.y) { // Se é adjacente não diagonal
                    if (Matriz[p.x][p.y] == 'A') {
                        arvores_sem_par++;
                        a = p;
                        if (isPaired && estacao_alta)
                            push(points_toAnalyse, &a);
                    }
                }
            }
        }
    }
    // Se só há uma árvore sem par adj esta tem de ser a par
    if (!isPaired && arvores_sem_par == 1) {
        Matriz[a.x][a.y] = 'a';
        Matriz[tent.x][tent.y] = 't';
        // é mais fácil tratar já da 'a' enquanto se sabe a direção do
        // potencial open
        // push(points_toAnalyse, &a);
        // Se houver open ou tenda estará na direção de
        // 't' para 'a' a duas células de distância
        p.x = tent.x + 2 * (a.x - tent.x);
        p.y = tent.y + 2 * (a.y - tent.y);
        if (p.x >= 0 && p.x < L && p.y >= 0 && p.y < C) {
            if (Matriz[p.x][p.y] == '0' || Matriz[p.x][p.y] == 'T') {
                push(points_toAnalyse, &p);
            }
        }
    }
    // Verificar limites linha e coluna
    p = tent;
    if (Ltents[tent.x] == 0) {
        Ltents[tent.x] = -1; // Para não repetir a limpeza
        // Limpa opens da linha
        for (p.y = 0; p.y < C; p.y++) {
            if (Matriz[p.x][p.y] == '0') {
                Matriz[p.x][p.y] = '.';
                push(points_toAnalyse, &p);
            }
        }
    }
    p = tent;
    if (Ctents[tent.y] == 0) {
        Ctents[tent.y] = -1; // Para não repetir a limpeza
        // Limpa opens da coluna
        for (p.x = 0; p.x < L; p.x++) {
            if (Matriz[p.x][p.y] == '0') {
                Matriz[p.x][p.y] = '.';
                push(points_toAnalyse, &p);
            }
        }
    }
}
void AnalyseTree(Point tree) {
    Point p;
    Point tent;
    int flag = 0;
    int tendas_e_opens = 0;
    p = tree;
    if (p.x > 0) {
        if (Matriz[p.x - 1][p.y] == '0' || Matriz[p.x - 1][p.y] == 'T') {
            tendas_e_opens++;
            flag = 1;
        }
    }
    if (p.x < L - 1) {
        if (Matriz[p.x + 1][p.y] == '0' || Matriz[p.x + 1][p.y] == 'T') {
            tendas_e_opens++;
            flag = 2;
        }
    }
    if (p.y > 0) {
        if (Matriz[p.x][p.y - 1] == '0' || Matriz[p.x][p.y - 1] == 'T') {
            tendas_e_opens++;
            flag = 3;
        }
    }
    if (p.y < C - 1) {
        if (Matriz[p.x][p.y + 1] == '0' || Matriz[p.x][p.y + 1] == 'T') {
            tendas_e_opens++;
            flag = 4;
        }
    }
    if (tendas_e_opens == 1) {
        Matriz[p.x][p.y] = 'a';
        if (flag == 1) {
            tent.x = p.x - 1;
            tent.y = p.y;
            if (Matriz[tent.x][tent.y] == '0') {
                Ltents[tent.x]--;
                Ctents[tent.y]--;
                tendas_rest--;
            }
            Matriz[tent.x][tent.y] = 't';
            push(points_toAnalyse, &tent);
        }
        if (flag == 2) {
            tent.x = p.x + 1;
            tent.y = p.y;
            if (Matriz[tent.x][tent.y] == '0') {
                Ltents[tent.x]--;
                Ctents[tent.y]--;
                tendas_rest--;
            }
            Matriz[tent.x][tent.y] = 't';
            push(points_toAnalyse, &tent);
        }
        if (flag == 3) {
            tent.x = p.x;
            tent.y = p.y - 1;
            if (Matriz[tent.x][tent.y] == '0') {
                Ltents[tent.x]--;
                Ctents[tent.y]--;
                tendas_rest--;
            }
            Matriz[tent.x][tent.y] = 't';
            push(points_toAnalyse, &tent);
        }
        if (flag == 4) {
            tent.x = p.x;
            tent.y = p.y + 1;
            if (Matriz[tent.x][tent.y] == '0') {
                Ltents[tent.x]--;
                Ctents[tent.y]--;
                tendas_rest--;
            }
            Matriz[tent.x][tent.y] = 't';
            push(points_toAnalyse, &tent);
        }
    }
}

void AnalysePoint(Point ponto) {
    Point p;
    int abertos;

    if (Ltents[ponto.x] > 0) {
        // Vê se a linha se tornou fácil
        p.x = ponto.x;
        abertos = 0;
        for (p.y = 0; p.y < C; p.y++) {
            if (Matriz[p.x][p.y] == '0')
                abertos++;
        }
        if (abertos == Ltents[p.x]) {
            Ltents[p.x] = 0;
            for (p.y = 0; p.y < C; p.y++) {
                if (Matriz[p.x][p.y] == '0') {
                    Matriz[p.x][p.y] = 'T';
                    Ctents[p.y]--;
                    tendas_rest--;
                    push(points_toAnalyse, &p);
                }
            }
        }
    }
    if (Ctents[ponto.y] > 0) {
        // Vê se a coluna se tornou fácil
        abertos = 0;
        p.y = ponto.y;
        for (p.x = 0; p.x < L; p.x++) {
            if (Matriz[p.x][p.y] == '0')
                abertos++;
        }
        if (abertos == Ctents[p.y]) {
            Ctents[p.y] = -1;
            for (p.x = 0; p.x < L; p.x++) {
                if (Matriz[p.x][p.y] == '0') {
                    Matriz[p.x][p.y] = 'T';
                    Ltents[p.x]--;
                    tendas_rest--;
                    push(points_toAnalyse, &p);
                }
            }
        }
    }
    if (estacao_alta) {
        // Ver se árvores adj podem ter-se tornado fáceis
        if (ponto.x > 0) {
            if (Matriz[ponto.x - 1][ponto.y] == 'A') {
                p.x = ponto.x - 1;
                p.y = ponto.y;
                push(points_toAnalyse, &p);
            }
        }
        if (ponto.x < L - 1) {
            if (Matriz[ponto.x + 1][ponto.y] == 'A') {
                p.x = ponto.x + 1;
                p.y = ponto.y;
                push(points_toAnalyse, &p);
            }
        }

        if (ponto.y > 0) {
            if (Matriz[ponto.x][ponto.y - 1] == 'A') {
                p.x = ponto.x;
                p.y = ponto.y - 1;
                push(points_toAnalyse, &p);
            }
        }
        if (ponto.y < C - 1) {
            if (Matriz[ponto.x][ponto.y + 1] == 'A') {
                p.x = ponto.x;
                p.y = ponto.y + 1;
                push(points_toAnalyse, &p);
            }
        }
    }
}
/*void AnalyseSmallTree(Stack *points_toAnalyse, Point smalltree) {
  Point p;
  p = smalltree;
  if (p.x > 0) {
    if (Matriz[p.x - 1][p.y] == '0') {
      p.x = p.x - 1;
      p.y = p.y;
      push(points_toAnalyse, &p);
      return;
    }
  }
  if (p.x < L - 1) {
    if (Matriz[p.x + 1][p.y] == '0') {
      p.x = p.x + 1;
      p.y = p.y;
      push(points_toAnalyse, &p);
      return;
    }
  }
  if (p.y > 0) {
    if (Matriz[p.x][p.y - 1] == '0') {
      p.x = p.x;
      p.y = p.y - 1;
      push(points_toAnalyse, &p);
      return;
    }
  }
  if (p.y < C - 1) {
    if (Matriz[p.x][p.y + 1] == '0') {
      p.x = p.x;
      p.y = p.y + 1;
      push(points_toAnalyse, &p);
      return;
    }
  }
}*/
void AnalyseOpen(Point open) {
    Point p;
    p = open;
    if (p.x > 0) {
        if (Matriz[p.x - 1][p.y] == 'A')
            return;
    }
    if (p.x < L - 1) {
        if (Matriz[p.x + 1][p.y] == 'A')
            return;
    }
    if (p.y > 0) {
        if (Matriz[p.x][p.y - 1] == 'A')
            return;
    }
    if (p.y < C - 1) {
        if (Matriz[p.x][p.y + 1] == 'A')
            return;
    }
    Matriz[p.x][p.y] = '.';
    push(points_toAnalyse, &p);
}

/* Descrição: Avalia o ponto fornecido e altera o resto do mapa
 * consoante for possível com a nova informação.
 * Argumentos: O ponto (x, y) e uma flag que indica se deve ser feito
 * o teste correspondente ao char na Matriz ou verificar se a linha ou
 * coluna do ponto são fáceis - número de opens igual a tendas restantes.
 * */
void ChangePropagator(int x, int y, int line_column_test) {
    Point p;
    char c;
    p.x = x;
    p.y = y;

    if(line_column_test) {
        // AnalysePoint verifica se
        // a coluna ou a linham são fáceis,
        // mas faz mais coisas se estacao_alta não
        // for 0;
        c = estacao_alta;
        estacao_alta = 0;
        AnalysePoint(p);
        estacao_alta = c;
    } else {
        push(points_toAnalyse, &p);
    }
    while (!isEmpty(points_toAnalyse)) {
        pop(points_toAnalyse, &p);
        /*printf("%d, %d\n", p.x, p.y);
        printMatriz(Matriz, L, C);*/
        c = Matriz[p.x][p.y];
        if (c == 'T' || c == 't') {
            AnalyseTent(p, c == 't');
        } else if (c == '.') {
            AnalysePoint(p);
        } else if (c == 'A' && estacao_alta) {
            AnalyseTree(p);
        } else if (c == 'O') {
            AnalyseOpen(p);
        }
    }
}

int Guesser() {
    Point p;
    Stack *jogadas;

    jogadas = initStack(8, sizeof(Point));
    p.x = 0;
    p.y = 0;
    while (1) {
        /*printMatriz(Matriz, L, C);
        if(Matriz[0][6] == 'T'){
            p.x++;
            p.x--;
        }*/
        for (; p.x < L; p.x++) {
            for (; p.y < C; p.y++) {
                if (Matriz[p.x][p.y] == '0') {
                    if (Ltents[p.x] > 0 && Ctents[p.y] > 0) {
                        Matriz[p.x][p.y] = 'T';
                        add_around(p.x, p.y, 1, Matriz, L, C);
                        push(jogadas, &p);
                        tendas_rest--;
                        Ltents[p.x]--;
                        Ctents[p.y]--;
                        break;
                    }
                }
            }
            if (p.y != C)
                break;
            p.y = 0;
        }
        if (p.x == L) {
            if (isEmpty(jogadas)) {
                freeStack(jogadas);
                return -1; // Impossible
            }
            pop(jogadas, &p);
            Matriz[p.x][p.y] = '0';
            add_around(p.x, p.y, -1, Matriz, L, C);
            tendas_rest++;
            Ltents[p.x]++;
            Ctents[p.y]++;
            p.y++;
            continue;
        }
        if (tendas_rest == 0) {
            for (p.x = 0; p.x < L; p.x++) {
                for (p.y = 0; p.y < C; p.y++) {
                    if (Matriz[p.x][p.y] == 'T' && isT_alone_iter(p.x, p.y, Matriz, L, C))
                        break;
                }
                if (p.y != C)
                    break;
            }
            repair_matriz(Matriz, L, C);
            if (p.x == L) {
                freeStack(jogadas);
                beautify_matriz(Matriz, L, C);
                return 1; // Win;
            }
        }
    }
}

/* Descrição: Filtra o mapa inicial de modo a facilitar a resolução do
 * problema
 * Retorno: 1 caso o problema esteja resolvido, -1 caso detetada a sua
 * impossíbilidade, 0 caso contrário.
 * */
int teste() {
    int i, j;
    /* Verifica as posições que são '.' */
    for (i = 0; i < L; i++) {
        for (j = 0; j < C; j++) {
            if (Matriz[i][j] == 'A') {
                check_adj_for_opens(i, j, Matriz, L, C, Ltents, Ctents);
            }
        }
    }
    // verificar se a linha ou coluna são fáceis em cada linha e coluna.
    i = 0;
    //Diagonais
    for(i=0; i<L && i<C; i++) {
        ChangePropagator(i, i, 1);
    }
    if(i == L) {
        // Colunas restantes
        for(; i<C; i++) {
            ChangePropagator(0, i, 1);
        }
    } else { // i == C
        // Linhas restantes;
        for(; i<L; i++) {
            ChangePropagator(i, 0, 1);
        }
    }
    /* se for época alta, então uma tenda para cada árvore */
    if (estacao_alta) {
        /* Verifica se alguma árvore tem só 1 lugar possível para tendas */
        for (i = 0; i < L; i++) {
            for (j = 0; j < C; j++) {
                if (Matriz[i][j] == 'A') {
                    ChangePropagator(i, j, 0);
                }
            }
        }
    }
    // Verificar se alguma das tendas
    // colocadas estão sozinhas
    for (i = 0; i < L; i++) {
        for (j = 0; j < C; j++) {
            if (Matriz[i][j] == 'T' && isT_alone_iter(i, j, Matriz, L, C))
                break;
        }
        if (j != C)
            break;
    }
    repair_matriz(Matriz, L, C);
    if (i != L) {
        return -1; // Impossible
    }
    if(tendas_rest == 0 && i == L)
        return 1; // Solved
    return 0;
}

int Solver(FILE *fpointer, unsigned int l, unsigned int c, FILE *fp2) {
    int i, j, res;
    // Point *open_stack;

    L = l;
    C = c;

    Ltents = (int *)malloc(L * sizeof(int));
    if (Ltents == NULL)
        exit(0);
    Ctents = (int *)malloc(C * sizeof(int));
    if (Ctents == NULL) {
        free(Ltents); // Read Linhas, Colunas
        exit(0);
    }

    tendas_rest = Fill_Hints_checkSums(fpointer, L, C, Ltents, Ctents);

    if (tendas_rest < 0) {

        fprintf(fp2, "%d %d %d\n\n", L, C, -1);
        return 0;
    }

    Matriz = (char **)calloc(L, sizeof(char *));
    if (Matriz == NULL)
        return -1;

    arvores = Fill_Matriz_easy(fpointer, Matriz, L, C);

    if (tendas_rest > arvores) {
        free(Ltents);
        free(Ctents);
        _free_matriz(Matriz, L);
        return -1;
    }

    estacao_alta = tendas_rest == arvores;

    points_toAnalyse = initStack(8, sizeof(Point));
    /*cria opens na matriz e verifica opens obvios */
    res = teste();
    if (res != 0) {
        fprintf(fp2, "%d %d %d\n", L, C, res);
        if(res == 1) {
            beautify_matriz(Matriz, L, C);
            for (i = 0; i < L; i++) {
                for (j = 0; j < C; j++) {
                    fprintf(fp2, "%c", Matriz[i][j]);
                }
                fprintf(fp2, "\n");
            }
        }
        fprintf(fp2, "\n");
        free(Ltents);
        free(Ctents);
        _free_matriz(Matriz, L);
        freeStack(points_toAnalyse);
        return 0;
    }

    res = Guesser();

    fprintf(fp2, "%d %d %d\n", L, C, res);
    if (res != -1) {
        for (i = 0; i < L; i++) {
            for (j = 0; j < C; j++) {
                fprintf(fp2, "%c", Matriz[i][j]);
            }
            fprintf(fp2, "\n");
        }
    }
    fprintf(fp2, "\n");
    free(Ltents);
    free(Ctents);
    _free_matriz(Matriz, L);
    freeStack(points_toAnalyse);
    return 0;
}
