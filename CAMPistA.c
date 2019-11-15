#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "variants.h"

#define BUFFER_SIZE 1024
/* Abre o ficheiro */
FILE *AbreFicheiro(char *ficheiro, char *mode) {
    FILE *fp;
    fp=fopen(ficheiro, mode);
    if(fp==NULL) exit(0);

    return(fp);
}

/* Escreve os dados no ficheiro de saída */
void EscreveFicheiro(int L, int C, char variant, int resposta, FILE *fp2) {
    fprintf(fp2, "%d %d %c %d\n", L, C, variant, resposta);
    return;
}

// Descrição:
// Argumentos:
// Retorno: 1 se há mais problemas, 0 se não.
int Ha_mais_problemas(FILE* fp) {
    int b;
    int nbytes;
    char buffer[BUFFER_SIZE];
    nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
    b = -1;
    while(1) {
        // Clear all untill number;
        for(b++; b < nbytes; b++) {
            if(buffer[b] >= '0' && buffer[b] <= '9'){
                fseek(fp, b - nbytes , SEEK_CUR);
                return 1;
            }

        }
        if(nbytes==b) {
            nbytes = fread(buffer, 1, BUFFER_SIZE, fp);
            if(nbytes == 0)
                return 0;
            b = -1;
            continue;
        }
    }
}

/*Função que lê e analisa os dados do ficheiro de entrada*/
void LeituraDados(FILE* fp, FILE* fp2) {
    int L, C;
    int l0, c0;
    char variante, resposta;

    while(1) {
        if( fscanf(fp, " %d", &L) != 1) exit(0);
        if( fscanf(fp, " %d", &C) != 1) exit(0);
        if( fscanf(fp, " %c", &variante) != 1) exit(0);
        InitSolver(fp, L, C);
        switch(variante) {
        case 'A':
            resposta = SolveAfromFile();
            fprintf(fp2, "%d %d %c %d\n\n", L, C, variante, resposta);
            break;
        case 'B':
            if( fscanf(fp, " %d", &l0) != 1) exit(0);
            if( fscanf(fp, " %d", &c0) != 1) exit(0);
            resposta = SolveBfromFile(l0, c0);
            fprintf(fp2, "%d %d %c %d %d %d\n\n", L, C, variante, l0, c0, resposta);
            break;
        case 'C':
            resposta = SolveCfromFile();
            fprintf(fp2, "%d %d %c %d\n\n", L, C, variante, resposta);
            break;
        default:
            resposta = -1;
            fprintf(fp2, "%d %d %c %d\n\n", L, C, variante, resposta);
            // Limpar os números antes de saír
            do {
                L = fgetc(fp);
            } while(L != 'A' && L != 'T' && L != '.');
            break;
        }

        /* verificar se há mais problemas no ficheiro de entrada */
        if(!Ha_mais_problemas(fp))
            break;
    }
}

int main(int argc, char *argv[]) {
    char *p;
    FILE *fp, *fp2;
    char *ficheirosaida;

    if(argc<2) exit(0);

    /* verificar se o ficheiro de entrada tem extensão '.camp0' */
    p=strrchr(argv[1], '.');
    if(p == NULL || strcmp(p, ".camp0")) exit(0);

    /*Abertura do ficheiro de entrada*/
    fp=AbreFicheiro(argv[1], "rb");

    /*Alocar memória para o nome do ficheiro de saída*/
    ficheirosaida = (char*)malloc((strlen(argv[1])+ 2)*sizeof(char));
    if(ficheirosaida==NULL) exit(0);
    strcpy(ficheirosaida, argv[1]);
    p=strrchr(ficheirosaida, '.');
    strcpy(p, ".tents0");

    /*Abrir ficheiro de saída para escrever */
    fp2=AbreFicheiro(ficheirosaida, "w");

    /* Análise de Dados */
    LeituraDados(fp, fp2);

    /* Fechar Ficheiros */
    fclose(fp);
    fclose(fp2);

    /* free memory */
    free(ficheirosaida);

    return 0;
}

