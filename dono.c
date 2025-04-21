#include "dono.h"
#include "structsGenericas.h"
#include "constantes.h"
#include "bdados.h"

/**
 * @brief Introduz o dono na base de dados
 * 
 * @param bd Base de Dados
 * @param nome Nome do dono
 * @param nif NIF do dono
 * @param codigoPostal Cópigo postal do dono
 * @return int 1 se sucesso, 0 se erro
 */
int inserirDonoLido(Bdados *bd, char *nome, int nif, CodPostal codigoPostal) {
    if (!bd) return 0;

    Dono *dono = (Dono *)malloc(sizeof(Dono));
    if (!dono) return 0;
    //nif
    dono->nif = nif;
    //Nome
    dono->nome = (char *)malloc(strlen(nome) * sizeof(char) + 1);
    if (!dono->nome) {
        free(dono);
        return 0;
    }
    strcpy(dono->nome, nome);
    //Codigo Postal
    dono->codigoPostal.local = codigoPostal.local;
    dono->codigoPostal.zona = codigoPostal.zona;
    
    if (!appendToDict(bd->donosNif, (void *)dono, compChaveDonoNif, criarChaveDonoNif, hashChaveDonoNif, freeDono, freeChaveDonoNif)) {
        free(dono->nome);
        free(dono);
        return 0;
    }
    if (!appendToDict(bd->donosAlfabeticamente, (void *)dono, compChaveDonoAlfabeticamente, criarChaveDonoAlfabeticamente, hashChaveDonoAlfabeticamente, freeDono, freeChaveDonoAlfabeticamente)) {
        free(dono->nome);
        free(dono);
        return 0;
    }
        
    return 1;
}

/**
 * @brief Comparara donos pelo NIF
 * 
 * @param dono1 
 * @param dono2 
 * @return int -1 se dono1 < dono2, 0 se iguais, 1 se dono1 > dono2
 */
int compDonosNif(void *dono1, void *dono2) {
    if (dono1 == NULL && dono2 == NULL) return 0;
    if (dono1 == NULL) return -1; //NULL < qualquer coisa
    if (dono2 == NULL) return 1;

    Dono *x = (Dono *)dono1;
    Dono *y = (Dono *)dono2;

    if (x->nif > y->nif) return 1;
    if (y->nif < x->nif) return -1;
    return 0;
}

/**
 * @brief Compara os donos 
 * 
 * @param dono1 Dono 1
 * @param dono2 Dono 2
 * @return int -1 se dono1 < dono 2, 0 se iguais, 1 se dono1 > dono2
 */
int compDonosNome(void *dono1, void *dono2) { 
    if (dono1 == NULL && dono2 == NULL) return 0;
    if (dono1 == NULL) return -1; //NULL < qualquer coisa
    if (dono2 == NULL) return 1;

    Dono *x = (Dono *)dono1;
    Dono *y = (Dono *)dono2;
    char *xNorm = normString(x->nome);
    char *yNorm = normString(y->nome);

    int res = strcmp(xNorm, yNorm);

    free(xNorm);
    free(yNorm);
    
    return res;
}

/**
 * @brief Compara um dono(Nif) com um código
 * 
 * @param dono Dono
 * @param codigo Código
 * @return int 
 */
int compCodDono(void *dono, void *codigo) {
    if (!dono || !codigo) return 1;

    Dono *x = (Dono *)dono;
    int *cod = (int *)codigo;
    if (x->nif == *cod) return 0;
    return 1;
}

/**
 * @brief Liberta a memória de um dono
 * 
 * @param dono Dono
 */
void freeDono(void *dono) {
    Dono *obj = (Dono *)dono;
    if (obj->nome) free(obj->nome);
    free(dono);
}

/**
 * @brief Mostra um dono
 * 
 * @param dono Dono
 */
void printDono(void *dono){
    if (!dono) return;
    Dono  *x = (Dono*) dono;
    printf ("\nNome: %s\n", x->nome);
    printf ("NIF: %d\n", x->nif);
    printf ("Código Postal: %hd-%hd\n", x->codigoPostal.local, x->codigoPostal.zona);
}

/**
 * @brief Guarda um elemento do tipo Dono em ficheiro binário
 * 
 * @param obj Dono
 * @param file Ficheiro binário, aberto
 */
void guardarDonoBin(void *obj, FILE *file) {
    if (!obj || !file) return;

    Dono *x = (Dono *)obj;
    fwrite(&x->codigoPostal, sizeof(CodPostal), 1, file);
    fwrite(&x->nif, sizeof(int), 1, file);

    size_t tamanhoNome = strlen(x->nome) + 1;
    fwrite(&tamanhoNome, sizeof(size_t), 1, file);
    fwrite(x->nome, tamanhoNome, 1, file);
}

/**
 * @brief Lê um dono de um ficheiro binário
 * 
 * @param file Ficheiro binário, aberto
 * @return void* Dono ou NULL se erro
 */
void *readDonoBin(FILE *file) {
    if (!file) return NULL;

    Dono *x = (Dono *)malloc(sizeof(Dono));
    if (!x) return NULL;
    
    fread(&x->codigoPostal, sizeof(CodPostal), 1, file);
    fread(&x->nif, sizeof(int), 1, file);

    size_t tamanho;
    fread(&tamanho, sizeof(size_t), 1, file);

    x->nome = (char *)malloc(tamanho);
    fread(x->nome, tamanho, 1, file);
    return (void *)x;
}

