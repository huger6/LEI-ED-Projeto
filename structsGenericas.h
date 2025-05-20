#ifndef STRUCTS_GENERICAS_HEADERS
#define STRUCTS_GENERICAS_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_TABELA_HASH 16007 // load factor 0,62; Nºprimo para não haver tantas colisões

extern int listagemFlag;
extern int printingDict;

extern void pressEnter();
extern int enter_espaco_esc();

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
    struct noHash *prox; // Necessário caso chaves diferentes gerem o mesmo índice
} NoHashing;

typedef struct {
    NoHashing *tabela[TAMANHO_TABELA_HASH];
    int nelDict;
} Dict;

typedef struct noRankings {
    void *mainInfo;
    void *compInfo;
    struct noRankings *prox;
} NoRankings;

typedef struct {
    NoRankings *inicio;
    int nel;
} Ranking;


// Listas

Lista *criarLista();
int addInicioLista(Lista *li, void *elemento);
void printLista(Lista *li, void (*printObj)(void *obj, FILE *file), FILE *file, int pausa);
void exportarListaXML(Lista *li, char *nomeLista, void (*printObj)(void *obj, int indentacao, FILE *file), int indentacao, FILE *file);
void exportarListaCSV(Lista *li, void (*printHeader)(FILE *file), void (*printObj)(void *obj, FILE *file), FILE *file);
void exportarListaHTML(Lista *li, char *pagename, void (*printTableHead)(FILE *file), void  (*printObj)(void *obj, FILE *file), FILE *file);
void freeLista(Lista *li, void (*freeObj)(void *obj));
int pesquisarLista(Lista *li, int (*compObjs)(void *obj1, void *obj2), void *obj);
void mergeSortLista(Lista* li, int (*compObjs)(void *obj1, void *obj2));
void ordenarLista(Lista *li, int (*compObjs)(void *obj1, void *obj2));
void *searchLista(Lista *li, int (*compCod)(void *codObj, void *chave), void *chave);
void guardarListaBin(Lista *li, void (*saveInfo)(void *obj, FILE *fileObj), FILE *file);
Lista *readListaBin(void *(*readInfo)(FILE *fileObj), FILE *file);
size_t listaMemUsage(Lista *li, size_t (*objMemUsage)(void *obj));

// Hashing

Dict *criarDict();
NoHashing *posicaoInsercao(Dict *has, int indice, void *chave, int (*compChave)(void *chave, void *chave2));
Lista *obterListaDoDict(Dict *has, void *chave, int (*compChave)(void *chave, void *chave2), int (*hashChave)(void *obj));
int appendToDict(Dict *has, void *obj, int (*compChave)(void *chave, void *obj), void *(*criarChave)(void *obj), int (*hashChave)(void *obj), void (*freeObj)(void *obj), void (*freeChave)(void *chave));
void printDict(Dict *has, void (*printObj)(void *obj, FILE *file),FILE *file, int pausa);
void exportarDictXML(Dict *has, char *nomeDict, void (*printObj)(void *obj, int indentacao, FILE *file), int indentacao, FILE *file);
void exportarDictCSV(Dict *has, void (*printHeader)(FILE *file), void (*printObj)(void *obj, FILE *file), FILE *file);
void exportarDictHTML(Dict *has, char *pagename, void (*printTableHead)(FILE *file), void  (*printObj)(void *obj, FILE *file), FILE *file);
void freeDict(Dict *has, void (*freeChave)(void *chave), void (*freeObj)(void *obj));
void *searchDict(Dict *has, void *chave, int (*compChave)(void *chave, void *obj), int (*compCod)(void *codObj, void *chave), int (*hashChave)(void *chave));
void guardarDadosDictBin(Dict *has, void (*saveInfo)(void *obj, FILE *fileObj), FILE *file);
Dict *readToDictBin(void *(*criarChave)(void *obj), int (*hashChave)(void *obj), void (*freeObj)(void *obj), void (*freeChave)(void *chave), void *(*readInfo)(FILE *fileObj), FILE *file);
size_t dictMemUsage(Dict *has, size_t (*objMemUsage)(void *obj), size_t (*chaveMemUsage)(void *chave));
Lista *dictToLista(Dict *has);

// Listas Rankings

Ranking *criarRanking();
int addToRanking(Ranking *r, void *mainInfo, void *compInfo);
void printRanking(Ranking *r, void printCarroRanking(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file), void (*printHeaderCompObj)(FILE *file), void (*printCompObj)(void *compInfo, FILE *file), FILE *file, int pausa);
void freeRanking(Ranking *r, void (*freeMainObj)(void *obj), void (*freeCompObj)(void *obj));
void mergeSortRanking(Ranking *r, int (*compObjs)(void *obj1, void *obj2));


#endif