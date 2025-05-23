#include "structsGenericas.h"

int listagemFlag = 0; // Inicializar a 0, caso seja 1 a listagem é interrompida
int printingDict = 0;

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
 * @param li Lista
 * @param printObj  Função para mostrar o elemento
 * @param pausa Número de elementos a mostrar antes de pedir pausa (pressionar enter), 0 para ignorar
 * 
 * @note Adiciona um espaço entre cada elemento
 * @note listagemFlag deve ser colocado a 0 depois do uso da função
 */
void printLista(Lista *li, void (*printObj)(void *obj, FILE *file), FILE *file, int pausa) {
    if (!li || !printObj || !file || pausa < 0) return;

    int noStop = 1;
    int count = 0;
    listagemFlag = 0;

    No *p = li->inicio;
    while(p) {
        (*printObj)(p->info, file);
        p = p->prox;

        if (file == stdout && pausa) {
            count++;
            if (count % pausa == 0 && count < li->nel && noStop) {
                printf("\n");
                int opcao = enter_espaco_esc();
                switch (opcao) {
                    case 0:
                        break;
                    case 1:
                        if (printingDict == 1) {
                            listagemFlag = -1;
                            return;
                        } 
                        while(count < li->nel - pausa) {
                            count++;
                            p = p->prox;
                        }
                        noStop = 0;
                        break;
                    case 2:
                        listagemFlag = 1; // Avisar possíveis funções nestadas
                        return;
                    default:
                        break;
                }
            }
        }

        if (file == stdout) printf("\n"); // Para distinguir cada elemento
    }
}

/**
 * @brief Exporta uma lista para formato XML
 * 
 * @param li Lista a exportar
 * @param nomeLista Nome da tag principal (pode ser NULL)
 * @param printObj Função que mostre a tag de cada objeto e a sua informação
 * @param indentacao Número de "\t" a utilizar
 * @param file Ficheiro do tipo .xml (ou .txt), aberto
 */
void exportarListaXML(Lista *li, char *nomeLista, void (*printObj)(void *obj, int indentacao, FILE *file), int indentacao, FILE *file) {
    if (!li || !li->inicio || !printObj || !file || indentacao < 0) return;

    if (nomeLista) {
        for (int i = 0; i < indentacao; i++) fprintf(file, "\t");
        fprintf(file, "<%s>\n", nomeLista);
    }

    No *p = li->inicio;
    while(p) {
        if (nomeLista) printObj(p->info, indentacao + 1, file);
        else printObj(p->info, indentacao, file);
        p = p->prox;
    }

    if (nomeLista) {
        for (int i = 0; i < indentacao; i++) fprintf(file, "\t");
        fprintf(file, "</%s>\n", nomeLista);
    }
}

/**
 * @brief Exporta uma lista para formato CSV
 * 
 * @param li Lista a exportar
 * @param printObj Função para mostrar o objeto
 * @param file Ficheiro .csv, aberto
 */
void exportarListaCSV(Lista *li, void (*printHeader)(FILE *file), void (*printObj)(void *obj, FILE *file), FILE *file) {
    if (!li || !li->inicio || !printObj || !file) return;

    if (printHeader) printHeader(file);

    No *p = li->inicio;
    while(p) {
        printObj(p->info, file);
        p = p->prox;
    }
}

/**
 * @brief Exporta uma lista para formato .html
 * 
 * @param li Lista
 * @param pagename Nome da página
 * @param printTableHead Função para mostrar o cabeçalho da tabela 
 * @param printObj Função para mostrar o objeto
 * @param file Ficheiro .html, aberto
 */
