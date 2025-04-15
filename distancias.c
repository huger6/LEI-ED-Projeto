#include "distancias.h"
#include "bdados.h"

/**
 * @brief Insere a distância entre os dois sensores na matriz
 * 
 * @param bd Ponteiro para a base de dados
 * @param codSensor1 Sensor 1
 * @param codSensor2 Sensor 2
 * @param distancia Distância entre os sensores
 * @return int 0 se erro, 1 se sucesso
 */
int inserirDistanciaLido(Bdados *bd, int codSensor1, int codSensor2, float distancia) {
    if (!bd || !bd->distancias) return 0;

    bd->distancias->matriz[codSensor1 * bd->distancias->nColunas + codSensor2] = distancia;
    return 1;
}

/**
 * @brief Aloca memória para 1 elemento da matriz das distâncias
 * 
 * @param bd Ponteiro para a base de dados
 */
void inicializarMatrizDistancias(Bdados *bd) {
    bd->distancias = (Distancias *)malloc(sizeof(Distancias));
    if (!bd->distancias) return;

    bd->distancias->matriz = (float *)malloc(sizeof(float));
    if (!bd->distancias->matriz) return;
    bd->distancias->nColunas = 1;
}

/**
 * @brief Realoca a matriz das distâncias para o tamanho pretendido
 * 
 * @param bd Ponteiro para a base de dados
 * @param tamanho Tamanho a (re)alocar para a matriz
 * @return int 0 se erro, 1 se sucesso
 * 
 * @note O tamanho anterior não é tido em conta
 */
int realocarMatrizDistancias(Bdados *bd, int tamanho) {
    float *matriz = (float *)realloc(bd->distancias->matriz, tamanho*tamanho);
    if (!matriz) return 0;

    bd->distancias->matriz = matriz;
    bd->distancias->nColunas = tamanho;
    return 1;
}

/**
 * @brief Liberta toda a memória associada às distâncias
 * 
 * @param distancia Distâncias (na base de dados)
 */
void freeMatrizDistancias(Distancias *distancia) {
    if (!distancia) return;

    free(distancia->matriz);
    free(distancia);
}

/**
 * @brief Guarda as distâncias num ficheiro binário
 * 
 * @param distancia Distâncias
 * @param file Ficheiro binário, aberto
 */
void guardarDistanciasBin(Distancias *distancia, FILE *file) {
    fwrite(&distancia->nColunas, sizeof(int), 1, file);
    fwrite(distancia->matriz, sizeof(float), distancia->nColunas * distancia->nColunas, file);
}   

/**
 * @brief Lê as distâncias para memória
 * 
 * @param file Ficheiro binário, aberto
 * @return Distancias* Distâncias ou NULL se erro
 */
Distancias *readDistanciasBin(FILE *file) {
    Distancias *d = (Distancias *)malloc(sizeof(Distancias));
    d->matriz = NULL;
    fread(&d->nColunas, sizeof(int), 1, file);
    if (d->nColunas < 0) {
        free(d);
        return NULL;
    }

    float *matriz = (float *)malloc(d->nColunas * d->nColunas);
    if (!matriz) {
        free(d);
        return NULL;
    }
    fread(matriz, d->nColunas, 1, file);
    d->matriz = matriz;
    return d;
}

/**
 * @brief Obter e colocar na estrutura de Viagem os dados estatísticos da mesma
 * 
 * @param bd Base de dados
 * @param v Viagem 
 */
void getStatsViagem(Bdados *bd, Viagem *v) {
    v->kms = bd->distancias->matriz[v->entrada->idSensor * bd->distancias->nColunas + v->saida->idSensor];
    v->tempo = calcularIntervaloTempo(&v->entrada->data, &v->saida->data); //min
}