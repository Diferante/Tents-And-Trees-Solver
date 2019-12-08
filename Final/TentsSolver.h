/******************************************************************************
 * 2019-2020
 * Autores - Haohua Dong e Diogo Antunes
 *
 * DESCRIÇÃO
 *  Header de uma implementação de um gerador de soluções para problemas do jogo
 *	Trees and Tents. A entrada do problema e a saída do resultado são
 *	realizadas através de ficheiros.
 *
 *	Dependências:
 *    stdio.h
 *    stdlib.h
 *    TreesTents.h
 *	  generalStack.h
 *
 *****************************************************************************/

#ifndef TENTSSOLVER_H_INCLUDED
#define TENTSSOLVER_H_INCLUDED

#include <stdio.h>

/* Descrição: Lê de um ficheiro e resolve um problema de Trees and Tents com
 * procura de solução e escreve o resultado num ficheiro de saída.
 * Argumentos: Apontadores para o ficheiro com o problema e para o ficheiro de
 * saída. Retorno: 0 se não chegou a nenhuma conclusão, 1 se encontrou solução
 * ou -1 se não existir solução. Retorno: -1 se ocorreu erro, 0 caso contrário.
 * */
int Solver(FILE *fp_problema, FILE *fp_saida);

#endif // TENTSSOLVER_H_INCLUDED
