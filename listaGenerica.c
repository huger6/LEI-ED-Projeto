#include "listaGenerica.h"

/**
 * @brief Cria uma lista genérica
 * 
 * @return Lista* ou NULL em caso de erro
 */
Lista * criarLista() {
    Lista * li = (Lista *)malloc(sizeof(Lista));
    if (!li) return NULL;
    
    li->inicio = NULL;
    li->nel = 0;
    return li;
}

/**
 * @brief Adiciona um elemento ao início da lista
 * 
 * @param li    Lista
 * @param elemento  Elemento a adicionar
 * @return int 1 em caso de sucesso e 0 em caso de erro
 */
int addInicioLista(Lista * li, void * elemento) {
    if (!li || !elemento) {
        return 0;
    }

    No * aux = (No *)malloc(sizeof(No));
    if (!aux) return 0;

    aux->info = elemento;
    aux->prox = li->inicio;
    li->inicio = aux;
    li->nel++;

    return 1;
}

/**
 * @brief Coloca um elemento no final da lista
 * 
 * @param li    Lista
 * @param elemento  Elemento a colocar no final
 * 
 * @return int  0 em caso de erro e 1 em caso de sucesso
 */
int addFimLista(Lista * li, void * elemento) {
    if (!li || !elemento) {
        return 0;
    }

    No * aux = (No *)malloc(sizeof(No));
    if (!aux) return 0;

    aux->info = elemento;
    aux->prox = NULL;

    if (li->nel == 0) {
        li->inicio = aux;
    }
    else {
        No * p = li->inicio;
        while(p->prox) {
            p = p->prox;
        }
        p->prox = aux;
    }
    li->nel++;
    return 1;
}

/**
 * @brief Mostra todos os elementos da lista
 * 
 * @param li    Lista
 * @param printObj  Função para mostrar o elemento
 */
void printLista(Lista * li, void (*printObj)(void * obj)) {
    if (!li) return;

    No * p = li->inicio;
    while(p) {
        (*printObj)(p->info);
        p = p->prox;
    }
}

/**
 * @brief Liberta a memória alocada para a lista
 * 
 * @param li    Lista
 * @param freeObj   Função para libertar a memória de cada x elemento
 */
void freeLista(Lista * li, void (*freeObj)(void * obj)) {
    if (!li) return;

    No * p = li->inicio;
    No * seg;

    while(p) {
        seg = p->prox;
        (*freeObj)(p->info);
        free(p);
        p = seg;
    }
    free(li);
}

/**
 * @brief Procura um determinado elemento na lista
 * 
 * @param li    Lista
 * @param compObjs  Função para comparar os elementos
 * @param obj   Elemento a procurar
 * @return int 
 */
int pesquisarLista(Lista * li, int (*compObjs)(void * obj1, void * obj2), void * obj) {
    if (!li || !li->inicio) return 0;

    No * p = li->inicio;
    while(p) {
        if ((*compObjs)(p->info, obj) == 0) 
            return 1;
        p = p->prox;
    }

    return 0; //Não encontrou
}

/**
 * @brief Inverte a lista
 * 
 * @param li    Lista
 * @return void 
 */
void inverterLista(Lista * li) {
    No * p = li->inicio;
    No * ant = NULL;
    No * seg = NULL;
    
    while(p) {
        seg = p->prox;
        p->prox = ant;
        ant = p;
        p = seg;
    }
    li->inicio = ant;
}

/**
 * @brief Ordena a lista
 * 
 * @param li    Lista
 * @param compObjs  Função para comparar os elementos (ordem de ordenação)
 * 
 * @note Troca os elementos obj1 com obj2 quando a função compObjs retorna 1
 */
void ordenarLista(Lista * li, int (*compObjs)(void * obj1, void * obj2)) {
    if (!li || li->n_elementos < 2) return;

    char trocou;
    do {
        No * ant = NULL;
        No * a = li->inicio; //atual
        No * p = atual->prox; //proximo
        trocou = '0';
        
        while (p) {
            if ((*compObjs)(a->info, p->info) > 0) {
                trocou = '1'; //trocar p para a posicao de atual
                a->prox = p->prox;
                p->prox = a;

                if (ant) ant->prox = p;
                else li->inicio = p; //atual é o inicio

                ant = p;
                p = a->prox;
            } else {
                ant = a;
                a = p;
                p = prox->prox;
            }
        }
    } while (trocou == '1');
}

