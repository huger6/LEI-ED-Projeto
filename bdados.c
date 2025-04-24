#include "bdados.h"
#include "carro.h"
#include "distancias.h"
#include "dono.h"
#include "passagens.h"
#include "sensores.h"

/**
 * @brief Inicializa a base de dados criando as estruturas necessárias
 * 
 * @param bd Base de dados
 * @return int 1 se sucesso, 0 se erro
 */
int inicializarBD(Bdados *bd) {
    bd->carrosMarca = criarDict();
    bd->carrosCod = criarDict();
    bd->donosNif = criarDict();
    bd->donosAlfabeticamente = criarDict();
    bd->viagens = criarLista();
    bd->sensores = criarLista();
    inicializarMatrizDistancias(bd);
    if (!bd->carrosMarca || !bd->carrosCod|| !bd->distancias || !bd->distancias->matriz || !bd->donosNif ||
         !bd->donosAlfabeticamente	|| !bd->viagens || !bd->sensores) return 0;
    return 1;
}

/**
 * @brief Liberta toda a memória utilizada pelo programa
 * 
 * @param bd Base de dados
 */
void freeTudo(Bdados *bd) {
    if (!bd) return;
    printf("Memória antes de ser libertada: %.10f\n\n", obterUsoMemoria());

    freeDict(bd->carrosMarca, freeChaveCarroMarca, NULL);
    printf("Memória depois de libertar carrosMarca: %.10f\n\n", obterUsoMemoria());
    freeDict(bd->carrosCod, freeChaveCarroCod, freeCarro);
    printf("Memória depois de libertar carrosCod: %.10f\n\n", obterUsoMemoria());

    freeDict(bd->donosAlfabeticamente, freeChaveDonoAlfabeticamente, NULL);
    printf("Memória depois de libertar donosAlf: %.10f\n\n", obterUsoMemoria());
    freeDict(bd->donosNif, freeChaveDonoNif, freeDono);
    printf("Memória depois de libertar donosNif: %.10f\n\n", obterUsoMemoria());

    freeMatrizDistancias(bd->distancias);
    printf("Memória depois de libertar matriz: %.10f\n\n", obterUsoMemoria());

    freeLista(bd->viagens, freeViagem);
    printf("Memória depois de libertar viagens: %.10f\n\n", obterUsoMemoria());

    freeLista(bd->sensores, freeSensor);
    printf("Memória depois de libertar sensores: %.10f\n\n", obterUsoMemoria());

    free(bd);
    printf("Memória depois de libertar tudo: %.10f\n\n", obterUsoMemoria());
}


/*   Exportação da base de dados    */


/**
 * @brief Exporta a base de dados para XML
 * 
 * @param bd Base de dados
 * @param filename Nome do ficheiro (com extensão .xml ou .txt) onde guardar
 */
void exportarTudoXML(Bdados *bd, const char *filename) {
    if (!bd || !filename) return;

    FILE *file = fopen(filename, "w");
    if (!file) return;

    fprintf(file, "<database>\n");

    exportarDictXML(bd->donosNif, "donos", printDonoXML, 1, file);

    exportarDictXML(bd->carrosCod, "carros", printCarroXML, 1, file);

    exportarDistanciasXML(bd->distancias, 1, file);

    exportarListaXML(bd->sensores, "sensores", printSensorXML, 1, file);

    exportarListaXML(bd->viagens, "viagens", printViagemXML, 1, file);

    fprintf(file, "</database>");

    fclose(file);

    printf("Os dados foram exportados para o ficheiro XML (%s) com sucesso!\n\n", filename);
}

/**
 * @brief Exporta a base de dados para formato CSV
 * 
 * @param bd Base de dados
 * @param donosFilename Nome do ficheiro dos donos (.csv)
 * @param carrosFilename Nome do ficheiro dos carros (.csv)
 * @param sensoresFilename Nome do ficheiro dos sensores (.csv)
 * @param distanciasFilename Nome do ficheiro das distâncias (.csv)
 * @param viagensFilename Nome do ficheiro das viagens (.csv)
 */
void exportarTudoCSV(Bdados *bd, const char *donosFilename, const char *carrosFilename, const char *sensoresFilename, const char *distanciasFilename, const char *viagensFilename) {
    if (!bd || !donosFilename || !carrosFilename || !sensoresFilename || !viagensFilename) return;

    FILE *donos = fopen(donosFilename, "w");
    if (donos) {
        exportarDictCSV(bd->donosNif, printHeaderDonosCSV, printDonoCSV, donos);
        fclose(donos);
    }

    FILE *carros = fopen(carrosFilename, "w");
    if (carros) {
        exportarDictCSV(bd->carrosCod, printHeaderCarrosCSV, printCarroCSV, carros);
        fclose(carros);
    }

    FILE *sensores = fopen(sensoresFilename, "w");
    if (sensores) {
        exportarListaCSV(bd->sensores, printHeaderSensoresCSV, printSensorCSV, sensores);
        fclose(sensores);
    }

    FILE *distancias = fopen(distanciasFilename, "w");
    if (distancias) {
        exportarDistanciasCSV(bd->distancias, distancias);
        fclose(distancias);
    }

    FILE *viagens = fopen(viagensFilename, "w");
    if (viagens) {
        exportarListaCSV(bd->viagens, printHeaderViagensCSV, printViagemCSV, viagens);
        fclose(viagens);
    }
}

size_t memUsageTudo(Bdados *bd) {
    
}
