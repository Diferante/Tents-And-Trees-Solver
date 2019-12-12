/******************************************************************************
 *  Copyright (C) 2019 - Haohua Dong e Diogo Antunes
 *
 *  This file is a part of Trees-And-Tents-Solver.
 *
 *  Trees-And-Tents-Solver is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Trees-And-Tents-Solver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *
 * DESCRIÇÃO
 *  Implementação de um gerador de soluções para problemas do jogo Trees and
 *  Tents.
 *
 *
 *  Detalhes de implementação:
 *    Para encontrar uma solução são utilizados um verificador das
 *    regras de jogo, ChangePropagator, e um algoritmo de procura de
 *    solução, Guesser. Tenta-se primeiro determinar a solução
 *    diretamente com o verificador, utilizando a procura apenas
 *    quando o verificador é inconclusivo. Na procura são registadas
 *    as alterações entre cada decisão de modo a permitir o
 *    backtracking para estados anteriores.
 *    NEW_T_PAIRED e NEW_T_UNPAIRED são macros de TentsTrees.h.
 *
 *  Código utilizado durante processamento:
 *      Tenda sem par já analisada  T
 *      Tenda com par já analisada  t
 *      Tenda nova sem par          NEW_T_UNPAIRED
 *      Tenda nova com par          NEW_T_PAIRED
 *      Árvore sem par              A
 *      Árvore com par              a
 *      Ponto                       .
 *      Tenda ou Ponto = Open       0
 *
 *****************************************************************************/

#include "TreesTents.h"
#include "generalStack.h" // IN IT'S OWN REPOSITORY

#include <stdio.h>
#include <stdlib.h>

char **Matriz;           // Matriz que representa o mapa
unsigned int L;          // Linhas da Matriz
unsigned int C;          // Colunas da Matriz
int *Lrests;             // Vetor de tendas por colocar em cada linha
int *Crests;             // Vetor de tendas por colocar em cada coluna
int tendas_rest;         // Tendas por colocar
int arvores;             // Árvores do mapa
char epoca_alta;         // 1 se for época alta, 0 se não
Stack *points_toAnalyse; // Pilha de pontos por analisar pelo ChangePropagator
// Variáveis para backtracking: ----
int record_changes;   // 1 se deve registar alterações à Matriz, 0 se não
Stack *b_chars;       // Os char alterados
Stack *b_points;      // As coordenadas dos char alterados
Stack *b_n;           // O nmr de char's alterados após cada snapshot
int n_since_snapshot; // O nmr de char's alterados após o último snapshot
// ----

typedef struct {
  int x, y;
} Point;

/* Descrição: 1 se os dois objetos Point nas posições apontadas são iguais, 0 se
 * não.
 * */
int PointsAreEqual(void *p1, void *p2) {
  return ((Point *)p1)->x == ((Point *)p2)->x &&
         ((Point *)p1)->y == ((Point *)p2)->y;
}

/* Descrição: Regista a alteração de um ponto para permitir backtracking.
 * */
void regista_alteracao(Point point, char old_char) {
  // Se o ponto já foi alterado desde o último snapshot já está registado como
  // alterado e não interessa o valor intermédio
  if (!itemExists(b_points, &point, n_since_snapshot, PointsAreEqual)) {
    push(b_points, &point);
    push(b_chars, &old_char);
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
    // Se está a retirar uma tenda
    if (isTent(Matriz[p.x][p.y]) && !isTent(old_value)) {
      tendas_rest++;
      Lrests[p.x]++;
      Crests[p.y]++;
    }
    Matriz[p.x][p.y] = old_value;
  }
  // Obter número de alterações desde o snapshot anterior a este
  pop(b_n, &n_since_snapshot);
}

/* Descrição: Altera a matriz e se record_changes não for 0 guarda a alteração
 * */
void edit_matriz(Point point, char new_char) {
  if (record_changes) {
    regista_alteracao(point, Matriz[point.x][point.y]);
  }
  Matriz[point.x][point.y] = new_char;
}

