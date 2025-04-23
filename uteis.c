#include "uteis.h"

Data DATA_ATUAL = {0,0,0,0,0,0.0f}; //var global para guardar a data atual

/* Limpa buffer do teclado
 *
 * @return void
 *         
 * @note Lê caracteres até encontrar \n ou EOF
 * @note Usar apenas se houver certeza de que há dados no buffer
 */
void limpar_buffer() {
    int lixo;
    while ((lixo = getchar()) != '\n' && lixo != EOF);
}

/* Verifica e limpa buffer do teclado
 *
 * @return int  1 se buffer só tem \n, 0 se tiver outros caracteres
 *         
 * @note Verifica primeiro caractere no buffer
 * @note Limpa sempre todo o buffer
 * @note Usar apenas se houver certeza de que há dados no buffer
 */
int verificar_e_limpar_buffer() {
    //Verificar o caracter que está no buffer
    char lixo = getchar();
    //Se não for o \n, ou seja, a entrada foi inválida
    if (lixo != '\n') {
        limpar_buffer(); //limpamos o buffer (pois ainda há o enter, pelo menos)
        return 0;
    }
    //Se estamos aqui, então o caracter é o \n, e já foi lido, logo não precisamos fazer nada
    return 1;
}

/* Limpa o terminal
 *
 * @return void
 *         
 * @note Multi-plataforma (Windows/Unix)
 */
void limpar_terminal() {
    #ifdef _WIN32 //_WIN32 é uma variável local automaticamente pre-definida pelo windows em todos os sistemas
        system("cls"); //Sistemas windows
    #else
        system("clear"); //Sistemas linux, macos, etc
    #endif
}

/* Pede ao utilizador para pressionar Enter
 *
 * @return void
 *         
 * @note Limpa buffer se necessário
 * @note Bloqueia até receber \n
 */
void pressEnter() {
    printf("Pressione Enter para continuar.\n");
    while (getchar() != '\n');
}

/* Gera um int aleatório entre min e max
 *
 * @param min   Número mínimo a ser gerado
 * @param max   Número máximo a ser gerado
 * 
 * @return int    
 *         
 * @note Necessário fazer srand(time(NULL)) antes de usar (apenas uma vez)
 */
int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}

/* Atualiza a data atual global
 *
 * @return void
 *         
 * @note Usa time.h para data do sistema
 * @note Ajusta mês de 0-11 para 1-12
 * @note Ajusta ano desde 1900
 */
void data_atual() {
    time_t t = time(NULL);
    struct tm * tm_atual = localtime(&t);
    
    //Passar para a variável global
    DATA_ATUAL.dia = tm_atual->tm_mday;
    DATA_ATUAL.mes = tm_atual->tm_mon + 1; //tm_mon vai de 0-11
    DATA_ATUAL.ano = tm_atual->tm_year + 1900;
    DATA_ATUAL.hora = tm_atual->tm_hour;
    DATA_ATUAL.min = tm_atual->tm_min;
    DATA_ATUAL.seg = tm_atual->tm_sec;
}

/* Pede confirmação S/N ao utilizador
 *
 * @param mensagem   Mensagem/pergunta seguida de uma resposta do tipo sim/não
 * 
 * @return int    1 se S/s, 0 se N/n
 *         
 * @note Case insensitive
 * @note Limpa buffer
 * @note Só retorna se obter resposta
 */
int sim_nao(char *mensagem) {
    char opcao;
    do {
        printf("%s (S/N): ", mensagem);
        scanf(" %c", &opcao);
        //Verificar entradas inválidas
        if (!verificar_e_limpar_buffer()) {
            printf("Entrada inválida! Por favor tente novamente. Utilize 'S' ou 'N': ");
            continue;
        }
        if (opcao == 's' || opcao == 'S') {
            return 1;
        }
        else if (opcao == 'n' || opcao == 'N') {
            return 0;
        }
        printf("\nOpção inválida. Use 'S' ou 'N': ");
    } while (1);
}

