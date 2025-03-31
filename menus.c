//Ficheiro com todas as funções relativas a menus

#include "menus.h"
#include "uteis.h"

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
char mostrar_menu(void (*escrever_menu)(), char min_opcao, char max_opcao) { 
    short valido = 0;
    char opcao = '0';
    do {
        limpar_terminal();
        escrever_menu();
        printf("=>Escolha uma opção: ");

        valido = scanf(" %c", &opcao);
        validacao_menus(&valido, opcao, min_opcao, max_opcao);

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

/* Menu principal da aplicação
 *
 * @return void
 *         
 * @note Opções: 0-6
 */
void menu_principal() {
    //https://desenvolvedorinteroperavel.wordpress.com/2011/09/11/tabela-ascii-completa/
    //Link da tabela ASCII completa de onde foram retirados as duplas barras do menu (a partir do 185 decimal)
    printf("╔══════════════════════════════════╗\n");
    printf("║          MENU PRINCIPAL          ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Registos                     ║\n");
    printf("║  2. Listagens                    ║\n");
    printf("║  3. Análises de Tráfego          ║\n");
    printf("║  4. Infrações de Trânsito        ║\n");
    printf("║  5. Velocidades e Estatísticas   ║\n");
    printf("║  6. Opções                       ║\n");
    printf("║  0. Sair do programa             ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}
/*
*@note Opções:0-4
*/
void menu_Registos(){
    printf("╔══════════════════════════════════╗\n");
    printf("║         MENU DE REGISTOS         ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Registar dono                ║\n");
    printf("║  2. Registar veículo             ║\n");
    printf("║  3. Registar Passagem            ║\n");
    printf("║  4. Retornar                     ║\n");
    printf("║  0. Sair do programa             ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}
/*
*@note Opções:0-5
*/
void menu_Listagens(){
    printf("╔══════════════════════════════════╗\n");
    printf("║         MENU DE LISTAGENS        ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Listar donos                 ║\n");
    printf("║  2. Listar veículos              ║\n");
    printf("║  3. Listar o Nº e nome           ║\n");
    printf("║  4. Listar matrícula             ║\n");
    printf("║  5. Retornar                     ║\n");
    printf("║  0. Sair do programa             ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}
/*
*@note Opções:0-3
*/
void menu_Trafego(){
    printf("╔══════════════════════════════════╗\n");
    printf("║        ANÁLISE DE TRAFEGO        ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Ranking de circulação        ║\n");
    printf("║  2. Ranking por marca            ║\n");
    printf("║  3. Retornar                     ║\n");
    printf("║  0. Sair do programa             ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}
/*
*@note Opções:0-3
*/
void menu_infracoes(){
    printf("╔══════════════════════════════════╗\n");
    printf("║        MENU DE INFRAÇÕES         ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Listagem de Infrações        ║\n");
    printf("║  2. Ranking de Infrações         ║\n");
    printf("║  3. Retornar                     ║\n");
    printf("║  0. Sair do programa             ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}
/*
*@note Opções:0-6
*/
void menu_velocidades(){
    printf("╔══════════════════════════════════╗\n");
    printf("║    VELOCIDADES E ESTATÍSTICAS    ║\n");
    printf("╠══════════════════════════════════╣\n");
    printf("║  1. Velocidades médias           ║\n");
    printf("║  2. Carro com maior velocidade   ║\n");
    printf("║  3. Dono com maior velocidade    ║\n");
    printf("║  4. Vel. média por Cod. Post.    ║\n");
    printf("║  5. Marca mais comum             ║\n");
    printf("║  6. Retornar                     ║\n");
    printf("║  0. Sair do programa             ║\n");
    printf("╚══════════════════════════════════╝\n\n");
}

//

//Validações

void validacao_menus(short * valido, const char opcao, const char limInf, const char limSup) { 
    if (*valido != 1) {
        printf("Entrada inválida! Introduza um número do menu (%c a %c)\n", limInf, limSup); 
        pressione_enter();
    }
    else if (opcao < limInf || opcao > limSup) { 
        *valido = 0; 
        printf("Por favor, escolha um número do menu (%c a %c).\n", limInf, limSup);
        pressione_enter();
    }
    //Verificar entradas com mais de um char
    char lixo = getchar();
    if (lixo != '\n') {
        *valido = 0;
        limpar_buffer(); //\n
        printf("Por favor, escolha um número do menu (%c a %c).\n", limInf, limSup);
        pressione_enter();
    }
}