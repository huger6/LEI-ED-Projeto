#include "configs.h"
#include "bdados.h"
#include "uteis.h"
#include "menus.h"
#include "constantes.h"
#include "dados.h"

int autosaveON = 0;
int pausaListagem = PAUSA_LISTAGEM;

char *donosFilename = DONOS_TXT;
char *carrosFilename = CARROS_TXT;
char *sensoresFilename = SENSORES_TXT;
char *distanciasFilename = DISTANCIAS_TXT;
char *passagensFilename = PASSAGEM_TXT;

char *donosExportacaoFilename = DONOS;
char *carrosExportacaoFilename = CARROS;
char *sensoresExportacaoFilename = SENSORES;
char *distanciasExportacaoFilename = DISTANCIAS;
char *viagensExportacaoFilename = VIAGENS;
char *databaseExportacaoXML = DATABASE_XML;

/* Coloca terminal para UTF-8
 *
 * @return void
 *         
 * @note Windows: CP_UTF8 + locale pt para números
 * @note Unix: locale para tudo
 * @note Avisa se configuração falhar
 */
void colocarTerminalUTF8() {
    #ifdef _WIN32
        //SetConsoleOutputCP retorna 0 se houver um erro
        if ((SetConsoleOutputCP(CP_UTF8) == 0)||(SetConsoleCP(CP_UTF8) == 0)) {
            printf("Ocorreu um erro ao configurar o terminal do Windows para UTF-8.\n");
            printf("A aplicação irá continuar. Desformatação será visível. Para resolver, reinicie a aplicação.\n");
        }
        setlocale(LC_NUMERIC, "Portuguese"); //Apenas afeta os números , ou seja, muda a notação de floats de '.' para ','
    #else
        setlocale(LC_ALL, "pt_PT.UTF-8");
    #endif
}

/* Verifica o estado de instalação do programa através de um ficheiro flag
 *
 * @param flag    Nome do ficheiro de configuração
 * @param abrir   Modo de operação ('1' para criar ficheiro flag, '0' para apenas verificar a sua presença)
 *
 * @return 1 se:
 *         - Primeira execução do programa
 *         - Erro ao aceder ao ficheiro
 *         0 se programa já estiver instalado
 *         
 * @note O ficheiro é essencial para o correto funcionamento do programa
 */
int faseInstalacao(const char *flag, const char abrir) {
    if (!flag) return 1;

    FILE *ficheiro = fopen(flag, "r");
    // Verificar primeira abertura
    if (!ficheiro && abrir == '0') return 1;
    // Abrir o ficheiro quando pedido (na primeira abertura)
    else if (!ficheiro && abrir == '1') {
        ficheiro = fopen(flag, "w");
        fprintf(ficheiro, "NÃO ELIMINAR ESTE FICHEIRO SOB QUALQUER CIRCUNSTÂNCIA!");
        fclose(ficheiro);
        return 0; // A partir deste momento já passamos a ter o ficheiro aberto
    }
    // Só chega aqui se o ficheiro já existir previamente
    fclose(ficheiro);
    return 0;
}

/**
 * @brief Restaura o programa ao seu estado original
 * 
 * @param bd Base de dados (passar NULL caso já tenha sido libertada)
 * 
 * @note Liberta memória para toda a estrutura
 */
void reset(Bdados *bd) {
    limpar_terminal();

    printf("Todos os dados serão excluídos e será necessário carregar os ficheiros .txt.\n");
    printf("O ficheiro de dados \"%s\" será eliminado!\n", AUTOSAVE_BIN);

    if (!sim_nao("Tem a certeza que quer continuar?")) return;

    // Eliminar o ficheiro de instalação
    if (!deleteFile(CONFIG_TXT, '1')){
        printf("Por favor, elimine o ficheiro '%s' manualmente.\n", CONFIG_TXT);
        printf("Caso não o faça, o programa pode malfuncionar.\n\n");
    }

    // Verificar se há dados binários
    FILE *dados = fopen(AUTOSAVE_BIN, "r");
    if (dados) {
        fclose(dados);
        //Eliminar logs
        if (!deleteFile(AUTOSAVE_BIN, '1')) {
            printf("Por favor, elimine o ficheiro '%s' manualmente.\n", AUTOSAVE_BIN);
            printf("Caso não o faça, o programa pode malfuncionar.\n");
        }
    }
    
    printf("\nPara concluir a reposição do programa, a aplicação será fechada.\n");
    pressEnter();

    freeTudo(bd);
    exit(EXIT_SUCCESS);
}

