#ifndef VALIDACOES_HEADERS
#define VALIDACOES_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "uteis.h"

int validarNif(int nif);
char *validarNome(char * nome);
int validarCodPostal(CodPostal *CP);
int validarAnoCarro(short ano);

#endif