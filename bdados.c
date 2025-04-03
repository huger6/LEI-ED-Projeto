#include "bdados.h"
#include "carro.h"
#include "distancias.h"
#include "dono.h"
#include "passagens.h"
#include "sensores.h"

int inicializarListasBD(Bdados *bd) {
    bd->carros = criarLista();
    bd->distancias = criarLista();
    bd->donos = criarLista();
    bd->passagens = criarLista();
    bd->sensores = criarLista();
    if (!bd->carros || !bd->distancias || !bd->donos || !bd->passagens || !bd->sensores) return 0;
    return 1;
}

void freeTudo(Bdados *bd) {
    freeLista(bd->carros, freeCarro);
    freeLista(bd->distancias, freeDistancia);
    freeLista(bd->donos, freeDono);
    freeLista(bd->passagens, freePassagem);
    freeLista(bd->sensores, freeSensor);
    free(bd);
}