#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct{
    char endereco[50];
    char opcode[50];
    char sigla[50];
    char rd[50];
    char rm[50];
    char rn[50];
}Instrucao;

typedef struct{
    Instrucao lista_de_instrucoes[50];
    int total;
}GuardaInstrucao;

void remover_comentario(char *linhaLida){
    char *coment = strstr(linhaLida, "//"); // strstr retorna a posição exata do elemento buscado
    if(coment != NULL) *coment = '\0';
}

void busca_elementos(FILE *arq, GuardaInstrucao* instrucao){
    
    char linhaLida[256];
    int i = 0;

    while(fgets(linhaLida, sizeof(linhaLida), arq) && i < 50){
        remover_comentario(linhaLida);
        char *ptr_endereco = strtok(linhaLida, " \t\n");
        char *ptr_opCode = strtok(NULL, " \t\n");
        char *ptr_sliga = strtok(NULL, " \t\n");
        char *ptr_rd = strtok(NULL, " \t\n,#");
        char *ptr_rm = strtok(NULL, " \t\n,#");
        char *ptr_rn = strtok(NULL, " \t\n,#");
        if(ptr_endereco != NULL && ptr_opCode != NULL){
            strcpy(instrucao->lista_de_instrucoes[i].endereco, ptr_endereco);
            strcpy(instrucao->lista_de_instrucoes[i].opcode, ptr_opCode);
            strcpy(instrucao->lista_de_instrucoes[i].sigla, ptr_sliga);
            if(ptr_rd != NULL) strcpy(instrucao->lista_de_instrucoes[i].rd, ptr_rd);
            if(ptr_rm) strcpy(instrucao->lista_de_instrucoes[i].rm, ptr_rm);
            if(ptr_rn) strcpy(instrucao->lista_de_instrucoes[i].rn, ptr_rn);
            i++;
        }
    }
    instrucao->total = i;
}

int main(){

    FILE *file = fopen("example1.hex", "r");
    GuardaInstrucao *instrucao = malloc(sizeof(GuardaInstrucao));
    busca_elementos(file, instrucao);

    int i = 0;
    while(i < 11){
        printf("Endereço: %s | RD: %s\n", instrucao->lista_de_instrucoes[i].endereco, instrucao->lista_de_instrucoes[i].sigla);
        i++;
    }


    fclose(file);
    free(instrucao);
    return 0;

}
