#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TreesTents.h"
#include "generalStack.h"

char **Matriz;
unsigned int L;
unsigned int C;
int *Lrests;
int *Crests;
int tendas_rest;
int arvores;
char estacao_alta;
Stack *points_toAnalyse;
// Variaveis para backtracking: ----
int record_changes;
Stack *b_chars;       // Os char alterados.
Stack *b_points;      // As coordenadas dos char alterados.
Stack *b_n;           // O nmr de char's alterados após cada snapshot.
int n_since_snapshot; // O nmr de char's alterados após o último snapshot.
// ----
typedef struct {
  int x, y;
} Point;

int PointsAreEqual(void *p1, void *p2) {
  // printf("(%d,%d) -- (%d,%d) \n", ((Point *)p1)->x, ((Point *)p1)->y, ((Point
  // *)p2)->x, ((Point *)p2)->y);
  return ((Point *)p1)->x == ((Point *)p2)->x &&
         ((Point *)p1)->y == ((Point *)p2)->y;
}
/* Descrição: Regista a alterção de um ponto para permitir backtracking.
 * */
void regista_alteracao(Point point, char old_value) {
  // Se o ponto já foi guardado não interessa guardar o valor intermedio
  if (!itemExists(b_points, &point, n_since_snapshot, PointsAreEqual)) {
    push(b_points, &point);
    push(b_chars, &old_value);
    n_since_snapshot++;
  }
}
/* Descrição: Cria um snapshot para onde é possível fazer backtrack.
 * */
void create_snapshot() {
  push(b_n, &n_since_snapshot);
  n_since_snapshot = 0;  
}
/* Descrição: Reverte as alterações desde o último snapshot;
 * */
void revert_snapshot() {
  Point p;
  char old_value;
  for (; n_since_snapshot > 0; n_since_snapshot--) {
    pop(b_points, &p);
    pop(b_chars, &old_value);
    if (isTent(Matriz[p.x][p.y]) && !isTent(old_value)) {
      tendas_rest++;
      Lrests[p.x]++;
      Crests[p.y]++;
    }
    Matriz[p.x][p.y] = old_value;
  }
  pop(b_n, &n_since_snapshot);
}
/* Descrição: Altera a matriz e opcionalmente guarda a alteração
 * */
void edit_matriz(Point point, char new_char) {
  if (record_changes) {
    regista_alteracao(point, Matriz[point.x][point.y]);
  }
  Matriz[point.x][point.y] = new_char;
}

