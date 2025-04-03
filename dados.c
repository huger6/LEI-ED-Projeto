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
    FILE *logs = fopen(logFile, "a");
    if (!logs) {
        printf("Ocorreu um erro grave ao abrir o ficheiro de logs '%s'.\n", logFile);
        return 0;
    }

    char erro = '0';
    fprintf(logs, "#ÍNICIO DA LEITURA DOS DADOS#\n\n\n");
    if (!carregarDonosTxt(bd, fDonos, logs) || !carregarCarrosTxt(bd, fCarros, logs) || !carregarSensoresTxt(bd, fSensores, logs) || 
        !carregarDistanciasTxt(bd, fDistancias, logs) || !carregarPassagensTxt(bd, fPassagem, logs)) {
            erro = '1';
        }

    fprintf(logs, "\n#FIM DA LEITURA DOS DADOS#");
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
    fprintf(logs, "#FICHEIRO DONOS#\n\n");

    FILE *donos = fopen(donosFile, "r");
    if (donos) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(donos, &nLinhas)) != NULL) {
            char *parametros[PARAM_DONOS];
            int numParam = 0; //Nº real de param lidos
            separarParametros(linha, parametros, &numParam);
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
                    fprintf(logs, "Razão: %s", nomeInvalido);
                    erro = '1';
                }
                //codPostal
                short zona, local;
                if (!converterCodPostal(parametros[2], &zona, &local) || !validarCodPostal(zona, local)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: O código postal é inválido");
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
                        fprintf(logs, "Razão: Ocorreu um erro fatal a carregar a linha para a memória");
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
        }
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Donos: '%s'.", donosFile);
        return 0;
    }
    //Ordenar a lista
    ordenarLista(bd->donos, compararDonos);
    fprintf(logs, "\n#FIM FICHEIRO DONOS#\n\n");
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
    fprintf(logs, "#FICHEIRO CARROS#\n\n");

    FILE *carros = fopen(carrosFile, "r");
    if (carros) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(carros, &nLinhas)) != NULL) {
            char *parametros[PARAM_CARROS];
            int numParam = 0; //Nº real de param lidos
            separarParametros(linha, parametros, &numParam);
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
        }
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Carros: '%s'.", carrosFile);
        return 0;
    }
    ordenarLista(bd->carros, compararCarros);
    fprintf(logs, "\n#FIM FICHEIRO CARROS#\n\n");
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
    fprintf(logs, "#FICHEIRO SENSORES#\n\n");

    FILE *sens = fopen(sensoresFile, "r");
    if (sens) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(sens, &nLinhas)) != NULL) {
            char *parametros[PARAM_SENSORES];
            int numParam = 0; //Nº real de param lidos
            separarParametros(linha, parametros, &numParam);
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
        }
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Sensores: '%s'.", sensoresFile);
        return 0;
    }
    //Ordenar a lista
    ordenarLista(bd->sensores, compararSensores);
    fprintf(logs, "\n#FIM FICHEIRO SENSORES#\n\n");
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
    fprintf(logs, "#FICHEIRO DISTANCIAS#\n\n");

    FILE *dists = fopen(distanciasFile, "r");
    if (dists) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(dists, &nLinhas)) != NULL) {
            char *parametros[PARAM_DISTANCIAS];
            int numParam = 0; //Nº real de param lidos
            separarParametros(linha, parametros, &numParam);
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
                if (!stringToFloat(parametros[2], &distancia) || !validarDistancia(distancia)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Distância inválida\n\n");
                    erro = '1';
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
        }
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Distancias: '%s'.", distanciasFile);
        return 0;
    }
    //Ordenar a lista
    ordenarLista(bd->distancias, compararDistancias);
    fprintf(logs, "\n#FIM FICHEIRO DISTANCIAS#\n\n");
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
    fprintf(logs, "#FICHEIRO DISTANCIAS#\n\n");

    FILE *passagem = fopen(passagensFile, "r");
    if (passagem) {
        int nLinhas = 0;
        char *linha = NULL;
        while((linha = lerLinhaTxt(passagem, &nLinhas)) != NULL) {
            char *parametros[PARAM_PASSAGEM];
            int numParam = 0; //Nº real de param lidos
            separarParametros(linha, parametros, &numParam);
            char erro = '0';

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
                //TODO
                Data date;
                char * mensagemData = converterParaData(parametros[2], &date);
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
                if (!validarTipoRegisto((char)parametros[3])) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Tipo de registo inválido\n\n");
                    erro = '1';
                }
                //Caso não haja erro passar os dados para as estruturas
                if (erro == '0') {
                    if(!inserirPassagemLido(bd, idSensor, codVeiculo, date, (char)parametros[3])) {
                        linhaInvalida(linha, nLinhas, logs);
                        fprintf(logs, "Razão: Ocorreu um erro fatal a carregar a linha para a memória");
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
        }
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Passagens: '%s'.", passagensFile);
        return 0;
    }
    //Ordenar a lista
    ordenarLista(bd->passagens, compararPassagens);
    fprintf(logs, "\n#FIM FICHEIRO PASSAGENS#\n\n");
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
 * @param num_parametros   Ponteiro para contar os parâmetros
 *
 * @return void
 *         
 * @note Remove espaços extra via remover_espacos()
 * @note Aloca memória para cada parâmetro
 * @note Em caso de erro:
 *       - Define num_parametros = 0
 *       - Liberta memória já alocada
 */
void separarParametros(const char *linha, char **parametros, int *num_parametros) { // char ** parametros serve para armazenar os ponteiros dos parametros, de modo a que não sejam perdidos
    if(linha == NULL || parametros == NULL || num_parametros == NULL) return;
    char *inicio = strdup(linha); //Ponteiro para o inicio da linha
    //!!NOTA IMPORTANTE - É feita uma cópia da linha pois caso contrário, caso a função tenha um erro em qualquer parte, a linha original será perdida!!
    char *fim = NULL;
    //Não colocamos *num_parametros = 0 pois esta função poderá ser chamada várias vezes numa linha, e não queremos alterar a var nesse caso
    int indice = 0; //Indice do array

    while(*inicio != '\0') { //Se não for o fim da linha entramos no loop
        fim = inicio; 

        //Vamos veriricar se o ponteiro atual de fim é um separador ou o fim da linha, caso não seja avançamos
        while(*fim != SEPARADOR && *fim != '\0' && *fim != '\n') fim++;
        //Aqui fim está a apontar para o separador, o fim da linha ou um \n (se bem que neste caso \n é o fim da linha)
        char temp = *fim; //Armazena o tab ou o nul char
        *fim = '\0'; //vai terminar a string de inicio (ou seja, um parâmetro); também corta o \n aqui, se exitir
        removerEspacos(inicio);

        if (*inicio != '\0') { //Se o inicio não for o fim da linha, então temos um parametro
            //Alocamos memória para o parametro e copia o conteúdo
            parametros[indice] = malloc(strlen(inicio) + 1); //Lembrar que parâmetros recebe um ponteiro
            if (parametros[indice] != NULL) {
                strcpy(parametros[indice], inicio);
                indice++;
                (*num_parametros)++;
            }
            else {
                //ERRO!!
                *num_parametros = 0; //Usamos num_parametros como uma flag para evitar que a linha seja lida
                //Libertar a memória alocada até ao indice atual
                for (int i = 0; i < indice; i++) 
                    free(parametros[i]); 
                break;
            }
        }
        *fim = temp; //Volta a colocar o tab ou '\0' onde estava 
        inicio = (*fim == '\0') ? fim : fim + 1; //Verifica se já estamos no fim da string, se sim, inicio = fim, se não inicio = fim +1 (avança uma casa)
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


