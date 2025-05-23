#include "bdados.h"
#include "carro.h"
#include "distancias.h"
#include "dono.h"
#include "passagens.h"
#include "sensores.h"
#include "configs.h"
#include "uteis.h"

/**
 * @brief Inicializa a base de dados criando as estruturas necessárias
 * 
 * @param bd Base de dados
 * @return int 1 se sucesso, 0 se erro
 */
int inicializarBD(Bdados *bd) {
    bd->carrosMarca = criarDict();
    bd->carrosCod = criarDict();
    bd->carrosMat = criarDict();

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
    
    freeFilenames(); // só serão válidos na primeira execução (mas para evitar saídas inesperadas)
    freeExportacaoFilenames();

    freeDict(bd->carrosMarca, freeChaveCarroMarca, NULL);
    freeDict(bd->carrosMat, freeChaveCarroMatricula, NULL);
    freeDict(bd->carrosCod, freeChaveCarroCod, freeCarro);

    freeDict(bd->donosAlfabeticamente, freeChaveDonoAlfabeticamente, NULL);
    freeDict(bd->donosNif, freeChaveDonoNif, freeDono);

    freeMatrizDistancias(bd->distancias);

    freeLista(bd->viagens, freeViagem);

    freeLista(bd->sensores, freeSensor);

    free(bd);
}


/*   Exportação da base de dados    */


/**
 * @brief Exporta a base de dados para XML
 * 
 * @param bd Base de dados
 * @param filename Nome do ficheiro (sem extensão) onde guardar
 */
void exportarTudoXML(Bdados *bd, const char *filename) {
    if (!bd || !filename) return;
    limpar_terminal();

    FILE *file = fopen(filename, "w");
    if (!file) return;

    printf("A exportar dados...\n\n");

    fprintf(file, "<database>\n");

    exportarDictXML(bd->donosNif, "donos", printDonoXML, 1, file);

    exportarDictXML(bd->carrosCod, "carros", printCarroXML, 1, file);

    exportarDistanciasXML(bd->distancias, 1, file);

    exportarListaXML(bd->sensores, "sensores", printSensorXML, 1, file);

    exportarListaXML(bd->viagens, "viagens", printViagemXML, 1, file);

    fprintf(file, "</database>");

    fclose(file);

    printf("Os dados foram exportados para o ficheiro XML (%s) com sucesso!\n", filename);
    pressEnter();
}

/**
 * @brief Exporta a base de dados para formato CSV
 * 
 * @param bd Base de dados
 * @param donosFilename Nome do ficheiro dos donos 
 * @param carrosFilename Nome do ficheiro dos carros
 * @param sensoresFilename Nome do ficheiro dos sensores
 * @param distanciasFilename Nome do ficheiro das distâncias
 * @param viagensFilename Nome do ficheiro das viagens
 * 
 * @note Os nomes dos ficheiros devem vir sem extensão de ficheiro
 */
void exportarTudoCSV(Bdados *bd, const char *donosFilename, const char *carrosFilename, const char *sensoresFilename, const char *distanciasFilename, const char *viagensFilename) {
    if (!bd || !donosFilename || !carrosFilename || !sensoresFilename || !viagensFilename) return;
    limpar_terminal();
    
    printf("A exportar dados...\n\n");
    
    char *d = appendFileExtension(donosFilename, DOT_CSV);
    char *c = appendFileExtension(carrosFilename, DOT_CSV);
    char *s = appendFileExtension(sensoresFilename, DOT_CSV);
    char *di = appendFileExtension(distanciasFilename, DOT_CSV);
    char *v = appendFileExtension(viagensFilename, DOT_CSV);

    FILE *donos = fopen(d, "w");
    if (donos) {
        exportarDictCSV(bd->donosNif, printHeaderDonosCSV, printDonoCSV, donos);
        fclose(donos);
    }

    FILE *carros = fopen(c, "w");
    if (carros) {
        exportarDictCSV(bd->carrosCod, printHeaderCarrosCSV, printCarroCSV, carros);
        fclose(carros);
    }

    FILE *sensores = fopen(s, "w");
    if (sensores) {
        exportarListaCSV(bd->sensores, printHeaderSensoresCSV, printSensorCSV, sensores);
        fclose(sensores);
    }

    FILE *distancias = fopen(di, "w");
    if (distancias) {
        exportarDistanciasCSV(bd->distancias, distancias);
        fclose(distancias);
    }

    FILE *viagens = fopen(v, "w");
    if (viagens) {
        exportarListaCSV(bd->viagens, printHeaderViagensCSV, printViagemCSV, viagens);
        fclose(viagens);
    }

    if (d) free(d);
    if (c) free(c);
    if (s) free(s);
    if (di) free(di);
    if (v) free(v);

    printf("A exportação foi concluída!\n");
    pressEnter();
}

