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
    
    if (!appendToDict(bd->donosNif, (void *)dono, compChaveDonoNif, criarChaveDonoNif)) {
        free(dono->nome);
        free(dono);
        return 0;
    }
    if (!appendToDict(bd->donosAlfabeticamente, (void *)dono, compChaveDonoAlfabeticamente, criarChaveDonoAlfabeticamente)) {
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
 * @brief Ordena 
 * 
 * @param dono1 
 * @param dono2 
 * @return int 
 */
/* ERRO AQUI
int compDonosNome(void *dono1, void *dono2){
    if (!dono1 || !dono2) return 0;
    Dono *n1 = (Dono*) dono1;
    Dono *n2 = (Dono*) dono2;
    normalizar_string(n1->nome);
    normalizar_string(n2->nome);
    return strcmp((n1->nome), (n2->nome));
}
    */

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

    *chaveNif = x->nif % HASH_DONOS_NIF;
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
int compChaveDonoNif(void *chave, void *dono) {
    if (!chave || !dono) return -1;

    int *key = (int *)chave;
    Dono *x = (Dono *)dono;

    if (*key == (x->nif % HASH_DONOS_NIF)) return 0;
    return 1;
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
 * @param chave Chave
 * @param dono Dono
 * @return int -1 se erro, 0 se iguais, 1 se diferentes
 */
int compChaveDonoAlfabeticamente(void *chave, void *dono) {
    if (!chave || !dono) return -1;

    char *key = (char *)chave;
    Dono *x = (Dono *)dono;

    if (*key == tolower(x->nome[0])) return 0;
    return 1;
} 

