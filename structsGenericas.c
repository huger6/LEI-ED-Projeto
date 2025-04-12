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
 * @param compObjs Função para verificar se é a chave pretendida (deve retornar 0 caso seja)
 * @param chave Ponteiro para o dado a procurar
 * @return void* Fazer cast consoante o tipo de dados
 */
void *pesquisarPorChave(Lista *li, int (*compCod)(void *codObj, void *chave), void *chave) {
    if (!li || !li->inicio) return NULL;

    No *p = li->inicio;
    while(p) {
        if ((*compCod)(p->info, chave) == 0) {
            return p->info;
        }
        p = p->prox;
    }
    return NULL;
}

/**
 * @brief Guarda o conteúdo de uma lista em ficheiro binário
 * 
 * @param li Lista
 * @param saveInfo Função para guardar cada elemento da lista
 * @param file Ficheiro binário aberto
 * 
 * @note Guarda o Nº de elementos da lista primeiro
 */
void guardarListaBin(Lista *li, void (*saveInfo)(void *obj, FILE *fileObj), FILE *file) {
    if (!li || !li->inicio || !file) return;

    No *p = li->inicio;
    fwrite(&li->nel, sizeof(int), 1, file);
    for (int i = 0; i < li->nel; i++) {
        (*saveInfo)(p->info, file);
        p = p->prox;
    }
}

/**
 * @brief Ler os dados binários para uma lista
 * 
 * @param readInfo Função para ler cada elemento da lista
 * @param file Ficheiro binário
 * @return Lista* Lista com os elementos lidos ou NULL se erro
 */
Lista *lerListaBin(void (*readInfo)(Lista *li, FILE *fileObj), FILE *file) {
    if (!file) return NULL;

    Lista *li = criarLista();
    if (!li) return NULL;

    fread(&li->nel, sizeof(int), 1, file);
    if (li->nel < 0) {
        free(li);
        return NULL;
    }

    for (int i = 0;i < li->nel; i++) {
        (*readInfo)(li, file);
    }
    return li;
}

// Hashing/Dicts

/**
 * @brief Cria um dicionário/hashing
 * 
 * @return Dict* NULL em caso de erro
 */
Dict *criarDict() {
    Dict *has = (Dict *)malloc(sizeof(Dict));
    has->inicio = NULL;
    has->nelDict = 0;
    return has;
}

/**
 * @brief Obtém o nó de inserção de um elemento
 * 
 * @param has Dicionário
 * @param obj Elemento
 * @param compChave Função para comparar a chave (deve retornar 0 se iguais)
 * @return NoHashing* de inserção ou NULL se erro ou essa chave ainda não existe
 */
NoHashing *posicaoInsercao(Dict *has, void *obj, int (*compChave)(void *chave, void *obj)) {
    if (!has || !obj) return NULL;

    NoHashing *p = has->inicio;
    while(p) {
        if ((*compChave)(p->chave, obj) == 0)
            return p;
        p = p->prox;
    }
    return NULL;
}

/**
 * @brief Adiciona um elemento ao dicionário
 * 
 * @param has Dicionário
 * @param obj Elemento a adicionar
 * @param compChave Função para comparar as chaves (deve retornar 0 se iguais)
 * @param criarChave Função para criar uma chave
 * @return int 0 se erro, 1 se sucesso
 */
int appendToDict(Dict *has, void *obj, int (*compChave)(void *chave, void *obj), void *(*criarChave)(void *obj)) {
    if (!has || !obj) return 0;

    NoHashing *p = posicaoInsercao(has, obj, compChave);
    // Caso não haja esse tipo de entrada ainda criada
    if (!p) {
        p = (NoHashing *)malloc(sizeof(NoHashing));
        // Criar a chave 
        p->chave = criarChave(obj);
        p->dados = criarLista();
        p->prox = has->inicio;
        has->inicio = p;
        has->nelDict++;
    }
    addInicioLista(p->Dados,obj);
    return 1;
}

/**
 * @brief Mostra todos os elementos do dicionário
 * 
 * @param has Dicionário
 * @param printObj Função para mostrar cada elemento
 */
void printDict(Dict *has, void (*printObj)(void *obj)) {
    if (!has) return;

    NoHashing *p = has->inicio;
    while(p)
    {
        printLista(p->dados, printObj);
        p = p->prox;
    }
}

/**
 * @brief Liberta a memória do dicionário e os seus elementos
 * 
 * @param has Dicionário
 * @param freeChave Função para libertar a memória da chave
 * @param freeObj Função para libertar a memória de cada elemento
 */
void freeDict(Dict *has, void (*freeChave)(void *chave), void (*freeObj)(void *obj)) {
    if (!has) return;

    NoHashing *p = has->inicio;
    while(p) {
        freeChave(p->chave);
        freeLista(p->dados, freeObj);
        p = p->prox;
    }
}

/**
 * @brief Ordena um dicionário pelas chaves
 * 
 * @param has Dicionário
 * @param compChave Função para comparar as chaves(troca quando compChave(chave1, chave2) > 0)
 */
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

/**
 * @brief Guarda o conteúdo de um dicionário em ficheiro binário
 * 
 * @param has Dicionário
 * @param guardarChave Função para guardar a chave no ficheiro binário
 * @param saveInfo Função para guardar cada elemento da lista 
 * @param file Ficheiro onde guardar, aberto
 */
void guardarDictBin(Dict *has, void (*guardarChave)(void *chave, FILE *fileObj), void (*saveInfo)(void *obj, FILE *fileObj), FILE *file) {
    if (!has || !file) return;

    NoHashing *p = has->inicio;
    fwrite(&has->nelDict, sizeof(int), 1, file);
    while(p) {
        (*guardarChave)(p->chave, file);
        guardarListaBin(p->dados, saveInfo, file);
        p = p->prox;
    }
}

/**
 * @brief Ler os dados de um ficheiro binário para um dicionário
 * 
 * @param readChave Função para ler uma chave
 * @param readInfo Função para ler um elemento
 * @param file Ficheiro binário, aberto
 * @param freeChave Função para libertar a chave, caso necessário (erro)
 * @param freeObj Função para libertar o elemento, caso necessário (erro)
 * @return Dict* Dicionário ou NULL
 */
Dict *lerDictBin(void *(*readChave)(FILE *fileObj), void (*readInfo)(Lista *li, FILE *fileObj), FILE *file, void (*freeChave)(void *chave), void (*freeObj)(void *obj)) {
    if (!file) return NULL;

    Dict *has = criarDict();
    if (!has) return NULL;

    fread(&has->nelDict, sizeof(int), 1, file);
    if (has->nelDict < 0) {
        free(has);
        return NULL;
    }

    char darFree = '0';
    NoHashing *ant = NULL;
    for (int i = 0; i < has->nelDict; i++) {
        NoHashing *atual = (NoHashing *)malloc(sizeof(NoHashing));
        if (!atual) {
            darFree = '1';
            break;
        }
        atual->chave = readChave(file);
        atual->dados = lerListaBin(readInfo, file);
        atual->prox = NULL;

        if (!ant) {
            has->inicio = atual;
        }
        else {
            ant->prox = atual;
        }
        ant = atual;
    }
    if (darFree == '1') {
        freeDict(has, freeChave, freeObj);
        return NULL;
    }
    return has;
}

