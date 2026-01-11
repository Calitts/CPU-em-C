#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Intruções
#define JMP             (0x0) // Salto incondicional: PC recebe o endereço do alvo
#define JEQ             (0x1) // Salto se for igual: salta quando a flag zero está ativa
#define JNE             (0x1) // Salto se for diferente: salta quando a flag zero não está ativa
#define JLT             (0x1) // Salto se for menor que: salta quando a flag zero não está ativa e a carry está
#define JGE             (0x1) // Salto se for maior ou igual: salta quando a flag carry não está ativa e a zero está
#define LDR             (0x2) // Load Register: Carrega dado da memória para um registrador
#define STR             (0x3) // Store Register: Salva dado de um registrador na memória
#define MOV             (0x4) // Move: Copia o valor de um registrador para outro
#define ADD             (0x5) // Soma: Rd = Rn + Rm
#define ADDI            (0x6) // Soma Imediata: Rd = Rn + ValorConstante
#define SUB             (0x7) // Subtração: Rd = Rn - Rm
#define SUBI            (0x8) // Subtração Imediata: Rd = Rn - ValorConstante
#define AND             (0x9) // E Lógico: Operação bit a bit entre dois registradores
#define OR              (0xA) // OU Lógico: Operação bit a bit entre dois registradores
#define SHR             (0xB) // Shift Right: Desloca os bits para a direita (divide por 2)
#define SHL             (0xC) // Shift Left: Desloca os bits para a esquerda (multiplica por 2)
#define CMP             (0xD) // Compare: Subtrai valores apenas para atualizar as Flags (Z e C)
#define PUSH            (0xE) // Empilhar: Coloca o valor do registrador no topo da pilha
#define POP             (0xF) // Desempilhar: Remove o valor do topo da pilha para um registrador
#define HALT            (0xF) // Parar: Interrompe a execução do processador


int main(int argc, char* argv[]){
    // Checa argumentos
    if (argc < 2) {
        printf("Programa precisa de pelo menos 1 argumento.\n");
        return 1;
    }
    // Abre o arquivo
    char* arquivo = argv[1];
    FILE* arquivo_entrada = fopen(arquivo, "r");
    if (!arquivo_entrada) {
        printf("Erro abrindo %s!\n", arquivo);
        return 1;
    }



    return 0;

}