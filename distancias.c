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

    //-1 para ter em conta o índice
    bd->distancias->matriz[(codSensor1-1) * bd->distancias->nColunas + codSensor2-1] = distancia;
    bd->distancias->matriz[(codSensor2-1) * bd->distancias->nColunas + codSensor1-1] = distancia;
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
int realocarMatrizDistancias(Bdados *bd, int nColunas) {
    float *matriz = (float *)realloc(bd->distancias->matriz, nColunas * nColunas * sizeof(float));
    if (!matriz) return 0;

    bd->distancias->matriz = matriz;
    bd->distancias->nColunas = nColunas;
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
    if (!d) return NULL;
    d->matriz = NULL;
    fread(&d->nColunas, sizeof(int), 1, file);
    if (d->nColunas < 0) {
        free(d);
        return NULL;
    }

    float *matriz = (float *)malloc(d->nColunas * d->nColunas * sizeof(float));
    if (!matriz) {
        free(d);
        return NULL;
    }
    fread(matriz, sizeof(float),d->nColunas * d->nColunas, file);
    d->matriz = matriz;
    return d;
}

/**
 * @brief Exporta os dados das distâncias para um ficheiro XML
 * 
 * @param d Distâncias
 * @param indentacao Indentação no início
 * @param file Ficheiro .xml (ou .txt) aberto
 */
void exportarDistanciasXML(Distancias *d, int indentacao, FILE *file) {
    if (!d || !d->matriz || indentacao < 0 || !file) return;

    indent(indentacao, file);
    fprintf(file, "<distancias>\n");

    for (int i = 0; i < d->nColunas; i++) {
        for (int j = i; j < d->nColunas; j++) {
            if (i == j) continue;

            indent(indentacao + 1, file);
            fprintf(file, "<parSensores>\n");

            indent(indentacao + 2, file);
            fprintf(file, "<sensor1>%d</sensor1>\n", i + 1);
            indent(indentacao + 2, file);
            fprintf(file, "<sensor2>%d</sensor2>\n", j + 1);
            indent(indentacao + 2, file);
            fprintf(file, "<distancia>%.1f</distancia>\n", d->matriz[(i+1) * d->nColunas + j+1]);

            indent(indentacao + 1, file);
            fprintf(file, "</parSensores>\n");
        }
    }

    indent(indentacao, file);
    fprintf(file, "</distancias>\n");
}

/**
 * @brief Exporta as distâncias para formato CSV
 * 
 * @param d Distância
 * @param file Ficheiro .csv, aberto
 */
void exportarDistanciasCSV(Distancias *d, FILE *file) {
    if (!d || !file) return;

    fprintf(file, "Sensor1, Sensor2, Distância\n");

    for (int i = 0; i < d->nColunas; i++) {
        for (int j = i; j < d->nColunas; j++) {
            if (i == j) continue;

            char *distanciaStr = floatToStringPontoDecimal(d->matriz[(i+1) * d->nColunas + j+1], 1);
            fprintf(file, "%d, %d, %s\n", i + 1, j + 1, distanciaStr);
            free(distanciaStr);
        }
    }
}

