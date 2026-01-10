#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct {
    char opcode[8];
    char op1[16];
    char op2[16];
    char op3[16];
} Instrucao;

typedef struct {
    char name[32];
    int addr;
} Label;

Label labels[128];
int label_count = 0;

// ===== Funções utilitárias =====
bool isnt_empty_line(char *line) {
    while (*line && isspace(*line)) line++;
    return (*line && *line != ';');
}

char *trim(char *s) {
    while (isspace(*s)) s++;
    char *e = s + strlen(s) - 1;
    while (e > s && isspace(*e)) *e-- = 0;
    return s;
}

void add_label(const char *name, int addr) {
    strcpy(labels[label_count].name, name);
    labels[label_count].addr = addr;
    label_count++;
}

int find_label(const char *name) {
    for (int i = 0; i < label_count; i++)
        if (!strcmp(labels[i].name, name))
            return labels[i].addr;
    return -1;
}

uint16_t parse_imm(char *s) {
    s = trim(s);
    if (*s == '#') s++;
    if (isdigit(*s)) return atoi(s);
    int addr = find_label(s);
    if (addr >= 0) return addr;
    fprintf(stderr, "Erro: valor/imediato inválido '%s'\n", s);
    exit(1);
}

// Retorna número do registrador R0-R15
int reg(char *s) {
    if (s[0] != 'R' && s[0] != 'r') {
        fprintf(stderr, "Erro: registrador inválido '%s'\n", s);
        exit(1);
    }
    int r = atoi(s + 1);
    if (r < 0 || r > 15) {
        fprintf(stderr, "Erro: registrador fora do range (R0-R15) '%s'\n", s);
        exit(1);
    }
    return r;
}

// ===== Codificação da instrução =====
uint16_t encode(Instrucao *i) {
    int rd, rs, rt;
    uint16_t imm;

    if (!strcmp(i->opcode, "MOV")) {
        rd = reg(i->op1);
        imm = parse_imm(i->op2);
        return (0x0 << 12) | (rd << 8) | (imm & 0xFF);
    }
    if (!strcmp(i->opcode, "ADDI")) {
        rd = reg(i->op1);
        imm = parse_imm(i->op3);
        return (0x1 << 12) | (rd << 8) | (imm & 0xFF);
    }
    if (!strcmp(i->opcode, "SUBI")) {
        rd = reg(i->op1);
        imm = parse_imm(i->op3);
        return (0x2 << 12) | (rd << 8) | (imm & 0xFF);
    }
    if (!strcmp(i->opcode, "ADD")) {
        rd = reg(i->op1);
        rs = reg(i->op2);
        rt = reg(i->op3);
        return (0x3 << 12) | (rd << 8) | (rs << 4) | rt;
    }
    if (!strcmp(i->opcode, "SUB")) {
        rd = reg(i->op1);
        rs = reg(i->op2);
        rt = reg(i->op3);
        return (0x4 << 12) | (rd << 8) | (rs << 4) | rt;
    }
    if (!strcmp(i->opcode, "OR")) {
        rd = reg(i->op1);
        rs = reg(i->op2);
        rt = reg(i->op3);
        return (0x5 << 12) | (rd << 8) | (rs << 4) | rt;
    }
    if (!strcmp(i->opcode, "AND")) {
        rd = reg(i->op1);
        rs = reg(i->op2);
        rt = reg(i->op3);
        return (0x6 << 12) | (rd << 8) | (rs << 4) | rt;
    }
    if (!strcmp(i->opcode, "LDR")) {
        rd = reg(i->op1);
        // LDR Rdest, [Rsrc, #offset]
        char *p = i->op2;
        if (*p != '[') {
            fprintf(stderr, "Erro: LDR deve ter formato [R?, #offset]\n");
            exit(1);
        }
        rs = reg(p + 1); // pega R? dentro de [R?...
        char *comma = strchr(p, ',');
        imm = 0;
        if (comma) imm = parse_imm(comma + 1);
        return (0x7 << 12) | (rd << 8) | (rs << 4) | (imm & 0xF);
    }
    if (!strcmp(i->opcode, "STR")) {
        rd = reg(i->op1);
        char *p = i->op2;
        if (*p != '[') {
            fprintf(stderr, "Erro: STR deve ter formato [R?, #offset]\n");
            exit(1);
        }
        rs = reg(p + 1);
        char *comma = strchr(p, ',');
        imm = 0;
        if (comma) imm = parse_imm(comma + 1);
        return (0x8 << 12) | (rd << 8) | (rs << 4) | (imm & 0xF);
    }
    if (!strcmp(i->opcode, "PUSH")) {
        rd = reg(i->op1);
        return (0x9 << 12) | (rd << 8);
    }
    if (!strcmp(i->opcode, "POP")) {
        rd = reg(i->op1);
        return (0xA << 12) | (rd << 8);
    }
    if (!strcmp(i->opcode, "JMP")) {
        imm = find_label(i->op1);
        if (imm < 0) { fprintf(stderr, "Label '%s' não encontrada\n", i->op1); exit(1); }
        return (0xB << 12) | (imm & 0x0FFF);
    }
    if (!strcmp(i->opcode, "JEQ")) {
        imm = find_label(i->op1);
        if (imm < 0) { fprintf(stderr, "Label '%s' não encontrada\n", i->op1); exit(1); }
        return (0xC << 12) | (imm & 0x0FFF);
    }
    if (!strcmp(i->opcode, "SHL")) {
        rd = reg(i->op1);
        imm = parse_imm(i->op3);
        return (0xD << 12) | (rd << 8) | (imm & 0xF);
    }
    if (!strcmp(i->opcode, "SHR")) {
        rd = reg(i->op1);
        imm = parse_imm(i->op3);
        return (0xE << 12) | (rd << 8) | (imm & 0xF);
    }
    if (!strcmp(i->opcode, "HALT")) {
        return 0xFFFF;
    }

    fprintf(stderr, "Instrucao desconhecida: '%s'\n", i->opcode);
    exit(1);
}

