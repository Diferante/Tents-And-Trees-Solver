#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "variants.h"


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


/*Função que lê e analisa os dados do ficheiro de entrada*/
void LeituraDados(FILE* fp, FILE* fp2) {
    int L, C;
    int l0, c0;
    char variante;
    char resposta;

    printf("olá, im here \n");

    fscanf(fp, " %d", &L);
    fscanf(fp, " %d", &C);
    fscanf(fp, " %c", &variante);
    /* falta meter o C */
    switch(variante) {
    case 'A':
        resposta = solveA_fromFile(fp, L, C);
        fprintf(fp2, "%d %d %c %d\n", L, C, variante, resposta);
        break;
    case 'B':
        fscanf(fp, " %d", &l0);
        fscanf(fp, " %d", &c0);
        resposta = solveB_fromFile(fp, L, C, l0, c0);
        fprintf(fp2, "%d %d %c %d %d %d\n", L, C, variante, l0, c0, resposta);
        break;
    default:
        resposta = -1;
        fprintf(fp2, "%d %d %c %d\n", L, C, variante, resposta);
        break;

    }

    /* verificar se há mais problemas no ficheiro de entrada */

    // Apagar potencial resto da matriz atual
    do{
        L = fgetc(fp);
        if(L == EOF){
            return;
        }
    }while(L < '0' || L > '9');
    fseek(fp, -1L, SEEK_CUR);
    LeituraDados(fp, fp2);

}

int main(int argc, char *argv[]) {
    char *p;
    FILE *fp, *fp2;
    char *ficheirosaida;

    if(argc<2) exit(0);

    /* verificar se o ficheiro de entrada tem extensão '.camp0' */
    p=strrchr(argv[1], '.');
    if(strcmp(p, ".camp0")) exit(0);

    /*Abertura do ficheiro de entrada*/
    fp=AbreFicheiro(argv[1], "r");

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

