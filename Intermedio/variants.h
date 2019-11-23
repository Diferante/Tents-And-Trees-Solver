#ifndef VARIANTS_H_INCLUDED
#define VARIANTS_H_INCLUDED

#include <stdio.h>

void InitSolver(FILE* fpointer, int Linhas, int Colunas);

// Descrição: Determina se o problema é admissível, segundo os critérios de A, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de 'A', número de linhas e número de colunas.
// Retorno: 0 caso seja detetada a inadmissibilidade do problema, 1 caso contrário.
int SolveAfromFile();

// Descrição: Determina se pode existir uma tenda nas coordenadas fornecidas, de acordo com os critérios de B,
//            lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de "B l0 c0", linha e coluna.
// Retorno: 1 caso seja detetada a impossibilidade de existir uma tenda, 0 caso contrário.
int SolveBfromFile(int l0, int c0);

// Descrição: Determina se há tendas ilegais, lendo diretamente do ficheiro.
// Argumentos: Ficheiro com o problema apontando para depois de 'C', número de linhas e número de colunas.
// Retorno: 1 caso exista alguma tenda ilegal, 0 caso não exista, -1 caso ocorra um erro na alocação de memória.
int SolveCfromFile();


#endif // VARIANTS_H_INCLUDED
