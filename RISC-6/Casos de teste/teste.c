#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int numero_linhas(FILE *arq);
bool isnt_empty_line(char *line);

typedef struct {
    char opcode[8];
    char op1[8];
    char op2[8];
    char op3[8];
} Instrucao;

typedef struct {
    char name[32];
    int addr;
} Label;

Label labels[64];
int contador_labbel = 0;

bool isnt_empty_line(char *line) {
    const char *l = line;
    while (*l && isspace(*l)) l++;

    if (*l == '\0' || *l == '\n') {
        return false;
    }

    if (*l == ';') {
        return false;
    }

    const char *final = l + strlen(l) - 1;
    while (final > l && isspace(*final)) final--;

    if (*final == ':') return false;

    if (strchr(final, ':') && strchr(final, '\'')) {
        return false;
    }

    return true;
}

int numero_linhas(FILE *arq) {
    int counter = 0;
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), arq)) {
        if (isnt_empty_line(buffer)) {
            counter++;
        }
    }
    return counter;
}

void add_label(char *nome, int addr) {
    strcpy(labels[contador_labbel].name, nome);
    labels[contador_labbel].addr = addr;
    contador_labbel++;
}

int find_label(char *nome) {
    for (int i = 0; i < contador_labbel; i++) {
        if (strcmp(labels[i].name, nome) == 0) {
            return labels[i].addr;
        }
    }
    return -1;
}

unsigned short char_literal(char *s) {
    if (s[0] == '\'' && s[2] == '\'') {
        return (unsigned short)s[1];
    }
    return 0;
}

unsigned short hexadump(Instrucao *inst) {
    if (strcmp(inst->opcode, "MOV") == 0) {
        int registrador = inst->op1[1] - '0';
        int val;
        char *op = inst->op2;

        if (*op == '#') op++;
        if (isdigit(*op)) {
            val = atoi(op);
        } else {
            val = find_label(inst->op2);
        }

        return (0x00 << 8) | (registrador << 4) | (val & 0x0F);
    }

    if (strcmp(inst->opcode, "ADD") == 0) {
        int r1 = inst->op1[1] - '0';
        int r2 = inst->op2[1] - '0';
        int r3 = inst->op3[1] - '0';
        return (0x40 << 8) | (r1 << 4) | (r2 << 2) | r3;
    }

    if (strcmp(inst->opcode, "ADDI") == 0) {
        int r1 = inst->op1[1] - '0';
        int val;
        char *op = inst->op3;

        if (*op == '#') op++;
        if (isdigit(*op)) val = atoi(op);
        else val = find_label(inst->op3);

        return (0x10 << 8) | (r1 << 4) | (val & 0x0F);
    }

    if (strcmp(inst->opcode, "PUSH") == 0) {
        int r = inst->op1[1] - '0';
        return (0x00 << 8) | (r & 0x0F);
    }

    if (strcmp(inst->opcode, "POP") == 0) {
        int r = inst->op1[1] - '0';
        return (0x50 << 8) | (r & 0x0F);
    }

    if (strcmp(inst->opcode, "HALT") == 0) return 0xFFFF;

    if (strcmp(inst->opcode, "JMP") == 0) {
        int addr = find_label(inst->op1);
        return (0x60 << 8) | (addr & 0xFF);
    }

    if (strcmp(inst->opcode, "JEQ") == 0) {
        int addr = find_label(inst->op1);
        return (0x70 << 8) | (addr & 0xFF);
    }

    if (strcmp(inst->opcode, "STR") == 0) return 0x3000;
    if (strcmp(inst->opcode, "LDR") == 0) return 0x1000;
    if (strcmp(inst->opcode, "OR") == 0) return 0x2000;
    if (strcmp(inst->opcode, "SHL") == 0) return 0xF000;

    return 0;
}

char* rlstrip(char *conteudo) {
    while (isspace(*conteudo)) conteudo++;
    char *final = conteudo + strlen(conteudo) - 1;
    while (final > conteudo && isspace(*final)) *(final--) = 0;
    return conteudo;
}

int main() {
    FILE *file = fopen("example5.asm", "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    int total_linhas = numero_linhas(file);
    rewind(file); // volta ao início do arquivo para ler novamente

    Instrucao assembly[total_linhas];
    int contador_inst = 0;

    unsigned short memoria[total_linhas];
    int contador_memoria = 0;

    char linha[256];
    int addr = 0;

    while (fgets(linha, sizeof(linha), file)) {
        char *init = rlstrip(linha);
        if (strlen(init) == 0) continue;

        char *label = strchr(init, ':');
        if (label) {
            *label = '\0';
            add_label(init, addr);
            init = label + 1;
            init = rlstrip(init);
            if (*init == '\0') continue;
        }

        if (init[0] == '\'') {
            add_label(init, addr);
            memoria[contador_memoria++] = char_literal(init);
            addr++;
            continue;
        }

        Instrucao inst = {0};
        char *palavra = strtok(init, " \t,");
        if (!palavra) continue;
        strcpy(inst.opcode, palavra);

        palavra = strtok(NULL, " \t,");
        if (palavra) strcpy(inst.op1, palavra);

        palavra = strtok(NULL, " \t,");
        if (palavra) strcpy(inst.op2, palavra);

        palavra = strtok(NULL, " \t,");
        if (palavra) strcpy(inst.op3, palavra);

        assembly[contador_inst++] = inst;
        addr++;
    }

    fclose(file);

    for (int i = 0; i < contador_inst; i++) {
        memoria[contador_memoria++] = hexadump(&assembly[i]);
    }

    FILE *saida = fopen("saida.hex", "w");
    for (int i = 0; i < contador_memoria; i++) {
        fprintf(saida, "%04X %04X\n", i, memoria[i]);
    }
    fclose(saida);

    return 0;
}
