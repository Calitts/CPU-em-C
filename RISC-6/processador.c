#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//  Tamanho de memoria (16KB) 
#define TAMANHO_MEMORIA 16<<10

// Intruções
#define JMP             (0x0)
#define JEQ             (0x1)
#define JNE             (0x1)
#define JLT             (0x1)
#define JGE             (0x1)
#define LDR             (0x2)
#define STR             (0x3)
#define MOV             (0x4)
#define ADD             (0x5)
#define ADDI            (0x6)
#define SUB             (0x7)
#define SUBI            (0x8)
#define AND             (0x9)
#define OR              (0xA)
#define SHR             (0xB)
#define SHL             (0xC)
#define CMP             (0xD)
#define PUSH            (0xE)
#define POP             (0xF)
#define HALT            (0xF)

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

int main(){



    return 0;
}