#include "configs.h"
#include "uteis.h"
#include "menus.h"
#include "structsGenericas.h"
#include "carro.h"
#include "dono.h"
#include "distancias.h"
#include "passagens.h"
#include "sensores.h"
#include "dados.h"


/*
Aplicação desenvolvida no âmbito da cadeira "Estruturas de Dados" da ESTGV, em 2024-2025.
Membros do grupo:
    -Hugo Afonso, NºMEC 30032;
    -Mateus Silva, NºMEC 29989;
    -Guilherme Pais, NºMEC 30007;

Todas as versões do programa estão disponíveis no seguinte repositório do github:
https://github.com/huger6/ProjetoED

Para compilar em Windows, usar:
	gcc -Wall -Wextra -g -O0 -std=c23 -o **FILENAME** main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c

	Testado com o compilador GGC em C23, no Windows 11 Home 23H2 (64bits)

Para compilar em Linux, usar:
	gcc -std=c2x -Wall -Wextra -o **FILENAME** main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c -D_XOPEN_SOURCE=700

	Testado em Linux Ubuntu 20.04.6 LTS com gcc13 (C23) na versão 13.1.0
*/

int main(void) {
    limpar_terminal();
    colocarTerminalUTF8();
    data_atual();
    srand(time(NULL));

    Bdados *bd = (Bdados *)malloc(sizeof(Bdados));
    
    // Primeira iteração do programa
    if (faseInstalacao(CONFIG_TXT, '0') == 1) {
        inicializarBD(bd);
        setFilenames(); // Obter os nomes dos ficheiros a carregar
        if (!carregarDadosTxt(bd, donosFilename,  carrosFilename, sensoresFilename, distanciasFilename, passagensFilename, LOGS_TXT)) {
            printf("Ocorreu um erro a carregar os dados dos ficheiro .txt dados.\n");
            printf("Por favor, certifique-se de que os ficheiros estão na mesma localização do programa (%s)!\n", __FILE__);
            pressEnter();
            exit(EXIT_FAILURE);
        }
        // Abrir o ficheiro flag (não é aberto antes para evitar ter de o fechar, em caso de erro)
		(void) faseInstalacao(CONFIG_TXT, '1');
        (void) guardarDadosBin(bd, AUTOSAVE_BIN);
    }
    else {
        // Carregar binário
        if (!carregarDadosBin(bd, AUTOSAVE_BIN)) {
            printf("Ocorreu um erro a carregar os dados do ficheiro de autosave (\"%s\").\n", AUTOSAVE_BIN);
            // Tentar carregar um ficheiro à escolha do user
            if (sim_nao("Deseja carregar um ficheiro .bin diferente?")) {
                inicializarBD(bd); // Necessário porque carregar dados guarda e liberta os dados atuais
                if (!carregarDadosBinFicheiro(&bd)) {
                    printf("\nO programa será reiniciado. Siga as instruções.\n");
                    reset(bd);
                }
                (void) guardarDadosBin(bd, AUTOSAVE_BIN);
            }
            else {
                printf("\nO programa será reiniciado. Siga as instruções.\n");
                reset(bd);
            }
        }
    }
    
    the_architect(bd);
    
    limpar_terminal();

    if(guardarDadosBin(bd, AUTOSAVE_BIN)) {
        printf("Os dados foram guardados com sucesso!\n");
    }
    else {
        printf("Ocorreu um erro ao guardar os dados!\n");
    }
    freeTudo(bd); 
    return EXIT_SUCCESS;
}