void exportarListaHTML(Lista *li, char *pagename, void (*printTableHead)(FILE *file), void  (*printObj)(void *obj, FILE *file), FILE *file) {
    if (!li || !li->inicio || !printObj || !file) return;

    // Lista de caracteres HTML especiais que precisam ser substituídos
    if (!pagename) return;

    const char *invalidos = "<>\"'&";
    for (int i = 0; pagename[i]; i++) {
        if (strchr(invalidos, pagename[i]) != NULL) {
            pagename = NULL; // Nome da página contém caracteres HTML inválidos (usar default)
        }
    }

    fprintf(file,
        "<!DOCTYPE html>\n"
        "<html lang=\"pt\">\n"
        "<head>\n"
        "\t<meta charset=\"UTF-8\">\n"
        "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
        "\t<title>%s</title>\n"
        "\t<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n"
        "</head>\n"
        "<body>\n"
        "\t<div class=\"container my-4\">\n"
        "\t\t<div class=\"table-responsive\">\n"
        "\t\t\t<table class=\"table table-bordered table-striped table-hover\">\n"
        "\t\t\t\t<thead class=\"table-dark\">\n", pagename ? pagename : "Exportação HTML");
        (*printTableHead)(file); 
        fprintf(file, "\t\t\t\t<tbody>\n");

        No *p = li->inicio;
        while(p) {
            printObj(p->info, file);
            p = p->prox;
        }

        fprintf(file,   "\t\t\t\t</tbody>\n"
                        "\t\t\t</table>\n"
                        "\t\t</div>\n"
                        "\t</div>\n"
                        "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js\"></script>\n"
                        "</body>\n"
                        "</html>\n");
}

/**
 * @brief Liberta a memória alocada para a lista
 * 
 * @param li    Lista
 * @param freeObj   Função para libertar a memória de cada x elemento
 * 
 * @note freeObj pode ser passada como NULL para ignorar a libertação de cada elemento
 */
