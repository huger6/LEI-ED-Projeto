#ifndef PASSAGENS_HEADERS
#define PASSAGENS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#include "sensores.h"
#include "carro.h"
#include "bdados.h"
#include "uteis.h"

typedef struct {
    int idSensor; //PRIMARY KEY / FOREIGN KEY(idSensor) - REFERENCES Sensor(codSensor)
    Carro *veiculo;
    Data data;
    char tipoRegisto;
} Passagem, Pass, *ptPassagem, *ptPass;

typedef struct {
    Passagem *entrada;
    Passagem *saida;
    float kms;
    float tempo; //Em horas
} Viagem;


Passagem * obterPassagem(Bdados *bd, int idSensor, int codVeiculo, Data date, char tipoRegisto);
int inserirViagemLido(Viagem *v, Passagem *entrada, Passagem *saida);
int compararPassagens(void *passagem1, void *passagem2);
int compCodPassagem(void *passagem, void *codigo);
void freePassagem(void *passagem);
void freeViagem(void *viagem);
void guardarViagemBin(void *viagem, FILE *file);
void guardarPassagemBin(void *passagem, FILE *file);
void mostrarPassagem(void *passagem);


#endif