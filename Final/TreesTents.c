/******************************************************************************
 * 2019-2020
 * Autores - Haohua Dong e Diogo Antunes
 *
 * DESCRIÇÃO
 *  Implementação de funções de auxílio das funções em TentsSolver na resolução
 *  de problemas do jogo Tents and Trees.
 *
 *  Detalhes de implementação:
 *      A leitura dos vetores de tendas por linha e coluna, em
 *      Fill_Hints_checkSums, é feita char a char, sendo que a leitura da
 *      matriz, em Fill_Matriz, é feita linha a linha.
 *
 *****************************************************************************/

#include "TreesTents.h"

#include <stdio.h>
#include <stdlib.h>

/* Descrição: Liberta a matriz de L linhas.
 * */
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

/* Descrição: Lê os vetores com as Hints, o número de tendas em cada linha e
 * coluna, de um ficheiro, guarda-os e calcula a sua soma.
 * Argumentos: Apontador para o ficheiro de entrada, número de linhas e colunas
 * da matriz e apontadores para os vetores onde guardar as Hints.
 * Retorno: -2 se ocorrer erro, -1 se os vetores forem incoerentes ou alguma
 * Hint for negativa, caso contrário o número de tendas.
 * */
int Fill_Hints_checkSums(FILE *fp, int L, int C, int *Lrests, int *Crests) {
  int i, j, tendas, res, somaC;

  res = 0;
  tendas = 0;
  somaC = 0;
  for (i = 0; i < L; i++) {
    if (fscanf(fp, " %d", &j) != 1) {
      // Erro de leitura
      res = -2;
      break;
    }
    if (j < 0) {
      // Hint negativa
      res = -1;
    }
    tendas += j;
    Lrests[i] = j;
  }
  for (i = 0; i < C; i++) {
    if (fscanf(fp, " %d", &j) != 1) {
      // Erro de leitura
      res = -2;
      break;
    }
    if (j < 0) {
      // Hint negativa
      res = -1;
    }
    somaC += j;
    Crests[i] = j;
  }
  if (res < 0)
    return res;
  if (tendas != somaC) // Vetores incoerentes
    return -1;
  return tendas;
}

/* Descrição: Lê a matriz do ficheiro, guarda-a e conta as árvores.
 * Argumentos:
 *  fp          - Apontador para o ficheiro com a Matriz.
 *  Matriz      - Apontador para uma tabela de L apontadores para char onde
 *                guardar a matriz lida.
 *  L e C       - Números de linhas e colunas da matriz.
 * Retorno: O número de árvores se ler bem, -1 se ocorrer erro de
 * alocação ou leitura.
 * */
int Fill_Matriz(FILE *fp, char **Matriz, int L, int C) {
  int i, j, arvores = 0;
  char format[32]; // String passada a fscanf que define o formato da leitura.

  // Se C for 16 escreve "%16c", para ler 16 caracteres, uma linha, em cada
  // fscanf.
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

/* Descrição: Prepara matriz para saída, troca os char's de processamento para
 * os seus valores finais.
 * Argumentos: Apontador para a Matriz e os números de linhas e colunas da
 * matriz.
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

/* Descrição: Verifica se pode haver alguma tenda nas quatro direções de uma
 * árvore, marca como open se for possível.
 * Argumentos: Posição (x,y) da árvore, apontador para Matriz, os números de
 * linhas e colunas da matriz e os vetores com as tendas de cada linha e coluna.
 * */
void check_adj_for_opens(int x, int y, char **Matriz, int L, int C, int *Lrests,
                         int *Crests) {
  // Em cada posição adjacente à árvore, coloca '0' se for um ponto, se não
  // existerem tendas adjacentes e se puderem existir tendas nas suas linha e
  // coluna.
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

/* Descrição: Verifica se existem tendas em todas posições adjacentes.
 * Argumentos: Posição (x,y) da tenda, apontador para a matriz e os números de
 * linhas e colunas da matriz.
 * Retorno: Retorna 0 se existir pelo menos uma tenda na adjacência, 1 se não
 * existir nenhuma tenda.
 * */
int sem_tendas_adj(int x, int y, char **Matriz, int L, int C) {
  int i, j;

  for (i = -1; i <= 1; i++) {
    for (j = -1; j <= 1; j++) {
      // Se (x+i, y+j) está dentro dos limites e é adjacente
      if (x + i >= 0 && x + i < L && y + j >= 0 && y + j < C &&
          (i != 0 || j != 0)) {
        if (Matriz[x + i][y + j] == 'T' || Matriz[x + i][y + j] == 't')
          return 0;
      }
    }
  }
  return 1;
}