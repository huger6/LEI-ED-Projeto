#include "bdados.h"
#include "carro.h"
#include "distancias.h"
#include "dono.h"
#include "passagens.h"
#include "sensores.h"

/**
 * @brief Inicializa a base de dados criando as estruturas necessárias
 * 
 * @param bd Base de dados
 * @return int 1 se uscesso, 0 se erro
 */
int inicializarBD(Bdados *bd) {
    bd->carrosMarca = criarDict();
    bd->donosNif = criarDict();
    bd->donosAlfabeticamente = criarDict();
    bd->viagens = criarLista();
    bd->sensores = criarLista();
    inicializarMatrizDistancias(bd);
    if (!bd->carrosMarca || !bd->distancias || !bd->distancias->matriz || !bd->donosNif ||
         !bd->donosAlfabeticamente	|| !bd->viagens || !bd->sensores) return 0;
    return 1;
}

/**
 * @brief Liberta toda a memória utilizada pelo programa
 * 
 * @param bd Base de dados
 */
void freeTudo(Bdados *bd) {
    freeDict(bd->carrosMarca, freeChaveCarroMarca, freeCarro);
    freeDict(bd->donosNif, freeChaveDonoNif, freeDono);
    freeDict(bd->donosAlfabeticamente, freeChaveDonoAlfabeticamente, NULL);
    freeMatrizDistancias(bd->distancias);
    freeLista(bd->viagens, freeViagem);
    freeLista(bd->sensores, freeSensor);
    free(bd);
}