// Arquivo para a ULA

#include "registrador.c"

void ADD(registrador c, registrador a, registrador b){
    c.valor = a.valor + b.valor;
}

void SUB(registrador c, registrador a, registrador b){
    c.valor = a.valor - b.valor;
}