/* Normaliza uma string removendo acentos e convertendo para minúsculas
 *
 * @param str    String a normalizar. Deve estar codificada em UTF-8.
 *               Não é modificada pela função.
 *
 * @return char* Nova string alocada dinamicamente contendo:
 *         - String normalizada sem acentos e em minúsculas
 *         - NULL se str for NULL ou ocorrer erro de memória
 *         
 * @note Arrays estáticos usados para mapear caracteres acentuados para não acentuados
 * @note A string retornada deve ser libertada com free()
 * @note Suporta caracteres UTF-8 multi-byte através de strstr()
 */
char *normString(const char *str) {
    if (!str) return NULL;
    
    const char * acentos[] = {"á","à","ã","â","ä","é","è","ê","ë","í","ì","î","ï",
                            "ó","ò","õ","ô","ö","ú","ù","û","ü","ý","ÿ","ñ","ç",
                            "Á","À","Ã","Â","Ä","É","È","Ê","Ë","Í","Ì","Î","Ï",
                            "Ó","Ò","Õ","Ô","Ö","Ú","Ù","Û","Ü","Ý","Ÿ","Ñ","Ç"};
    const char * sem_acentos[] = {"a","a","a","a","a","e","e","e","e","i","i","i","i",
                                "o","o","o","o","o","u","u","u","u","y","y","n","c",
                                "A","A","A","A","A","E","E","E","E","I","I","I","I",
                                "O","O","O","O","O","U","U","U","U","Y","Y","N","C"};
    const int n_chars = sizeof(acentos)/sizeof(acentos[0]);

    //Duplicar string original
    char * resultado = strdup(str);
    if (!resultado) return NULL;

    //Para cada caracter acentuado possível
    for (int i = 0; i < n_chars; i++) {
        //Alocar string temporária
        char * temp = NULL;
        //Enquanto encontrar ocorrências do acento atual
        const char * pos = strstr(resultado, acentos[i]);
        while (pos != NULL) {
            int indice = pos - resultado;
            //Realocar string com espaço para substituição
            temp = malloc(strlen(resultado) + 1);
            if (!temp) {
                free(resultado);
                return NULL;
            }
            //Copiar parte antes do acento
            strncpy(temp, resultado, indice);
            temp[indice] = '\0';
            //Adicionar char sem acento
            strcat(temp, sem_acentos[i]);
            //Adicionar resto da string
            strcat(temp, pos + strlen(acentos[i]));
            //Atualizar resultado
            free(resultado);
            resultado = temp;
            pos = strstr(resultado, acentos[i]);
        }
    }

    //Converter para minúsculas
    char *res = strlwrSafe(resultado);
    free(resultado);
    return res;
}
 
/* Converte string para minúsculas
 *
 * @param str   String a converter
 * 
 * @return Ponteiro para a string convertida ou NULL se:
 *         - str for NULL
 *         
 * @note Usa unsigned char para suportar caracteres estendidos ASCII
 * @note Versão segura do strlwr() da string.h (para linux)
 * @note Aloca memória para a cópia
 */
char *strlwrSafe(const char *str) {
    if (!str) return NULL;

    char *copia = (char *)malloc(strlen(str) + 1);
    if (!copia) return NULL;

    strcpy(copia, str);

    char *p = copia;
    while (*p) {
        *p = tolower((unsigned char)*p);
        p++;
    }

    return copia;
} 

/* Converte string para inteiro
 *
 * @param str         String a converter
 * @param resultado   Ponteiro para guardar resultado
 *
 * @return int      1 se sucesso, 0 se erro
 *         
 * @note Usa strtol() para conversão
 * @note Valida limites MIN_INT/MAX_INT
 * @note Valida conteúdo externo ao short
 * @note Requer string terminada em \0
 */
int stringToInt(const char * str, int * resultado) {
    char * ptr_fim;
    long valor = strtol(str, &ptr_fim, 10);
    //strtol trunca em caso de overflow, daí o <= abaixo

    //Verificar erros de conversão
    if (ptr_fim == str || *ptr_fim != '\0') {
        return 0;  //Se o ponteiro de fim da str for igual ao ptr do inicio, não há um int; Se for diferente de nul char então também houve erro porque há caracteres indesejados
    }
    
    //Como strtol converte para longs e nós só queremos ints, pode ler números mais altos que o int conseguiria guardar
    if (valor >= INT_MAX || valor <= INT_MIN) {
        return 0; 
    }
    
    *resultado = (int) valor;
    return 1;
}

