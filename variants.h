#ifndef VARIANTS_H_INCLUDED
#define VARIANTS_H_INCLUDED

#include <stdio.h>

int SolveBfromFile(FILE *fp, int L, int C, int l0, int c0);
int SolveAfromFile(FILE* fp, int L, int C);

// Descrição: Determina se há tendas ilegais, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de 'C', número de linhas e número de colunas.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
int SolveCfromFile(FILE* fp, int L, int C);

#endif // VARIANTS_H_INCLUDED
