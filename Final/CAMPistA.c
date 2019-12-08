/******************************************************************************
 * 2019-2020
 * Autores - Haohua Dong e Diogo Antunes
 *
 * DESCRIÇÃO
 *  Programa que utiliza o TentsSolver para resolver um conjunto de problemas
 *  do jogo Trees and Tents fornecidos num ficheiro .camp e cria um ficheiro
 *  .tents com os resultados. O programa não escreve nada no terminal.
 *
 *  Utilização:
 *      <nome_do_programa> nome_do_ficheiro_com_problema.camp
 *****************************************************************************/

#include "TentsSolver.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024
/* Abre o ficheiro */
FILE *AbreFicheiro(char *ficheiro, char *mode) {
  FILE *fp;
  fp = fopen(ficheiro, mode);
  if (fp == NULL)
    exit(0);

  return (fp);
}

/* Escreve os dados no ficheiro de saída */
void EscreveFicheiro(int L, int C, char variant, int resposta, FILE *fp2) {
  fprintf(fp2, "%d %d %c %d\n", L, C, variant, resposta);
  return;
}

/* Descrição: Lê do ficheiro, ignora eventuais restos de matriz de um problema
 * anterior, até encontrar o início de outro problema ou chegar ao fim do
 * ficheiro. Utiliza um buffer para ler em blocos. Assume que os primeiros
 * números que encontrar são o cabeçalho do próximo problema, por isso não pode
 * ser chamada se ainda sobrarem os vetores de números do problema anterior.
 * Argumentos: Apontador para o ficheiro de entrada.
 * Retorno: 1 se há mais problemas, 0 se não.
 * */
int Ha_mais_problemas(FILE *fp) {
  int b;                    // Índice no buffer do char atual
  int nbytes;               // Bytes lidos
  char buffer[BUFFER_SIZE]; // Buffer de leitura
  // Ler do ficheiro um bloco
  nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
  b = -1;
  while (1) {
    // O próximo problema será a primeira ocorrência de números
    for (b++; b < nbytes; b++) {
      if (buffer[b] >= '0' && buffer[b] <= '9') {
        // Encontrou o próximo problema
        // Retroceder o FILE* o que leu a mais, para o início do próximo
        // problema
        fseek(fp, b - nbytes, SEEK_CUR);
        return 1;
      }
    }
    if (nbytes == b) {
      // Buffer processado, ler mais um bloco
      nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
      if (nbytes == 0) // Fim do ficheiro
        return 0;
      b = -1;
      continue;
    }
  }
}

/* Descrição: Chama Solver de TentsSolver.h para resolver cada problema
 * individual no ficheiro de entrada e escrever o seu resultado no ficheiro de
 * saída.
 * Argumentos: Apontadores para o ficheiro de entrada e o de saída.
 * */
void Resolucao(FILE *fp_in, FILE *fp_out) {
  while (1) {
    Solver(fp_in, fp_out);

    /* verificar se há mais problemas no ficheiro de entrada */
    if (!Ha_mais_problemas(fp_in))
      break;
  }
}
/* Descrição: Lê um ficheiro .camp recebido como argumento com problemas do
 * jogo Trees and Tents, resolve-os com o Solver de TentsSolver.h e cria um
 * ficheiro .tents para guardar os resultados.
 * */
int main(int argc, char *argv[]) {
  char *p;
  FILE *fp_in, *fp_out;
  char *ficheirosaida;

  if (argc < 2)
    exit(0);

  // verificar se o ficheiro de entrada tem extensão '.camp'
  p = strrchr(argv[1], '.');
  if (p == NULL || strcmp(p, ".camp"))
    exit(0);

  fp_in = AbreFicheiro(argv[1], "rb");

  // Ao passar de .camp para .tents é preciso mais 1 char
  ficheirosaida = (char *)malloc(strlen(argv[1]) + 1 + 1);
  if (ficheirosaida == NULL)
    exit(0);

  // Cria o nome do ficheiro de saída a partir do de entrada
  strcpy(ficheirosaida, argv[1]);
  p = strrchr(ficheirosaida, '.');
  strcpy(p, ".tents");

  fp_out = AbreFicheiro(ficheirosaida, "w");

  Resolucao(fp_in, fp_out);

  fclose(fp_in);
  fclose(fp_out);

  free(ficheirosaida);

  return 0;
}
