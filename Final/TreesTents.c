#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreesTents.h"
#include "generalStack.h"

#define DIR 2 // direita, esquerda, cima e baixo
#define ESQ -2
#define CIMA 1
#define BAIXO -1

void _free_matriz(char **Matriz, int L) {
  int i;

  if (Matriz == NULL)
    return;
  for (i = 0; i < L; i++) {
    if (Matriz[i] == NULL)
      break;
    free(Matriz[i]);
  }
  free(Matriz);
}

// Descrição: Lẽ as hints do ficheiro e soma os vetores.
// Argumentos:
// Retorno: -2 se ocorrer erro, se não devolve o número de tendas ou -1 caso os
// vetores sejam coerentes ou alguma seja negativo.
int Fill_Hints_checkSums(FILE *fp, int L, int C, int *Lrests, int *Crests) {
  int i, j, tendas, res, somaC;

  res = 0;
  tendas = 0;
  somaC = 0;
  for (i = 0; i < L; i++) {
    if (fscanf(fp, " %d", &j) != 1) {
      res = -2;
      break;
    }
    if (j < 0) {
      res = -1;
    }
    tendas += j;
    Lrests[i] = j;
  }
  for (i = 0; i < C; i++) {
    if (fscanf(fp, " %d", &j) != 1) {
      res = -2;
      break;
    }
    if (j < 0) {
      res = -1;
    }
    somaC += j;
    Crests[i] = j;
  }
  if (res < 0)
    return res;
  if (tendas != somaC)
    return -1;
  return tendas;
}

// Descrição: Lê a matriz do ficheiro e conta as árvores.
// Argumentos:
// Retorno: O número de árvores se ler bem, -1 se chegar ao fim do ficheiro ou
// erro de alocação.
int Fill_Matriz_easy(FILE *fp, char **Matriz, int L, int C) {
  int i, j, arvores = 0;
  char format[32];
  sprintf(format, " %%%dc", C);

  for (i = 0; i < L; i++) {
    Matriz[i] = (char *)malloc(C * sizeof(char));
    if (Matriz[i] == NULL)
      return -1;
    if (fscanf(fp, format, Matriz[i]) != 1)
      return -1;
    for (j = 0; j < C; j++) {
      if (Matriz[i][j] == 'A')
        arvores++;
    }
  }
  return arvores;
}

/* Descrição: 1 se c for uma tenda, 0 se não.
 * */
int isTent(char c) {
  return c == 't' || c == 'T' || c == NEW_T_UNPAIRED || c == NEW_T_PAIRED;
}

/* Descrição: 1 se c for uma tenda com par, 0 se não.
 * */
int isPairedTent(char c) { return c == 't' || c == NEW_T_PAIRED; }

/* Descrição: 1 se c for uma nova tenda, 0 se não.
 * */
int isNewTent(char c) { return c == NEW_T_UNPAIRED || c == NEW_T_PAIRED; }

/* Descrição: Prepara matriz para escrita,
 * troca os char's de processamento para os seus valores finais.
 * */
void beautify_matriz(char **Matriz, int L, int C) {
  int i, j;
  for (i = 0; i < L; i++) {
    for (j = 0; j < C; j++) {
      if (Matriz[i][j] == 't')
        Matriz[i][j] = 'T';
      else if (Matriz[i][j] == 'a')
        Matriz[i][j] = 'A';
      else if (Matriz[i][j] != 'T' && Matriz[i][j] != 'A')
        Matriz[i][j] = '.';
    }
  }
}

/* Descrição: Verifica se pode haver tenda nas quatro direções,
 * coloca '0' nos que puderem
 * Argumentos: Posição (x,y) da árvore a ser verificada
 * */
void check_adj_for_opens(int x, int y, char **Matriz, int L, int C, int *Lrests,
                         int *Crests) {
  if (Crests[y] > 0) {
    if (x > 0) {
      if (Lrests[x - 1] > 0 && Matriz[x - 1][y] == '.' &&
          sem_tendas_adj(x - 1, y, Matriz, L, C)) {
        Matriz[x - 1][y] = '0';
      }
    }
    if (x < L - 1) {
      if (Lrests[x + 1] > 0 && Matriz[x + 1][y] == '.' &&
          sem_tendas_adj(x + 1, y, Matriz, L, C)) {
        Matriz[x + 1][y] = '0';
      }
    }
  }

  if (Lrests[x] > 0) {
    if (y > 0) {
      if (Crests[y - 1] > 0 && Matriz[x][y - 1] == '.' &&
          sem_tendas_adj(x, y - 1, Matriz, L, C)) {
        Matriz[x][y - 1] = '0';
      }
    }
    if (y < C - 1) {
      if (Crests[y + 1] > 0 && Matriz[x][y + 1] == '.' &&
          sem_tendas_adj(x, y + 1, Matriz, L, C)) {
        Matriz[x][y + 1] = '0';
      }
    }
  }
}
/* Descrição: Verifica tendas em todas posições adjacentes
 * Argumentos: Posição (x,y) do mapa
 * Retorno: Retorna 0 se existir pelo menos uma tenda na adjacência, 1 se não
 * existir nenhuma tenda
 * */
int sem_tendas_adj(int x, int y, char **Matriz, int L, int C) {
  /* vê tendas nas adjacentes */
  int i, j;

  for (i = -1; i <= 1; i++) {
    for (j = -1; j <= 1; j++) {
      if (x + i >= 0 && x + i < L && y + j >= 0 && y + j < C) {
        if (Matriz[x + i][y + j] == 'T' || Matriz[x + i][y + j] == 't')
          return 0;
      }
    }
  }
  return 1;
}