void freeLista(Lista *li, void (*freeObj)(void *obj)) {
    if (!li) return;

    No *p = li->inicio;
    No *seg;

    while(p) {
        seg = p->prox;
        if (freeObj) {
            (*freeObj)(p->info);
        }
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
    if (!li || !li->inicio || !compObjs) return 0;

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
 * @brief 
 * 
 * @param a Nó da primeira lista
 * @param b Nó da segunda lista
 * @param compObjs Função para comparar elementos (deve retornar < 0 se obj1 < obj2)
 * @return No* Lista agrupada
 */
static No *mergeListas(No *a, No *b, int (*compObjs)(void *obj1, void *obj2)) {
    if (!a) return b;
    if (!b) return a;
    
    No *result = NULL;
    
    if (compObjs(a->info, b->info) <= 0) {
        result = a;
        result->prox = mergeListas(a->prox, b, compObjs);
    } else {
        result = b;
        result->prox = mergeListas(a, b->prox, compObjs);
    }
    
    return result;
}

/**
 * @brief Separar a lista em duas metades
 * 
 * @param head Lista a separar
 * @param front Ponteiro para a primeira metade
 * @param back Ponteiro para a segunda metade
 */
static void splitLista(No *head, No **front, No **back) {
    No *rapido = head->prox;
    No *lento = head;
    
    while (rapido != NULL) {
        rapido = rapido->prox;
        if (rapido != NULL) {
            lento = lento->prox;
            rapido = rapido->prox;
        }
    }
    
    *front = head;
    *back = lento->prox;
    lento->prox = NULL;
}

/**
 * @brief MergeSort recursivo para os nós da função
 * 
 * @param headRef Head da função
 * @param compObjs Função para comparar objetos
 */
static void mergesortRecursivo(No **headRef, int (*compObjs)(void *obj1, void *obj2)) {
    No *head = *headRef;
    if (!head || !head->prox) return;
    
    No *a;
    No *b;
    
    splitLista(head, &a, &b);
    mergesortRecursivo(&a, compObjs);
    mergesortRecursivo(&b, compObjs);
    
    *headRef = mergeListas(a, b, compObjs);
}

/**
 * @brief Ordenar lista genérica com merge sort
 * 
 * @param li Lista a ordenar
 * @param compObjs Função para comparar elementos
 */
void mergeSortLista(Lista* li, int (*compObjs)(void *obj1, void *obj2)) {
    if (!li || !li->inicio || !compObjs) return;
    // Código adaptado de https://www.geeksforgeeks.org/merge-sort-for-linked-list/

    mergesortRecursivo(&li->inicio, compObjs);
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
    if (!li || li->nel < 2 || !compObjs) return;

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
 * @brief Pesquisa um código na lista dada
 * 
 * @param li Lista onde procurar
 * @param compObjs Função para verificar se é a chave pretendida (deve retornar 0 caso seja)
 * @param chave Ponteiro para o dado a procurar
 * @return void* Fazer cast consoante o tipo de dados
 */
void *searchLista(Lista *li, int (*compCod)(void *codObj, void *chave), void *chave) {
    if (!li || !li->inicio || !compCod) return NULL;

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
    if (!li || !li->inicio || !file || !saveInfo) return;

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
Lista *readListaBin(void *(*readInfo)(FILE *fileObj), FILE *file) {
    if (!file || !readInfo) return NULL;

    Lista *li = criarLista();
    if (!li) return NULL;

    fread(&li->nel, sizeof(int), 1, file);
    if (li->nel < 0) {
        free(li);
        return NULL;
    }

    No *ant = NULL;
    for (int i = 0; i < li->nel; i++) {
        No *atual = (No *)malloc(sizeof(No));
        if (!atual) {
            freeLista(li, NULL);
            return NULL;
        }
        atual->info = (*readInfo)(file);
        atual->prox = NULL;

        if (!ant) {
            li->inicio = atual;
        }
        else {
            ant->prox = atual;
        }
        ant = atual;
    }
    return li;
}

/**
 * @brief Obter a memória ocupada por uma lista
 * 
 * @param li Lista
 * @param objMemUsage Função para determinar a memória ocupada por um elemento
 * @return size_t Memória ocupada pela lista ou 0 se erro
 */
size_t listaMemUsage(Lista *li, size_t (*objMemUsage)(void *obj)) {
    if (!li) return 0;

    size_t mem = 0;
    mem += sizeof(*li);

    No *p = li->inicio;
    while(p) {
        mem += sizeof(*p);
        if (objMemUsage) mem += objMemUsage(p->info);

        p = p->prox;
    }

    return mem;
}

// Hashing/Dicts

/**
 * @brief Cria um dicionário/hashing
 * 
 * @return Dict* NULL em caso de erro
 */
Dict *criarDict() {
    Dict *has = (Dict *)malloc(sizeof(Dict));
    if (!has) return NULL;
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        has->tabela[i] = NULL;
    }
    has->nelDict = 0;
    return has;
}

/**
 * @brief Obtém o nó de inserção de um elemento
 * 
 * @param has Dicionário
 * @param indice Índice na tabela hash
 * @param obj Elemento
 * @param compChave Função para comparar a chave (deve retornar 0 se iguais)
 * @return NoHashing* de inserção ou NULL se erro ou essa chave ainda não existe
 */
NoHashing *posicaoInsercao(Dict *has, int indice, void *chave, int (*compChave)(void *chave, void *chave2)) {
    if (!has || !chave || !compChave || indice < 0 || indice >= TAMANHO_TABELA_HASH) return NULL;
    
    if (has->tabela[indice]) {
        NoHashing *p = has->tabela[indice];
    
        while(p) {
            if ((*compChave)(p->chave, chave) == 0)
                return p;
            p = p->prox;
        }
    }
    return NULL;
}

/**
 * @brief Obtém a lista do bucket associado a uma dada chave
 * 
 * @param has Dicionário
 * @param chave Chave
 * @param compChave Função para comparar chaves (deve retornar 0 se iguais)
 * @param hashChave Função para obter o hash da chave
 * @return Lista* Lista ou NULL se erro
 */
Lista *obterListaDoDict(Dict *has, void *chave, int (*compChave)(void *chave, void *chave2), int (*hashChave)(void *obj)) {
    if (!has || !chave || !compChave || !hashChave) return NULL;

    int indice = hashChave(chave);
    if (indice < 0) return NULL;

    indice %= TAMANHO_TABELA_HASH;

    NoHashing *p = posicaoInsercao(has, indice, chave, compChave);

    if (p) {
        return p->dados;
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
 * @param hashChave Função para obter o hash da chave
 * @param freeObj Função para libertar o objeto (pode ser NULL caso se queira ignorar)
 * @param freeChave Função para libertar a chave
 * @return int 0 se erro, 1 se sucesso
 */
int appendToDict(Dict *has, void *obj, int (*compChave)(void *chave, void *obj), void *(*criarChave)(void *obj), int (*hashChave)(void *obj), void (*freeObj)(void *obj), void (*freeChave)(void *chave)) {
    if (!has || !obj || !compChave || !criarChave || !hashChave || !freeChave) return 0;
    
    void *chave = criarChave(obj);
    if (!chave) return 0;

    int indice = hashChave(chave);
    if (indice < 0) return 0;
    
    indice %= TAMANHO_TABELA_HASH;

    NoHashing *p = posicaoInsercao(has, indice, chave, compChave);

    if (p) {
        freeChave(chave); // A chave já não é necessária
        return addInicioLista(p->dados, obj);
    }

    // Criar nova entrada
    NoHashing *novo = (NoHashing *)malloc(sizeof(NoHashing));
    if (!novo) return 0;
    
    novo->dados = criarLista();
    if (!novo->dados) {
        free(novo);
        return 0;
    }
    novo->chave = chave;
    
    if (!addInicioLista(novo->dados, obj)) {
        freeChave(novo->chave);
        freeLista(novo->dados, freeObj);
        free(novo);
        return 0;
    }
    novo->prox = NULL;
    
    novo->prox = has->tabela[indice];
    has->tabela[indice] = novo;
    has->nelDict++;
    
    return 1;
}

/**
 * @brief Mostra todos os elementos do dicionário
 * 
 * @param has Dicionário
 * @param printObj Função para mostrar cada elemento
 * 
 * @note Adiciona um espaço entre cada elemento
 */
void printDict(Dict *has, void (*printObj)(void *obj, FILE *file),FILE *file, int pausa) {
    if (!has || !printObj || !file || pausa < 0) return;

    int count = 0;
    listagemFlag = 0;
    printingDict = 1;

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = has->tabela[i];
        while (p) {
            printLista(p->dados, printObj, file, pausa);
            if (listagemFlag == 1) {
                listagemFlag = 0;
                return;
            }
            if (listagemFlag == -1) {
                int nel = 0;
                for (int j = TAMANHO_TABELA_HASH - 1; j > 0 && nel < pausa; j--) {
                    NoHashing *k = has->tabela[j];
                    while(k) {
                        if (k->dados) {
                            nel += k->dados->nel;
                        }
                        k = k->prox;
                    }
                    i = j;
                }
                i++; // avançar até ao próximo elemento, de modo a ficar com menos de pausa elementos
                p = has->tabela[i];
                // Ecnontrar p não nulo
                while(!p) {
                    i++;
                    p = has->tabela[i];
                }
            }
            p = p->prox;

            if (file == stdout && pausa) {
                count++;
                if (count % pausa == 0) {
                    printf("\n");
                    int opcao = enter_espaco_esc();
                    switch (opcao) {
                        case 0:
                            break;
                        case 1:
                            int nel = 0;
                            for (int j = TAMANHO_TABELA_HASH - 1; j > 0 && nel < pausa; j--) {
                                NoHashing *k = has->tabela[j];
                                while(k) {
                                    if (k->dados) {
                                        nel += k->dados->nel;
                                    }
                                    k = k->prox;
                                }
                                i = j;
                            }
                            i++; // avançar até ao próximo elemento, de modo a ficar com menos de pausa elementos
                            p = has->tabela[i];
                            // Encontrar p não nulo
                            while(!p) {
                                i++;
                                p = has->tabela[i];
                            }
                            break;
                        case 2:
                            return;
                        default:
                            break;
                    }
                }
            }

            if (file == stdout) printf("\n");
        }
    }
    printingDict = 0;
}

/**
 * @brief Exporta um dicionário para formato XML
 * 
 * @param has Dicionário
 * @param nomeDict Nome do Dict para colocar na tag principal (pode ser NULL)
 * @param printObj Função para mostrar cada obj em formato XML
 * @param indentacao Indentação no início
 * @param file Ficheiro .xml (ou .txt), aberto
 */
void exportarDictXML(Dict *has, char *nomeDict, void (*printObj)(void *obj, int indentacao, FILE *file), int indentacao, FILE *file) {
    if (!has || !printObj || indentacao < 0 || !file) return;

    if (nomeDict) {
        for (int i = 0; i < indentacao; i++) fprintf(file, "\t");
        fprintf(file, "<%s>\n", nomeDict);
    }

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = has->tabela[i];
        while(p) {
            if (nomeDict) exportarListaXML(p->dados, NULL, printObj, indentacao + 1, file);
            else exportarListaXML(p->dados, NULL, printObj, indentacao, file);
            p = p->prox;
        }
    }

    if (nomeDict) {
        for (int i = 0; i < indentacao; i++) fprintf(file, "\t");
        fprintf(file, "</%s>\n", nomeDict);
    }
}

/**
 * @brief Exporta um dicionário para formato CSV
 * 
 * @param has Dicionário a exportar
 * @param printObj Função para mostrar um elemento
 * @param file Ficheiro .csv, aberto
 */
void exportarDictCSV(Dict *has, void (*printHeader)(FILE *file), void (*printObj)(void *obj, FILE *file), FILE *file) {
    if (!has || !printObj || !file) return;

    printHeader(file);

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = has->tabela[i];
        while(p) {
            exportarListaCSV(p->dados, NULL, printObj, file);
            p = p->prox;
        }
    }
}

/**
 * @brief Exporta um dicionário para formato .html
 * 
 * @param has Dicionário
 * @param pagename Nome da página
 * @param printTableHead Função para mostrar o cabeçalho da tabela 
 * @param printObj Função para mostrar o objeto
 * @param file Ficheiro .html, aberto
 */
void exportarDictHTML(Dict *has, char *pagename, void (*printTableHead)(FILE *file), void  (*printObj)(void *obj, FILE *file), FILE *file) {
    if (!has || !printObj || !file) return;

    // Lista de caracteres HTML especiais que precisam ser substituídos
    if (!pagename) return;

    const char *invalidos = "<>\"'&";
    for (int i = 0; pagename[i]; i++) {
        if (strchr(invalidos, pagename[i]) != NULL) {
            pagename = NULL; // Nome da página contém caracteres HTML inválidos (usar default)
        }
    }

    fprintf(file,
        "<!DOCTYPE html>\n"
        "<html lang=\"pt\">\n"
        "<head>\n"
        "\t<meta charset=\"UTF-8\">\n"
        "\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
        "\t<title>%s</title>\n"
        "\t<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css\" rel=\"stylesheet\">\n"
        "</head>\n"
        "<body>\n"
        "\t<div class=\"container my-4\">\n"
        "\t\t<div class=\"table-responsive\">\n"
        "\t\t\t<table class=\"table table-bordered table-striped table-hover\">\n"
        "\t\t\t\t<thead class=\"table-dark\">\n", pagename ? pagename : "Exportação HTML");
    (*printTableHead)(file);
    fprintf(file, "\t\t\t\t<tbody>\n");

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = has->tabela[i];
        while(p) {
            No *n = p->dados->inicio;
            while(n) {
                printObj(n->info, file);
                n = n->prox;
            }
            p = p->prox;
        }
    }

    fprintf(file,   "\t\t\t\t</tbody>\n"
                    "\t\t\t</table>\n"
                    "\t\t</div>\n"
                    "\t</div>\n"
                    "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/js/bootstrap.bundle.min.js\"></script>\n"
                    "</body>\n"
                    "</html>\n");
}

/**
 * @brief Liberta a memória do dicionário e os seus elementos
 * 
 * @param has Dicionário
 * @param freeChave Função para libertar a memória da chave
 * @param freeObj Função para libertar a memória de cada elemento
 * 
 * @note freeObj pode ser passada como NULL para ignorar a libertação de cada elemento da lista
 */
void freeDict(Dict *has, void (*freeChave)(void *chave), void (*freeObj)(void *obj)) {
    if (!has || !freeChave) return;

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = has->tabela[i];
        while (p) {
            NoHashing *temp = p;
            p = p->prox;
            freeChave(temp->chave);
            freeLista(temp->dados, freeObj);
            free(temp);
        }
    }
    free(has);
}

