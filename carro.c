#include "carro.h"
#include "validacoes.h"
#include "bdados.h"
#include "uteis.h"
#include "structsGenericas.h"

/**
 * @brief Inserir um carro na base de dados
 * 
 * @param bd Base de dados
 * @param matricula Matrícula
 * @param marca Marca
 * @param modelo Modelo
 * @param ano Ano
 * @param nif Nif
 * @param codVeiculo Código do veículo
 * @return int 0 se erro, 1 se sucesso
 * 
 * @note Não faz validações
 */
int inserirCarroLido(Bdados *bd, char *matricula, char *marca, char *modelo, short ano, int nif, int codVeiculo) {
    if (!bd) return 0;
    
    Carro *aut = (Carro *)malloc(sizeof(Carro));
    if (!aut) return 0;
    
    //Matrícula
    strcpy(aut->matricula, matricula);
    //Marca
    aut->marca = (char *)malloc(strlen(marca) * sizeof(char) + 1);
    if (!aut->marca) {
        free(aut);
        return 0;
    }
    strcpy(aut->marca, marca);
    //Modelo
    aut->modelo = (char *)malloc(strlen(modelo) * sizeof(char) + 1);
    if (!aut->modelo) {
        free(aut->marca);
        free(aut);
        return 0;
    }
    strcpy(aut->modelo, modelo);
    //Ano
    aut->ano = ano;
    //NIF (ptrPessoa)
    void *temp = (void *)&nif;
    aut->ptrPessoa = (Dono *)searchDict(bd->donosNif, temp, compChaveDonoNif ,compCodDono, hashChaveCarroCod);
    //Código Veículo
    aut->codVeiculo = codVeiculo;

    if (!appendToDict(bd->carrosCod, (void *)aut, compChaveCarroCod, criarChaveCarroCod, hashChaveCarroCod, freeCarro, freeChaveCarroCod)) {
        free(aut->modelo);
        free(aut->marca);
        free(aut);
        return 0;
    }

    if (!appendToDict(bd->carrosMarca, (void *)aut, compChaveCarroMarca, criarChaveCarroMarca, hashChaveCarroMarca, freeCarro, freeChaveCarroMarca)) {
        free(aut->modelo);
        free(aut->marca);
        free(aut);
        return 0;
    }
    
    return 1;
}

/**
 * @brief Compara 2 carros
 * 
 * @param carro1 
 * @param carro2 
 * @return int -1 se carro1 < carro2, 0 se iguais, 1 se carro1 > carro2
 */
int compararCarros(void *carro1, void *carro2) {
    if (carro1 == NULL && carro2 == NULL) return 0;
    if (carro1 == NULL) return -1; //NULL < qualquer coisa
    if (carro2 == NULL) return 1;

    Carro *x = (Carro *)carro1;
    Carro *y = (Carro *)carro2;

    if (x->codVeiculo > y->codVeiculo) return 1;
    if (x->codVeiculo < y->codVeiculo) return -1;
    return 0;
}

/**
 * @brief Compara o código de um veículo com um código
 * 
 * @param carro Carro
 * @param codigo Código (será convertido para int*)
 * @return int 0 se igual
 */
int compCodCarro(void *carro, void *codigo) {
    if (!carro || !codigo) return 1;

    Carro *x = (Carro *)carro;
    int *cod = (int *)codigo;
    if (x->codVeiculo == *cod) return 0;
    return 1;
}

/**
 * @brief Liberta a memória associada a um Carro
 * 
 * @param carro Carro
 */
void freeCarro(void *carro) {
    Carro *obj = (Carro *)carro;
    if (obj->marca) free(obj->marca);
    if (obj->modelo) free(obj->modelo);
    free(obj);
}


void printCarro(void *carro) {
    if (!carro) return;

    Carro *x = (Carro*)carro;
    printf("Código do Veículo: %d\n", x->codVeiculo);
    printf("Matrícula: %s\n", x->matricula);
    printf("Marca: %s\n", x->marca);
    printf("Modelo: %s\n", x->modelo);
    printf("Ano do Veículo: %d\n", x->ano);
    printf("Nome do Dono: %s\n", x->ptrPessoa->nome);
    printf("NIF do Dono: %d\n\n", x->ptrPessoa->nif);
}

