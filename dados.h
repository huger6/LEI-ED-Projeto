#ifndef DADOS_HEADERS
#define DADOS_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bdados.h"

int carregarDadosTxt(Bdados *bd, char *fDonos, char *fCarros, char *fSensores, char *fDistancias, char *fPassagem, char *fLogs);
int carregarDonosTxt(Bdados *bd, char *donosFilename, FILE *logs);
int carregarCarrosTxt(Bdados *bd, char *carrosFilename, FILE *logs);
int carregarSensoresTxt(Bdados *bd, char *sensoresFilename, FILE *logs);
int carregarDistanciasTxt(Bdados *bd, char *distanciasFilename, FILE *logs);
int carregarPassagensTxt(Bdados *bd, char *passagensFilename, FILE *logs);


void removerEspacos(char * str);
void separarParametros(char *linha, char **parametros, int *numParametros, const int paramEsperados);
void linhaInvalida(const char *linha, int nLinha, FILE *logs);


#endif