#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma region DEFINES

//  Tamanho de memoria (8KiB)  / SP em 0x2000
#define TAMANHO_MEMORIA 0x2000
#define INICIO_STACK 0x2000 - 1

// Constantes
bool computador_halt = false;

// Intruções
#define JMP             (0x0) // Salto incondicional: PC recebe o endereço alvo
#define JMPC            (0x1)
#define JEQ             (0x0) // Salto se for igual: salta quando a flag zero está ativa
#define JNE             (0x1) // Salto se for diferente: salta quando a flag zero não está ativa
#define JLT             (0x2) // Salto se for menor que: salta quando a flag zero não está ativa e a carry está
#define JGE             (0x3) // Salto se for maior ou igual: salta quando a flag carry não está ativa e a zero está
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

// Ajudantes

#define ZERO 1
#define CARRY 0

#pragma endregion

#pragma region TYPES

// Definição de registradores e memória
typedef struct cpu {

    uint16_t R[14];
    uint16_t SP; // Ponteiro de pilha
    uint16_t PC; // Contador de programa
    uint16_t IR; // Registrador de instrução
    uint16_t FLAGS; // Registrador de estado

    // MEMÓRIA
    uint16_t MEMORIA[TAMANHO_MEMORIA];

}CPU;

// MBR serve para salvar o valor que vem da intrução, para fácil acesso
typedef union mbr {
    int16_t data;
    struct {
        int16_t rd : 4;
        int16_t rm : 4;
        int16_t rn : 4;
    };

    struct {
        int16_t cond : 2;
        int16_t im_cond : 10;
    };

    struct {
        int16_t rd_mov : 4;
        int16_t im_mov : 8;
    };
} MBR;

#pragma endregion

#pragma region METHODS


void setZ(CPU* computador) {
    computador->FLAGS |= (1 << ZERO);
}

void resetZ(CPU* computador) {
    computador->FLAGS &= (1 << ZERO);
}

void setC(CPU* computador) {
    computador->FLAGS |= (1 << CARRY);
}

void resetC(CPU* computador) {
    computador->FLAGS &= ~(1 << CARRY);
}

// Executa e lê da memória
uint16_t memoria_leitura(CPU* computador) {
    if (computador->MEMORIA[computador->PC] == 0xFFFF) {
        computador_halt = true;
        return 0;
    }
    computador->IR = computador->MEMORIA[computador->PC] & 0x000F;
    return computador->MEMORIA[computador->PC] >> 4;
}

#pragma endregion
#pragma region MAIN