/*
void guardarDonoDupBin(void *obj, FILE *file) {
    if (!obj || !file) return;

    Dono *x = (Dono *)obj;
    fwrite(&x->nif, sizeof(int), 1, file);
}
*/

/**
 * @brief Guarda a chave do dono por Nif em ficheiro binário
 * 
 * @param chaveNif Chave(NIF)
 * @param file Ficheiro binário, aberto
 */
void guardarChaveDonoNif(void *chaveNif, FILE *file) {
    if (!chaveNif || !file) return;

    int *chave = (int *)chaveNif;
    fwrite(chave, sizeof(int), 1, file);
}

/**
 * @brief Lê e retorna a chave por Nif
 * 
 * @param file Ficheiro binário, aberto
 * @return void* chave ou NULL se erro
 */
void *readChaveDonoNif(FILE *file) {
    if (!file) return NULL;

    int *chave = (int *)malloc(sizeof(int));
    if (!chave) return NULL;

    fread(chave, sizeof(int), 1, file);
    return chave;
}

/**
 * @brief Cria uma chave para Donos por Nif
 * 
 * @param dono Dono
 * @return void* Chave ou NULL se erro
 */
void *criarChaveDonoNif(void *dono) {
    if (!dono) return NULL;

    Dono *x = (Dono *)dono;
    int *chaveNif = (int *)malloc(sizeof(int));
    if (!chaveNif) return NULL;

    *chaveNif = x->nif;
    return (void *)chaveNif;
}

/**
 * @brief Liberta a memória ocupada pela chave dos donos por Nif
 * 
 * @param chave Chave
 */
void freeChaveDonoNif(void *chave) {
    if (!chave) return;

    int *key = (int *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos donos por Nif
 * 
 * @param chave Chave
 * @param dono Dono
 * @return int 0 se igual, 1 se diferente 
 */
int compChaveDonoNif(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    int *key = (int *)chave;
    int *key2 = (int *)chave2;

    if (*key == *key2) return 0;
    return 1;
} 

/**
 * @brief Função de hash para chave do tipo NIF 
 * 
 * @param chave Dono
 * @return int Nif do dono ou -1 se erro
 */
int hashChaveDonoNif(void *chave) {
    if (!chave) return -1;

    int *key = (int *)chave;

    return *key;
}

/**
 * @brief Cria uma chave para donos alfabeticamente
 * 
 * @param dono Dono
 * @return void* Chave ou NULL se erro
 */
void *criarChaveDonoAlfabeticamente(void *dono) {
    if (!dono) return NULL;

    Dono *x = (Dono *)dono;
    char *chaveAlf = malloc(sizeof(char));
    if (!chaveAlf) return NULL;

    *chaveAlf = tolower(x->nome[0]);
    return (void *)chaveAlf;
}

/**
 * @brief Liberta a memória da chave dos donos por ordem alfabética
 * 
 * @param chave Chave
 */
void freeChaveDonoAlfabeticamente(void *chave) {
    if (!chave) return;

    char *key = (char *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos donos alfabeticamente
 * 
 * @param chave Chave 1
 * @param chave2 Chave 2
 * @return int -1 se erro, 0 se iguais, 1 se diferentes
 */
int compChaveDonoAlfabeticamente(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    char *key = (char *)chave;
    char *key2 = (char *)chave2;

    if (*key == *key2) return 0;
    return 1;
} 

/**
 * @brief Guarda a chave do dict dos donos por ordem alfabética
 * 
 * @param chave Chave
 * @param file Ficheiro binário
 */
void guardarChaveDonoAlfabeticamente(void *chave, FILE *file) {
    if (!chave || !file) return;

    char *key = (char *)chave;
    size_t comp = strlen(key) + 1;
    fwrite(&comp, sizeof(size_t), 1, file);
    fwrite(key, comp, 1, file);
}

/**
 * @brief Função de hash para o dono por ordem alfabética
 * 
 * @param chave 
 * @return int 
 */
int hashChaveDonoAlfabeticamente(void *chave) {
    if (!chave) return -1;

    char *letra = (char *)chave;
    
    return tolower(*letra) - 'a';
}

/**
 * @brief Mostra um dono em formato XML
 * 
 * @param dono Dono a mostrar
 * @param indentacao Indentação no início
 * @param file Ficheiro .xml (ou .txt), aberto
 */
void printDonoXML(void *dono, int indentacao, FILE *file) {
    if (!dono || indentacao < 0 || !file) return;

    Dono *d = (Dono *)dono;

    indent(indentacao, file);
	fprintf(file, "<dono>\n");
	
    indent(indentacao + 1, file);
    fprintf(file, "<nif>%d</nif>\n", d->nif);
    indent(indentacao + 1, file);
    fprintf(file, "<codPostal>%hd-%hd</codPostal>\n", d->codigoPostal.zona, d->codigoPostal.local);
    indent(indentacao + 1, file);
    fprintf(file, "<nome>%s</nome>\n", d->nome);
	
	indent(indentacao, file);
	fprintf(file, "</dono>\n");
}

