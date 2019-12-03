#include "generalStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024 * 16

#define DIR 2 // direita, esquerda, cima e baixo
#define ESQ -2
#define CIMA 1
#define BAIXO -1

/*#define T 'T'
#define A 'A'
#define O '0'
#define t 't'
#define a 'a'*/
#define TMP_T 'C'
#define TMP_A 'B'

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

void _free_matriz() {
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

void handle_int(int res, int i, int *Ltents, int *Ctents, int *somaL,
                int *somaC) {
  if (i < L) {
    Ltents[i] = res;
    *somaL += res;
  } else if (i - L < C) {
    Ctents[i - L] = res;
    *somaC += res;
  }
}
// Descrição: Lẽ as hints do ficheiro e soma os vetores.
// Argumentos:
// Retorno: -2 se ocorrer erro, se não devolve o número de tendas ou -1 caso os
// vetores sejam coerentes.
int Fill_Hints_checkSums(FILE *fp) {
  int i, n, res, somaL = 0, somaC = 0;
  int block_to_read, nbytes, bytes_skiped, digits_read, bytes_consumed;
  char *buffer, *pre_buffer, *end_ptr;

  n = (2 * L + 2 * C) / BUFFER_SIZE;
  block_to_read = BUFFER_SIZE;
  if (n == 0) {
    block_to_read = (2 * L + 2 * C);
    n = 1;
  }
  // The block is 16 extra chars before a buffer of size block_to_read, eith an
  // extra char in the end for '\0'.
  pre_buffer = (char *)malloc(16 + block_to_read + 1);
  if (pre_buffer == NULL)
    return -2;
  buffer = pre_buffer + 16;
  buffer[block_to_read] = '\0';

  i = 0;
  bytes_skiped = -1;
  digits_read = 0;
  while (1) {
    for (; n > 0; n--) {
      nbytes = fread(buffer, 1, block_to_read, fp);
      if (nbytes != block_to_read)
        return -2;
      bytes_consumed = 0;
      // Check if not first time
      if (digits_read != 0) {
        // Read the number that was supposed to be read last time
        res = strtol(buffer - digits_read, &end_ptr, 10);
        bytes_consumed = end_ptr - buffer;
        handle_int(res, i, Ltents, Ctents, &somaL, &somaC);
        i++;
      }
      // Read buffer int by int
      for (; i < L + C; i++) {
        sscanf(buffer + bytes_consumed, " %n", &bytes_skiped);
        bytes_consumed += bytes_skiped;
        res = strtol(buffer + bytes_consumed, &end_ptr, 10);
        if (end_ptr == &buffer[block_to_read]) {
          digits_read = end_ptr - buffer - bytes_consumed;
          // Write part of number into pre_buffer
          memcpy(buffer - digits_read, buffer + bytes_consumed, digits_read);
          break;
        }
        bytes_consumed = end_ptr - buffer;
        handle_int(res, i, Ltents, Ctents, &somaL, &somaC);
      }
    }
    // Se só restam 3 -> ler o resto diretamente
    if (L + C - i < 4)
      break;
    n = 2 * (L + C - i) / BUFFER_SIZE;
    block_to_read = BUFFER_SIZE;
    if (n == 0) {
      block_to_read = 2 * (L + C - i);
      n = 1;
    }
    buffer[block_to_read] = '\0';
  }
  if (i != L + C) {
    if (digits_read != 0) {
      // Read the number that was partially read last time
      if (fscanf(fp, "%[0-9]", buffer) != 1)
        buffer[0] = '\0';
      res = strtol(buffer - digits_read, &end_ptr, 10);
      handle_int(res, i, Ltents, Ctents, &somaL, &somaC);
      i++;
    }
    // Read remaining ints one by one
    for (; i < L + C; i++) {
      if (fscanf(fp, " %d", &res) != 1)
        return -2;
      handle_int(res, i, Ltents, Ctents, &somaL, &somaC);
    }
  }
  free(pre_buffer);
  if (somaL != somaC)
    return -1;
  return somaL;
}

// Descrição: Lê a matriz do ficheiro e conta as árvores.
// Argumentos:
// Retorno: O número de árvores se ler bem, -1 se chegar ao fim do ficheiro ou
// erro de alocação.
int Fill_Matriz_easy(FILE *fp) {
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

int check_linha_coluna(int l0, int c0) {
  int i, j, somaL = 0, somaC = 0;
  for (i = l0, j = 0; j < c0 - 1; j++) {
    if (Matriz[i][j] == 'T')
      somaL++;
  }
  for (j = c0 + 2; j < C; j++) {
    if (Matriz[i][j] == 'T')
      somaL++;
  }
  for (i = 0, j = c0; i < l0 - 1; i++) {
    if (Matriz[i][j] == 'T')
      somaC++;
  }
  for (i = l0 + 2; i < L; i++) {
    if (Matriz[i][j] == 'T')
      somaC++;
  }

  if (somaC >= Ctents[c0] || somaL >= Ltents[l0])
    return 0;
  return 1;
}

void move_dir(int *l_out, int *c_out, char dir) {
  if (dir == CIMA) {
    *l_out += -2;
  } else if (dir == BAIXO) {
    *l_out += 2;
  } else if (dir == ESQ) {
    *c_out += -2;
  } else
    *c_out += 2;
}

// Descrição: Determina se esta tenda possui árvores adjacentes disponíveis.
// Altera a matriz. Argumentos: Linha e coluna da tenda. Retorno: 0 caso a tenda
// tenha árvore disponível, 1 caso contrário.
char isT_alone_iter(int l0, int c0) {
  char from = 0;
  Stack *charStack;
  charStack = initStack(8, 1);
  while (1) {
    Matriz[l0][c0] = TMP_T;
    // Ver a direita
    if (c0 != C - 1) {
      if (Matriz[l0][c0 + 1] == 'A') {
        Matriz[l0][c0 + 1] = TMP_A;
        if (c0 == C - 2) {
          freeStack(charStack);
          return 0;
        }
        if (Matriz[l0][c0 + 2] != 'T') {
          freeStack(charStack);
          return 0;
        }
        push(charStack, &from);
        from = ESQ;
        move_dir(&l0, &c0, DIR);
        continue;
      }
    }
    // Ver a baixo
    if (l0 != L - 1) {
      if (Matriz[l0 + 1][c0] == 'A') {
        Matriz[l0 + 1][c0] = TMP_A;
        if (l0 == L - 2) {
          freeStack(charStack);
          return 0;
        }
        if (Matriz[l0 + 2][c0] != 'T') {
          freeStack(charStack);
          return 0;
        }
        push(charStack, &from);
        from = CIMA;
        move_dir(&l0, &c0, BAIXO);
        continue;
      }
    }
    // Ver a esquerda
    if (c0 != 0) {
      if (Matriz[l0][c0 - 1] == 'A') {
        Matriz[l0][c0 - 1] = TMP_A;
        if (c0 == 1) {
          freeStack(charStack);
          return 0;
        }
        if (Matriz[l0][c0 - 2] != 'T') {
          freeStack(charStack);
          return 0;
        }
        push(charStack, &from);
        from = DIR;
        move_dir(&l0, &c0, ESQ);
        continue;
      }
    }
    // Ver a cima
    if (l0 != 0) {
      if (Matriz[l0 - 1][c0] == 'A') {
        Matriz[l0 - 1][c0] = TMP_A;
        if (l0 == 1) {
          freeStack(charStack);
          return 0;
        }
        if (Matriz[l0 - 2][c0] != 'T') {
          freeStack(charStack);
          return 0;
        }

        push(charStack, &from);
        from = BAIXO;
        move_dir(&l0, &c0, CIMA);
        continue;
      }
    }
    if (from == 0) {
      freeStack(charStack);
      return 1;
    }
    move_dir(&l0, &c0, from);
    pop(charStack, &from);
  }
}

void repair_matriz() {
  int i, j;
  for (i = 0; i < L; i++) {
    for (j = 0; j < C; j++) {
      if (Matriz[i][j] == TMP_T)
        Matriz[i][j] = 'T';
      else if (Matriz[i][j] == TMP_A)
        Matriz[i][j] = 'A';
    }
  }
}
void remove_opens() {
  int i, j;
  for (i = 0; i < L; i++) {
    for (j = 0; j < C; j++) {
      if (Matriz[i][j] != 'T' && Matriz[i][j] != 'A')
        Matriz[i][j] = '.';
    }
  }
}

/* atualiza os Opens around (x, y)*/
void add_around(int x, int y, int value) {
  int j = 0;
  char c;
  if (y + 1 < C) {
    c = Matriz[x][y + 1];
    if (c >= '0' && c <= '8')
      Matriz[x][y + 1] += value;
  }
  if (x + 1 < L) {
    for (j = -1; j < 2; j++) {
      if (y + j >= 0 && y + j < C) {
        c = Matriz[x + 1][y + j];
        if (c >= '0' && c <= '8')
          Matriz[x + 1][y + j] += value;
      }
    }
  }
  return;
}

void printMatriz() {
  int i;
  char format[32];
  sprintf(format, " %%.%ds\n", C);
  for (i = 0; i < L; i++) {
    printf(format, Matriz[i]);
  }
  printf("\n");
}

int Guesser() {
  Point p;
  Stack *jogadas;

  jogadas = initStack(8, sizeof(Point));
  p.x = 0;
  p.y = 0;
  while (1) {
    // printMatriz();
    for (; p.x < L; p.x++) {
      for (; p.y < C; p.y++) {
        if (Matriz[p.x][p.y] == '0' && check_linha_coluna(p.x, p.y)) {
          Matriz[p.x][p.y] = 'T';
          add_around(p.x, p.y, 1);
          push(jogadas, &p);
          tendas_rest--;
          break;
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
      add_around(p.x, p.y, -1);
      tendas_rest++;
      p.y++;
      continue;
    }
    if (tendas_rest == 0) {
      for (p.x = 0; p.x < L; p.x++) {
        for (p.y = 0; p.y < C; p.y++) {
          if (Matriz[p.x][p.y] == 'T' && isT_alone_iter(p.x, p.y))
            break;
        }
        if (p.y != C)
          break;
      }
      repair_matriz();
      if (p.x == L) {
        freeStack(jogadas);
        remove_opens();
        return 1; // Win;
      }
    }
  }
}
int teste_arvore(int x, int y) {
  /* vê arvores nas quatro direcoes */
  int arvore = 0;

  if (x > 0) {
    if (Matriz[x - 1][y] == 'A')
      arvore++;
  }
  if (x < L - 1) {
    if (Matriz[x + 1][y] == 'A')
      arvore++;
  }

  if (y > 0) {
    if (Matriz[x][y - 1] == 'A')
      arvore++;
  }
  if (y < C - 1) {
    if (Matriz[x][y + 1] == 'A')
      arvore++;
  }
  return arvore;
}
int teste_tendas(int x, int y) {
  /* vê tendas nas adjacentes */
  int i = 0;
  int j = 0;
  int tendas_rest = 0;
  int k[3] = {-1, 0, 1};

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if (x + k[i] >= 0 && x + k[i] < L && y + k[j] >= 0 && y + k[j] < C) {
        if (Matriz[x + k[i]][y + k[j]] == 'T')
          tendas_rest++;
      }
    }
  }
  if (tendas_rest != 0)
    return 0;
  else
    return 1;
}

/* mete os Opens em '.'*/
void teste_tendas_2(int x, int y) {

  int i = 0;
  int j = 0;
  int k[3] = {-1, 0, 1};

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if (x + k[i] >= 0 && x + k[i] < L && y + k[j] >= 0 && y + k[j] < C) {
        if (Matriz[x + k[i]][y + k[j]] == '0')
          Matriz[x + k[i]][y + k[j]] = '.';
      }
    }
  }
  return;
}
/*Testa se alguma arvore esta isolada com so um Open, substitui o O por T e mete
 * os adjacentes que são Open a . */
