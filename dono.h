#ifndef DONO_HEADERS
#define DONO_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "uteis.h"

typedef struct {
    int nif; //PRIMARY KEY
    char *nome;
    CodPostal codigoPostal;
} Dono, Pessoa, *ptDono, *ptPessoa;

#endif