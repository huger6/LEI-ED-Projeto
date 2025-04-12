/* Ficheiro para tratar do carregamento e exportação dos dados */

#include "dados.h"
#include "uteis.h"
#include "validacoes.h"
#include "dono.h"
#include "carro.h"
#include "sensores.h"
#include "distancias.h"
#include "passagens.h"
#include "constantes.h"


/**
 * @brief Carrega todos os dados para a memória
 * 
 * @param bd Base de Dados
 * @param fDonos Ficheiro dos Donos
 * @param fCarros Ficheiro dos Carros
 * @param fSensores Ficheiro dos Sensores
 * @param fDistancias Ficheiro das Distâncias
 * @param fPassagem Ficheiro das Passagens
 * @param fLogs Ficheiro de logs
 * 
 * @return int 1 se sucesso, 0 se erro
 * 
 * @note Os ficheiros podem ser passados como NULL para usar o valor default
 */
int carregarDadosTxt(Bdados *bd, char *fDonos, char *fCarros, char *fSensores, char *fDistancias, char *fPassagem, char *fLogs) {
    const char *logFile = (fLogs) ? fLogs : LOGS_TXT; //Usar default em caso de não ser especificado

    //pode se criar um ficheiro html com o logs
    FILE *logsCheck = fopen(logFile, "r");
    char logsExiste = '0';
    if (logsCheck) { //Ficheiro já existe
        logsExiste = '1';
        fclose(logsCheck);
    }
    FILE *logs = fopen(logFile, "a");
    if (!logs) {
        printf("Ocorreu um erro grave ao abrir o ficheiro de logs '%s'.\n", logFile);
        return 0;
    }

    char erro = '0';
    if (logsExiste == '1') {
        fprintf(logs, "\n\n\n");
    }
    time_t inicio = time(NULL);
    fprintf(logs, "#ÍNICIO DA LEITURA DOS DADOS#\t\t%s\n\n", ctime(&inicio));

    if (!carregarDonosTxt(bd, fDonos, logs) || !carregarCarrosTxt(bd, fCarros, logs) || !carregarSensoresTxt(bd, fSensores, logs) || 
        !carregarDistanciasTxt(bd, fDistancias, logs) || !carregarPassagensTxt(bd, fPassagem, logs)) {
            erro = '1';
        }
    time_t fim = time(NULL);
    char *tempoFinal = ctime(&fim); // Não precisa de free
    tempoFinal[strcspn(tempoFinal, "\n")] = '\0';
    fprintf(logs, "\n#FIM DA LEITURA DOS DADOS#\t\t%s\t\tTEMPO DE CARREGAMENTO:%.0fsegundos\n\n", tempoFinal, difftime(fim, inicio));
    fclose(logs);

    if (erro == '1') {
        return 0; //Erro grave
    }
        
    return 1;
}

/**
 * @brief Carrega os dados relativos aos Donos para memória
 * 
 * @param bd Base de Dados
 * @param donosFilename Nome do ficheiro dos Donos
 * @param logs Ficheiro de logs
 * @return int 1 se sucesso, 0 se erro
 */
