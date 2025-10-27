#include <stdio.h>
#include <string.h>

// CONSTANTES

#define NUMERO_REGISTRADORES_ESPECIAIS 6
#define NUMERO_REGISTRADORES_GERAIS 5
#define TAMANHO_MEMORIA 1<<10 // MEMORIA 1 KB

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
    char registradores_speciais[NUMERO_REGISTRADORES_ESPECIAIS][20] = { "pc", "mbr", "mar", "ir" , "mq", "ac" };
    registrador especiais[NUMERO_REGISTRADORES_ESPECIAIS];
    registrador gerais[NUMERO_REGISTRADORES_GERAIS];

    for (int i = 0; i < NUMERO_REGISTRADORES_ESPECIAIS; i++) {
        strcpy(especiais[i].tipo, registradores_speciais[i]);
    }

    // DEFINIÇÃO DE MEMORIA
    int memoria[TAMANHO_MEMORIA];

    // ENTRADA


    // Estou pensando em adicionar cada comando como uma palavra 
    // e os elementos em seguida como os argumentos, que nem um terminal

    // ^ Comandos que pensei em adicionar:
    // show - Mostra o conteudo dos registradores. (-m para mostrar conteudo da memória e -a para tudo)
    // run - Roda um arquivo escolhido como assembly. (O ideal é usar show automaticamente para mostrar o programa rodando)
    // set - Define o tempo para cada passo do comando run em milisegundos.
    // step - Pula para o próximo passo e executa ele.
    // halt - Pausa o programa.
    // exit - Termina o programa.


    char entrada[40];
    while (1) {
        printf(">>> ");
        fgets(entrada, 40, stdin);

    }

    return 0;
}