/* Converte string para short
 *
 * @param str         String a converter
 * @param resultado   Ponteiro para guardar resultado
 *
 * @return int      1 se sucesso, 0 se erro
 *         
 * @note Usa strtol() para conversão 
 * @note Valida limites MIN_SHORT/MAX_SHORT
 * @note Valida conteúdo externo ao int
 * @note Requer string terminada em \0
 */
int stringToShort(const char * str, short * resultado) {
    char * ptr_fim;
    long valor = strtol(str, &ptr_fim, 10);
    
    //Verificar erros de conversão
    if (ptr_fim == str || *ptr_fim != '\0') {
        return 0; 
    }
    //Verificar limites
    if (valor >= SHRT_MAX || valor <= SHRT_MIN) {
        return 0; 
    }
    
    *resultado = (short) valor;
    return 1;
}

/* Converte string para float
 *
 * @param str         String a converter
 * @param resultado   Ponteiro para guardar resultado
 *
 * @return int      1 se sucesso, 0 se erro
 *         
 * @note Usa strtof() para conversão
 * @note Não precisa validar limites (strtof já o faz)
 * @note Valida conteúdo externo ao float
 * @note Requer string terminada em \0
 */
int stringToFloat(const char * str, float * resultado) {
    char * ptr_fim;
    float valor = strtof(str, &ptr_fim);
    //Verificar erros de conversão
    if (ptr_fim == str || *ptr_fim != '\0') {
        return 0; 
    }
    //Já não há necessidade de verificar limites pois converte diretamente para float
    *resultado = (float) valor;
    return 1;
}

/**
 * @brief Converte todos os pontos de str para vírgula
 * 
 * @param str String a converter
 */
void converterPontoVirgulaDecimal(char *str) {
    size_t pos = strcspn(str, ".");
    if (str[pos] == '.') {
        str[pos] = ',';
    }
}

/**
 * @brief Substitui um caracter de uma string por outro
 * 
 * @param str String
 * @param out Caracter a substituir
 * @param in Substituto
 */
void replaceStrObj(char *str, const char out, const char in) {
    if (!str) return;
    
    char *pos = strchr(str, out);
    if (pos) {
        *pos = in;
    }
}

/* Lê uma linha completa de um ficheiro, qualquer que seja o tamanho
 *
 * @param ficheiro    Ponteiro para o ficheiro a ler (pode ser stdin)
 * @param n_linhas    Ponteiro para contador de linhas (pode ser NULL); 
 *                    incrementado para cada linha lida
 *
 * @return Uma string alocada dinamicamente ou NULL se:
 *         - O ponteiro do ficheiro é NULL
 *         - A alocação de memória falhar
 *         - O ficheiro estiver vazio
 *         
 * @note A string retornada tem de ser libertada 
 * @note O caractere de nova linha é removido da string retornada
 * @note Se a alocação de memória falhar mas alguns dados foram lidos, retorna a linha parcial
 * @note n_linhas não é inicializado
 * @note ficheiro deve estar aberto; não é fechado
 */