int carregarDonosTxt(Bdados *bd, char *donosFilename, FILE *logs) {
    const char *donosFile = (donosFilename) ? donosFilename : DONOS_TXT;

    time_t inicio = time(NULL);   
    fprintf(logs, "#FICHEIRO DONOS#\t\t%s\n", ctime(&inicio));

    FILE *donos = fopen(donosFile, "r");
    if (donos) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(donos, &nLinhas)) != NULL) {
            char *parametros[PARAM_DONOS];
            int numParam = 0; //Nº real de param lidos
            char *copiaLinha = strdup(linha); //Cópia da linha para passar para separarParam e evitar alterações à original
            separarParametros(copiaLinha, parametros, &numParam, PARAM_DONOS);
            char erro = '0';

            if (numParam == PARAM_DONOS) {
                //nif
                int nif;
                if (!stringToInt(parametros[0], &nif) || !validarNif(nif)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Número de contribuinte inválido\n\n");
                    erro = '1';
                }
                //Nome
                char *nomeInvalido = NULL;
                nomeInvalido = validarNome(parametros[1]);
                if (nomeInvalido) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: %s\n\n", nomeInvalido);
                    erro = '1';
                }
                //codPostal
                short zona, local;
                if (!converterCodPostal(parametros[2], &zona, &local) || !validarCodPostal(zona, local)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: O código postal é inválido\n\n");
                    erro = '1';
                }
                //Converter codigo Postal
                CodPostal postal;
                postal.local = local;
                postal.zona = zona;
                //Caso não haja erro passar os dados para as estruturas
                if (erro == '0') {
                    if(!inserirDonoLido(bd, parametros[1], nif, postal)) {
                        linhaInvalida(linha, nLinhas, logs);
                        fprintf(logs, "Razão: Ocorreu um erro fatal a carregar a linha para a memória\n\n");
                    }
                }
            }
            else if (numParam < PARAM_DONOS) {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Parametros insuficientes (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_DONOS, numParam);
            }
            else {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Demasiados parametros (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_DONOS, numParam);
            }
            free(copiaLinha);
            free(linha); 
        }
        fclose(donos);
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Donos: '%s'\n\n", donosFile);
        return 0;
    }

    time_t fim = time(NULL);
    char *tempoFinal = ctime(&fim); // Não precisa de free
    tempoFinal[strcspn(tempoFinal, "\n")] = '\0';
    fprintf(logs, "\n#FIM FICHEIRO DONOS#\t\t%s\t\tTEMPO DE CARREGAMENTO:%.0fsegundos\n\n", tempoFinal, difftime(fim, inicio));
    return 1;
}

/**
 * @brief Carrega os dados relativos aos Carros para memória
 * 
 * @param bd Base de Dados
 * @param carrosFilename Nome do ficheiro dos Carros
 * @param logs Ficheiro de logs
 * @return int 1 se sucesso, 0 se erro
 */
int carregarCarrosTxt(Bdados *bd, char *carrosFilename, FILE *logs) {
    const char *carrosFile = (carrosFilename) ? carrosFilename : CARROS_TXT;

    time_t inicio = time(NULL);   
    fprintf(logs, "#FICHEIRO CARROS#\t\t%s\n", ctime(&inicio));

    FILE *carros = fopen(carrosFile, "r");
    if (carros) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(carros, &nLinhas)) != NULL) {
            char *parametros[PARAM_CARROS];
            int numParam = 0; //Nº real de param lidos
            char *copiaLinha = strdup(linha);
            separarParametros(copiaLinha, parametros, &numParam, PARAM_CARROS);
            char erro = '0';

            if (numParam == PARAM_CARROS) {
                //Matrícula
                if (!validarMatricula(parametros[0])) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Matrícula inválida\n\n");
                    erro = '1';
                }
                //Marca
                char *marca = NULL;
                marca = validarMarca(parametros[1]);
                if (marca) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: %s", marca);
                    erro = '1';
                }
                //Modelo
                char *modelo = NULL;
                modelo = validarModelo(parametros[2]);
                if (modelo) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: %s", modelo);
                    erro = '1';
                }
                //Ano
                short ano;
                if (!stringToShort(parametros[3], &ano) || !validarAnoCarro(ano)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Ano inválido\n\n");
                    erro = '1';
                }
                //NIF
                int nif;
                if (!stringToInt(parametros[4], &nif) || !validarNif(nif)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Número de contribuinte inválido\n\n");
                    erro = '1';
                }
                //CodVeiculo
                int codVeiculo;
                if (!stringToInt(parametros[5], &codVeiculo) || !validarCodVeiculo(codVeiculo)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Código do veículo errado\n\n");
                    erro = '1';
                }
                //Caso não haja erro passar os dados para as estruturas
                if (erro == '0') {
                    if(!inserirCarroLido(bd, parametros[0], parametros[1], parametros[2], ano, nif, codVeiculo)) {
                        linhaInvalida(linha, nLinhas, logs);
                        fprintf(logs, "Razão: Ocorreu um erro fatal a carregar a linha para a memória");
                    }
                }
            }
            else if (numParam < PARAM_CARROS) {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Parametros insuficientes (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_CARROS, numParam);
            }
            else {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Demasiados parametros (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_CARROS, numParam);
            }
            free(copiaLinha);
            free(linha); 
        }
        fclose(carros);
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Carros: '%s'.", carrosFile);
        return 0;
    }
    ordenarLista(bd->carros, compararCarros);
    time_t fim = time(NULL);
    char *tempoFinal = ctime(&fim); // Não precisa de free
    tempoFinal[strcspn(tempoFinal, "\n")] = '\0';
    fprintf(logs, "\n#FIM FICHEIRO CARROS#\t\t%s\t\tTEMPO DE CARREGAMENTO:%.0fsegundos\n\n", tempoFinal, difftime(fim, inicio));
    return 1;
}

