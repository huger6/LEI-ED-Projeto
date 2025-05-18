#include "configs.h"
#include "bdados.h"
#include "uteis.h"
#include "menus.h"
#include "constantes.h"
#include "dados.h"

int autosaveON = 0;

char *donosFilename = DONOS_TXT;
char *carrosFilename = CARROS_TXT;
char *sensoresFilename = SENSORES_TXT;
char *distanciasFilename = DISTANCIAS_TXT;
char *passagensFilename = PASSAGEM_TXT;

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
    /* GUARDAR CÓPIAS DOS FICHEIROS
    printf("Quer guardar uma cópia dos dados em ficheiro .txt? (S/N) ");
    if(sim_nao()) { //Guardar cópias
        FILE * dados;
        FILE * escolar;
        char * dados_nome;
        char * escolar_nome;
        do {
            dados = NULL;
            dados_nome = NULL;
            printf("Escreva o nome do ficheiro cópia de '%s': ", DADOS_TXT);
            dados_nome = ler_linha_txt(stdin, NULL);
            if (!dados_nome) continue;

            if (!verificar_extensao(dados_nome)) {
                free(dados_nome);
                continue;
            }
            if ((dados = validar_ficheiro_e_abrir(dados_nome)) == NULL) {
                free(dados_nome);
                continue;
            }
            fclose(dados); //Neste caso, não queremos o ficheiro aberto porque guardar_dados já o abre
            break;
        } while(1);
        do {
            escolar = NULL;
            escolar_nome = NULL;
            printf("Escreva o nome do ficheiro cópia de '%s': ", SITUACAO_ESCOLAR_TXT);
            escolar_nome = ler_linha_txt(stdin, NULL);
            if (!escolar_nome) continue;

            if (!verificar_extensao(escolar_nome)) {
                free(escolar_nome);
                continue;
            }
            if ((escolar = validar_ficheiro_e_abrir(escolar_nome)) == NULL) {
                free(escolar_nome);
                continue;
            }
            fclose(escolar);
            break;
        } while(1);
        guardar_dados_txt(dados_nome, escolar_nome, bd);
    }
    */
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
 * @brief Pergunta ao utilizador quais são os nomes dos ficheiros a utilizar
 * 
 */
void setFilenames() {
    char opcao;
    do {
        //autosave(bd); //Guarda automaticamente caso autosaveON ativo
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
                    }
                    if (strcmp(donos, DONOS_TXT) == 0) {
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
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
                    }
                    if (strcmp(carros, CARROS_TXT) == 0) {
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
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
                    }
                    if (strcmp(sensores, SENSORES_TXT) == 0) {
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
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
                    }
                    if (strcmp(distancias, DISTANCIAS_TXT) == 0) {
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
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
                    }
                    if (strcmp(passagens, PASSAGEM_TXT) == 0) {
                        printf("O nome é igual ao padrão!\n");
                        pressEnter();
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
 * @brief Obtém a memória utilizada por todas as variáveis locais
 * 
 * @return size_t 
 */
size_t memUsageVarGlobais() {
    size_t mem = 0;

    mem += sizeof(autosaveON);

    mem += sizeof(donosFilename);
    mem += sizeof(carrosFilename);
    mem += sizeof(sensoresFilename);
    mem += sizeof(distanciasFilename);
    mem += sizeof(passagensFilename);

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