int AnalyseTent(Point tent, int isPaired, int isNew) {
  Point p, a;
  int arvores_sem_par = 0;
  // Verificar quadrado 3x3
  for (p.x = tent.x - 1; p.x <= tent.x + 1; p.x++) {
    for (p.y = tent.y - 1; p.y <= tent.y + 1; p.y++) {
      // Se está dentro dos limites e excluindo o centro
      if (p.x >= 0 && p.x < L && p.y >= 0 && p.y < C &&
          (p.x != tent.x || p.y != tent.y)) {
        if (isNew && Matriz[p.x][p.y] == '0') {
          edit_matriz(p, '.');
          push(points_toAnalyse, &p);
        } else if (isNew && isTent(Matriz[p.x][p.y])) {
          return -1;
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
    isPaired = 1;
    edit_matriz(a, 'a');
    edit_matriz(tent, 't');
    // Se houver open ou tenda adj a 'a' estará na direção de
    // 't' para 'a' a duas células de distância
    p.x = tent.x + 2 * (a.x - tent.x);
    p.y = tent.y + 2 * (a.y - tent.y);
    if (p.x >= 0 && p.x < L && p.y >= 0 && p.y < C) {
      if (Matriz[p.x][p.y] == '0' || Matriz[p.x][p.y] == 'T') {
        push(points_toAnalyse, &p);
      }
    }
  }
  if (isNew) {
    if (isPaired)
      edit_matriz(tent, 't');
    else
      edit_matriz(tent, 'T');
    // Verificar limites linha e coluna
    if (Lrests[tent.x] == 0) {
      p = tent;
      // Limpa opens da linha
      for (p.y = 0; p.y < C; p.y++) {
        if (Matriz[p.x][p.y] == '0') {
          edit_matriz(p, '.');
          push(points_toAnalyse, &p);
        }
      }
    }
    if (Crests[tent.y] == 0) {
      p = tent;
      // Limpa opens da coluna
      for (p.x = 0; p.x < L; p.x++) {
        if (Matriz[p.x][p.y] == '0') {
          edit_matriz(p, '.');
          push(points_toAnalyse, &p);
        }
      }
    }
  }
  return 0;
}
int AnalyseTree(Point tree) {
  Point p;
  Point tent;
  int flag = 0;
  int tendas_e_opens = 0;
  p = tree;
  if (p.x > 0) {
    if (Matriz[p.x - 1][p.y] == '0' || Matriz[p.x - 1][p.y] == 'T' ||
        Matriz[p.x - 1][p.y] == NEW_T_UNPAIRED) {
      tendas_e_opens++;
      flag = 1;
    }
  }
  if (p.x < L - 1) {
    if (Matriz[p.x + 1][p.y] == '0' || Matriz[p.x + 1][p.y] == 'T' ||
        Matriz[p.x + 1][p.y] == NEW_T_UNPAIRED) {
      tendas_e_opens++;
      flag = 2;
    }
  }
  if (p.y > 0) {
    if (Matriz[p.x][p.y - 1] == '0' || Matriz[p.x][p.y - 1] == 'T' ||
        Matriz[p.x][p.y - 1] == NEW_T_UNPAIRED) {
      tendas_e_opens++;
      flag = 3;
    }
  }
  if (p.y < C - 1) {
    if (Matriz[p.x][p.y + 1] == '0' || Matriz[p.x][p.y + 1] == 'T' ||
        Matriz[p.x][p.y + 1] == NEW_T_UNPAIRED) {
      tendas_e_opens++;
      flag = 4;
    }
  }
  // Árvore sozinha.
  if (tendas_e_opens == 0)
    return -1;
  if (tendas_e_opens == 1) {
    edit_matriz(p, 'a');
    if (flag == 1) {
      tent.x = p.x - 1;
      tent.y = p.y;
    } else if (flag == 2) {
      tent.x = p.x + 1;
      tent.y = p.y;
    } else if (flag == 3) {
      tent.x = p.x;
      tent.y = p.y - 1;
    } else if (flag == 4) {
      tent.x = p.x;
      tent.y = p.y + 1;
    }
    if (Matriz[tent.x][tent.y] != 'T') {
      if (Matriz[tent.x][tent.y] == '0') {
        Lrests[tent.x]--;
        Crests[tent.y]--;
        tendas_rest--;
      }
      edit_matriz(tent, NEW_T_PAIRED);
    } else {
      edit_matriz(tent, 't');
    }
    push(points_toAnalyse, &tent);
  }
  return 0;
}

int AnalyseLinhaColunaLimits(Point ponto) {
  Point p;
  int espacos_livres, comprimento, i;
  if (Lrests[ponto.x] > 0) {
    // Vê espacos livres e abertos da linha
    p.x = ponto.x;
    comprimento = 0;
    espacos_livres = 0;
    for (p.y = 0; p.y < C; p.y++) {
      if (Matriz[p.x][p.y] == '0') {
        comprimento++;
      } else if (comprimento != 0) {
        espacos_livres += comprimento / 2 + comprimento % 2;
        comprimento = 0;
      }
    }
    // se o ultimo for open ainda falta adicionar o ultimo bloco
    if (comprimento != 0) {
      espacos_livres += comprimento / 2 + comprimento % 2;
    }
    if (espacos_livres < Lrests[p.x])
      return -1;
    else if (espacos_livres == Lrests[p.x]) {
      comprimento = 0;
      for (i = 0; i < C; i++) {
        if (Matriz[p.x][i] == '0')
          comprimento++;
        else {
          if (comprimento % 2 == 1) {
            // em blocos impares as tendas são as pontas e dois em dois
            for (p.y = i - 1; i - p.y <= comprimento; p.y -= 2) {
              edit_matriz(p, NEW_T_UNPAIRED);
              Crests[p.y]--;
              Lrests[p.x]--;
              tendas_rest--;
              push(points_toAnalyse, &p);
            }
          }
          // nada se conclui em blocos pares
          comprimento = 0;
        }
      }
      // se o ultimo for de um bloco impar ainda falta alterar o bloco
      if (comprimento % 2 == 1) {
        for (p.y = i - 1; i - p.y <= comprimento; p.y -= 2) {
          edit_matriz(p, NEW_T_UNPAIRED);
          Crests[p.y]--;
          Lrests[p.x]--;
          tendas_rest--;
          push(points_toAnalyse, &p);
        }
      }
      // nada se conclui em blocos pares
      comprimento = 0;
    }
  }
  if (Crests[ponto.y] > 0) {
    // Vê espacos livres e abertos da coluna
    p.y = ponto.y;
    comprimento = 0;
    espacos_livres = 0;
    for (p.x = 0; p.x < L; p.x++) {
      if (Matriz[p.x][p.y] == '0') {
        comprimento++;
      } else if (comprimento != 0) {
        espacos_livres += comprimento / 2 + comprimento % 2;
        comprimento = 0;
      }
    }
    // se o ultimo for open ainda falta adicionar o ultimo bloco
    if (comprimento != 0) {
      espacos_livres += comprimento / 2 + comprimento % 2;
    }

    if (espacos_livres < Crests[p.y])
      return -1;
    else if (espacos_livres == Crests[p.y]) {
      comprimento = 0;
      for (i = 0; i < L; i++) {
        if (Matriz[i][p.y] == '0') {
          comprimento++;
        } else {
          if (comprimento % 2 == 1) {
            // em blocos impares as tendas são as pontas e dois em dois
            for (p.x = i - 1; i - p.x <= comprimento; p.x -= 2) {
              edit_matriz(p, NEW_T_UNPAIRED);
              Lrests[p.x]--;
              Crests[p.y]--;
              tendas_rest--;
              push(points_toAnalyse, &p);
            }
          }
          // nada se conclui em blocos pares
          comprimento = 0;
        }
      }
      // se o ultimo for de um bloco impar ainda falta alterar o bloco
      if (comprimento % 2 == 1) {
        for (p.x = i - 1; i - p.x <= comprimento; p.x -= 2) {
          edit_matriz(p, NEW_T_UNPAIRED);
          Lrests[p.x]--;
          Crests[p.y]--;
          tendas_rest--;
          push(points_toAnalyse, &p);
        }
      }
    }
  }
  return 0;
}

int AnalysePoint(Point ponto) {
  Point p;
  if (AnalyseLinhaColunaLimits(ponto) == -1)
    return -1;
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
  return 0;
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
  edit_matriz(p, '.');
  push(points_toAnalyse, &p);
}

/* Descrição: Avalia o ponto fornecido e altera o resto do mapa
 * consoante for possível com a nova informação.
 * Argumentos: O ponto (x, y) e uma flag que indica se deve ser feito
 * o teste correspondente ao char na Matriz ou verificar se a linha ou
 * coluna do ponto são fáceis - número de opens igual a tendas restantes.
 * Retorno: Devolve -1 se detetar que  problema é impossível, 0 caso contrário.
 * */
int ChangePropagator(int x, int y, int line_column_test) {
  Point p;
  char c;
  p.x = x;
  p.y = y;
  //printf("\nnew Prop\n");
  if (line_column_test) {
    AnalyseLinhaColunaLimits(p);
  } else {
    push(points_toAnalyse, &p);
  }
  while (!isEmpty(points_toAnalyse)) {
    pop(points_toAnalyse, &p);
    // printf("%d, %d\n", p.x, p.y);
    //printf("rests %d\n", tendas_rest);
    //printMatriz(Matriz, L, C);
    //getchar();
    /*if (Matriz[7][2] == 't') {
      p.x++;
      p.x--;
    }*/
    c = Matriz[p.x][p.y];
    if (c == 'T' || c == 't' || c == NEW_T_UNPAIRED || c == NEW_T_PAIRED) {
      if (AnalyseTent(p, isPairedTent(c), isNewTent(c)) == -1)
        return -1;
    } else if (c == '.') {
      if (AnalysePoint(p) == -1)
        return -1;
    } else if (c == 'A' && estacao_alta) {
      if (AnalyseTree(p) == -1)
        return -1;
    } else if (c == '0') {
      AnalyseOpen(p);
    }
  }
  return 0;
}

int Guesser() {
  Point p;
  Stack *jogadas; // Pontos adivinhados como tendas.

  jogadas = initStack(8, sizeof(Point));
  b_points = initStack(8, sizeof(Point));
  b_chars = initStack(8, 1);
  b_n = initStack(8, sizeof(int));
  n_since_snapshot = 0;
  record_changes = 1;
  p.x = 0;
  p.y = 0;
  while (1) {
    // printMatriz(Matriz, L, C);
    for (; p.x < L; p.x++) {
      for (; p.y < C; p.y++) {
        if (Matriz[p.x][p.y] == '0') {
          if (Lrests[p.x] > 0 && Crests[p.y] > 0) {
            Matriz[p.x][p.y] = NEW_T_UNPAIRED;
            // add_around(p.x, p.y, 1, Matriz, L, C);
            push(jogadas, &p);
            tendas_rest--;
            Lrests[p.x]--;
            Crests[p.y]--;
            create_snapshot();
            if (ChangePropagator(p.x, p.y, 0) == -1) {
              p.x = L;
            }
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
        freeStack(b_chars);
        freeStack(b_points);
        freeStack(b_n);
        return -1; // Impossible
      }
      revert_snapshot();
      pop(jogadas, &p);
      tendas_rest++;
      Lrests[p.x]++;
      Crests[p.y]++;
      Matriz[p.x][p.y] = '0';
      edit_matriz(p, '.');
      if (ChangePropagator(p.x, p.y, 0) == -1) {
        p.x = L-1;
        p.y = C - 1;
      }
      // add_around(p.x, p.y, -1, Matriz, L, C);
      p.y++;
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
        freeStack(b_chars);
        freeStack(b_points);
        freeStack(b_n);
        beautify_matriz(Matriz, L, C);
        return 1; // Win;
      }
      p.x = L;
    }
  }
}

/* Descrição: Filtra o mapa inicial de modo a facilitar a resolução do
 * problema
 * Retorno: 1 caso o problema esteja resolvido, -1 caso detetada a sua
 * impossíbilidade, 0 caso contrário.
 * */
int teste() {
  int i, j, opens;

  record_changes = 0;
  /* Verifica as posições que são '.' */
  for (i = 0; i < L; i++) {
    for (j = 0; j < C; j++) {
      if (Matriz[i][j] == 'A') {
        check_adj_for_opens(i, j, Matriz, L, C, Lrests, Crests);
      }
    }
  }
  // verificar se a linha ou coluna são fáceis em cada linha e coluna.
  i = 0;
  // Diagonais
  for (i = 0; i < L && i < C; i++) {
    if (ChangePropagator(i, i, 1) == -1)
      return -1;
  }
  if (i == L) {
    // Colunas restantes
    for (; i < C; i++) {
      if (ChangePropagator(0, i, 1) == -1)
        return -1;
    }
  } else { // i == C
    // Linhas restantes;
    for (; i < L; i++) {
      if (ChangePropagator(i, 0, 1) == -1)
        return -1;
    }
  }
  /* se for época alta, então uma tenda para cada árvore */
  if (estacao_alta) {
    /* Verifica se alguma árvore tem só 1 lugar possível para tendas */
    for (i = 0; i < L; i++) {
      for (j = 0; j < C; j++) {
        if (Matriz[i][j] == 'A') {
          if (ChangePropagator(i, j, 0) == -1)
            return -1;
        }
      }
    }
  }
  // Verificar se alguma das tendas
  // colocadas estão sozinhas e contar opens
  opens = 0;
  for (i = 0; i < L; i++) {
    for (j = 0; j < C; j++) {
      if (Matriz[i][j] == '0')
        opens++;
      else if (Matriz[i][j] == 'T' && isT_alone_iter(i, j, Matriz, L, C))
        break;
    }
    if (j != C)
      break;
  }
  repair_matriz(Matriz, L, C);
  if (i != L || (opens == 0 && tendas_rest != 0)) {
    return -1; // Impossible
  }
  if (tendas_rest == 0 && i == L)
    return 1; // Solved
  return 0;
}

int Solver(FILE *fpointer, unsigned int l, unsigned int c, FILE *fp2) {
  int i, j, res;
  // Point *open_stack;

  L = l;
  C = c;
  Lrests = (int *)malloc(L * sizeof(int));
  if (Lrests == NULL)
    exit(0);
  Crests = (int *)malloc(C * sizeof(int));
  if (Crests == NULL) {
    free(Lrests);
    exit(0);
  }

  tendas_rest = Fill_Hints_checkSums(fpointer, L, C, Lrests, Crests);

  if (tendas_rest < 0) {

    fprintf(fp2, "%d %d %d\n\n", L, C, -1);
    return 0;
  }

  Matriz = (char **)calloc(L, sizeof(char *));
  if (Matriz == NULL)
    return -1;

  arvores = Fill_Matriz_easy(fpointer, Matriz, L, C);

  if (tendas_rest > arvores) {
    free(Lrests);
    free(Crests);
    _free_matriz(Matriz, L);
    return -1;
  }

  estacao_alta = tendas_rest == arvores;

  points_toAnalyse = initStack(8, sizeof(Point));
  /*cria opens na matriz e verifica opens obvios */
  res = teste();
  if (res != 0) {
    fprintf(fp2, "%d %d %d\n", L, C, res);
    if (res == 1) {
      beautify_matriz(Matriz, L, C);
      for (i = 0; i < L; i++) {
        for (j = 0; j < C; j++) {
          fprintf(fp2, "%c", Matriz[i][j]);
        }
        fprintf(fp2, "\n");
      }
    }
    fprintf(fp2, "\n");
    free(Lrests);
    free(Crests);
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
  free(Lrests);
  free(Crests);
  _free_matriz(Matriz, L);
  freeStack(points_toAnalyse);
  return 0;
}
