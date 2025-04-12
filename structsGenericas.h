#ifndef STRUCTS_GENERICAS_HEADERS
#define STRUCTS_GENERICAS_HEADERS

#include <stdio.h>
#include <stdlib.h>

typedef struct no {
   void *info;
   struct no *prox;
} No;

typedef struct {
    No *inicio;
    int nel;
} Lista;

typedef struct noHash {
    void *chave;
    Lista *dados;
    struct noHash *prox;
} NoHashing;

typedef struct {
    NoHashing *inicio;
    int nelDict;
} Dict;


// Listas

Lista *criarLista();
int addInicioLista(Lista *li, void *elemento);
void printLista(Lista *li, void (*printObj)(void *obj));
void freeLista(Lista *li, void (*freeObj)(void *obj));
int pesquisarLista(Lista *li, int (*compObjs)(void *obj1, void *obj2), void *obj);
void ordenarLista(Lista *li, int (*compObjs)(void *obj1, void *obj2));
void *pesquisarPorChave(Lista *li, int (*compCod)(void *codObj, void *chave), void *chave);
void guardarListaBin(Lista *li, void (*saveInfo)(void *obj, FILE *fileObj), FILE *file);
Lista *lerListaBin(void (*readInfo)(Lista *li, FILE *fileObj), FILE *file);

// Hashing

Dict *criarDict();
NoHashing *posicaoInsercao(Dict *has, void *obj, int (*compChave)(void *chave, void *obj));
int appendToDict(Dict *has, void *obj, int (*compChave)(void *chave, void *obj), void *(*criarChave)(void *obj));
void printDict(Dict *has, void (*printObj)(void *obj));
void freeDict(Dict *has, void (*freeChave)(void *chave), void (*freeObj)(void *obj));
void ordenarDict(Dict *has, void (*compChave)(void *chave1, void *obj));
void guardarDictBin(Dict *has, void (*guardarChave)(void *chave, FILE *fileObj), void (*saveInfo)(void *obj, FILE *fileObj), FILE *file);
Dict *lerDictBin(void *(*readChave)(FILE *fileObj), void (*readInfo)(Lista *li, FILE *fileObj), FILE *file, void (*freeChave)(void *chave), void (*freeObj)(void *obj));

#endif