// ===== Literal de caractere =====
uint16_t char_literal(char *s) {
    if (s[0] == '\'' && s[2] == '\'') return (unsigned char)s[1];
    if (s[0] == '\'' && s[1] == '\\' && s[2] == '0' && s[3] == '\'') return 0;
    return 0;
}

// ===== Programa principal =====
int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: assembler arquivo.asm\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Erro ao abrir arquivo '%s'\n", argv[1]);
        return 1;
    }

    Instrucao inst[1024];
    int icount = 0, pc = 0;
    char line[256];

    // ===== PRIMEIRO PASSO: LER INSTRUCOES E LABELS =====
    while (fgets(line, sizeof(line), f)) {
        char *s = trim(line);
        if (!isnt_empty_line(s)) continue;

        char *c = strchr(s, ':');
        if (c) {
            *c = 0;
            add_label(s, pc);
            s = trim(c + 1);
            if (!*s) continue;
        }

        if (s[0] == '\'') continue; // literal, processaremos depois

        Instrucao i = {0};
        char *t = strtok(s, " ,\t");
        if (!t) continue;
        strcpy(i.opcode, t);
        if ((t = strtok(NULL, " ,\t"))) strcpy(i.op1, t);
        if ((t = strtok(NULL, " ,\t"))) strcpy(i.op2, t);
        if ((t = strtok(NULL, " ,\t"))) strcpy(i.op3, t);

        inst[icount++] = i;
        pc++;
    }

    // ===== SEGUNDO PASSO: PROCESSAR LITERALS =====
    rewind(f);
    uint16_t mem[2048];
    int m = 0;

    // codificar instruções
    for (int i = 0; i < icount; i++)
        mem[m++] = encode(&inst[i]);

    while (fgets(line, sizeof(line), f)) {
        char *s = trim(line);
        if (!isnt_empty_line(s)) continue;
        char *c = strchr(s, ':');
        if (c) s = trim(c + 1);

        if (s[0] == '\'') {
            add_label(line, m);
            mem[m++] = char_literal(s);
        }
    }

    fclose(f);

    FILE *o = fopen("saida.hex", "w");
    if (!o) {
        printf("Erro ao criar arquivo de saída\n");
        return 1;
    }
    for (int i = 0; i < m; i++)
        fprintf(o, "%04X %04X\n", i, mem[i]);
    fclose(o);

    printf("Assembler OK - saida.hex gerado\n");
    return 0;
}