/**
 * @brief Define o número de elementos a mostrar de cada vez
 * 
 */
void setPausaListagem() {
    limpar_terminal();
    int num = 0;
    do {
        pedirInt(&num, "Insira o número de elementos a mostrar de cada vez numa listagem: ", NULL);
        if (num < 0) {
            printf("Entrada inválida!\n\n");
            pressEnter();
            continue;
        }
        break;
    } while(1);
    pausaListagem = num;
}

/**
 * @brief Pergunta ao utilizador quais são os nomes dos ficheiros a utilizar
 * 
 */
void setFilenames() {
    char opcao;
    do {
        opcao = mostrarMenu(menuNomeFicheirosDados, '0', '5');
        switch(opcao) {
            case '0':
                limpar_terminal();
                printf("\n\nA carregar dados...\n\n");
                break;
            case '1':
                do {
                    printf("Escreva o novo nome para o ficheiro dos donos (extensão .txt incluída): ");
                    char *donos = lerLinhaTxt(stdin, NULL);
                    if (!donos) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(donos, DONOS_TXT) == 0) {
                        free(donos);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(donos)) {
                        free(donos);
                        pressEnter();
                        continue;
                    }
                    donosFilename = donos;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '2':
                do {
                    printf("Escreva o novo nome para o ficheiro dos carros (extensão .txt incluída): ");
                    char *carros = lerLinhaTxt(stdin, NULL);
                    if (!carros) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(carros, CARROS_TXT) == 0) {
                        free(carros);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(carros)) {
                        free(carros);
                        pressEnter();
                        continue;
                    }
                    carrosFilename = carros;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '3':
                do {
                    printf("Escreva o novo nome para o ficheiro dos sensores (extensão .txt incluída): ");
                    char *sensores = lerLinhaTxt(stdin, NULL);
                    if (!sensores) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(sensores, SENSORES_TXT) == 0) {
                        free(sensores);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(sensores)) {
                        free(sensores);
                        pressEnter();
                        continue;
                    }
                    sensoresFilename = sensores;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '4':
                do {
                    printf("Escreva o novo nome para o ficheiro das distâncias (extensão .txt incluída): ");
                    char *distancias = lerLinhaTxt(stdin, NULL);
                    if (!distancias) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(distancias, DISTANCIAS_TXT) == 0) {
                        free(distancias);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(distancias)) {
                        free(distancias);
                        pressEnter();
                        continue;
                    }
                    distanciasFilename = distancias;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '5': 
                do {
                    printf("Escreva o novo nome para o ficheiro das passagens (extensão .txt incluída): ");
                    char *passagens = lerLinhaTxt(stdin, NULL);
                    if (!passagens) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(passagens, PASSAGEM_TXT) == 0) {
                        free(passagens);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(passagens)) {
                        free(passagens);
                        pressEnter();
                        continue;
                    }
                    passagensFilename = passagens;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            default:
                opcao = '0';
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Permite ao utilizador escolher que nome utilizar nos ficheiros de exportação da base de dados
 * 
 */
void setExportacaoFilenames() {
    char opcao;
    do {
        opcao = mostrarMenu(menuNomeFicheirosExportacao, '0', '6');
        limpar_terminal();
        switch(opcao) {
            case '0':
                break;
            case '1':
                do {
                    printf("Nome do ficheiro a usar na exportação dos donos (sem extensão): ");
                    char *donos = lerLinhaTxt(stdin, NULL);
                    if (!donos) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(donos, DONOS) == 0) {
                        free(donos);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(donos)) {
                        free(donos);
                        pressEnter();
                        continue;
                    }
                    donosExportacaoFilename = donos;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '2':
                do {
                    printf("Nome do ficheiro a usar na exportação dos carros (sem extensão): ");
                    char *carros = lerLinhaTxt(stdin, NULL);
                    if (!carros) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(carros, CARROS) == 0) {
                        free(carros);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(carros)) {
                        free(carros);
                        pressEnter();
                        continue;
                    }
                    carrosExportacaoFilename = carros;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '3':
                do {
                    printf("Nome do ficheiro a usar na exportação dos sensores (sem extensão): ");
                    char *sensores = lerLinhaTxt(stdin, NULL);
                    if (!sensores) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(sensores, SENSORES) == 0) {
                        free(sensores);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(sensores)) {
                        free(sensores);
                        pressEnter();
                        continue;
                    }
                    sensoresExportacaoFilename = sensores;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '4':
                do {
                    printf("Nome do ficheiro a usar na exportação das distâncias (sem extensão): ");
                    char *distancias = lerLinhaTxt(stdin, NULL);
                    if (!distancias) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(distancias, DISTANCIAS) == 0) {
                        free(distancias);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(distancias)) {
                        free(distancias);
                        pressEnter();
                        continue;
                    }
                    distanciasExportacaoFilename = distancias;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '5': 
                do {
                    printf("Nome do ficheiro a usar na exportação das viagens (sem extensão): ");
                    char *viagens = lerLinhaTxt(stdin, NULL);
                    if (!viagens) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(viagens, VIAGENS) == 0) {
                        free(viagens);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(viagens)) {
                        free(viagens);
                        pressEnter();
                        continue;
                    }
                    viagensExportacaoFilename = viagens;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            case '6': 
                do {
                    printf("Nome do ficheiro a usar na exportação das viagens (sem extensão): ");
                    char *dbXML = lerLinhaTxt(stdin, NULL);
                    if (!dbXML) {
                        printf("Ocorreu um erro a ler o nome do ficheiro.\n");
                        pressEnter();
                        continue;
                    }
                    if (strcmp(dbXML, DATABASE_XML) == 0) {
                        free(dbXML);
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
                        continue;
                    }
                    if (!validarNomeFicheiro(dbXML)) {
                        free(dbXML);
                        pressEnter();
                        continue;
                    }
                    databaseExportacaoXML = dbXML;
                    break;
                } while(1);
                printf("O nome foi alterado com sucesso!\n");
                pressEnter();
                break;
            default:
                opcao = '0';
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Liberta a memória alocada para os nomes dos ficheiros, caso tenham sido alocados
 * 
 */
void freeFilenames() {
    // Verificar pelo nome
    if (strcmp(donosFilename, DONOS_TXT) != 0) {
        free(donosFilename);
        donosFilename = DONOS_TXT;
    }
    if (strcmp(carrosFilename, CARROS_TXT) != 0) {
        free(carrosFilename);
        carrosFilename = CARROS_TXT;
    }
    if (strcmp(sensoresFilename, SENSORES_TXT) != 0) {
        free(sensoresFilename);
        sensoresFilename = SENSORES_TXT;
    }
    if (strcmp(distanciasFilename, DISTANCIAS_TXT) != 0) {
        free(distanciasFilename);
        distanciasFilename = DISTANCIAS_TXT;
    }
    if (strcmp(passagensFilename, PASSAGEM_TXT) != 0) {
        free(passagensFilename);
        passagensFilename = PASSAGEM_TXT;
    }
}

/**
 * @brief Liberta a memória alocada para o nome dos ficheiros de exportação
 * 
 */
void freeExportacaoFilenames() {
    // Verificar pelo nome
    if (strcmp(donosExportacaoFilename, DONOS) != 0) {
        free(donosExportacaoFilename);
        donosExportacaoFilename = DONOS;
    }
    if (strcmp(carrosExportacaoFilename, CARROS) != 0) {
        free(carrosExportacaoFilename);
        carrosExportacaoFilename = CARROS;
    }
    if (strcmp(sensoresExportacaoFilename, SENSORES) != 0) {
        free(sensoresExportacaoFilename);
        sensoresExportacaoFilename = SENSORES;
    }
    if (strcmp(distanciasExportacaoFilename, DISTANCIAS) != 0) {
        free(distanciasExportacaoFilename);
        distanciasExportacaoFilename = DISTANCIAS;
    }
    if (strcmp(viagensExportacaoFilename, VIAGENS) != 0) {
        free(viagensExportacaoFilename);
        viagensExportacaoFilename = VIAGENS;
    }
    if (strcmp(databaseExportacaoXML, DATABASE_XML) != 0) {
        free(databaseExportacaoXML);
        databaseExportacaoXML = DATABASE_XML;
    }
}

/**
 * @brief Obtém a memória utilizada por todas as variáveis locais
 * 
 * @return size_t 
 */
size_t memUsageVarGlobais() {
    size_t mem = 0;

    mem += sizeof(autosaveON);
    mem += sizeof(pausaListagem);

    mem += sizeof(donosFilename);
    mem += sizeof(carrosFilename);
    mem += sizeof(sensoresFilename);
    mem += sizeof(distanciasFilename);
    mem += sizeof(passagensFilename);

    mem += sizeof(donosExportacaoFilename);
    mem += sizeof(carrosExportacaoFilename);
    mem += sizeof(sensoresExportacaoFilename);
    mem += sizeof(distanciasExportacaoFilename);
    mem += sizeof(viagensExportacaoFilename);
    mem += sizeof(databaseExportacaoXML);

    if (strcmp(donosFilename, DONOS_TXT) != 0) {
        mem += strlen(donosFilename) + 1;
    }
    if (strcmp(carrosFilename, CARROS_TXT) != 0) {
        mem += strlen(carrosFilename) + 1;
    }
    if (strcmp(sensoresFilename, SENSORES_TXT) != 0) {
        mem += strlen(sensoresFilename) + 1;
    }
    if (strcmp(distanciasFilename, DISTANCIAS_TXT) != 0) {
        mem += strlen(distanciasFilename) + 1;
    }
    if (strcmp(passagensFilename, PASSAGEM_TXT) != 0) {
        mem += strlen(passagensFilename) + 1;
    }

    if (strcmp(donosExportacaoFilename, DONOS) != 0) {
        mem += strlen(donosExportacaoFilename) + 1;
    }
    if (strcmp(carrosExportacaoFilename, CARROS) != 0) {
        mem += strlen(carrosExportacaoFilename) + 1;
    }
    if (strcmp(sensoresExportacaoFilename, SENSORES) != 0) {
        mem += strlen(sensoresExportacaoFilename) + 1;
    }
    if (strcmp(distanciasExportacaoFilename, DISTANCIAS) != 0) {
        mem += strlen(distanciasExportacaoFilename) + 1;
    }
    if (strcmp(viagensExportacaoFilename, VIAGENS) != 0) {
        mem += strlen(viagensExportacaoFilename) + 1;
    }
    if (strcmp(databaseExportacaoXML, DATABASE_XML) != 0) {
        mem += strlen(databaseExportacaoXML) + 1;
    }

    return mem;
}

/**
 * @brief Guarda automaticamente os dados se autosaveON == 1
 * 
 * @param bd Base de dados
 */
void autosave(Bdados *bd) {
    if (!bd) return;

    if (autosaveON == 1) {
        (void) guardarDadosBin(bd, AUTOSAVE_BIN);
    }
}