/**
 * @brief Pesquisa pela chave principal de um dicionário
 * 
 * @param has Dicionário
 * @param chave Chave a procurar (Deve ser do tipo objeto a retornar com a chave no respetivo campo)
 * @param compChave Função para comparar as chaves do Dict
 * @param compCod Função para comparar a chave com o elemento da lista
 * @param hashChave Função para obter o hash da chave
 * @return void* ou NULL se erro
 */
void *searchDict(Dict *has, void *chave, int (*compChave)(void *chave, void *obj), int (*compCod)(void *codObj, void *chave), int (*hashChave)(void *chave)) {
    if (!has || !chave || !compChave|| !compCod || !hashChave) return NULL;

    int indice = hashChave(chave);
    if (indice < 0) return NULL;
    
    indice %= TAMANHO_TABELA_HASH;

    NoHashing *p = has->tabela[indice];
    while (p) {
        if (compChave(p->chave, chave) == 0)
            return searchLista(p->dados, compCod, chave);
        p = p->prox;
    }

    return NULL;
}

/**
 * @brief Guarda um dicionário em ficheiro binário
 * 
 * @param has Dicionário
 * @param guardarChave Função para guardar a chave
 * @param saveInfo Função para guardar a informação de cada elemento
 * @param file Ficheiro binário, aberto
 */