/**
 * @brief Exporta todos os dados para formato HTML
 * 
 * @param bd Base de dados
 * @param donosFilename Nome do ficheiro de donos 
 * @param carrosFilename Nome do ficheiro de carros 
 * @param sensoresFilename Nome do ficheiro de sensores
 * @param distanciasFilename Nome do ficheiro das distâncias 
 * @param viagensFilename Nome do ficheiro das viagens 
 * 
 * @note Os nomes dos ficheiros devem vir sem extensão de ficheiro
 */
void exportarTudoHTML(Bdados *bd, const char *donosFilename, const char *carrosFilename, const char *sensoresFilename, const char *distanciasFilename, const char *viagensFilename) {
    if (!bd || !donosFilename || !carrosFilename || !sensoresFilename || !viagensFilename) return;
    limpar_terminal();

    printf("A exportar dados...\n\n");

    char *d = appendFileExtension(donosFilename, DOT_HTML);
    char *c = appendFileExtension(carrosFilename, DOT_HTML);
    char *s = appendFileExtension(sensoresFilename, DOT_HTML);
    char *di = appendFileExtension(distanciasFilename, DOT_HTML);
    char *v = appendFileExtension(viagensFilename, DOT_HTML);

    FILE *donos = fopen(d, "w");
    if (donos) {
        exportarDictHTML(bd->donosNif, "Donos Database",printHeaderDonosHTML, printDonoHTML, donos);
        fclose(donos);
    }

    FILE *carros = fopen(c, "w");
    if (carros) {
        exportarDictHTML(bd->carrosCod, "Carros Database",printHeaderCarrosHTML, printCarroHTML, carros);
        fclose(carros);
    }

    FILE *sensores = fopen(s, "w");
    if (sensores) {
        exportarListaHTML(bd->sensores, "Sensores Database", printHeaderSensoresHTML, printSensorHTML, sensores);
        fclose(sensores);
    }
    
    FILE *distancias = fopen(di, "w");
    if (distancias) {
        exportarDistanciasHTML(bd->distancias, "Distâncias Database", distancias);
        fclose(distancias);
    }
    
    // O ficheiro é demasiado grande, e como tal, não abre (seria necessário modificar com JS)
    FILE *viagens = fopen(v, "w");
    if (viagens) {
        exportarListaHTML(bd->viagens, "Viagens Database",printHeaderViagensHTML, printViagemHTML, viagens);
        fclose(viagens);
    }

    if (d) free(d);
    if (c) free(c);
    if (s) free(s);
    if (di) free(di);
    if (v) free(v);

    printf("A exportação foi concluída!\n");
    pressEnter();
}

/**
 * @brief Calcula a memória ocupada por toda a base de dados
 * 
 * @param bd Base de dados
 * @return size_t Memória ocupada ou 0 se erro
 */
size_t memUsageTudo(Bdados *bd) {
    if (!bd) return 0;

    size_t memTotal = sizeof(Bdados);

    memTotal += memUsageVarGlobais();

    memTotal += dictMemUsage(bd->donosNif, memUsageDono, memUsageChaveDonoCod);
    memTotal += dictMemUsage(bd->donosAlfabeticamente, NULL, memUsageChaveDonoAlfabeticamente);

    memTotal += dictMemUsage(bd->carrosCod, memUsageCarro, memUsageChaveCarroCod);
    memTotal += dictMemUsage(bd->carrosMarca, NULL, memUsageChaveCarroMarca);
    memTotal += dictMemUsage(bd->carrosMat, NULL, memUsageChaveCarroMatricula);

    memTotal += listaMemUsage(bd->sensores, memUsageSensor);

    memTotal += listaMemUsage(bd->viagens, memUsageViagem);

    memTotal += memUsageDistancias(bd->distancias);

    return memTotal;
}

 