char * lerLinhaTxt(FILE * ficheiro, int * n_linhas) {
    if(!ficheiro) return NULL;
    char buffer[TAMANHO_INICIAL_BUFFER]; //Buffer para armazenar parte da linha
    size_t tamanho_total = 0; //Comprimento da linha; size_t pois é sempre >0 e evita conversões que podem levar a erros com outras funções
    char * linha = NULL;

    while (fgets(buffer, sizeof(buffer), ficheiro)) { //fgets le ate buffer -1 caracteres ou '\n' ou EOF
        size_t tamanho = strlen(buffer); //Calcula o tamanho do texto lido
        char * temp = realloc(linha, tamanho_total + tamanho + 1); //+1 para o nul char

        //Verificar erros na realocação/alocação
        if (!temp) {
            //Evitar ao máximo retornar NULL, pois isso terminaria o loop em carregar_dados (ou a ler qualquer ficheiro)
            if (linha) {
                linha[tamanho_total] = '\0'; //Linha incompleta mas pelo menos retorna
                if (n_linhas != NULL) (*n_linhas)++;
                return linha;
            }
            return NULL; //Caso haja um erro fatal, temos que interromper
        }
        linha = temp; //atualizar o ponteiro linha para apontar para a nova memória

        //Copiar o conteúdo lido para a linha total
        //linha + tamanho_total é um ponteiro para a posição da memória seguinte para onde a próxima parte de buffer será copiada
        memmove(linha + tamanho_total, buffer, tamanho); 
        tamanho_total += tamanho;
        //Se houver mais que uma leitura, o primeiro char da segunda leitura de fgets irá substituir o nul char, pelo que fica sempre no fim
        linha[tamanho_total] = '\0'; 

        //Verificamos se a linha está completa
        if (tamanho_total > 0 && linha[tamanho_total - 1] == '\n') { //se tudo tiver sido copiado, o ultimo caracter do buffer(e da linha também) será o '\n'
            tamanho_total--;
            linha[tamanho_total] = '\0';
            //Remover \r caso exista 
            if (tamanho_total > 0 && linha[tamanho_total - 1] == '\r') {
                tamanho_total--;
                linha[tamanho_total] = '\0';
            }
            if (n_linhas != NULL) (*n_linhas)++;
            return linha;
        }
    }

    //Linha sem '\n' mas tem conteúdo (por ex: última linha)
    if (linha && tamanho_total > 0) {
        // Remove \r final se presente
        if (linha[tamanho_total - 1] == '\r') {
            linha[tamanho_total - 1] = '\0';
        }
        if (n_linhas != NULL) (*n_linhas)++;
        return linha;
    }

    //Se chegarmos aqui é porque aconteceu algum erro ou o ficheiro está vazio
    free(linha);
    return NULL;
}

/**
 * @brief Converte os dados de codPostal para os parâmetros do Código Postal
 * 
 * @param codPostal Source (String)
 * @param zona Parametro 1 do Código Postal
 * @param local Parametro 2 do Código Postal
 * @return int 1 se sucesso, 0 se erro
 */
int converterCodPostal(const char *codPostal, short *zona, short *local) {
    if (!codPostal || !zona || !local) {
        return 0;
    }
    char extra; //guardar /n ou outros caracteres
    if (sscanf(codPostal, "%hd-%hd%c", zona, local, &extra) != 4 && extra !='\0') {
        return 0;
    }
    return 1;
}

/**
 * @brief Compara duas datas
 * 
 * @param data1 Data a comparar
 * @param data2 Data a comparar
 * @return int -1 se data1 < data2, 0 se iguais, 1 se data1 > data2
 */
int compararDatas(Data data1, Data data2) {
    //Ano
    if (data1.ano < data2.ano) return -1;
    if (data1.ano > data2.ano) return 1;
    //Mês
    if (data1.mes < data2.mes) return -1;
    if (data1.mes > data2.mes) return 1;
    //Dia
    if (data1.dia < data2.dia) return -1;
    if (data1.dia > data2.dia) return 1;
    //Hora
    if (data1.hora < data2.hora) return -1;
    if (data1.hora > data2.hora) return 1;
    //Minuto
    if (data1.min < data2.min) return -1;
    if (data1.min > data2.min) return 1;
    //Segundos
    if (data1.seg < data2.seg) return -1;
    if (data1.seg > data2.seg) return 1;
    
    //São iguais
    return 0;
}

/**
 * @brief Converte os dados de strData para data
 * 
 * @param strData Source (String)
 * @param data Destino (Data)
 * @return char* NULL se sucesso, mensagem de erro caso contrário
 */
char *converterParaData(const char *strData, Data *data) {
    if (!strData) return "\nA data fornecida é inválida('')\n";
    if (!data) return "\nO ponteiro para a data é NULL\n";

    char *temp = strdup(strData);
    converterPontoVirgulaDecimal(temp);

    if (sscanf(temp, "%hd-%hd-%hd %hd:%hd:%f", &data->dia, &data->mes, &data->ano, &data->hora, &data->min, &data->seg) != 6) {
        free(temp);
        return "\nA data não contém todos os parâmetros necessários\n";
    }
    free(temp);
    return NULL;
}

