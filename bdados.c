#include "bdados.h"
#include "carro.h"
#include "distancias.h"
#include "dono.h"
#include "passagens.h"
#include "sensores.h"

int inicializarBD(Bdados *bd) {
    bd->carros = criarDict();
    bd->donosNif = criarDict();
    bd->donosAlfabeticamente = criarDict();
    bd->viagens = criarLista();
    bd->sensores = criarLista();
    inicializarMatrizDistancias(bd);
    if (!bd->carros || !bd->distancias || !bd->distancias->matriz || !bd->donosNif ||
         !bd->donosAlfabeticamente	|| !bd->viagens || !bd->sensores) return 0;
    return 1;
}

//TODO CHAVES
void freeTudo(Bdados *bd) {
    freeDict(bd->carros, freeCarro);
    freeDict(bd->donosNif, freeDono);
    freeDict(bd->donosAlfabeticamente, freeDono);
    freeMatrizDistancias(bd->matrizDistancias);
    freeLista(bd->viagens, freeViagem);
    freeLista(bd->sensores, freeSensor);
    free(bd);
}