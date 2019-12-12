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
 *  Header de um implementação de funções de auxílio das funções em TentsSolver
 *  na resolução de problemas do jogo Trees and Tents.
 *
 *  Lista de funções:
 *    A) Inicialização e Terminação
 *        Fill_Hints_checkSums
 *        Fill_Matriz
 *	      _free_matriz
 *
 *    B) Alteração da matriz
 *        beautify_matriz
 *		  check_adj_for_opens
 *
 *    C) Testes
 *        isTent
 *		  isPairedTent
 *		  isNewTent
 *		  sem_tendas_adj
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
 *	Dependências:
 *    stdio.h
 *    stdlib.h
 *
 *****************************************************************************/

#ifndef TREESTENTS_H_INCLUDED
#define TREESTENTS_H_INCLUDED

#include <stdio.h>

#define NEW_T_UNPAIRED 'N'
#define NEW_T_PAIRED 'n'

/* Descrição: Liberta a matriz de L linhas.
 * */
void _free_matriz(char **Matriz, int L);

/* Descrição: Lê os vetores com as Hints, o número de tendas em cada linha e
 * coluna, de um ficheiro, guarda-os e calcula a sua soma.
 * Argumentos: Apontador para o ficheiro de entrada, número de linhas e colunas
 * da matriz e apontadores para os vetores onde guardar as Hints.
 * Retorno: -2 se ocorrer erro, -1 se os vetores forem incoerentes ou alguma
 * Hint for negativa, caso contrário o número de tendas.
 * */
int Fill_Hints_checkSums(FILE *fp, int L, int C, int *Lrests, int *Crests);

/* Descrição: Lê a matriz do ficheiro, guarda-a e conta as árvores.
 * Argumentos:
 *  fp          - Apontador para o ficheiro com a Matriz.
 *  Matriz      - Apontador para uma tabela de L apontadores para char onde
 *                guardar a matriz lida.
 *  L e C       - Números de linhas e colunas da matriz.
 * Retorno: O número de árvores se ler bem, -1 se ocorrer erro de
 * alocação ou leitura.
 * */
int Fill_Matriz(FILE *fp, char **Matriz, int L, int C);

/* Descrição: 1 se c for uma tenda, 0 se não.
 * */
int isTent(char c);

/* Descrição: 1 se c for uma tenda com par, 0 se não.
 * */
int isPairedTent(char c);

/* Descrição: 1 se c for uma nova tenda, 0 se não.
 * */
int isNewTent(char c);

/* Descrição: Prepara matriz para saída, troca os char's de processamento para
 * os seus valores finais.
 * Argumentos: Apontador para a Matriz e os números de linhas e colunas da
 * matriz.
 * */
void beautify_matriz(char **Matriz, int L, int C);

/* Descrição: Verifica se pode haver alguma tenda nas quatro direções de uma
 * árvore, marca como open se for possível.
 * Argumentos: Posição (x,y) da árvore, apontador para Matriz, os números de
 * linhas e colunas da matriz e os vetores com as tendas de cada linha e coluna.
 * */
void check_adj_for_opens(int x, int y, char **Matriz, int L, int C, int *Lrests,
                         int *Crests);
/* Descrição: Verifica se existem tendas em todas posições adjacentes.
 * Argumentos: Posição (x,y) da tenda, apontador para a matriz e os números de
 * linhas e colunas da matriz.
 * Retorno: Retorna 0 se existir pelo menos uma tenda na adjacência, 1 se não
 * existir nenhuma tenda.
 * */
int sem_tendas_adj(int x, int y, char **Matriz, int L, int C);

#endif // TREESTENTS_H_INCLUDED