void guardarDadosDictBin(Dict *has, void (*saveInfo)(void *obj, FILE *fileObj), FILE *file) {
    if (!has || !file || !saveInfo) return;

    fwrite(&has->nelDict, sizeof(int), 1, file);

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = has->tabela[i];
        while (p) {
            guardarListaBin(p->dados, saveInfo, file);
            p = p->prox;
        }
    }
}

/**
 * @brief Lê os dados de um ficheiro binário para um dicionário
 * 
 * @param compChave Função para comparar a chave (deve retornar 0 se igual)
 * @param criarChave Função para criar a chave
 * @param hashChave Função para obter o hash da chave
 * @param freeObj Função para libertar a memória ocupada por um objeto
 * @param freeChave Função para libertar a memória ocupada por uma chave
 * @param readInfo Função para ler um elemento do ficheiro binário
 * @param file Ficheiro binário, aberto
 * @return Dict* Dicionário com os dados ou NULL se erro
 */
Dict *readToDictBin(void *(*criarChave)(void *obj), int (*hashChave)(void *obj), void (*freeObj)(void *obj),
                        void (*freeChave)(void *chave), void *(*readInfo)(FILE *fileObj), FILE *file) {
    if (!file || !readInfo || !freeChave || !freeObj || !hashChave || !criarChave) return NULL;

    Dict *has = criarDict();
    if (!has) return NULL;

    // NEL Dict
    fread(&has->nelDict, sizeof(int), 1, file);
    if (has->nelDict < 0) {
        free(has);
        return NULL;
    }

    Lista *lista = NULL;
    for (int i = 0; i < has->nelDict; i++) {
        // Ler a lista guardada
        lista = readListaBin(readInfo, file);
        if (!lista || !lista->inicio->info) {
            freeDict(has, freeChave, freeObj);
            return NULL;
        }

        // O primeiro elemento cria a chave e obtém o índice
        void *primeiroObj = lista->inicio->info;
        void *chave = criarChave(primeiroObj);
        int indice = hashChave(chave) % TAMANHO_TABELA_HASH;
        if (indice < 0) {
            freeChave(chave);
            if (lista->inicio->info) {
                freeLista(lista, freeObj);
            }
            freeDict(has, freeChave, freeObj);
            return NULL;
        }

        // Criar o nó e adicionar ao Dict
        NoHashing *novo = (NoHashing *)malloc(sizeof(NoHashing));
        if (!novo) {
            freeLista(lista, freeObj);
            freeChave(chave);
            freeDict(has, freeChave, freeObj);
            return NULL;
        }

        novo->chave = chave;
        novo->dados = lista;
        novo->prox = has->tabela[indice];
        has->tabela[indice] = novo;
    }

    return has;
}

