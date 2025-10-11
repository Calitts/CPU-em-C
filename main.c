#include <stdio.h>
#include <string.h>

// CONSTANTES

#define NUMERO_REGISTRADORES_ESPECIAIS 6
#define NUMERO_REGISTRADORES_GERAIS 5
#define TAMANHO_MEMORIA 1<<20 // MEMORIA 1 MB

// REGISTRADORES
typedef struct {
    char tipo[15];
    int valor;
} registrador;


// ULA

void ADD(registrador c, registrador a, registrador b) {
    c.valor = a.valor + b.valor;
}

void SUB(registrador c, registrador a, registrador b) {
    c.valor = a.valor - b.valor;
}

void MUL(registrador c, registrador a, registrador b) {
    c.valor = a.valor * b.valor;
}

void DIV(registrador c, registrador a, registrador b) {
    c.valor = a.valor / b.valor;
}


// ENTRADA E SAÍDA + DRIVER

int main() {
    // DEFINIÇÃO REGISTRADORES ESPECIAIS E DE USO GERAL
    char registradores_speciais[NUMERO_REGISTRADORES_ESPECIAIS][20] = { "pc", "mbr", "mar", "ir" , "mq", "ac"};
    registrador especiais[NUMERO_REGISTRADORES_ESPECIAIS];
    registrador gerais[NUMERO_REGISTRADORES_GERAIS]; 

    for (int i = 0; i < NUMERO_REGISTRADORES_ESPECIAIS; i++) {
        strcpy(especiais[i].tipo, registradores_speciais[i]);
    }
    printf(especiais[0].tipo);

    // DEFINIÇÃO DE MEMORIA
    int memoria[TAMANHO_MEMORIA];

    // ENTRADA

}