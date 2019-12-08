/******************************************************************************
 * 2019-2020
 * Autores - Haohua Dong e Diogo Antunes
 *
 * DESCRIÇÃO
 *  Header de uma implementação de um solucionador de problemas do jogo Trees
 *	and Tents.
 *
 *****************************************************************************/

#ifndef TENTSSOLVER_H_INCLUDED
#define TENTSSOLVER_H_INCLUDED

#include <stdio.h>

/* Descrição: Lê de um ficheiro e resolve um problema de Tents And Trees com
 * procura de solução e escreve o resultado num ficheiro de saida.
 * Argumentos: Apontadores para o ficheiro com o problema e para o ficheiro de
 * saída. Retorno: 0 se não checgou a nenhuma conclusão, 1 se encontrou solução
 * ou -1 se não existir solução. Retorno: -1 se ocorreu erro, 0 caso contrário.
 * */
int Solver(FILE *fp_problema, FILE *fp_saida);

#endif // TENTSSOLVER_H_INCLUDED
