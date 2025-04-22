#ifndef DONO_HEADERS
#define DONO_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Bdados;

typedef struct {
    short zona; //Primeiros 4 dígitos
    short local; //Últimos 3 dígitos
} CodPostal;

typedef struct {
    int nif; //PRIMARY KEY
    char *nome;
    CodPostal codigoPostal;
} Dono, Pessoa, *ptDono, *ptPessoa;

int inserirDonoLido(struct Bdados *bd, char *nome, int nif, CodPostal codigoPostal);
int compDonosNif(void *dono1, void *dono2);
int compDonosNome(void *dono1, void *dono2);
int compCodDono(void *dono, void *codigo);
void freeDono(void *dono);
void printDono(void *dono);
void guardarDonoBin(void *obj, FILE *file);
void *readDonoBin(FILE *file);
void guardarChaveDonoNif(void *chaveNif, FILE *file);
void *readChaveDonoNif(FILE *file);
void *criarChaveDonoNif(void *dono);
void freeChaveDonoNif(void *chave);
int compChaveDonoNif(void *chave, void *chave2);
int hashChaveDonoNif(void *chave);
void *criarChaveDonoAlfabeticamente(void *dono);
void freeChaveDonoAlfabeticamente(void *chave);
int compChaveDonoAlfabeticamente(void *chave, void *chave2);
void guardarChaveDonoAlfabeticamente(void *chave, FILE *file);
int hashChaveDonoAlfabeticamente(void *chave);
void printDonoXML(void *dono, int indentacao, FILE *file);
void printHeaderDonosCSV(FILE *file);
void printDonoCSV(void *dono, FILE *file);




#endif