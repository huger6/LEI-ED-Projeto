#ifndef BD_HEADERS
#define BD_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "structsGenericas.h"
#include "distancias.h"

typedef struct {
    Dict *donosNif;
    Dict *donosAlfabeticamente;
    Dict *carrosMarca;
    Lista *sensores;
    Distancias *distancias;
    Lista *viagens;
} Bdados;


int inicializarBD(Bdados *bd);
void freeTudo(Bdados *bd);


#endif