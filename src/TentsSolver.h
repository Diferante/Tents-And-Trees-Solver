/******************************************************************************
 *  Copyright (C) 2019 - Haohua Dong & Diogo Antunes
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
 *  Header de uma implementação de um gerador de soluções para problemas do jogo
 *	Trees and Tents. A entrada do problema e a saída do resultado são
 *	realizadas através de ficheiros.
 *
 *	Dependências:
 *    stdio.h
 *    stdlib.h
 *    TreesTents.h
 *	  generalStack.h // IN IT'S OWN REPOSITORY
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