/**
 * @brief Obtém a memória utilizada por um dicionário
 * 
 * @param has Dicionário
 * @param objMemUsage Função para obter a memória ocupada por cada elemento
 * @param chaveMemUsage Função para obter a memória ocupada pela chave
 * @return size_t Memória ocupada ou 0 se erro
 */
size_t dictMemUsage(Dict *has, size_t (*objMemUsage)(void *obj), size_t (*chaveMemUsage)(void *chave)) {
    if (!has) return 0;

    size_t mem = 0;
    mem += sizeof(*has); //ptr + nel

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = has->tabela[i];
        
        while (p) {
            mem += sizeof(*p); // NoHashing

            if (chaveMemUsage) {
                mem += chaveMemUsage(p->chave);
            }
            mem += listaMemUsage(p->dados, objMemUsage);

            p = p->prox;
        }
    }

    return mem;
}

/**
 * @brief Adiciona todos os elementos de um dicionário a uma lista
 * 
 * @param has Dicionário
 * @return Lista* Lista ou NULL se erro
 */
Lista *dictToLista(Dict *has) {
    if (!has) return NULL;

    Lista *li = criarLista();
    if (!li) return NULL;

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = has->tabela[i];

        while(p) {
            No *x = p->dados->inicio;

            while(x) {
                addInicioLista(li, x->info);

                x = x->prox;
            }
            p = p->prox;
        }
    }

    return li;
}