int main(int argc, char* argv[]) {
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
    MBR REG = { 0 };
    computador.SP = INICIO_STACK;

    // Zera a memoria 
    memset(&computador.MEMORIA, 0x0000, TAMANHO_MEMORIA * sizeof(uint16_t));

    // Enche a memoria 
    uint16_t endereco, buffer;
    while (fscanf(arquivo_entrada, "%hX %hX%*[^\n]", &endereco, &buffer) == 2) {
        computador.MEMORIA[endereco] = buffer;
        // printf("%04hX\n", computador.MEMORIA[endereco]);
    }


    // Processador rodando
    do {

        // Ciclo de leitura e Ciclo de decodificação
        REG.data = memoria_leitura(&computador);
        if (computador_halt) {
            goto end;
        }

#pragma region CYCLE
        // Executa ciclo
        switch (computador.IR) {
        case JMP:
            computador.PC += REG.data;
            break;
        case JMPC:
            bool pass = false;
            switch (REG.cond) {
            case JEQ:
                if (computador.FLAGS & (1 << ZERO)) {
                    pass = true;
                }
                break;
            case JNE:
                if (~(computador.FLAGS & (1 << ZERO))) {
                    pass = true;
                }
                break;
            case JLT:
                if (~(computador.FLAGS & (1 << ZERO)) && (computador.FLAGS & (1 << CARRY))) {
                    pass = true;
                }
                break;
            case JGE:
                if ((computador.FLAGS & (1 << ZERO)) && ~(computador.FLAGS & (1 << CARRY))) {
                    pass = true;
                }
                break;
            }
            if (pass) {
                computador.PC += REG.im_cond;
            }
            break;
        case LDR:
            bool io = false;
            char temp;
            if (computador.R[REG.rm] + REG.rn == 0xF000 || computador.R[REG.rm] + REG.rn == 0xF002) {
                scanf("IN => %c\n", &temp);
                io = true;
            }
            // else if (computador.R[REG.rm] + REG.rn == 0xF002) {
            //     scanf("ENTRADA INT: %c\n", &temp);
            //     io = true;
            // }
            if (io) {
                computador.R[REG.rd] = temp;
                break;
            }

            computador.R[REG.rd] = computador.MEMORIA[computador.R[REG.rm] + REG.rn];
            break;
        case STR:
            if (computador.R[REG.rm] + REG.rd == 0xF001) {
                printf("OUT <= %c\n", computador.MEMORIA[computador.R[REG.rm] + REG.rd]);

            }
            else if (computador.R[REG.rm] + REG.rd == 0xF003) {
                printf("OUT <= %d\n", computador.MEMORIA[computador.R[REG.rm] + REG.rd]);
            }

            computador.MEMORIA[computador.R[REG.rm] + REG.rd] = computador.R[REG.rn];
            break;
        case MOV:
            computador.R[REG.rd_mov] = REG.im_mov;
            break;
        case ADD:
            computador.R[REG.rd] = computador.R[REG.rm] + computador.R[REG.rn];
            if (computador.R[REG.rd] == 0) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }
            if ((computador.R[REG.rm] & (1 >> 15)) && (computador.R[REG.rn] & (1 >> 15))) {

            }

            break;
        case ADDI:
            computador.R[REG.rd] = computador.R[REG.rm] + REG.rn;
            if (computador.R[REG.rd] == 0) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }
            break;
        case SUB:
            computador.R[REG.rd] = computador.R[REG.rm] - computador.R[REG.rn];
            if (computador.R[REG.rd] == 0) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }
            break;
        case SUBI:
            computador.R[REG.rd] = computador.R[REG.rm] - REG.rn;
            if (computador.R[REG.rd] == 0) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }
            break;
        case AND:
            computador.R[REG.rd] = computador.R[REG.rm] & computador.R[REG.rn];
            if (computador.R[REG.rd] == 0) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }
            break;
        case OR:
            computador.R[REG.rd] = computador.R[REG.rm] | computador.R[REG.rn];
            if (computador.R[REG.rd] == 0) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }
            break;
        case SHR:
            computador.R[REG.rd] = computador.R[REG.rm] >> REG.rn;
            if (computador.R[REG.rd] == 0) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }
            break;
        case SHL:
            computador.R[REG.rd] = computador.R[REG.rm] << computador.R[REG.rn];
            if (computador.R[REG.rd] == 0) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }
            break;
        case CMP:
            if (computador.R[REG.rm] == computador.R[REG.rn]) {
                setZ(&computador);
            }
            else {
                resetZ(&computador);
            }

            if (computador.R[REG.rm] < computador.R[REG.rn]) {
                setC(&computador);
            }
            else {
                resetC(&computador);
            }

            break;
        case PUSH:
            computador.MEMORIA[computador.SP] = computador.R[REG.rn];
            computador.SP--;
            break;
        case POP:
            computador.R[REG.rd] = computador.MEMORIA[computador.SP];
            computador.SP++;
            break;
        }
#pragma endregion
        // Ciclo do Breakpoint 
        // if (breakpoints[computador.PC]) {
        if (1) {
        end:
            printf("<== Registradores ==>\n");
            // printf("PC = 0x%04hX\n", computador.PC);
            printf("IR = 0x%04hX\n", computador.IR);
            for (int i = 0; i < 14; i++) {
                printf("R%0d = 0x%04hX\n", i, computador.R[i]);
            }
            printf("R14 = 0x%04hX\n", computador.SP);
            printf("R15 = 0x%04hX\n", computador.PC);
            printf("Z = %d\n", !!(computador.FLAGS & (1 << ZERO)));
            printf("C = %d\n", !!(computador.FLAGS & (1 << CARRY)));
        }

        computador.PC++;

    } while (!computador_halt);


    return 0;
}
#pragma endregion