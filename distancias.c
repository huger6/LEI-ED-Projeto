#include "distancias.h"
#include "bdados.h"
#include "passagens.h"

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
    if (!bd || !bd->matrizDistancias) return 0;

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
    float *matriz = (float *)realloc(bd->matrizDistancias->distancias, tamanho*tamanho);
    if (!matriz) return 0;

    bd->distancias->matriz = matriz;
    bd->distancias->nColunas = tamanho;
    return 1;
}

void getStatsViagem(Bdados *bd, Viagem *v) {
    v->kms = bd->distancias->matriz[v->entrada->idSensor * bd->distancias->nColunas + v->saida->idSensor];
    v->tempo = calcularIntervaloTempo(&v->entrada->data, &v->saida->data); //min
}