/**
 * @brief Carrega os dados relativos aos Sensores para memória
 * 
 * @param bd Base de Dados
 * @param sensoresFilename Nome do ficheiro dos Sensores
 * @param logs Ficheiro de logs
 * @return int 1 se sucesso, 0 se erro
 */
int carregarSensoresTxt(Bdados *bd, char *sensoresFilename, FILE *logs) {
    const char *sensoresFile = (sensoresFilename) ? sensoresFilename : SENSORES_TXT;
    
    time_t inicio = time(NULL);   
    fprintf(logs, "#FICHEIRO SENSORES#\t\t%s\n", ctime(&inicio));

    FILE *sens = fopen(sensoresFile, "r");
    if (sens) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(sens, &nLinhas)) != NULL) {
            char *parametros[PARAM_SENSORES];
            int numParam = 0; //Nº real de param lidos
            char *copiaLinha = strdup(linha);
            separarParametros(copiaLinha, parametros, &numParam, PARAM_SENSORES);
            char erro = '0';

            if (numParam == PARAM_SENSORES) {
                //Código do sensor
                int codSensor;
                if (!stringToInt(parametros[0], &codSensor) || !validarCodSensor(codSensor)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Código do sensor inválido\n\n");
                    erro = '1';
                }
                //Caso não haja erro passar os dados para as estruturas
                if (erro == '0') {
                    if(!inserirSensorLido(bd, codSensor, parametros[1], parametros[2], parametros[3])) {
                        linhaInvalida(linha, nLinhas, logs);
                        fprintf(logs, "Razão: Ocorreu um erro fatal a carregar a linha para a memória");
                    }
                }
            }
            else if (numParam < PARAM_SENSORES) {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Parametros insuficientes (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_SENSORES, numParam);
            }
            else {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Demasiados parametros (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_SENSORES, numParam);
            }
            free(copiaLinha);
            free(linha); 
        }
        fclose(sens);
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Sensores: '%s'.", sensoresFile);
        return 0;
    }
    //Ordenar a lista
    ordenarLista(bd->sensores, compararSensores);
    time_t fim = time(NULL);
    char *tempoFinal = ctime(&fim); // Não precisa de free
    tempoFinal[strcspn(tempoFinal, "\n")] = '\0';
    fprintf(logs, "\n#FIM FICHEIRO SENSORES#\t\t%s\t\tTEMPO DE CARREGAMENTO:%.0fsegundos\n\n", tempoFinal, difftime(fim, inicio));
    return 1;
}

/**
 * @brief Carrega os dados relativos às Distâncias para memória
 * 
 * @param bd Base de Dados
 * @param distanciasFilename Nome do ficheiro das Distancias
 * @param logs Ficheiro de logs
 * @return int 1 se sucesso, 0 se erro
 */