void teste_arvore_opens(int x, int y) {
  int opens = 0;
  int flag = 0;

  if (x > 0) {
    /*cima*/
    if (Matriz[x - 1][y] == '0') {
      opens++;
      flag = 1;
    }
  }
  /*baixo*/
  if (x < L - 1) {
    if (Matriz[x + 1][y] == '0') {
      opens++;
      flag = 2;
    }
  }
  /* esquerda */
  if (y > 0) {
    if (Matriz[x][y - 1] == '0') {
      opens++;
      flag = 3;
    }
  }
  /* direita */
  if (y < C - 1) {
    if (Matriz[x][y + 1] == '0') {
      opens++;
      flag = 4;
    }
  }
  /* se alguma arvore so tiver um open, meter tenda ai e meter os adjacentes a
   * '.' */
  if (opens == 1) {
    if (flag == 1) {
      Matriz[x - 1][y] = 'T';
      tendas_rest--;
      teste_tendas_2(x - 1, y);
      return;
    }
    if (flag == 2) {
      Matriz[x + 1][y] = 'T';
      tendas_rest--;
      teste_tendas_2(x + 1, y);
      return;
    }
    if (flag == 3) {
      Matriz[x][y - 1] = 'T';
      tendas_rest--;
      teste_tendas_2(x, y - 1);
      return;
    }
    if (flag == 4) {
      Matriz[x][y + 1] = 'T';
      tendas_rest--;
      teste_tendas_2(x, y + 1);
      return;
    }
  }
  return;
}

void teste() {
  int i, j;
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
        if (teste_arvore(i, j) && teste_tendas(i, j)) {
          Matriz[i][j] = '0';
          /*new_open.x = i;
          new_open.y = j;
          push((void*)opens_stack, &new_open);*/
        }
      }
    }
  }
  if (estacao_alta) {
    /* Verifica se alguma árvore tem só 1 Open */
    for (i = 0; i < L; i++) {
      for (j = 0; j < C; j++) {
        if (Matriz[i][j] == 'A')
          teste_arvore_opens(i, j);
      }
    }
  }
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

  tendas_rest = Fill_Hints_checkSums(fpointer);

  if (tendas_rest < 0) {

    fprintf(fp2, "%d %d %d\n\n", L, C, -1);
    return 0;
  }

  Matriz = (char **)calloc(L, sizeof(char *));
  if (Matriz == NULL)
    return -1;

  arvores = Fill_Matriz_easy(fpointer);

  if (tendas_rest > arvores) {
    free(Ltents);
    free(Ctents);
    _free_matriz();
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
