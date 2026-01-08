#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//  Tamanho de memoria (16KB) 
#define TAMANHO_MEMORIA 16<<10

// Intruções
#define JMP_I           (0x0)
#define JEQ_I           (0x1)
#define JNE_I           (0x1)
#define JLT_I           (0x1)
#define JGE_I           (0x1)
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

    int16_t r0;
    int16_t r1;
    int16_t r2;
    int16_t r3;
    int16_t r4;
    int16_t r5;
    int16_t r6;
    int16_t r7;
    int16_t r8;
    int16_t r9;
    int16_t r10;
    int16_t r11;
    int16_t FLAGS;
    int16_t IR;
    int16_t SP;
    int16_t PC;

    // MEMÓRIA
    uint16_t MEMORIA[TAMANHO_MEMORIA];

}CPU;

int main(){



    return 0;
}