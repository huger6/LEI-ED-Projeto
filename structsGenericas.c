#include "structsGenericas.h"

/**
 * @brief Cria uma lista genérica
 * 
 * @return Lista* ou NULL em caso de erro
 */
Lista *criarLista() {
    Lista *li = (Lista *)malloc(sizeof(Lista));
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
int addInicioLista(Lista *li, void *elemento) {
    if (!li || !elemento) {
        return 0;
    }

    No *aux = (No *)malloc(sizeof(No));
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
int addFimLista(Lista *li, void *elemento) {
    if (!li || !elemento) {
        return 0;
    }

    No *aux = (No *)malloc(sizeof(No));
    if (!aux) return 0;

    aux->info = elemento;
    aux->prox = NULL;

    if (li->nel == 0) {
        li->inicio = aux;
    }
    else {
        No *p = li->inicio;
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
void printLista(Lista *li, void (*printObj)(void *obj)) {
    if (!li) return;

    No *p = li->inicio;
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
void freeLista(Lista *li, void (*freeObj)(void *obj)) {
    if (!li) return;

    No *p = li->inicio;
    No *seg;

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
int pesquisarLista(Lista *li, int (*compObjs)(void *obj1, void *obj2), void *obj) {
    if (!li || !li->inicio) return 0;

    No *p = li->inicio;
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
void inverterLista(Lista *li) {
    No *p = li->inicio;
    No *ant = NULL;
    No *seg = NULL;
    
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
void ordenarLista(Lista * li, int (*compObjs)(void *obj1, void *obj2)) {
    if (!li || li->nel < 2) return;

    char trocou;
    do {
        No *ant = NULL;
        No *a = li->inicio; //atual
        No *p = a->prox; //proximo
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
                p = p->prox;
            }
        }
    } while (trocou == '1');
}

/**
 * @brief Pesquisa um código do tipo int na lista dada
 * 
 * @param li Lista onde procurar
 * @param compObjs Função para verificar se é a chave pretendida (deve retornar 1 caso seja)
 * @param chave Ponteiro para o dado a procurar
 * @return void* Fazer cast consoante o tipo de dados
 */
void *pesquisarPorChave(Lista *li, int (*compCod)(void *codObj, void *chave), void *chave) {
    if (!li || !li->inicio) return NULL;

    No *p = li->inicio;
    while(p) {
        if ((*compCod)(p->info, chave) == 1) {
            return p->info;
        }
        p = p->prox;
    }
    return NULL;
}

void guardarListaBin(Lista *li, void (*saveInfo)(void *obj, FILE *fileObj), FILE *file) {
    if (!li || !li->inicio || !file) return;
    No *p = li->inicio;
    fwrite(&li->nel, sizeof(int), 1, file);
    for (int i = 0; i < li.nel; i++) {
        (*saveInfo)(p->info, file);
        p = p->prox;
    }
}

// Hashing/Dicts

Dict *criarDict() {
    Dict *has = (Dict *)malloc(sizeof(Dict));
    has->Inicio = NULL;
    has->N_CHAVES = 0;
    return has;
}

//Deve retornar o ponteiro para o nó onde inserir
NoHashing *posicaoInsercao(Dict *has, void *obj, void (*compChave)(void *chave, void *obj)) {
    if (!has || !obj) return NULL;

    NoHashing *p = has->inicio;
    while(p) {
        if ((*compChave)(p->chave, obj))
            return p;
        p = p->prox;
    }
    return NULL;
}

int appendToDict(Dict *has, void *obj, void (*compChave)(void *chave, void *obj), void (*criarChave)(void *chave, void *obj)) {
    if (!has || !obj) return 0;

    NoHashing *p = posicaoInsercao(has, obj, compChave);
    // Caso não haja esse tipo de entrada ainda criada
    if (!p) {
        p = (NoHashing *)malloc(sizeof(NoHashing));
        // Criar a chave 
        criarChave(p->chave, obj);
        p->dados = criarLista();
        p->prox = has->inicio;
        has->inicio = p;
        has->nelDict++;
    }
    addInicioLista(p->Dados,obj);
    return 1;
}

void printDict(Dict *has, void (*printObj)(void *obj)) {
    if (!has) return;

    NoHashing *p = has->inicio;
    while(p)
    {
        printLista(p->dados, printObj);
        p = p->prox;
    }
}

void freeDict(Dict *has, void (*freeChave)(void *chave), void (*freeObj)(void *obj)) {
    if (!has) return;

    NoHashing *p = has->inicio;
    while(p) {
        freeChave(p->chave);
        freeLista(p->dados, freeObj);
        p = p->prox;
    }
}

void ordenarDict(Dict *has, void (*compChave)(void *chave1, void *obj)) {
    if (!has || has->nelDict < 2) return;

    char trocou;
    do {
        NoHashing *ant = NULL;
        NoHashing *a = has->inicio; //atual
        NoHashing *p = a->prox; //proximo
        trocou = '0';
        
        while (p) {
            if ((*compChave)(a->chave, p->chave) > 0) {
                trocou = '1'; //trocar p para a posicao de atual
                a->prox = p->prox;
                p->prox = a;

                if (ant) ant->prox = p;
                else has->inicio = p; //atual é o inicio

                ant = p;
                p = a->prox;
            } else {
                ant = a;
                a = p;
                p = p->prox;
            }
        }
    } while (trocou == '1');
}