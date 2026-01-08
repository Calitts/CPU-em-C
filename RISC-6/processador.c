#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//  Tamanho de memoria (16KB) 
#define TAMANHO_MEMORIA 16<<10

// Intruções
#define JMP             (0x0) // Salto incondicional: PC recebe o endereço alvo
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

// Definição de registradores e memória
typedef struct cpu{

    int16_t R0;
    int16_t R1;
    int16_t R2;
    int16_t R3;
    int16_t R4;
    int16_t R5;
    int16_t R6;
    int16_t R7;
    int16_t R8;
    int16_t R9;
    int16_t R10;
    int16_t R11;
    int16_t FLAGS; // Registrador de estado
    int16_t IR; // Registrador de instrução
    int16_t SP; // Ponteiro de pilha
    int16_t PC; // Contador de programa

    // MEMÓRIA
    uint16_t MEMORIA[TAMANHO_MEMORIA];

}CPU;

// Executa e lê da memória
void memoria_leitura(CPU* computador) {
    
}

// Executa e escreve na memória
void memoria_escrita(CPU* computador, bool modificar_endereco) {
   
}

// Lê inteiro do usuario 
void io_leitura(CPU* computador) {
    
}

// Imprime inteiro do usuario 
void io_escrita(CPU* computador) {
    
}

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

    // Define breakpoints 
    bool breakpoints[TAMANHO_MEMORIA] = { false };
    for (int i = 2; i < argc; i++) {
        int endereco = strtol(argv[i], NULL, 16);
        breakpoints[endereco] = true;
    }

    // Inicia registradores com zero 
    CPU computador = { 0 };

    // Zera a memoria 
    memset(&computador.MEMORIA, 0x0000, TAMANHO_MEMORIA);

    // Enche a memoria 
    uint16_t endereco, buffer;
    while (fscanf(arquivo_entrada, "%hX %hX%*[^\n]", &endereco, &buffer) == 2) {
        computador.MEMORIA[endereco] = buffer;
    }

    // Processador rodando
    bool computador_halt = false;
    do {
        // PC antes de modificações
        uint16_t original_pc = computador.PC;

        // Ciclo de leitura
       

        // Ciclo de decodificação
       

        // Executa ciclo
        switch (computador.IR) {
            
        }

        // Ciclo do Breakpoint 
        if (breakpoints[original_pc]) {
        // if (1) {
            printf("<== Registradores ==>\n");
            printf("PC = 0x%04hX\n", original_pc);
            printf("PC+ = 0x%04hX\n", computador.PC);
            printf("IR = 0x%04hX\n", computador.IR);
        }

    } while (!computador_halt);

    return 0;
}