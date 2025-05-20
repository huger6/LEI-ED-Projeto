#ifndef BD_HEADERS
#define BD_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "structsGenericas.h"
#include "distancias.h"

typedef struct Bdados{
    Dict *donosNif;
    Dict *donosAlfabeticamente;
    Dict *carrosCod;
    Dict *carrosMarca;
    Dict *carrosMat;
    Lista *sensores;
    Distancias *distancias;
    Lista *viagens;
} Bdados;


int inicializarBD(Bdados *bd);
void freeTudo(Bdados *bd);

// Exportação fica aqui
void exportarTudoXML(Bdados *bd, const char *filename);
void exportarTudoCSV(Bdados *bd, const char *donosFilename, const char *carrosFilename, const char *sensoresFilename, const char *distanciasFilename, const char *viagensFilename);
void exportarTudoHTML(Bdados *bd, const char *donosFilename, const char *carrosFilename, const char *sensoresFilename, const char *distanciasFilename, const char *viagensFilename);
size_t memUsageTudo(Bdados *bd);


#endif