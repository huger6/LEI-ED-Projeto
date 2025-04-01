#ifndef DONO_HEADERS
#define DONO_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    short zona; //Primeiros 4 dígitos
    short local; //Últimos 3 dígitos
} CodPostal;

typedef struct {
    int nif; //PRIMARY KEY
    char *nome;
    CodPostal codigoPostal;
} Dono, Pessoa, *ptDono, *ptPessoa;

int inserirDonoLido(Bdados *bd, char *nome, int nif, CodPostal codigoPostal);
int compararDonos(void *dono1, void *dono2);
void freeDono(void *dono);

#endif