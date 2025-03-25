/* Ficheiro para tratar do carregamento e exportação dos dados */

#include "dados.h"
#include "uteis.h"

int carregarDadosTxt(const char * fDonos, char * fCarros, char * fSensores, char * fDistancias, char * fPassagem, char * fLogs) {
    const char * logFile = (fLogs) ? fLogs : LOGS_TXT; //Usar default em caso de não ser especificado

    //pode se criar um ficheiro html com o logs
    FILE * logs = fopen(logFile, "a");
    if (!logs) {
        printf("Ocorreu um erro grave ao abrir o ficheiro de logs '%s'.\n", logFile);
        return 1;
    }

    char erro = '0';
    fprintf(logs, "#ÍNICIO DA LEITURA DOS DADOS#\n\n\n");
    if (carregarDonosTxt(fDonos, logs) || carregarCarrosTxt(fCarros, logs) || carregarSensoresTxt(fSensores, logs) || 
        carregarDistanciasTxt(fDistancias, logs) || carregarPassagensTxt(fPassagem, logs)) {
            erro = '1';
        }

    fprintf(logs, "\n#FIM DA LEITURA DOS DADOS#");
    fclose(logs);

    if (erro == '1') {
        return 1; //Erro grave
    }
        
    return 0;
}

int carregarDonosTxt(char * donosFilename, FILE * logs) {
    const char * donosFile = (donosFilename) ? donosFilename : DONOS_TXT;
    fprintf(logs, "#FICHEIRO DONOS#\n\n");

    if (!donosFilename) {
        fprintf(logs, "O nome do ficheiro é inválido('').\n\n");
        return 0;
    }

    FILE * donos = fopen(donosFile, "r");
    if (donos) {
        int nLinhas = 0;
        char * linha = NULL;
        int nif_temp;
        while((linha = lerLinhaTxt(donos, &nLinhas)) != NULL) {
            char * parametros[PARAM_DONOS];
            int numParam = 0; //Nº real de param lidos
            separarParametros(linha, parametros, &numParam);

            if (numParam == PARAM_DONOS) {
                if (!stringToInt(parametros[0], &nif_temp) || validarNif(nif_temp)) {
                    linhaInvalida(linha, nLinhas, logs);
                    fprintf(logs, "Razão: Número de contribuinte inválido\n\n");
                }
            }
            else if (numParam < PARAM_DONOS) {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Parametros insuficientes (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_DONOS, numParam);
            }
            else {
                linhaInvalida(linha, nLinhas, logs);
                fprintf(logs, "Razão: Demasiado parametros (%d NECESSÁRIOS, %d LIDOS)\n\n", PARAM_DONOS, numParam);
            }

        }
    }
    else {
        fprintf(logs, "Ocorreu um erro ao abrir o ficheiro de Donos: '%s'.");
        return 1;
    }

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
void removerEspacos(char * str) {
    if(str == NULL) return;
    char * inicio = str;
    char * fim = NULL;

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
void separarParametros(const char * linha, char ** parametros, int * num_parametros) { // char ** parametros serve para armazenar os ponteiros dos parametros, de modo a que não sejam perdidos
    if(linha == NULL || parametros == NULL || num_parametros == NULL) return;
    char * inicio = strdup(linha); //Ponteiro para o inicio da linha
    //!!NOTA IMPORTANTE - É feita uma cópia da linha pois caso contrário, caso a função tenha um erro em qualquer parte, a linha original será perdida!!
    char * fim = NULL;
    //Não colocamos *num_parametros = 0 pois esta função poderá ser chamada várias vezes numa linha, e não queremos alterar a var nesse caso
    int indice = 0; //Indice do array

    while(*inicio != '\0') { //Se não for o fim da linha entramos no loop
        fim = inicio; 

        //Vamos veriricar se o ponteiro atual de fim é um separador ou o fim da linha, caso não seja avançamos
        while(*fim != SEPARADOR && *fim != '\0' && *fim != '\n') fim++;
        //Aqui fim está a apontar para o separador, o fim da linha ou um \n (se bem que neste caso \n é o fim da linha)
        char temp = *fim; //Armazena o tab ou o nul char
        *fim = '\0'; //vai terminar a string de inicio (ou seja, um parâmetro); também corta o \n aqui, se exitir
        remover_espacos(inicio);

        if (*inicio != '\0') { //Se o inicio não for o fim da linha, então temos um parametro
            //Alocamos memória para o parametro e copia o conteúdo
            parametros[indice] = malloc(strlen(inicio) + 1); //Lembrar que parâmetros recebe um ponteiro
            if (parametros[indice] != NULL) {
                strcpy(parametros[indice], inicio); //inicio é copiado, logo não necessita de free() (apenas em carregar_dados)
                indice++;
                (*num_parametros)++;
            }
            else {
                //ERRO!!
                *num_parametros = 0; //Usamos num_parametros como uma flag para evitar que a linha seja lida
                //Talvez fosse bom encontrar uma forma diferente de fazer isso sem levar o user a erro
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

void linhaInvalida(const char * linha, int nLinha, FILE * logs) {
    fprintf(logs, "Linha %d inválida: %s\n", nLinha, linha);
}



