#ifndef DADOS_HEADERS
#define DADOS_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEPARADOR '\t' //separador dos ficheiros

//Nomes default para os ficheiros
#define LOGS_TXT "logs.txt"
#define DONOS_TXT "donos.txt"
#define CARROS_TXT "carros.txt"
#define SENSORES_TXT "sensores.txt"
#define DISTANCIAS_TXT "distancias.txt"
#define PASSAGEM_TXT "passagem.txt"

//Número de paraâmetros por cada ficheiro
#define PARAM_DONOS 3
#define PARAM_CARROS 6
#define PARAM_SENSORES 4
#define PARAM_DISTANCIAS 3
#define PARAM_PASSAGEM 4

int carregarDadosTxt(char * fDonos, char * fCarros, char * fSensores, char * fDistancias, char * fPassagem, const char * fLogs);
int carregarDonosTxt(BaseDados *bd, char *donosFilename, FILE *logs);
int carregarCarrosTxt(BaseDados *bd, char *carrosFilename, FILE *logs);
int carregarSensoresTxt(BaseDados *bd, char *sensoresFilename, FILE *logs);


void removerEspacos(char * str);
void separarParametros(const char * linha, char ** parametros, int * num_parametros);


#endif