/**
 * @brief Guarda um elemento do tipo Carro num ficheiro binário
 * 
 * @param carro Carro
 * @param file Ficheiro binário
 */
void guardarCarroBin(void *carro, FILE *file) {
    if (!carro || !file) return;

    Carro *x = (Carro *)carro;
    fwrite(&x->ano, sizeof(short), 1, file);
    fwrite(&x->codVeiculo, sizeof(int), 1, file);
    fwrite(x->matricula, sizeof(x->matricula), 1, file);

    size_t tamanhoMarca = strlen(x->marca) + 1;
    fwrite(&tamanhoMarca, sizeof(size_t), 1, file);
    fwrite(x->marca, tamanhoMarca, 1, file);

    size_t tamanhoModelo = strlen(x->modelo) + 1;
    fwrite(&tamanhoModelo, sizeof(size_t), 1, file);
    fwrite(x->modelo, tamanhoModelo, 1, file);

    fwrite(&x->ptrPessoa->nif, sizeof(int), 1, file);
}

/**
 * @brief Lê um carro para memória
 * 
 * @param file Ficheiro binário, aberto
 * @return void* Carro ou NULL se erro
 * 
 * @note Cria uma estrutura Dono para guardar o Nif, deve ser posteriormente eliminada e obtido o respetivo ponteiro
 */
void *readCarroBin(FILE *file) {
    if (!file) return NULL;

    Carro *x = (Carro *)malloc(sizeof(Carro));
    if (!x) return NULL;

    fread(&x->ano, sizeof(short), 1, file);
    fread(&x->codVeiculo, sizeof(int), 1, file);
    fread(x->matricula, sizeof(x->matricula), 1, file);

    size_t tamanhoMarca;
    fread(&tamanhoMarca, sizeof(size_t), 1, file);
    x->marca = (char *)malloc(tamanhoMarca);
    if (!x->marca) {
        free(x);
        return NULL;
    }
    fread(x->marca, tamanhoMarca, 1, file);

    size_t tamanhoModelo;
    fread(&tamanhoModelo, sizeof(size_t), 1, file);
    x->modelo = (char *)malloc(tamanhoModelo);
    if (!x->modelo) {
        free(x->marca);
        free(x);
        return NULL;
    }
    fread(x->modelo, tamanhoModelo, 1, file);

    Dono *d = (Dono *)malloc(sizeof(Dono));
    if (!d) {
        free(x->marca);
        free(x->modelo);
        free(x);
        return NULL;
    }
    fread(&d->nif, sizeof(int), 1, file);
    x->ptrPessoa = d;
    return (void *)x;
}

// Chave por Marca

/**
 * @brief Guarda a chave do carro por marca em ficheiro binário
 * 
 * @param chaveMarca Chave
 * @param file Ficheiro binário, aberto
 */
void guardarChaveCarroMarca(void *chaveMarca, FILE *file) {
    if (!chaveMarca || !file) return;

    char *chave = (char *)chaveMarca;

    size_t comp = strlen(chave) + 1;
    fwrite(&comp, sizeof(size_t), 1, file);
    fwrite(chave, comp, 1, file);
}

/**
 * @brief Cria uma chave para o carro consoante a marca
 * 
 * @param carro Carro
 * @return void* Chave(tipo void) ou NULL se erro
 * 
 * @note Coloca a chave em maiúscula
 */
void *criarChaveCarroMarca(void *carro) {
    if (!carro) return NULL;

    Carro *x = (Carro *)carro;

    return (void *)strlwrSafe(x->marca);
}

/**
 * @brief Função de hash para o carro por marca
 * 
 * @param chave Chave (marca)
 * @return int hash ou -1 se erro
 */
int hashChaveCarroMarca(void *chave) {
    if (!chave) return -1;

    char *key = (char *)chave;
    char *marcaNorm = normString(key);
    int hash = hashString(marcaNorm);
    free(marcaNorm); // Liberta a cópia criada por normString
    return hash;
}

/**
 * @brief Liberta a memória da chave dos carros por marca
 * 
 * @param chave 
 */
