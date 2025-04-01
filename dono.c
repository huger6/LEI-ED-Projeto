#include "dono.h"
#include "bdados.h"
#include "listaGenerica.h"

/**
 * @brief Introduz o dono na base de dados
 * 
 * @param bd Base de Dados
 * @param nome Nome do dono
 * @param nif NIF do dono
 * @param codigoPostal Cópigo postal do dono
 * @return int 1 se sucesso, 0 se erro
 */
int inserirDonoLido(Bdados *bd, char *nome, int nif, CodPostal codigoPostal) {
    if (!bd) return 0;

    Dono *dono = (Dono *)malloc(sizeof(Dono));
    if (!dono) return 0;
    //nif
    dono->nif = nif;
    //Nome
    dono->nome = (char *)malloc(strlen(nome) * sizeof(char) + 1);
    if (!dono->nome) {
        free(dono);
        return 0;
    }
    strcpy(dono->nome, nome);
    //Codigo Postal
    dono->codigoPostal.local = codigoPostal.local;
    dono->codigoPostal.zona = codigoPostal.zona;
    
    if (!addInicioLista(bd->donos, (void *)dono)) return 0;

    return 1;
}

/**
 * @brief Comparara donos pelo NIF
 * 
 * @param dono1 
 * @param dono2 
 * @return int -1 se dono1 < dono2, 0 se iguais, 1 se dono1 > dono2
 */
int compararDonos(void *dono1, void *dono2) {
    if (dono1 == NULL && dono2 == NULL) return 0;
    if (dono1 == NULL) return -1; //NULL < qualquer coisa
    if (dono2 == NULL) return 1;

    Dono *x = (Dono *)dono1;
    Dono *y = (Dono *)dono2;

    if (x->nif > y->nif) return 1;
    if (y->nif < x->nif) return -1;
    return 0;
}

void freeDono(void *dono) {
    Dono *obj = (Dono *)dono;
    if (obj->nome) free(obj->nome);
    free(dono);
}
