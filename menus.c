//Ficheiro com todas as funções relativas a menus

#include "menus.h"
#include "configs.h"
#include "uteis.h"
#include "dono.h"
#include "passagens.h"

/* Mostra menu e processa entrada do utilizador
 *
 * @param escrever_menu   Ponteiro para função que mostra um menu específico
 * @param min_opcao       Valor mínimo aceite do menu
 * @param max_opcao       Valor máximo aceite do menu
 *
 * @return Opção escolhida ou '0' se erro
 *         
 * @note Limpa terminal antes de mostrar menu com limpar_terminal()
 * @note Valida entrada via validacao_menus()
 * @note Só sai quando adquire uma entrada válida
 */
char mostrarMenu(void (*escrever_menu)(), char min_opcao, char max_opcao) { 
    int valido = 0;
    char opcao = '0';
    do {
        limpar_terminal();
        escrever_menu();
        printf("\n Escolha uma opção! ");

        opcao = (char) getKeyStroked();
        valido = validacaoMenus(opcao, min_opcao, max_opcao);

        if (valido == 1) {
            return opcao;
        }
    } while (valido == 0);

    return '0';
}

/*
As seguintes funções que começam por "menu" servem apenas para printar o menu
Nas notas das funções estão os limites de cada menu
*/

/**
 * @brief Mostra o menu principal
 * 
 * @note Opções: 0-8
 */