int carregarDistanciasTxt(Bdados *bd, char *distanciasFilename, FILE *logs) {
    const char *distanciasFile = (distanciasFilename) ? distanciasFilename : DISTANCIAS_TXT;

    time_t inicio = time(NULL);   
    fprintf(logs, "#FICHEIRO DISTANCIAS#\t\t%s\n", ctime(&inicio));
    int tamMatriz = contarLinhas(distanciasFile);
    if (tamMatriz < 0) {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Distancias: '%s'.", distanciasFile);
        return 0;
    }
    if (!realocarMatrizDistancias(bd, tamMatriz)) {
        fprintf(logs, "Ocorreu um erro a realocar a matriz das distâncias\n\n");
        return 0;
    }
    FILE *dists = fopen(distanciasFile, "r");
    if (dists) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(dists, &nLinhas)) != NULL) {
            char *parametros[PARAM_DISTANCIAS];
            int numParam = 0; //Nº real de param lidos
            char *copiaLinha = strdup(linha);
            separarParametros(copiaLinha, parametros, &numParam, PARAM_DISTANCIAS);
            char erro = '0';

            if (numParam == PARAM_DISTANCIAS) {
                //Código do sensor 1
                int codSensor1;
                if (!stringToInt(parametros[0], &codSensor1) || !validarCodSensor(codSensor1)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Código do sensor 1 inválido\n\n");
                    erro = '1';
                }
                //Código do sensor 2
                int codSensor2;
                if (!stringToInt(parametros[1], &codSensor2) || !validarCodSensor(codSensor2)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Código do sensor 2 inválido\n\n");
                    erro = '1';
                }
                float distancia;
                converterPontoVirgulaDecimal(parametros[2]); // Passa notação de floats para vírgulas caso necessário
                if (!stringToFloat(parametros[2], &distancia) || !validarDistancia(distancia)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Distância inválida\n\n");
                    erro = '1';
                    printf("Distancia: %.2f\n", distancia);
                }
                //Caso não haja erro passar os dados para as estruturas
                if (erro == '0') {
                    if(!inserirDistanciaLido(bd, codSensor1, codSensor2, distancia)) {
                        linhaInvalida(linha, nLinhas, logs);
                        fprintf(logs, "Razão: Ocorreu um erro fatal a carregar a linha para a memória");
                    }
                }
            }
            else if (numParam < PARAM_DISTANCIAS) {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Parametros insuficientes (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_DISTANCIAS, numParam);
            }
            else {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Demasiados parametros (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_DISTANCIAS, numParam);
            }
            free(copiaLinha);
            free(linha); 
        }
        fclose(dists);
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Distancias: '%s'.", distanciasFile);
        return 0;
    }
    time_t fim = time(NULL);
    char *tempoFinal = ctime(&fim); // Não precisa de free
    tempoFinal[strcspn(tempoFinal, "\n")] = '\0';
    fprintf(logs, "\n#FIM FICHEIRO DISTANCIAS#\t\t%s\t\tTEMPO DE CARREGAMENTO:%.0fsegundos\n\n", tempoFinal, difftime(fim, inicio));
    return 1;
}

/**
 * @brief Carrega os dados relativos às Passagens para memória
 * 
 * @param bd Base de Dados
 * @param passagensFilename Nome do ficheiro das Passagens
 * @param logs Ficheiro de logs
 * @return int 1 se sucesso, 0 se erro
 */
