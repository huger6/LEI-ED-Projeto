#include "bdados.h"
#include "carro.h"
#include "distancias.h"
#include "dono.h"
#include "passagens.h"
#include "sensores.h"

int inicializarBD(Bdados *bd) {
    bd->carros = criarLista();
    bd->distancias = (float *)malloc(float); //Aloca 1 casa para a matriz
    bd->nColunasMatrizDistancias = 1;
    bd->donos = criarLista();
    bd->viagens = criarLista();
    bd->sensores = criarLista();
    if (!bd->carros || !bd->distancias || !bd->donos || !bd->passagens || !bd->sensores) return 0;
    return 1;
}

int realocarMatrizDistancias(Bdados *bd, int tamanho) {
    float *matriz = (float *)realloc(bd->distancias, tamanho*tamanho);
    if (!matriz) return 0;


    bd->distancias = matriz;
    bd->nColunasMatrizDistancias = tamanho;
    return 1;
}

void freeTudo(Bdados *bd) {
    freeLista(bd->carros, freeCarro);
    freeLista(bd->donos, freeDono);
    freeLista(bd->viagens, freeViagem);
    freeLista(bd->sensores, freeSensor);
    free(bd);
}