#include <stdio.h>
#include <string.h>

// CONSTANTES

#define NUMERO_REGISTRADORES_ESPECIAIS 6
#define NUMERO_REGISTRADORES_GERAIS 5
#define TAMANHO_MEMORIA 1<<10 // MEMORIA 1 KB
#define TAMANHO_ENTRADA_MAX 40
#define TAMANHO_PALAVRA_MAX 20

// REGISTRADORES
typedef struct {
    char tipo[TAMANHO_PALAVRA_MAX];
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
    char registradores_speciais[NUMERO_REGISTRADORES_ESPECIAIS][TAMANHO_PALAVRA_MAX] = { "pc", "mbr", "mar", "ir" , "mq", "ac" };
    registrador especiais[NUMERO_REGISTRADORES_ESPECIAIS];
    registrador gerais[NUMERO_REGISTRADORES_GERAIS];

    for (int i = 0; i < NUMERO_REGISTRADORES_ESPECIAIS; i++) {
        strcpy(especiais[i].tipo, registradores_speciais[i]);
    }

    // DEFINIÇÃO DA MEMORIA
    int memoria[TAMANHO_MEMORIA];
    for (int i = 0; i < TAMANHO_MEMORIA; i++) {
        memoria[i] = 0;
    }

    // ENTRADA

    // Estou pensando em adicionar cada comando como uma palavra principal
    // As palavras em seguida como os argumentos, que nem um terminal

    // ^ Comandos que pensei em adicionar: (Com asterisco já foram adicionadas)
    // show - Mostra o conteudo dos registradores. (-m para mostrar conteudo da memória e -a para tudo)
    // run - Roda um arquivo escolhido como assembly. (O ideal é usar show automaticamente para mostrar o programa rodando)
    // set - Define o tempo para cada passo do comando run em milisegundos.
    // step - Pula para o próximo passo e executa ele.
    // halt - Pausa o programa.
    //* echo - Imprime texto.
    //* exit - Termina o programa.


    char entrada[TAMANHO_ENTRADA_MAX];
    int posicao_entrada;
    while (1) {
        // Inicializa o terminal do programa com ">>>" e espera pela entrada
        printf(">>> ");
        fgets(entrada, TAMANHO_ENTRADA_MAX, stdin);
        posicao_entrada = 0;
        while (posicao_entrada < TAMANHO_ENTRADA_MAX) {
            if (entrada[posicao_entrada] != ' ') {
                break;
            }
            posicao_entrada += 1;
        }


        // Adiquire o comando principal da entrada e separa ela na string "comando"
        char comando[TAMANHO_PALAVRA_MAX];
        for (int k = 0; k < TAMANHO_ENTRADA_MAX; k++) {
            if (entrada[posicao_entrada] == ' ' || entrada[posicao_entrada] == '\n') {
                comando[k] = '\0';
                posicao_entrada++;
                break;
            }
            comando[k] = entrada[posicao_entrada];
            posicao_entrada++;
        }


        // Separa os argumentos da entrada no vetor de strings "argumentos"
        char argumentos[10][TAMANHO_PALAVRA_MAX];
        int quantidade_argumentos = 0; // Tamanho do vetor
        while (quantidade_argumentos < TAMANHO_ENTRADA_MAX) {
            if (entrada[posicao_entrada] == '\0') {
                break;
            }
            // Segundo loop for transcreve cada palavra da entrada em uma posição no vetor
            for (int k = 0; k < TAMANHO_PALAVRA_MAX; k++) {
                if (entrada[posicao_entrada] == ' ' || entrada[posicao_entrada] == '\n') {
                    argumentos[quantidade_argumentos][k] = '\0';
                    posicao_entrada++;
                    break;
                }
                argumentos[quantidade_argumentos][k] = entrada[posicao_entrada];
                posicao_entrada++;
            }
            quantidade_argumentos++;
        }


        // Comandos (Implementação Básica, tem como melhorar)
        if (!strcmp(comando, "echo")) {
            for (int k = 0; k < quantidade_argumentos; k++)
            {
                printf("%s ", argumentos[k]);
            }
            printf("\n");

        } else if (!strcmp(comando, "exit")) {
            break;
        }
        else if (comando[0] != '\0') {
            printf("Erro: Comando '%s' nao encontrado\n", comando);
        }

    }

    return 0;
}