// Rankings

/**
 * @brief Cria um ranking
 * 
 * @return Ranking* 
 */
Ranking *criarRanking() {
    Ranking *r = (Ranking *)malloc(sizeof(Ranking));
    if (!r) return NULL;
    
    r->inicio = NULL;
    r->nel = 0;
    return r;
}

/**
 * @brief Adiciona um elemento e a sua informação ao início de um ranking
 * 
 * @param r Ranking
 * @param mainInfo Objeto
 * @param compInfo Informação do objeto (que será comparada para formar o ranking)
 * @return int 0 se erro, 1 se sucesso
 */
int addToRanking(Ranking *r, void *mainInfo, void *compInfo) {
    if (!r || !mainInfo || !compInfo) return 0;

    NoRankings *aux = (NoRankings *)malloc(sizeof(NoRankings));
    if (!aux) return 0;

    aux->mainInfo = mainInfo;
    aux->compInfo = compInfo;
    aux->prox = r->inicio;
    r->inicio = aux;
    r->nel++;

    return 1;
}

/**
 * @brief Mostra um ranking
 * 
 * @param r Ranking
 * @param printObj Função para mostrar o objeto e a informação
 * @param file Ficheiro
 * @param pausa Pausa da listagem
 */
void printRanking(Ranking *r, void printCarroRanking(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file), 
        void (*printHeaderCompObj)(FILE *file), void (*printCompObj)(void *compInfo, FILE *file), FILE *file, int pausa) {
    if (!r || !printCompObj || !printHeaderCompObj || !file || pausa < 0) return;

    int noStop = 1;
    int count = 0;
    int pole = 1;
    listagemFlag = 0;

    NoRankings *p = r->inicio;
    (*printHeaderCompObj)(file);
    while(p) {
        if (file == stdout) {
            fprintf(file, "%d. \t", count + 1);
        }
        else {
            fprintf(file, "%d", pole++);
        }
        (*printCarroRanking)(p, printCompObj, file);
        p = p->prox;

        if (file == stdout && pausa) {
            count++;
            if (count % pausa == 0 && count < r->nel && noStop) {
                printf("\n");
                int opcao = enter_espaco_esc();
                switch (opcao) {
                    case 0:
                        break;
                    case 1:
                        while(count <= r->nel - pausa) {
                            count++;
                            p = p->prox;
                        }
                        noStop = 0;
                        break;
                    case 2:
                        return;
                    default:
                        break;
                }
            }
        }

        if (file == stdout) printf("\n");
    }
}

