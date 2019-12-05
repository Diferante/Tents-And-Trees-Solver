#ifndef TREESTENTS_H_INCLUDED
#define TREESTENTS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generalStack.h"
#include "TreesTents.h"

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

void _free_matriz(char **Matriz, int L);

void handle_int(int res, int i, int *somaL, int *somaC, int *Ltents, int *Ctents, int L, int C) ;
// Descrição: Lẽ as hints do ficheiro e soma os vetores.
// Argumentos:
// Retorno: -2 se ocorrer erro, se não devolve o número de tendas ou -1 caso os
// vetores sejam coerentes.
int Fill_Hints_checkSums(FILE *fp, int L, int C, int *Ltents, int *Ctents) ;

// Descrição: Lê a matriz do ficheiro e conta as árvores.
// Argumentos:
// Retorno: O número de árvores se ler bem, -1 se chegar ao fim do ficheiro ou
// erro de alocação.
int Fill_Matriz_easy(FILE *fp, char **Matriz, int L, int C) ;

void move_dir(int *l_out, int *c_out, char dir) ;

// Descrição: Determina se esta tenda possui árvores adjacentes disponíveis.
// Altera a matriz. Argumentos: Linha e coluna da tenda. Retorno: 0 caso a tenda
// tenha árvore disponível, 1 caso contrário.
char isT_alone_iter(int l0, int c0, char **Matriz, int L, int C) ;

void repair_matriz(char **Matriz, int L, int C) ;
void beautify_matriz(char **Matriz, int L, int C) ;

/* atualiza os Opens around (x, y)*/
void add_around(int x, int y, int value, char **Matriz, int L, int C) ;

void printMatriz(char **Matriz, int L, int C);


/* Descrição: Verifica se pode haver tenda nas quatro direções,
 * coloca '0' nos que puderem
 * Argumentos: Posição (x,y) da árvore a ser verificada
 * */
void check_adj_for_opens(int x, int y, char **Matriz, int L, int C, int *Ltents, int *Ctents) ;
/* Descrição: Verifica tendas em todas posições adjacentes
 * Argumentos: Posição (x,y) do mapa
 * Retorno: Retorna 0 se existir pelo menos uma tenda na adjacência, 1 se não
 * existir nenhuma tenda
 * */
int sem_tendas_adj(int x, int y, char **Matriz, int L, int C) ;

/* Descrição: Substitui todos os opens num quadrado 3x3 por '.' */
void pontos_around(int x, int y, char **Matriz, int L, int C) ;
/* Descrição:Dois modos dependendo do valor type:
 *
 * 0. Verifica a posição (x, y) para ver se tem 1 Open e que não tem 'T'
 * adjacentes
 *
 * 1. Testa se a Posição (x, y) está isolada com 1 Open e substitui o Open por
 * 't' e a árvore por 'a' se for ÉPOCA ALTA e chama a função teste_tendas_2 para
 * alterar Opens adjacentes a '.'
 * 2. Testa só a esquerda e a direita se a Posição (x, y) está isolada com 1
 * Open, Se estiver isolada, então 'T' aí e chama a função teste_tendas_2 para
 * alterar Opens adjacentes a '.'
 * */
void teste_opens(int x, int y, int type, char **Matriz, int L, int C) ;
/* Descrição: Testa se a Posição (x, y) está isolada com só 1 Open ou 'T' e
 * substitui-o por 't' e a árvore por 'a' e chama a função pontos_around para
 * alterar Opens adjacentes para '.'.
 * Retorno: 1 se era isolada com 1 Open/'T', 0 se não.
 * */
int arvore_facil(int x, int y, char **Matriz, int L, int C, int *Ltents, int *Ctents, int *tendas_rest);

/* Descrição: Transforma opens da linha em tendas e chama pontos_around para
 *remover os opens adjacentes
 * Argumentos: linha x
 * */
//void Linha_fill(int x, Jogo *jogo) ;


#endif // TREESTENTS_H_INCLUDED
