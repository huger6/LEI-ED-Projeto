#ifndef STRUCTS_GENERICAS_HEADERS
#define STRUCTS_GENERICAS_HEADERS

#include <stdio.h>
#include <stdlib.h>

#define TAMANHO_TABELA_HASH 16007 // load factor 0,62; Nºprimo para não haver tantas colisões

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
    NoHashing *tabela[TAMANHO_TABELA_HASH];
    int nelDict;
} Dict;

typedef struct noArvore {
    void *info;
    struct noArvore *esq;
    struct noArvore *dir;
} NoArvore;

typedef struct {
    NoArvore *raiz;
    int nelArvore;
} Arvore;


// Listas

Lista *criarLista();
int addInicioLista(Lista *li, void *elemento);
void printLista(Lista *li, void (*printObj)(void *obj));
void freeLista(Lista *li, void (*freeObj)(void *obj));
int pesquisarLista(Lista *li, int (*compObjs)(void *obj1, void *obj2), void *obj);
void ordenarLista(Lista *li, int (*compObjs)(void *obj1, void *obj2));
void *searchLista(Lista *li, int (*compCod)(void *codObj, void *chave), void *chave);
void guardarListaBin(Lista *li, void (*saveInfo)(void *obj, FILE *fileObj), FILE *file);
Lista *readListaBin(void *(*readInfo)(FILE *fileObj), FILE *file);

// Hashing

Dict *criarDict();
NoHashing *posicaoInsercao(Dict *has, int indice, void *obj, int (*compChave)(void *chave, void *obj));
int appendToDict(Dict *has, void *obj, int (*compChave)(void *chave, void *obj), void *(*criarChave)(void *obj), int (*hashChave)(void *obj), void (*freeChave)(void *chave));
void printDict(Dict *has, void (*printObj)(void *obj));
void freeDict(Dict *has, void (*freeChave)(void *chave), void (*freeObj)(void *obj));
void *searchDict(Dict *has, void *chave, int (*compChave)(void *chave, void *obj), int (*compCod)(void *codObj, void *chave), int (*hashChave)(void *chave));
void guardarDictBin(Dict *has, void (*guardarChave)(void *chave, FILE *fileObj), void (*saveInfo)(void *obj, FILE *fileObj), FILE *file);
Dict *readDictBin(void *(*readChave)(FILE *fileObj), void *(*readInfo)(FILE *fileObj), FILE *file, void (*freeChave)(void *chave), void (*freeObj)(void *obj), int (*hashChave)(void *chave));

// Árvores




#endif