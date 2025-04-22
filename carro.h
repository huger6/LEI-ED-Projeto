#ifndef CARRO_HEADERS
#define CARRO_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dono.h"
#include "constantes.h"

struct Bdados;

typedef struct {
    char matricula[MAX_MATRICULA + 1];
    char *marca;
    char *modelo;
    short ano;
    int codVeiculo; //PRIMARY KEY
    Dono *ptrPessoa;
} Carro, Car, *ptCar, *ptCarro;


int compararCarros(void *carro1, void *carro2);
int inserirCarroLido(struct Bdados *bd, char *matricula, char *marca, char *modelo, short ano, int nif, int codVeiculo);
int compCodCarro(void *carro, void *codigo);
void freeCarro(void *carro);
void printCarro(void *carro);
void guardarCarroBin(void *carro, FILE *file);
void *readCarroBin(FILE *file);
void guardarChaveCarroMarca(void *chaveMarca, FILE *file);
void *criarChaveCarroMarca(void *carro);
int hashChaveCarroMarca(void *chave);
void freeChaveCarroMarca(void *chave);
int compChaveCarroMarca(void *chave, void *chave2);
int compCarroMarca (void *carro1, void *carro2);
void guardarChaveCarroCod(void *chave, FILE *file);
void *readChaveCarroCod(FILE *file);
void *criarChaveCarroCod(void *carro);
int hashChaveCarroCod(void *carro);
void freeChaveCarroCod(void *chave);
int compChaveCarroCod(void *chave, void *chave2);
void printCarroXML(void *carro, int indentacao, FILE *file);
void printHeaderCarrosCSV(FILE *file);
void printCarroCSV(void *carro, FILE *file);


int ordenarAlfModelo (void *carro1, void *carro2);
int ordenarAlfMatricula (void *carro1, void *carro2);
int compChaveMarca(void *chave, void *carro);

void RegistarVeiculo(struct Bdados *bd);

#endif