int carregarPassagensTxt(Bdados *bd, char *passagensFilename, FILE *logs) {
    const char *passagensFile = (passagensFilename) ? passagensFilename : PASSAGEM_TXT;

    time_t inicio = time(NULL);   
    fprintf(logs, "#FICHEIRO PASSAGENS#\t\t%s\n", ctime(&inicio));

    FILE *passagem = fopen(passagensFile, "r");
    if (passagem) {
        int nLinhas = 0;
        int nPassagens = 0; //Para verificar os pares das viagens
        char *linha = NULL;

        Viagem *v = NULL;
        char skipPassagem2 = '0';
        while((linha = lerLinhaTxt(passagem, &nLinhas)) != NULL) {
            //Verificar se houve erro na Passagem1
            if (skipPassagem2 == '1') {
                free(linha);
                continue;
            }
            char *parametros[PARAM_PASSAGEM];
            int numParam = 0; //Nº real de param lidos
            char *copiaLinha = strdup(linha);
            separarParametros(copiaLinha, parametros, &numParam, PARAM_PASSAGEM);

            char erro = '0';
            char passagem1 = (nPassagens % 2 == 0) ? '1' : '0';

            if (passagem1 == '1') {
                v = (Viagem *)malloc(sizeof(Viagem));
                if (!v) {
                    fprintf(logs, "Erro a alocar memoria para viagem %d.\n\n", bd->viagens->nel + 1);
                    free(copiaLinha);
                    free(linha);
                    skipPassagem2 = '1';
                    continue;
                }
            }
            
            if (numParam == PARAM_PASSAGEM) {
                //ID do sensor
                int idSensor;
                if (!stringToInt(parametros[0], &idSensor) || !validarCodSensor(idSensor)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: ID do sensor inválido\n\n");
                    erro = '1';
                }
                //Código do veículo
                int codVeiculo;
                if (!stringToInt(parametros[1], &codVeiculo) || !validarCodVeiculo(codVeiculo)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Código do veículo inválido\n\n");
                    erro = '1';
                }
                //Data
                Data date;
                char *mensagemData = NULL;
                mensagemData = converterParaData(parametros[2], &date);
                if (mensagemData) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: %s\n\n", mensagemData);
                    erro = '1';
                }
                else if (!validarData(date, '0')) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Data inválida\n\n");
                    erro = '1';
                }
                //Tipo de registo
                if (!validarTipoRegisto(parametros[3][0])) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Tipo de registo inválido\n\n");
                    erro = '1';
                }
                //Caso não haja erro passar os dados para as estruturas
                if (erro == '0') {
                    Passagem *p = obterPassagem(bd, idSensor, codVeiculo, date, parametros[3][0]);
                    //Primeira passagem da viagem
                    if (passagem1 == '1') {
                        if (inserirViagemLido(bd, v, p, NULL) != -1) {
                            linhaInvalida(linha, nLinhas, logs);
                            fprintf(logs, "Razão: Ocorreu um erro a carregar a passagem de entrada da viagem\n\n");
                            erro = '1';
                        };
                    }
                    //Segunda passagem da viagem
                    else {
                        if (inserirViagemLido(bd, v, NULL, p) != 1) {
                            linhaInvalida(linha, nLinhas, logs);
                            fprintf(logs, "Razão: Ocorreu um erro a carregar a passagem de saída da viagem\n\n");
                            erro = '1';
                        }
                        getStatsViagem(bd, v);
                        addInicioLista(bd->viagens, (void *)v);
                    }
                    if (erro == '0') {
                        nPassagens++;
                    }
                    //Em caso de erro
                    else {
                        free(p);
                        // Segunda passagem errada
                        if (passagem1 == '0') {
                            freePassagem((void *)v->entrada);
                            free(v);
                            nPassagens--; // De modo a alocar memória para o próximo
                        }
                        //Primeira passagem errada 
                        else {
                            // Assumimos o v criado como a viagem do próximo, já que não tem nada
                            skipPassagem2 = '1'; // Passar à frente o respetivo par
                        }
                    }
                }
            }
            else if (numParam < PARAM_PASSAGEM) {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Parametros insuficientes (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_PASSAGEM, numParam);
            }
            else {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Demasiados parametros (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_PASSAGEM, numParam);
            }
            free(copiaLinha);
            free(linha); 
        }
        fclose(passagem);
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Passagens: '%s'.", passagensFile);
        return 0;
    }
    //Ordenar a lista
    //ordenarLista(bd->passagens, compararPassagens); Demoraria 5dias!
    time_t fim = time(NULL);
    char *tempoFinal = ctime(&fim); // Não precisa de free
    tempoFinal[strcspn(tempoFinal, "\n")] = '\0';
    fprintf(logs, "\n#FIM FICHEIRO PASSAGENS#\t\t%s\t\tTEMPO DE CARREGAMENTO:%.0fsegundos\n\n", tempoFinal, difftime(fim, inicio));
    return 1;
}


/* Remove espaços extra de uma string
 *
 * @param str    String a modificar (se necessário)
 *
 * @return void
 *         
 * @note Remove espaços:
 *       - No início
 *       - No fim  
 *       - Múltiplos entre palavras
 * @note Preserva espaço único entre palavras
 * @note Verifica string NULL/vazia
 */
