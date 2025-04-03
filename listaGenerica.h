#ifndef LISTAS_GENERICAS_HEADERS
#define LISTAS_GENERICAS_HEADERS

#include <stdio.h>
#include <stdlib.h>

typedef struct no {
   void * info;
   struct no * prox;
} No;

typedef struct {
    No * inicio;
    int nel;
} Lista;


Lista * criarLista();
int addInicioLista(Lista * li, void * elemento);
void printLista(Lista * li, void (*printObj)(void * obj));
void freeLista(Lista * li, void (*freeObj)(void * obj));
int pesquisarLista(Lista * li, int (*compObjs)(void * obj1, void * obj2), void * obj);
void ordenarLista(Lista * li, int (*compObjs)(void * obj1, void * obj2));
void * pesquisarPorChave(Lista *li, int (*compCod)(void *codObj, void *chave), void *chave);


#endif