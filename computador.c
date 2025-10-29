// TRABALHO LAB 3 - ARQUITETURA DE COMPUTADORES

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//  Tamanho de memoria (4KB) 
#define TAMANHO_MEMORIA 4<<10

//  Instruções
#define HALT			(0x0) 	// HALT 			| Para o processador
#define LOAD_M			(0x1)	// LOAD M(X) 		| AC <= MEMORY[X] 
#define LOAD_MQ		    (0x2)	// LOAD MQ 			| AC <= MQ 
#define LOAD_MQ_M		(0x3)	// LOAD MQ, M(X)	| MQ <= MEMORY[X] 
#define STOR			(0x4)	// STOR M(X) 		| MEMORY[X] <= AC 
#define STA			    (0x5) 	// STA M(X) 		| MEMORY[X](11:0) = AC 
#define ADD			    (0x6)	// ADD M(X) 		| AC <= AC + MEMORY[X] 
#define SUB			    (0x7)	// SUB M(X) 		| AC <= AC - MEMORY[X] 
#define MUL			    (0x8)	// MUL M(X) 		| AC(31:16):MQ(15:0) <= MQ * MEMORY[X] 
#define DIV			    (0x9)	// DIV M(X) 		| MQ <= AC / MEMORY[X], AC <= AC % MEMORY[X] 
#define JMP			    (0xA)	// JMP M(X) 		| PC = X 
#define JZ			    (0xB) 	// JZ M(X) 			| PC = (AC == 0) ? X : PC 
#define JNZ			    (0xC) 	// JNZ M(X) 		| PC = (AC != 0) ? X : PC 
#define JPOS			(0xD) 	// JPOS M(X) 		| PC = (AC >= 0) ? X : PC 
#define IN				(0xE) 	// IN 				| AC <= IO 
#define OUT			    (0xF) 	// OUT 				| IO <= AC 

// Definição de registradores e memória 
typedef struct computador_t {
    // UC 
    uint16_t pc; // Contador de programa
    uint16_t mar; // Endereço de memória
    uint16_t ibr; // Endereço de instrução
    uint16_t ir; // Instrução
    
    // ULA
    int16_t mbr; // Conteúdo da memória
    int16_t ac;	// Acumulador
    int16_t mq;	// Quociente

    // MEMÓRIA
    uint16_t memoria[TAMANHO_MEMORIA];
} computador_t;

// Executa e lê da memória
void memoria_read(computador_t* computador) {
    // TODO
}

// Executa e escreve na memória
void memoria_write(computador_t* computador, bool modificar_endereco) {
    if (modificar_endereco) {
        // TODO: Write only operand endereco field
    }
    else {
        // TODO
    }
}

// Lê inteiro do usuario 
void io_read(computador_t* computador) {
    printf("ENTRADA => ");
    scanf("%hd", &computador->ac);
}

// Imprime inteiro do usuario 
void io_write(computador_t* computador) {
    printf("SAIDA => 0x%04hX\n", computador->ac);
}

int main(int argc, char* argv[]) {
    // Checa argumentos
    if (argc < 2) {
        printf("O programa precisa de 2 argumentos.\n");
        return 1;
    }

    // Abre o arquivo
    FILE* arquivo_entrada = fopen(argv[1], "r");
    if (!arquivo_entrada) {
        printf("Erro abrindo %s!\n", argv[1]);
        return 1;
    }

    // Define breakpoints 
    bool breakpoints[TAMANHO_MEMORIA] = { false };
    for (int i = 2; i < argc; i++) {
        int endereco = strtol(argv[i], NULL, 16);
        breakpoints[endereco] = true;
    }

    // Inicia registradores com zero 
    computador_t computador = { 0 };

    // Zera a memoria 
    memset(&computador.memoria, 0x0000, TAMANHO_MEMORIA);

    // Enche a memoria 
    uint16_t endereco, buffer;
    while (fscanf(arquivo_entrada, "%hX %hX%*[^\n]", &endereco, &buffer) == 2) {
        computador.memoria[endereco] = buffer;
    }

    // Processador rodando
    bool computador_halt = false;
    do {
        // PC antes de modificações
        uint16_t original_pc = computador.pc;

        // Ciclo de leitura
        // TODO: Fetch instruction from memoria (like in IAS)

        // Ciclo de decodificação
        // TODO: Put instruction fields in registers

        // Executa ciclo
        switch (computador.ir) {
        case HALT:
            // TODO
            break;
        case LOAD_M:
            // TODO
            break;
        case LOAD_MQ:
            // TODO
            break;
        case LOAD_MQ_M:
            // TODO
            break;
        case STOR:
            // TODO
            break;
        case STA:
            // TODO
            break;
        case ADD:
            // TODO
            break;
        case SUB:
            // TODO
            break;
        case MUL:
            // TODO
            break;
        case DIV:
            // TODO
            break;
        case JMP:
            // TODO
            break;
        case JZ:
            // TODO
            break;
        case JNZ:
            // TODO
            break;
        case JPOS:
            // TODO
            break;
        case IN:
            // TODO
            break;
        case OUT:
            // TODO
            io_write(&computador);
            break;
        default:
            printf("Instrucao invalida %04X!\n", computador.ibr);
            computador_halt = true;
            break;
        }

        // Ciclo do Breakpoint 
        if (breakpoints[original_pc]) {
            printf("<== Registradores ==>\n");
            printf("PC = 0x%04hX\n", original_pc);
            printf("PC+ = 0x%04hX\n", computador.pc);
            printf("MAR = 0x%04hX\n", computador.mar);
            printf("IBR = 0x%04hX\n", computador.ibr);
            printf("IR = 0x%04hX\n", computador.ir);
            printf("MBR = 0x%04hX\n", computador.mbr);
            printf("AC = 0x%04hX\n", computador.ac);
            printf("MQ = 0x%04hX\n", computador.mq);
        }

    } while (!computador_halt);

    return 0;
}