/**
 * @brief Calcula o tempo entre duas datas em minutos
 * 
 * @param data1 Ponteiro para a primeira data
 * @param data2 Ponteiro para a segunda data
 * @return float Tempo em minutos
 */
float calcularIntervaloTempo(Data *data1, Data *data2) {
    if (!data1 || !data2) return 0;

    float minutos = 0;
    
    minutos += (data2->ano - data1->ano) * 525600;    // 365 dias * 24 h * 60 min
    minutos += (data2->mes - data1->mes) * 43200;     // 30 dias * 24 h * 60 min
    minutos += (data2->dia - data1->dia) * 1440;      // 24 h * 60 min
    minutos += (data2->hora - data1->hora) * 60;      // h p/ min
    minutos += (data2->min - data1->min);             // min
    minutos += (data2->seg - data1->seg) / 60.0f;     // seconds to minutes
    
    return minutos;
}

/**
 * @brief Devolve o hash de uma string
 * 
 * @param str String, em minúsculas
 * @return int hash ou -1 se erro
 */
int hashString(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c

    int result = (int)(hash % INT_MAX);
    return result;
}

/* Elimina um ficheiro especificado
 *
 * @param nome     Nome do ficheiro a eliminar
 * @param modo     Modo de operação ('1' para mostrar erros, '0' para silencioso)
 *
 * @return 1 se sucesso, 0 se:
 *         - Nome inválido
 *         - Erro ao eliminar ficheiro
 *         
 * @note Verifica se o ficheiro existe antes de tentar eliminar
 * @note Em modo '1' mostra mensagens de erro detalhadas
 * @note Não verifica se o ficheiro é o próprio programa
 */
int deleteFile(const char *nome, const char modo) {
    if (!nome) return 0;
    //Por segurança devia ser verificado se o nome do ficheiro a eliminar não é o nosso próprio programa
    if (remove(nome) == 0) {
        if (modo == '1') printf("O ficheiro '%s' foi eliminado com sucesso.\n", nome);
        return 1;
    }
    else {
        if (modo == '1') { 
            printf("Ocorreu um erro ao eliminar o ficheiro '%s'.\n", nome);
            printf("Por favor verifique que o nome do ficheiro inclui a extensão e se está no mesmo diretório do programa.\n");
            printf("Certifique-se ainda que o ficheiro '%s' não está aberto noutro lugar.\n", nome);
        }
        return 0;
    }
}

// Parece não funcionar em linux
double obterUsoMemoria() {
    #if defined(_WIN32) || defined(_WIN64)
        PROCESS_MEMORY_COUNTERS_EX pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
            return pmc.PrivateUsage / (1024.0 * 1024.0); // bytes para MB
        }
        return -1.0;

    #else   
        struct rusage usage;
        if (getrusage(RUSAGE_SELF, &usage) == 0) {
            return usage.ru_maxrss / 1024.0; // KB para MB
        }
        return -1.0; // erro
    
    #endif
}

/**
 * @brief Cria indentação num ficheiro
 * 
 * @param indentacao Número de indentações (\t) a criar
 * @param file Ficheiro, aberto
 */
void indent(int indentacao, FILE *file) {
    if (indentacao < 0 || !file) return;

    for (int i = 0; i < indentacao; i++) fprintf(file, "\t");
}

/**
 * @brief Cria uma string com o valor do float em notação de ponto (em vez de vírgula)
 * 
 * @param valor Float a transformar
 * @param casasDecimais Número de casas decimais a ter em conta
 * @return char* Resultado (free necessário) ou NULL se erro 
 */
char *floatToStringPontoDecimal(float valor, int casasDecimais) {
    int tamanhoBuffer = 20;
    char buffer[tamanhoBuffer];

    char formato[10];
    snprintf(formato, sizeof(formato), "%%.%df", casasDecimais);
    snprintf(buffer, tamanhoBuffer, formato, valor);

    // Substituir vírgula por ponto
    for (int i = 0; buffer[i]; i++) {
        if (buffer[i] == ',') {
            buffer[i] = '.';
        }
    }

    // Alocar memória para string final
    char *resultado = malloc(strlen(buffer) + 1);
    if (!resultado) return NULL;

    strcpy(resultado, buffer);
    return resultado;
}