/**
 * @brief Liberta a memória associada a um ranking
 * 
 * @param r Ranking
 * @param freeMainObj Função para libertar a memória do objeto
 * @param freeCompObj Função para libertar a memória do identificador do ranking
 */
void freeRanking(Ranking *r, void (*freeMainObj)(void *obj), void (*freeCompObj)(void *obj)) {
    if (!r) return;

    NoRankings *p = r->inicio;
    NoRankings *seg;

    while(p) {
        seg = p->prox;
        if (freeMainObj) {
            (*freeMainObj)(p->mainInfo);
        }
        if (freeCompObj) {
            (*freeCompObj)(p->compInfo);
        }
        free(p);
        p = seg;
    }
    free(r);
}

/**
 * @brief Combina 2 ranking ordenados em 1
 * 
 * @param a Nó do primeiro ranking
 * @param b Nó do segundo ranking
 * @param compObjs Função para comparar identificadores (deve retornar < 0 se obj1 < obj2)
 * @return NoRankings* Ranking agrupado
 */
static NoRankings *mergeRankings(NoRankings *a, NoRankings *b, int (*compObjs)(void *obj1, void *obj2)) {
    if (!a) return b;
    if (!b) return a;
    
    NoRankings *result = NULL;
    
    if (compObjs(a->compInfo, b->compInfo) <= 0) {
        result = a;
        result->prox = mergeRankings(a->prox, b, compObjs);
    } else {
        result = b;
        result->prox = mergeRankings(a, b->prox, compObjs);
    }
    
    return result;
}

/**
 * @brief Separa 2 rankings
 * 
 * @param head Ranking a separar
 * @param front Ponteiro para a primeira metade
 * @param back Ponteiro para a segunda metade
 */
static void splitRanking(NoRankings *head, NoRankings **front, NoRankings **back) {
    NoRankings *rapido = head->prox;
    NoRankings *lento = head;
    
    while (rapido != NULL) {
        rapido = rapido->prox;
        if (rapido != NULL) {
            lento = lento->prox;
            rapido = rapido->prox;
        }
    }
    
    *front = head;
    *back = lento->prox;
    lento->prox = NULL;
}

/**
 * @brief Mergesort recursivo para os nós da função
 * 
 * @param headRef Head da função
 * @param compObjs Função para comparar identificadores
 */
static void mergesortRecursivoRanking(NoRankings **headRef, int (*compObjs)(void *obj1, void *obj2)) {
    NoRankings *head = *headRef;
    if (!head || !head->prox) return;
    
    NoRankings *a;
    NoRankings *b;
    
    splitRanking(head, &a, &b);
    mergesortRecursivoRanking(&a, compObjs);
    mergesortRecursivoRanking(&b, compObjs);
    
    *headRef = mergeRankings(a, b, compObjs);
}

/**
 * @brief Ordena os rankings
 * 
 * @param r Ranking a ordenar
 * @param compObjs Função para comparar identificadores (ordem de ordenação)
 */
void mergeSortRanking(Ranking* r, int (*compObjs)(void *obj1, void *obj2)) {
    if (!r || !r->inicio || !compObjs) return;

    mergesortRecursivoRanking(&r->inicio, compObjs);
}
 
