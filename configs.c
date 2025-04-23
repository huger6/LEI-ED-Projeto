#include "configs.h"
#include "bdados.h"
#include "uteis.h"

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
        if ((SetConsoleOutputCP(CP_UTF8) == 0)||SetConsoleCP(CP_UTF8) == 0) {
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