void freeChaveCarroMarca(void *chave) {
    if (!chave) return;

    char *key = (char *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos carros pela marca
 * 
 * @param chave Chave
 * @param carro Carro
 * @return int -1 se erro, 0 se iguais, 1 se diferente
 */
int compChaveCarroMarca(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    char *key = (char *)chave;
    char *key2 = (char *)chave2;

    if (strcmp(key, key2) == 0) {
        return 0;
    }
    return 1;
} 

// Chave por Código

/**
 * @brief Guarda a chave dos carros por código em ficheiro binário
 * 
 * @param chave Chave
 * @param file Ficheiro binário
 */
void guardarChaveCarroCod(void *chave, FILE *file) {
    if (!chave || !file) return;

    int *key = (int *)chave;

    fwrite(key, sizeof(int), 1, file);
}

/**
 * @brief Lê a chave do carro por código
 * 
 * @param file Ficheiro binário, aberto
 * @return void* chave ou NULL se erro
 */
void *readChaveCarroCod(FILE *file) {
    if (!file) return NULL;

    int *chave = (int *)malloc(sizeof(int));
    if (!chave) return NULL;
    fread(chave, sizeof(int), 1, file);
    
    return (void *)chave;
}

/**
 * @brief Cria uma chave para um carro pelo código
 * 
 * @param carro Carro
 * @return void* chave ou NULL se erro
 */
void *criarChaveCarroCod(void *carro) {
    if (!carro) return NULL;

    Carro *x = (Carro *)carro;
    int *chave = (int *)malloc(sizeof(int));
    *chave = x->codVeiculo;

    return (void *)chave;
}

/**
 * @brief Função de hash para carro por Cod
 * 
 * @param carro Carro
 * @return int -1 se erro ou codVeiculo
 */
int hashChaveCarroCod(void *chave) {
    if (!chave) return -1;

    int *key = (int *)chave;
    return *key;
}

/**
 * @brief Liberta a memória alocada para a chave dos carros por código
 * 
 * @param chave Chave
 */
void freeChaveCarroCod(void *chave) {
    if (!chave) return;

    int *key = (int *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos carros pelo código
 * 
 * @param chave Chave
 * @param carro Carro
 * @return int 
 */
int compChaveCarroCod(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    int *key = (int *)chave;
    int *key2 = (int *)chave2;

    if (*key == *key2) return 0;
    return 1;
}

/**
 * @brief Compara os carros pela marca
 * 
 * @param carro1 Carro 1
 * @param carro2 Carro 2
 * @return int -1 se carro1 < carro2, 0 se iguais, 1 se carro1 > carro2
 */
int compCarroMarca (void *carro1, void *carro2) {
    if (!carro1 || !carro2) return 0;
    if (!carro1) return -1;
    if (!carro2) return 1;

    Carro *x = (Carro*)carro1;
    Carro *y = (Carro*)carro2;
    char *xNorm = normString(x->marca);
    char *yNorm = normString(y->marca);

    int res = strcmp(xNorm, yNorm);

    free(xNorm);
    free(yNorm);

    return res;
}

/**
 * @brief Mostra um carro em formato XML
 * 
 * @param carro Carro a mostrar
 * @param indentacao Indentação a usar no início
 * @param file Ficheiro to tipo .xml (ou .txt), aberto
 */
void printCarroXML(void *carro, int indentacao, FILE *file) {
    if (!carro || indentacao < 0 || !file) return;

    Carro *c = (Carro *)carro;
    
    indent(indentacao, file);
	fprintf(file, "<carro>\n");
	
    indent(indentacao + 1, file);
    fprintf(file, "<codVeiculo>%d</codVeiculo>\n", c->codVeiculo);
    indent(indentacao + 1, file);
    fprintf(file, "<matricula>%s</matricula>\n", c->matricula);
    indent(indentacao + 1, file);
    fprintf(file, "<ano>%d</ano>\n", c->ano);
    indent(indentacao + 1, file);
    fprintf(file, "<marca>%s</marca>\n", c->marca ? c->marca : "n/a");
    indent(indentacao + 1, file);
    fprintf(file, "<modelo>%s</modelo>\n", c->modelo ? c->modelo : "n/a");
    indent(indentacao + 1, file);
    if (c->ptrPessoa) fprintf(file, "<nifDono>%d</nifDono>\n", c->ptrPessoa->nif);
    else fprintf(file, "<nifDono>%s</nifDono>\n", "n/a");
	
	indent(indentacao, file);
	fprintf(file, "</carro>\n");
}

/**
 * @brief Escreve os headers dos carros em formato CSV
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderCarrosCSV(FILE *file) {
    if (!file) return;

    fprintf(file, "Código do veículo, Ano, Matrícula, Marca, Modelo, Nif Dono\n");
}

/**
 * @brief Mostra um carro em formato CSV
 * 
 * @param carro Carro a mostrar
 * @param file Ficheiro .csv, aberto
 */
void printCarroCSV(void *carro, FILE *file) {
    if (!carro || !file) return;

    Carro *c = (Carro *)carro;

    fprintf(file, "%d, %hd, %s, %s, %s, %d\n", c->codVeiculo, c->ano, c->matricula, c->marca ? c->marca : "n/a", 
        c->modelo ? c->modelo : "n/a", c->ptrPessoa ? c->ptrPessoa->nif : -1);
}

/**
 * @brief Obtém a marca mais comum de um dicionário de carros ordenados em listas de marcas
 * 
 * @param carrosMarca Dicionário
 * @return char* Marca mais comum ou NULL se erro
 */
char *obterMarcaMaisComum(Dict *carrosMarca) {
    if (!carrosMarca) return NULL;

    Lista *maisElementos = NULL;
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = carrosMarca->tabela[i];
        if (p && p->dados) {
            if (!maisElementos) {
                maisElementos = p->dados;
            }
            else if (p->dados->nel > maisElementos->nel) {
                maisElementos = p->dados;
            }   
        }
    }

    if (!maisElementos || !maisElementos->inicio) return NULL;

    Carro *c = (Carro *)maisElementos->inicio->info;
    if (!c) return NULL;

    return c->marca;
}



void RegistarVeiculo(Bdados *bd){
    do{
        int codVeiculo, nif;
        char *matricula;
        char *marca;
        char *modelo;
        short ano;
    
        printf ("\n\tIntroduza as Informações:\n");
        do{
            printf ("\nCódigo do Veículo: ");
            scanf("%d", &codVeiculo);
        }while (validarCodVeiculo(codVeiculo) == 0);
        
        do{
            printf ("\nMatrícula:");
            matricula = lerLinhaTxt (stdin, NULL);
        }while (validarMatricula (matricula) == 0);
        
        do{
            printf ("\nMarca:");
            marca = lerLinhaTxt (stdin, NULL);
        }while (!validarMarca (marca));
    
        do {
            printf ("\nModelo:");
            modelo = lerLinhaTxt (stdin, NULL);
        }while (!validarMarca (modelo));
    
        do{
            printf ("\nAno do Veículo:");
            scanf ("%hd", &ano);
        }while (validarAnoCarro(ano) == 0);
        
        do{
            printf ("\nNIF do Dono:");
            scanf ("%d", &nif);
        }while (validarNif(nif) == 0);
    
        inserirCarroLido(bd, matricula, marca, modelo, ano, nif, codVeiculo);  
        
        free (matricula);
        free (marca);
        free (modelo);
        
        if (sim_nao("\nQuer introduzir outro elemento?") == 0) return;

    } while(1);
}

/* ERRO tolower
int ordenarAlfModelo (void *carro1, void *carro2){
    if (!carro1 || !carro2) return 0;
    Carro *x1 = (Carro*) carro1;
    Carro *x2 = (Carro*) carro2;
    if (strcmp(tolower(x1->modelo),tolower(x2->modelo)) == 1) return 1;
    return 0;
}
    */

int ordenarAlfMatricula (void *carro1, void *carro2) {
    if (!carro1 || !carro2) return 0;
    Carro *x1 = (Carro*) carro1;
    Carro *x2 = (Carro*) carro2;
    if (strcmp(x1->matricula, x2->matricula) == 1) return 1;
    return 0;
}

void listarVeiculos(Bdados *bd) {
    return;
}

size_t memoriaOcupadaCarros(void *carro){
    if (!carro) return 0;
    Carro *aux = (Carro*) carro; 
}