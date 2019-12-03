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

typedef struct {
  int x, y;
} Point;

int Guesser() {
  Point p;
  Stack *jogadas;

  jogadas = initStack(8, sizeof(Point));
  p.x = 0;
  p.y = 0;
  while (1) {
    //printMatriz(Matriz, L, C);

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
        remove_opens(Matriz, L, C);
        return 1; // Win;
      }
    }
  }
}

/* Descrição: Filtra o mapa inicial de modo a facilitar a resolução do
 * problema
 * */
void teste() {
  int i, j;
  // int opens = 0;
  // Point new_open, *opens_stack;

  // opens_stack = (Point *) initStack(8 sizeof(Point));
  /* Verifica as posições que são '.' */
  for (i = 0; i < L; i++) {
    if (Ltents[i] == 0)
      continue;
    for (j = 0; j < C; j++) {
      if (Ctents[j] == 0)
        continue;
      if (Matriz[i][j] == '.') {
        /* só é Open se tiver pelo menos uma árvore e nenhuma tenda na sua
         * adjacencia */
        if (arvores_dir_adj(i, j, Matriz, L, C) >= 1 &&
            sem_tendas_adj(i, j, Matriz, L, C)) {
          Matriz[i][j] = '0';
          /*new_open.x = i;
          new_open.y = j;
          push((void*)opens_stack, &new_open);*/
        }
      }
    }
  }
  /* se for época alta, então uma tenda para cada árvore */
  if (estacao_alta) {
    /* Verifica se alguma árvore tem só 1 lugar possível para tendas */
    for (i = 0; i < L; i++) {
      for (j = 0; j < C; j++) {
        if (Matriz[i][j] == 'A') {
          if (arvore_facil(i, j, Matriz, L, C, Ltents, Ctents, &tendas_rest)) {
            // Começar de novo a procura com a nova informação.
            i = -1;
            j = C;
          }
        }
      }
    }
  }
  /*for (i = 0; i < L; i++) {
    opens = 0;
    for (j = 0; j < C; j++) {
      if (Matriz[i][j] == '0')
        opens++;
    }
    // Se número for igual ao número de tendas restantes para a linha
    if (opens == Ltents[i])
     opens_em_tendas(i, 0);
  }*/
  return;
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

  /*cria opens na matriz e verifica opens obvios */
  teste();

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

  return 0;
}