void removerEspacos(char *str) {
    if(str == NULL) return;
    char *inicio = str;
    char *fim = NULL;

    //Se o início conter um espaço, vai avançar o ponteiro uma casa, até essa casa deixar de ser um espaço
    while (*inicio == ' ') inicio++;

    // Copiar a string sem espaços para o array inicial
    if (inicio != str) {
        memmove(str, inicio, strlen(inicio) + 1); //É uma versão melhorada do memcpy (evita a sobreposição)
    }

    //Ponteiro para o último caractere
    fim = str + strlen(str) - 1;
    while (fim > str && *fim == ' ') { //verifica se o fim tem um espaço em branco, se sim, anda com o ponteiro uma casa para trás e repete
        fim--;
    }

    //Colocamos o nul char no final
    *(fim + 1) = '\0';
}

/* Separa linha em parâmetros usando SEPARADOR
 *
 * @param linha            String com linha completa a separar
 * @param parametros       Array de ponteiros para armazenar os parâmetros extraídos
 * @param numParametros   Ponteiro para contar os parâmetros
 * @param paramEsperados  Número de parâmetros esperados para separar
 *
 * @return void
 *         
 * @note Remove espaços extra via remover_espacos()
 * @note Em caso de erro:
 *       - Define num_parametros = 0
 */
void separarParametros(char *linha, char **parametros, int *numParametros, const int paramEsperados) { // char ** parametros serve para armazenar os ponteiros dos parametros, de modo a que não sejam perdidos
    if (!linha || !parametros || !numParametros) return;

    *numParametros = 0;

    char *token = strtok(linha, SEPARADOR_STR); 

    while (token != NULL && *numParametros < paramEsperados) {
        removerEspacos(token);  
        parametros[*numParametros] = token;
        (*numParametros)++;
        token = strtok(NULL, SEPARADOR_STR);
    }
}

/**
 * @brief Escreve a linha inválida no ficheiro de logs
 * 
 * @param linha Linha inválida
 * @param nLinha Número da linha
 * @param logs Ficheiro de logs
 */
void linhaInvalida(const char *linha, int nLinha, FILE *logs) {
    fprintf(logs, "Linha %d inválida: %s\n", nLinha, linha);
}

/**
 * @brief Conta as linhas de um ficheiro
 * 
 * @param filename Nome do ficheiro
 * @return int -1 se erro, Nº linhas caso contrário
 */
int contarLinhas(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        return -1;
    }

    int linhas = 0;
    int c;
    while ((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            linhas++;
        }
    }

    fclose(f);

    return linhas;
}

// Dados Binários

/**
 * @brief Guarda todos os dados em memória para um ficheiro binário 
 * 
 * @param bd Base de dados
 * @param nome Nome do ficheiro a abrir
 * @return int 0 se erro, 1 se sucesso
 */
int guardarDadosBin(Bdados *bd, const char *nome) {
    if (!bd || !nome) return 0;

    FILE *file = fopen(nome, "wb");
    if (!file) return 0;

    // Donos
    guardarDictBin(bd->donosNif, guardarChaveDonoNif, guardarDonoBin, file);

    // Carros
    guardarListaBin(bd->carros, guardarCarroBin, file);

    // Sensores
    guardarListaBin(bd->carros, guardarSensorBin, file);

    // Passagens/Viagens
    guardarListaBin(bd->viagens, guardarViagemBin, file);
 
    // Distâncias
    if (bd->matrizDist) {
        fwrite(&bd->tamMatrizDist, sizeof(int), 1, file);
        for (int i = 0; i < bd->tamMatrizDist; i++) {
            fwrite(bd->matrizDist[i], sizeof(float), 1, file);
        }
    }
    
    
    fclose(file);
    return 1;
}

int carregarDadosBin(Bdados *bd, const char *nome) {
    if (!bd || !nome) return 0;

    FILE *file = fopen(nome, "rb");
    if (!file) return 0;

    fread(&bd, sizeof(Bdados), 1, file);

    fclose(file);
    return 1;
}


