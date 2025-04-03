#ifndef VALIDACOES_HEADERS
#define VALIDACOES_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#include "uteis.h"

int validarNif(const int nif);
char * validarNome(char *nome);
int validarCodPostal(const short zona, const short local);
int validarAnoCarro(const short ano);
int validarMatricula(const char *matricula);
char * validarMarca(char *marca);
char * validarModelo(char *modelo);
int validarCodVeiculo(const int codigo);
int validarCodSensor(const int codSensor);
int validarDistancia(const float distancia);
int validarData(const Data date, const char modo);
int validarTipoRegisto(const char tipoRegisto);

#endif