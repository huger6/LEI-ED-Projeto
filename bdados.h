#ifndef BD_HEADERS
#define BD_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "listaGenerica.h"

typedef struct {
    Lista * donos;
    Lista * carros;
    Lista * sensores;
    Lista * distancias;
    Lista * passagens;
} Bdados;


int inicializarListasBD(Bdados *bd);
void freeTudo(Bdados *bd);


#endif