void menuPrincipal() {
    printf("╔══════════════════════════════════╗\n");
    printf("║           MENU PRINCIPAL         ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Gestão de Donos              ║\n");
    printf("║  2. Gestão de Veículos           ║\n");
    printf("║  3. Passagens/Viagens            ║\n");
    printf("║  4. Estatísticas e Rankings      ║\n");
    printf("║  5. Infrações                    ║\n");
    printf("║  6. Análise de Dados             ║\n");
    printf("║  7. Exportação                   ║\n");
    printf("║  8. Opções                       ║\n");
    printf("║  0. Sair                         ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o menu dos donos
 * 
 * @note Opções: 0-3
 */
void menuDonos() {
    printf("╔═════════════════════════════════════╗\n");
    printf("║           GESTÃO DE DONOS           ║\n");
    printf("╠═════════════════════════════════════╣\n");
    printf("║  1. Registar dono                   ║\n");
    printf("║  2. Listar donos (alfabeticamente)  ║\n");
    printf("║  3. Listar donos (por NIF)          ║\n");
    printf("║  0. Voltar ao menu anterior         ║\n");
    printf("╚═════════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o menu dos veículos
 * 
 * @note Opções: 0-4
 */
void menuVeiculos() {
    printf("╔════════════════════════════════════════════╗\n");
    printf("║             GESTÃO DE VEÍCULOS             ║\n");
    printf("╠════════════════════════════════════════════╣\n");
    printf("║  1. Registar veículo                       ║\n");
    printf("║  2. Mudar proprietário do veículo          ║\n");
    printf("║  3. Listar veículos                        ║\n");
    printf("║  4. Veículos por período de circulação     ║\n");
    printf("║  0. Voltar ao menu anterior                ║\n");
    printf("╚════════════════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o submenu da listagem de veículos
 * 
 * @note Submenu de menuVeiculos (menu dos veículos)
 * @note Opções: 0-4
 */
void menuListagemVeiculos() {
    printf("╔══════════════════════════════════╗\n");
    printf("║      LISTAGEM DE VEÍCULOS        ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Mostrar todos                ║\n");
    printf("║  2. Mostrar por matrícula        ║\n");
    printf("║  3. Mostrar por marca            ║\n");
    printf("║  4. Mostrar por modelo           ║\n");
    printf("║  0. Voltar ao menu anterior      ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o menu das passagens
 * 
 * @note Opções: 0-2
 */
void menuPassagens() {
    printf("╔═════════════════════════════════╗\n");
    printf("║       PASSAGENS/VIAGENS         ║\n");
    printf("╠═════════════════════════════════╣\n");
    printf("║  1. Registar viagem             ║\n");
    printf("║  2. Ver todas as viagens        ║\n");
    printf("║  0. Voltar ao menu anterior     ║\n");
    printf("╚═════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o menu das estatísticas e rankings
 * 
 * @note Opções: 0-4
 */
void menuEstatisticas() {
    printf("╔═══════════════════════════════════════╗\n");
    printf("║     ESTATÍSTICAS E RANKINGS           ║\n");
    printf("╠═══════════════════════════════════════╣\n");
    printf("║  1. Ranking KMS por período de tempo  ║\n");
    printf("║  2. Ranking KMS por marca             ║\n");
    printf("║  3. Ranking de infrações              ║\n");
    printf("║  4. Velocidades médias                ║\n");
    printf("║  0. Voltar ao menu anterior           ║\n");
    printf("╚═══════════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o submenu das velocidades médias
 * 
 * @note Submenu de menuEstatisticas (menu das estatísticas)
 * @note Opções: 0-4
 */
void menuVelocidadesMedias() {
    printf("╔═══════════════════════════════════╗\n");
    printf("║        VELOCIDADES MÉDIAS         ║\n");
    printf("╠═══════════════════════════════════╣\n");
    printf("║  1. Geral (cada condutor)         ║\n");
    printf("║  2. Marca mais rápida             ║\n");
    printf("║  3. Condutor mais rápido          ║\n");
    printf("║  4. Média por código postal       ║\n");
    printf("║  0. Voltar ao menu anterior       ║\n");
    printf("╚═══════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o menu das infrações
 * 
 * @note Opções: 0-2
 */
void menuInfracoes() {
    printf("╔════════════════════════════════════╗\n");
    printf("║             INFRAÇÕES              ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║  1. Veículos com infrações         ║\n");
    printf("║  2. Infrações por período de tempo ║\n");
    printf("║  0. Voltar ao menu anterior        ║\n");
    printf("╚════════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o menu de análise de dados
 * 
 * @return void
 * 
 * @note Opções: 0-2
 */
void menuAnaliseDados() {
    printf("╔══════════════════════════════════╗\n");
    printf("║         ANÁLISE DE DADOS         ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Memória ocupada              ║\n");
    printf("║  2. Marca automóvel mais comum   ║\n");
    printf("║  0. Voltar ao menu anterior      ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}

/**
 * @brief Mostra o menu de exportação
 * 
 * @note Opções: 0-3
 */
void menuExportacao() {
    printf("╔══════════════════════════════════╗\n");
    printf("║           EXPORTAÇÃO             ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Exportar dados para CSV      ║\n");
    printf("║  2. Exportar dados para XML      ║\n");
    printf("║  3. Exportar dados para HTML     ║\n");
    printf("║  0. Voltar ao menu anterior      ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}

/**
 * @brief Escreve o menu das opções
 * 
 * @note Opções: 0-4
 */
void menuOpcoes() {
    printf("╔════════════════════════════════════════╗\n");
    printf("║                 OPÇÕES                 ║\n");
    printf("╠════════════════════════════════════════╣\n");
    printf("║  1. Ativar/Desativar autosave          ║\n");
    printf("║  2. Definir nº elementos por listagem  ║\n");
    printf("║  3. Repor definições                   ║\n");
    printf("║  4. Guia de utilização                 ║\n");
    printf("║  0. Voltar ao menu anterior            ║\n");
    printf("╚════════════════════════════════════════╝\n\n");
}

/**
 * @brief Escreve o menu das opções de listagem disponíveis
 * 
 * @note Opções: 0-2
 */
void menuFormatosListagem() {
    printf("╔══════════════════════════╗\n");
    printf("║   FORMATOS DISPONÍVEIS   ║\n");
    printf("╠══════════════════════════╣\n");
    printf("║  1. .txt                 ║\n");
    printf("║  2. .csv                 ║\n");
    printf("║  0. Sair                 ║\n");
    printf("╚══════════════════════════╝\n\n");
}

/**
 * @brief Escreve o menu sobre os nomes dos ficheiros a carregar
 * 
 * @note Opções: 0-5
 */
void menuNomeFicheirosDados() {
    printf("=====================================================\n");
    printf("                 NOME DOS FICHEIROS\n");
    printf("  Nota: Incluir o nome do diretório (Default Dados/)\n");
    printf("=====================================================\n");
    printf(" %-15s | %s\n", "1. Donos",      donosFilename      ? donosFilename      : DONOS_TXT "(Default)");
    printf(" %-15s | %s\n", "2. Carros",     carrosFilename     ? carrosFilename     : CARROS_TXT "(Default)");
    printf(" %-15s | %s\n", "3. Sensores",   sensoresFilename   ? sensoresFilename   : SENSORES_TXT "(Default)");
    printf(" %-15s | %s\n", "4. Distancias", distanciasFilename ? distanciasFilename : DISTANCIAS_TXT "(Default)");
    printf(" %-15s | %s\n", "5. Passagens",  passagensFilename  ? passagensFilename  : PASSAGEM_TXT "(Default)");
    printf(" %-15s | %s\n", "0. Concluir",  "(Default)");
    printf("=====================================================\n");
}

/**
 * @brief Um guia meramente informativo sobre como usar o programa
 * 
 */
void menuGuiaUtilizacao() {
    limpar_terminal();
    printf("╔═════════════════════════════════════════════════╗\n");
    printf("║            GUIA DE UTILIZAÇÃO                   ║\n");
    printf("╠═════════════════════════════════════════════════╣\n");
    printf("║ * Gestão de Donos:                              ║\n");
    printf("║   - Registar e listar proprietários             ║\n");
    printf("║                                                 ║\n");
    printf("║ * Gestão de Veículos:                           ║\n");
    printf("║   - Registar e consultar veículos               ║\n");
    printf("║   - Pesquisar por matrícula/marca/modelo        ║\n");
    printf("║                                                 ║\n");
    printf("║ * Passagens/Viagens:                            ║\n");
    printf("║   - Registar passagens em portagens             ║\n");
    printf("║   - Consultar histórico de viagens              ║\n");
    printf("║                                                 ║\n");
    printf("║ * Exportação:                                   ║\n");
    printf("║   - Exportar dados para CSV/XML/HTML            ║\n");
    printf("║                                                 ║\n");
    printf("║ * Autosave:                                     ║\n");
    printf("║   - Guarda dados automaticamente                ║\n");
    printf("║   - Pode tornar o menu principal mais lento     ║\n");
    printf("╚═════════════════════════════════════════════════╝\n\n");
    pressEnter();
}

//Processar os menus

/**
 * @brief Coração do programa. Chama os menus com as suas funcionalidades
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void the_architect(Bdados *bd) {
    char opcao;
    do {
        autosave(bd); //Guarda automaticamente caso autosaveON ativo
        opcao = mostrarMenu(menuPrincipal, '0', '8');
        switch(opcao) {
            case '0':
                limpar_terminal();
                if (!sim_nao("Tem a certeza que quer sair do programa?")) {
                    opcao = '1'; //Forçar o loop a continuar
                    //Não há problemas porque no ínicio do loop pedimos sempre o menu
                    continue;
                }
                break;
            case '1':
                processarMenuDonos(bd);
                break;
            case '2':
                processarMenuVeiculos(bd);
                break;
            case '3':
                processarMenuPassagens(bd);
                break;
            case '4':
                processarMenuEstatisticas(bd);
                break;
            case '5': 
                processarMenuInfracoes(bd);
                break;
            case '6': 
                processarMenuAnaliseDados(bd);
                break;
            case '7': 
                processarMenuExportacao(bd);
                break;
            case '8': 
                processarMenuOpcoes(bd);
                break;
            default:
                opcao = '0'; //Sair caso haja erro
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades e/ou submenus associados aos Donos
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void processarMenuDonos(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuDonos, '0', '3');
        switch(opcao) {
            case '0':
                break;
            case '1':
                registarDono(bd);
                break;
            case '2':
                // Listar donos (alfabeticamente)
                listarDonosAlfabeticamente(bd);
                break;
            case '3':
                // Listar donos (por NIF)
                listarDonosNIF(bd);
                break;
            default:
                opcao = '0';
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades e/ou submenus associados aos veículos
 * 
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void processarMenuVeiculos(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuVeiculos, '0', '4');
        switch(opcao) {
            case '0':
                break;
            case '1':
                registarCarro(bd);
                break;
            case '2':
                mudarDonoCarro(bd);
                break;
            case '3':
                processarMenuListagemVeiculos(bd);
                break;
            case '4':
                // Veículos por período de circulação
                listarCarrosPorPeriodoTempo(bd);
                break;
            default:
                opcao = '0'; 
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades relativas às listagens de veículos
 * 
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 * @note Submenu do Menu de Veículos
 */
void processarMenuListagemVeiculos(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuListagemVeiculos, '0', '4');
        switch(opcao) {
            case '0':
                break;
            case '1':
                // Mostrar todos
                listarCarrosTodos(bd);
                break;
            case '2':
                // Mostrar por matrícula
                listarCarrosPorMatricula(bd);
                break;
            case '3':
                // Mostrar por marca
                listarCarrosPorMarca(bd);
                break;
            case '4':
                // Mostrar por modelo
                listarCarrosPorModelo(bd);
                break;
            default:
                opcao = '0';
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades e/ou submenus associados às passagens
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void processarMenuPassagens(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuPassagens, '0', '2');
        switch(opcao) {
            case '0':
                break;
            case '1':
                registarViagem(bd);
                break;
            case '2':
                // Ver todas as passagens
                listarViagensTodas(bd);
                break;
            default:
                opcao = '0'; 
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades e/ou submenus associados às estatísticas
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void processarMenuEstatisticas(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuEstatisticas, '0', '4');
        switch(opcao) {
            case '0':
                break;
            case '1':
                // Ranking total KMS por período de tempo
                rankingKMSPeriodoTempo(bd);
                break;
            case '2':
                // Ranking total KMS por marca
                rankingKMSMarca(bd);
                break;
            case '3': 
                // Ranking de infrações
                rankingInfracoes(bd);
                break;
            case '4': 
                processarMenuVelocidades(bd);
                break;
            default:
                opcao = '0';
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades relativas às velocidades médias
 * @param bd Ponteiro para a base de dados
 * 
 * @note Submenu do menu de estatísticas
 */
void processarMenuVelocidades(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuVelocidadesMedias, '0', '4');
        switch(opcao) {
            case '0':
                break;
            case '1':
                // Geral (cada condutor)
                listarDonosVelocidadesMedias(bd);
                break;
            case '2':
                // Marca mais rápida
                limpar_terminal();
                char *marcaMaisRapida = obterMarcaMaisVelocidadeMedia(bd);

                if (marcaMaisRapida) printf("A marca de automóveis mais rápida (velocidade média) é a \"%s\".\n\n", marcaMaisRapida);
                else printf("Ainda não há dados sobre nenhum carro!\n\n");

                pressEnter();
                break;
            case '3': 
                // Condutor mais rápido
                limpar_terminal();
                Dono *condutorMaisRapido = obterCondutorMaisVelocidadeMedia(bd);

                if (condutorMaisRapido) printf("O condutor mais rápido (velocidade média) é \"%s\".\n\n", condutorMaisRapido->nome);
                else printf("Ainda não há dados sobre nenhum condutor!\n\n");

                pressEnter();
                break;
            case '4': 
                // Média por código postal
                velocidadeMediaPorCodPostal(bd);
                break;
            default:
                opcao = '0';
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades e/ou submenus associados às infrações
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void processarMenuInfracoes(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuInfracoes, '0', '2');
        switch(opcao) {
            case '0':
                break;
            case '1':
                // Veículos com infrações
                listarCarrosComInfracoes(bd);
                break;
            case '2':
                // Infrações por período de tempo
                listarInfracoesPorPeriodoTempo(bd);
                break;
            default:
                opcao = '0'; 
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades e/ou submenus associados à análise de dados
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void processarMenuAnaliseDados(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuAnaliseDados, '0', '2');
        switch(opcao) {
            case '0':
                break;
            case '1':
                limpar_terminal();
                size_t mem = memUsageTudo(bd);
                printf("Memória ocupada pela base de dados:\n\n");
                printf("Unidade           | Valor\n");
                printf("------------------|-----------------\n");
                printf("Bytes (B)         | %zu\n", mem);
                printf("Kilobytes (kB)    | %.2f\n", mem / 1000.0);
                printf("Megabytes (MB)    | %.2f\n", mem / 1000000.0);
                printf("Kibibytes (KiB)   | %.2f\n", mem / 1024.0);
                printf("Mebibytes (MiB)   | %.2f\n\n", mem / (1024.0 * 1024.0));

                pressEnter();
                break;
            case '2':
                limpar_terminal();
                char *marcaMaisComum = obterMarcaMaisComum(bd->carrosMarca);

                if (marcaMaisComum) printf("A marca de automóveis mais comum é a \"%s\".\n\n", marcaMaisComum);
                else printf("Ainda não há dados sobre nenhum carro!\n\n");

                pressEnter();
                break;
            default:
                opcao = '0';
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades e/ou submenus associados à exportação dos dados
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void processarMenuExportacao(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuExportacao, '0', '3');
        switch(opcao) {
            case '0':
                break;
            case '1':
                exportarTudoCSV(bd, "donos.csv", "carros.csv", "sensores.csv", "distancias.csv", "viagens.csv");
                break;
            case '2':
                exportarTudoXML(bd, "database.xml");
                break;
            case '3':
                // Exportar dados para HTML
                exportarTudoHTML(bd, "donos.html", "carros.html", "sensores.html", "distancias.html", "viagens.html");
                break;
            default:
                opcao = '0';
                break;
        }
    } while(opcao != '0');
}

/**
 * @brief Chama as funcionalidades e/ou submenus associados às opções
 * @param bd Ponteiro para a base de dados
 * 
 * @return void
 */
void processarMenuOpcoes(Bdados *bd) {
    char opcao;
    do {
        opcao = mostrarMenu(menuOpcoes, '0', '4');
        switch(opcao) {
            case '0': break;
            case '1':
                limpar_terminal();
                if (autosaveON == 0) {
                    autosaveON = 1;
                    printf("O autosave foi ativado.\n");
                }
                else {
                    autosaveON = 0;
                    printf("O autosave foi desativado.\n");
                }
                pressEnter();
                break;
            case '2':
                setPausaListagem();
                break;
            case '3':
                reset(bd);
                break;
            case '4':
                menuGuiaUtilizacao();
                break;
            default: 
                opcao = '0';
                break;
        }
    } while (opcao != '0');
}


//Validações

/**
 * @brief Valida a opçção escolhida num menu
 * 
 * @param opcao Opção
 * @param limInf Limite inferior do menu
 * @param limSup Limite superior do menu
 * 
 * @return 1 se válido, 0 se inválido
 */
int validacaoMenus(const char opcao, const char limInf, const char limSup) { 
    if (opcao < limInf || opcao > limSup) { 
        printf("Por favor, escolha um número do menu (%c a %c).\n", limInf, limSup);
        pressEnter();
        return 0;
    }
    return 1;
}

