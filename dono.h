#ifndef DONO_HEADERS
#define DONO_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bdados.h"

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
int compCodDono(void *dono, void *codigo);
void freeDono(void *dono);
void printDono(void *dono);
void guardarDonoBin(void *obj, FILE *file);
void guardarChaveDonoNif(void *chaveNif, FILE *file);
void *criarChaveDonoNif(void *dono);
int compChaveDonoNif(void *chave, void *dono);
void *criarChaveDonoAlfabeticamente(void *dono);
int compChaveDonoAlfabeticamente(void *chave, void *dono);
int compDonosNome(void *dono1, void *dono2);

#endif