/* Funções Analyse... utilizadas pelo ChangePropagator * * * * * * * * * * * *
 */

/* Descrição: Compara os espaços livres com o número de tendas restantes na
 * linha e coluna do Point fornecido. Se record_changes não for 0 faz os
 * registos para backtracking.
 * */
int AnalyseLinhaColunaLimits(Point ponto) {
  Point p;
  int espacos_livres, comprimento, i;

  // Se há tendas por colocar na linha
  if (Lrests[ponto.x] > 0) {
    // Conta espaços livres da linha
    p.x = ponto.x;
    comprimento = 0; // Comprimento de um bloco contíguo de opens na matriz
    espacos_livres = 0;
    for (p.y = 0; p.y < C; p.y++) {
      if (Matriz[p.x][p.y] == '0') {
        comprimento++;
      } else if (comprimento != 0) {
        // Blocos de n opens podem conter (n +1)/2 tendas
        espacos_livres += (comprimento + 1) / 2;
        comprimento = 0;
      }
    }
    // se o último char for open ainda falta adicionar o último bloco
    if (comprimento != 0) {
      espacos_livres += (comprimento + 1) / 2;
    }
    if (espacos_livres < Lrests[p.x])
      return -1;
    if (espacos_livres == Lrests[p.x]) {
      // Há uma tenda para cada espaço livre
      comprimento = 0;
      for (i = 0; i < C; i++) {
        if (Matriz[p.x][i] == '0')
          comprimento++;
        else {
          if (comprimento % 2 == 1) {
            // em blocos ímpares as tendas são as pontas e dois em dois no seu
            // interior. Ex: se 0000000 tem 4 tendas têm de ser T0T0T0T
            for (p.y = i - 1; i - p.y <= comprimento; p.y -= 2) {
              edit_matriz(p, NEW_T_UNPAIRED);
              Crests[p.y]--;
              Lrests[p.x]--;
              tendas_rest--;
              push(points_toAnalyse, &p);
            }
          }
          // nada se conclui em blocos pares sobre a posição das tendas no seu
          // interior
          comprimento = 0;
        }
      }
      // se o ultimo char for de um bloco ímpar ainda falta alterar o bloco
      if (comprimento % 2 == 1) {
        for (p.y = i - 1; i - p.y <= comprimento; p.y -= 2) {
          edit_matriz(p, NEW_T_UNPAIRED);
          Crests[p.y]--;
          Lrests[p.x]--;
          tendas_rest--;
          push(points_toAnalyse, &p);
        }
      }
    }
  }
  // Se há tendas por colocar na coluna
  if (Crests[ponto.y] > 0) {
    // Conta espaços livres da coluna
    p.y = ponto.y;
    comprimento = 0;
    espacos_livres = 0;
    for (p.x = 0; p.x < L; p.x++) {
      if (Matriz[p.x][p.y] == '0') {
        comprimento++;
      } else if (comprimento != 0) {
        // Blocos de n opens podem conter (n +1)/2 tendas
        espacos_livres += (comprimento + 1) / 2;
        comprimento = 0;
      }
    }
    // se o último char for open ainda falta adicionar o último bloco
    if (comprimento != 0) {
      espacos_livres += (comprimento + 1) / 2;
    }

    if (espacos_livres < Crests[p.y])
      return -1;
    if (espacos_livres == Crests[p.y]) {
      // Há uma tenda para cada espaço livre
      comprimento = 0;
      for (i = 0; i < L; i++) {
        if (Matriz[i][p.y] == '0') {
          comprimento++;
        } else {
          if (comprimento % 2 == 1) {
            // em blocos ímpares as tendas são as pontas e dois em dois no seu
            // interior. Ex: se 0000000 tem 4 tendas têm de ser T0T0T0T
            for (p.x = i - 1; i - p.x <= comprimento; p.x -= 2) {
              edit_matriz(p, NEW_T_UNPAIRED);
              Lrests[p.x]--;
              Crests[p.y]--;
              tendas_rest--;
              push(points_toAnalyse, &p);
            }
          }
          // nada se conclui em blocos pares sobre a posição das tendas no seu
          // interior
          comprimento = 0;
        }
      }
      // se o ultimo char for de um bloco ímpar ainda falta alterar o bloco
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

/* Descrição das seguintes funções Analyse... :
 * Analisam um objeto no mapa (tenda, árvore, ponto ou open) e os pontos
 * possivelmente afetados por ele ou pela sua colocação, alteram o que for
 * possível determinar como consequência inevitável das regras e fazem push dos
 * pontos possivelmente afetados para análise posterior por ChangePropagator. Se
 * record_changes for 1 fazem os registos para backtracking.
 * Retorno: -1 se encontrarem uma violação das regras de jogo, 0 caso contrário.
 * */

/* Regras analisadas: Se a tenda não tiver par: a existência de árvores
 * disponíveis para par. Se a tenda for nova: o limite de tendas na linha e
 * coluna e a adjacência de tendas.
 * Argumentos: Posição da tenda e flags que indicam se a tenda tem par ou é
 * nova.
 * */
int AnalyseTent(Point tent, int isPaired, int isNew) {
  Point p, a;
  int arvores_sem_par = 0;
  // Análise local:
  // Se a tenda não é nova, não é preciso repetir análise dos opens ou das
  // tendas adjacentes
  for (p.x = tent.x - 1; p.x <= tent.x + 1; p.x++) {
    for (p.y = tent.y - 1; p.y <= tent.y + 1; p.y++) {
      // Se p está dentro dos limites e é adjacente
      if (p.x >= 0 && p.x < L && p.y >= 0 && p.y < C &&
          (p.x != tent.x || p.y != tent.y)) {

        if (isNew && Matriz[p.x][p.y] == '0') {
          edit_matriz(p, '.');
          push(points_toAnalyse, &p);
        } else if (isNew && isTent(Matriz[p.x][p.y])) {
          return -1;
        } else { // Se é adjacente não diagonal
          if (p.x == tent.x || p.y == tent.y) {
            if (Matriz[p.x][p.y] == 'A') {
              arvores_sem_par++;
              a = p;
              if (isPaired && epoca_alta)
                // Ver se a árvore ficou só com um possível par
                push(points_toAnalyse, &a);
            }
          }
        }
      }
    }
  }

  if (!isPaired && arvores_sem_par == 0)
    return -1;
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

    // Análise dos limites de linha e coluna
    if (Lrests[tent.x] < 0)
      return -1;
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
    if (Crests[tent.y] < 0)
      return -1;
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
/* Regras analisadas: A existência de tendas disponíveis para par. Assume que é
 * época alta e que a árvore não tem par.
 * Argumentos: Posição da árvore.
 * */
int AnalyseTree(Point tree) {
  Point disp; // Ponto disponível, se existir.
  int disponiveis = 0;
  // Contar pontos adjacentes que podem conter uma tenda ainda disponível para
  // ser par
  if (tree.x > 0) {
    if (Matriz[tree.x - 1][tree.y] == '0' ||
        Matriz[tree.x - 1][tree.y] == 'T' ||
        Matriz[tree.x - 1][tree.y] == NEW_T_UNPAIRED) {
      disponiveis++;
      disp.x = tree.x - 1;
      disp.y = tree.y;
    }
  }
  if (tree.x < L - 1) {
    if (Matriz[tree.x + 1][tree.y] == '0' ||
        Matriz[tree.x + 1][tree.y] == 'T' ||
        Matriz[tree.x + 1][tree.y] == NEW_T_UNPAIRED) {
      disponiveis++;
      disp.x = tree.x + 1;
      disp.y = tree.y;
    }
  }
  if (tree.y > 0) {
    if (Matriz[tree.x][tree.y - 1] == '0' ||
        Matriz[tree.x][tree.y - 1] == 'T' ||
        Matriz[tree.x][tree.y - 1] == NEW_T_UNPAIRED) {
      disponiveis++;
      disp.x = tree.x;
      disp.y = tree.y - 1;
    }
  }
  if (tree.y < C - 1) {
    if (Matriz[tree.x][tree.y + 1] == '0' ||
        Matriz[tree.x][tree.y + 1] == 'T' ||
        Matriz[tree.x][tree.y + 1] == NEW_T_UNPAIRED) {
      disponiveis++;
      disp.x = tree.x;
      disp.y = tree.y + 1;
    }
  }
  // Árvore sozinha.
  if (disponiveis == 0)
    return -1;
  if (disponiveis == 1) {
    edit_matriz(tree, 'a');
    // Se disp é open ou uma tenda nova marcar como nova
    if (Matriz[disp.x][disp.y] != 'T') {
      if (Matriz[disp.x][disp.y] == '0') {
        Lrests[disp.x]--;
        Crests[disp.y]--;
        tendas_rest--;
      }
      edit_matriz(disp, NEW_T_PAIRED);
    } else {
      // se disp é uma tenda já analisada, marcar como já analisada
      edit_matriz(disp, 't');
    }
    push(points_toAnalyse, &disp);
  }
  return 0;
}
/* Regras analisadas: O número de espaços livres na linha e coluna.
 * Argumentos: Posição do ponto.
 * */
int AnalysePoint(Point ponto) {
  Point p;
  if (AnalyseLinhaColunaLimits(ponto) == -1)
    return -1;
  if (epoca_alta) {
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
/* Regras analisadas: A existência de árvores disponíveis para par e, se o
 * primeiro falhar, o número de espaços livres na linha e coluna.
 * Argumentos: Posição do open.
 * */
int AnalyseOpen(Point open) {
  if (open.x > 0) {
    if (Matriz[open.x - 1][open.y] == 'A')
      return 0;
  }
  if (open.x < L - 1) {
    if (Matriz[open.x + 1][open.y] == 'A')
      return 0;
  }
  if (open.y > 0) {
    if (Matriz[open.x][open.y - 1] == 'A')
      return 0;
  }
  if (open.y < C - 1) {
    if (Matriz[open.x][open.y + 1] == 'A')
      return 0;
  }
  // Se já não há árvores disponíveis adjacentes, não pode haver tenda.
  edit_matriz(open, '.');
  return AnalyseLinhaColunaLimits(open);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/* Descrição: Avalia o ponto fornecido e altera o que for possível determinar
 * como consequência necessária das regras. Analisa os pontos afetados para
 * avaliar se consegue tirar conclusões a partir das alterações que fez. Se
 * record_changes for 1 faz os registos para backtracking.
 * Argumentos: O ponto (x, y) a analisar e uma flag que indica se deve ser feito
 * o teste correspondente ao char na Matriz (0) ou comparar os espaços livres e
 * as tendas restantes da linha e da coluna do ponto (1).
 * Retorno: Devolve -1 se detetar uma violação das regras, 0 caso contrário.
 * */
int ChangePropagator(int x, int y, int line_column_test) {
  Point p;
  char c;
  int res = 0;
  points_toAnalyse = initStack(8, sizeof(Point));
  p.x = x;
  p.y = y;
  if (line_column_test) {
    if (AnalyseLinhaColunaLimits(p) == -1)
      res = -1;
  } else {
    push(points_toAnalyse, &p);
  }
  while (res != -1 && !isStackEmpty(points_toAnalyse)) {
    pop(points_toAnalyse, &p);
    c = Matriz[p.x][p.y];
    if (isTent(c)) {
      if (AnalyseTent(p, isPairedTent(c), isNewTent(c)) == -1)
        res = -1;
    } else if (c == '.') {
      if (AnalysePoint(p) == -1)
        res = -1;
    } else if (c == 'A' && epoca_alta) {
      if (AnalyseTree(p) == -1)
        res = -1;
    } else if (c == '0') {
      if (AnalyseOpen(p) == -1)
        res = -1;
    }
  }
  freeStack(points_toAnalyse);
  return res;
}
/* Descrição: Encontra uma solução para um problema em que o ChangePropagator
 * não consiga tirar conclusões inevitáveis. Implementa uma procura depth-first
 * de entre os vários estados do mapa em que cada open pode ou não ter tenda.
 * Utiliza o Change Propagator para encontrar o estado resultante de cada
 * decisão tomada e as funções de backtracking para voltar aos estados
 * anteriores quando um estado não é legal.
 * Retorno: 1 se encontrar solução, -1 se for impossível.
 * */
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
    // Toma a próxima decisão no primeiro open a aparecer.
    for (; p.x < L; p.x++) {
      for (; p.y < C; p.y++) {
        if (Matriz[p.x][p.y] == '0') {
          // Tentar com tenda
          Matriz[p.x][p.y] = NEW_T_UNPAIRED;
          push(jogadas, &p);
          tendas_rest--;
          Lrests[p.x]--;
          Crests[p.y]--;
          create_snapshot();
          // Ver consequências necessárias
          if (ChangePropagator(p.x, p.y, 0) == -1) {
            // Violação de alguma regra, voltar atrás.
            p.x = L;
          }
          break;
        }
      }
      if (p.y != C)
        break;
      p.y = 0;
    }
    // Se não há mais opens ou houve alguma violação de regra
    if (p.x == L) {
      // Se estamos o mapa só tem alterações necessárias
      if (isStackEmpty(jogadas)) {
        freeStack(jogadas);
        freeStack(b_chars);
        freeStack(b_points);
        freeStack(b_n);
        return -1; // Impossível
      }
      revert_snapshot();

      // Se não pode ser tenda o open é '.'
      pop(jogadas, &p);
      tendas_rest++;
      Lrests[p.x]++;
      Crests[p.y]++;
      // Repõe-se diretamente o open para que edit_matriz guarde '0' nos
      // registos de backtrack
      Matriz[p.x][p.y] = '0';
      edit_matriz(p, '.');
      // Ver consequências necessárias
      if (ChangePropagator(p.x, p.y, 0) == -1) {
        // Violação de alguma regra, voltar atrás.
        p.x = L;
        continue;
      }
      p.y++;
    }
    // Se o programa chega aqui é porque ChangePropagator não encontrou violação
    // das regras
    if (tendas_rest == 0) {
      freeStack(jogadas);
      freeStack(b_chars);
      freeStack(b_points);
      freeStack(b_n);
      beautify_matriz(Matriz, L, C);
      return 1; // Solução encontrada
    }
  }
}

/* Descrição: Marca os pontos em que é possível existir tenda, chamados opens.
 * Utiliza o ChangePropagator para tirar conclusões inevitáveis sobre o mapa.
 * Retorno: 0 se não chegou a nenhuma conclusão, 1 se encontrou solução ou -1
 * se não existir solução.
 * */
int Prepare() {
  int i, j;

  record_changes = 0;
  /* Marca opens */
  for (i = 0; i < L; i++) {
    for (j = 0; j < C; j++) {
      if (Matriz[i][j] == 'A') {
        check_adj_for_opens(i, j, Matriz, L, C, Lrests, Crests);
      }
    }
  }

  // Avalia se há espaços livres suficientes em cada linha e coluna
  i = 0;
  // Diagonais primeiro para evitar repetir linhas ou colunas
  for (i = 0; i < L && i < C; i++) {
    if (ChangePropagator(i, i, 1) == -1)
      return -1;
  }
  // Se há mais colunas que linhas
  if (i == L) {
    // Colunas restantes
    for (; i < C; i++) {
      if (ChangePropagator(0, i, 1) == -1)
        return -1;
    }
  } else { // Se há mais linhas que colunas
    // Linhas restantes;
    for (; i < L; i++) {
      if (ChangePropagator(i, 0, 1) == -1)
        return -1;
    }
  }
  if (epoca_alta) {
    // Verifica se alguma árvore tem só 1 lugar disponível para tendas
    for (i = 0; i < L; i++) {
      for (j = 0; j < C; j++) {
        if (Matriz[i][j] == 'A') {
          if (ChangePropagator(i, j, 0) == -1)
            return -1;
        }
      }
    }
  }
  // Se o programa chega aqui é porque ChangePropagator não encontrou violação
  // das regras.
  if (tendas_rest == 0)
    return 1;
  return 0;
}
/* Descrição: Lê de um ficheiro e resolve um problema de Trees and Tents com
 * procura de solução e escreve o resultado num ficheiro de saída.
 * Argumentos: Apontadores para o ficheiro com o problema e para o ficheiro de
 * saída.
 * Retorno: -1 se ocorreu erro, 0 caso contrário.
 * */
int Solver(FILE *fp_problema, FILE *fp_saida) {
  int i, j, res;

  if (fscanf(fp_problema, " %d", &L) != 1)
    return -1;
  if (fscanf(fp_problema, " %d", &C) != 1)
    return -1;

  Lrests = (int *)malloc(L * sizeof(int));
  if (Lrests == NULL)
    return -1;
  Crests = (int *)malloc(C * sizeof(int));
  if (Crests == NULL) {
    free(Lrests);
    return -1;
  }
  // Ler vetores
  tendas_rest = Fill_Hints_checkSums(fp_problema, L, C, Lrests, Crests);

  // Se ocorreu erro
  if (tendas_rest == -2) {
    free(Crests);
    free(Lrests);
    return -1;
  }
  // Se os vetores não são coerentes ou algum valor é negativo
  if (tendas_rest == -1) {
    fprintf(fp_saida, "%d %d %d\n\n", L, C, -1);
    free(Crests);
    free(Lrests);
    return 0;
  }

  Matriz = (char **)calloc(L, sizeof(char *));
  if (Matriz == NULL)
    return -1;

  arvores = Fill_Matriz(fp_problema, Matriz, L, C);
  // Se ocorreu erro
  if (arvores < 0) {
    fprintf(fp_saida, "%d %d %d\n\n", L, C, -1);
    free(Crests);
    free(Lrests);
    _free_matriz(Matriz, L);
    return -1;
  }
  if (tendas_rest > arvores) {
    // Tem de haver pelo menos uma árvore por tenda
    fprintf(fp_saida, "%d %d %d\n\n", L, C, -1);
    free(Lrests);
    free(Crests);
    _free_matriz(Matriz, L);
    return 0;
  }

  epoca_alta = tendas_rest == arvores;
  // cria opens na matriz e aplica consequências inevitáveis
  res = Prepare();
  // Se resolveu ou é impossível
  if (res != 0) {
    fprintf(fp_saida, "%d %d %d\n", L, C, res);
    // Se resolveu escreve a solução na saída
    if (res == 1) {
      beautify_matriz(Matriz, L, C);
      for (i = 0; i < L; i++) {
        for (j = 0; j < C; j++) {
          fprintf(fp_saida, "%c", Matriz[i][j]);
        }
        fprintf(fp_saida, "\n");
      }
    }
    fprintf(fp_saida, "\n");
    free(Lrests);
    free(Crests);
    _free_matriz(Matriz, L);
    return 0;
  }
  // Como Prepare não foi conclusivo é preciso procurar de entre os estados
  // possíveis
  res = Guesser();

  fprintf(fp_saida, "%d %d %d\n", L, C, res);
  // Se resolveu escreve a solução na saída
  if (res != -1) {
    for (i = 0; i < L; i++) {
      for (j = 0; j < C; j++) {
        fprintf(fp_saida, "%c", Matriz[i][j]);
      }
      fprintf(fp_saida, "\n");
    }
  }
  fprintf(fp_saida, "\n");
  free(Lrests);
  free(Crests);
  _free_matriz(Matriz, L);
  return 0;
}
