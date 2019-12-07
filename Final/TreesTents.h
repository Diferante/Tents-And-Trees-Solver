#ifndef TREESTENTS_H_INCLUDED
#define TREESTENTS_H_INCLUDED

#include <stdio.h>

/*#define T 'T'
#define A 'A'
#define O '0'
#define t 't'
#define a 'a'
//Celulas para backtrack do Guesser:
	pontos: 1 - 9 
*/
#define TMP_T 'G'
#define TMP_A 'H'
#define NEW_T_PAIRED 'n'
#define NEW_T_UNPAIRED 'N'

void _free_matriz(char **Matriz, int L);

void handle_int(int res, int i, int *somaL, int *somaC, int *Lrests,
                int *Crests, int L, int C);
// Descrição: Lẽ as hints do ficheiro e soma os vetores.
// Argumentos:
// Retorno: -2 se ocorrer erro, se não devolve o número de tendas ou -1 caso os
// vetores sejam coerentes.
int Fill_Hints_checkSums(FILE *fp, int L, int C, int *Lrests, int *Crests);

// Descrição: Lê a matriz do ficheiro e conta as árvores.
// Argumentos:
// Retorno: O número de árvores se ler bem, -1 se chegar ao fim do ficheiro ou
// erro de alocação.
int Fill_Matriz_easy(FILE *fp, char **Matriz, int L, int C);

void move_dir(int *l_out, int *c_out, char dir);

// Descrição: Determina se esta tenda possui árvores adjacentes disponíveis.
// Altera a matriz. Argumentos: Linha e coluna da tenda. Retorno: 0 caso a tenda
// tenha árvore disponível, 1 caso contrário.
char isT_alone_iter(int l0, int c0, char **Matriz, int L, int C);
/* Descrição: 1 se c for uma tenda, 0 se não.
 * */
int isTent(char c);

/* Descrição: 1 se c for uma tenda com par, 0 se não.
 * */
int isPairedTent(char c);
/* Descrição: 1 se c for uma nova tenda, 0 se não.
 * */
int isNewTent(char c) ;
void repair_matriz(char **Matriz, int L, int C);

/* Descrição: Prepara matriz para escrita,
 * troca os char's de processamento para os seus valores finais.
 * */
void beautify_matriz(char **Matriz, int L, int C);

/* atualiza os Opens around (x, y)*/
void add_around(int x, int y, int value, char **Matriz, int L, int C);

// Ferramenta para debug
void printMatriz(char **Matriz, int L, int C);

/* Descrição: Verifica se pode haver tenda nas quatro direções,
 * coloca '0' nos que puderem
 * Argumentos: Posição (x,y) da árvore a ser verificada
 * */
void check_adj_for_opens(int x, int y, char **Matriz, int L, int C, int *Lrests,
                         int *Crests);
/* Descrição: Verifica tendas em todas posições adjacentes
 * Argumentos: Posição (x,y) do mapa
 * Retorno: Retorna 0 se existir pelo menos uma tenda na adjacência, 1 se não
 * existir nenhuma tenda
 * */
int sem_tendas_adj(int x, int y, char **Matriz, int L, int C);

#endif // TREESTENTS_H_INCLUDED
