#include "distancias.h"
#include "bdados.h"
#include "configs.h"

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
    if (!distancia || !file) return;
    
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
    if (!file) return NULL;

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
            fprintf(file, "<distancia>%.1f</distancia>\n", d->matriz[i * d->nColunas + j]);

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

            char *distanciaStr = floatToStringPontoDecimal(d->matriz[i * d->nColunas + j], 1);
            fprintf(file, "%d, %d, %s\n", i + 1, j + 1, distanciaStr);
            free(distanciaStr);
        }
    }
}

/**
 * @brief Exporta as distâncias para ficheiro HTML
 * 
 * @param d Distâncias
 * @param pagename Nome da página
 * @param file Ficheiro .html, aberto
 */
void exportarDistanciasHTML(Distancias *d, char *pagename, FILE *file) {
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
        fprintf(file,
            "\t\t\t\t\t<tr>\n"
            "\t\t\t\t\t\t<th><a href=\"%s.html\">Sensor 1</a></th>\n"
            "\t\t\t\t\t\t<th><a href=\"%s.html\">Sensor 2</a></th>\n"
            "\t\t\t\t\t\t<th>Distância</th>\n"
            "\t\t\t\t\t</tr>\n", sensoresExportacaoFilename, sensoresExportacaoFilename);
        fprintf(file, "\t\t\t\t<tbody>\n");

        for (int i = 0; i < d->nColunas; i++) {
            for (int j = i; j < d->nColunas; j++) {
                if (i == j) continue;

                fprintf(file,
                    "\t\t\t\t\t<tr>\n"
                    "\t\t\t\t\t\t<th>%d</th>\n"
                    "\t\t\t\t\t\t<th>%d</th>\n"
                    "\t\t\t\t\t\t<th>%.1f</th>\n"
                    "\t\t\t\t\t</tr>\n",
                    i + 1, j + 1, d->matriz[(i) * d->nColunas + j]);
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
 * @brief Memória ocupada pelas distâncias
 * 
 * @param d Distâncias
 * @return size_t Memória ocupada ou 0 se erro
 */
size_t memUsageDistancias(Distancias *d) {
    if (!d) return 0;

    size_t mem = sizeof(Distancias);
    mem += (d->nColunas * d->nColunas * sizeof(float));

    return mem;
}

