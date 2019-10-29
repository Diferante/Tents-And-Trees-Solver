#ifndef VARIANTS_H_INCLUDED
#define VARIANTS_H_INCLUDED

#include <stdio.h>

void InitSolver(FILE* fpointer, int Linhas, int Colunas);
int SolveAfromFile();
int SolveBfromFile(int l0, int c0);

// Descrição: Determina se há tendas ilegais, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de 'C', número de linhas e número de colunas.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
int SolveCfromFile();


#endif // VARIANTS_H_INCLUDED
