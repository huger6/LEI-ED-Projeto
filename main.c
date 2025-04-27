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
    -Guilherme Pais, NºMEC 30007;
    -Hugo Afonso, NºMEC 30032;
    -Mateus Silva, NºMEC 29989;

Todas as versões do programa estão disponíveis no seguinte repositório do github:
https://github.com/huger6/ProjetoED

Para compilar MATEUS (versão antiga compilador):  gcc -Wall -Wextra -g -O0 -std=c2x -o "filename" main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c

Para compilar em Windows, usar:
	gcc -Wall -Wextra -g -O0 -std=c23 -o (**FILENAME**) main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c


	Testado com o compilador GGC em C23, no Windows 11 Home 23H2 (64bits)

------Ainda não foi testado em Linux------
Para compilar em Linux, usar:
	gcc -std=c2x -Wall -Wextra -o (**FILENAME**) main.c uteis.c validacoes.c sensores.c passagens.c menus.c structsGenericas.c dono.c distancias.c dados.c carro.c bdados.c configs.c -D_XOPEN_SOURCE=700

	Garantir que estamos a usar gcc13 (C23) - Testado na versão 13.1.0
	Testado em Linux Ubuntu 20.04.6 LTS
*/


// Criar uma opção chamada "Atualizar os dados do condutor", que serviria para o efeito do Carro ser introduzido antes do dono, nesse caso, após o
// dono ser inserido, seria chamada e ela iria procurar pelo dono e encontrar o seu ponteiro, ou então pedir os dados nessa vez
// Listagens pode ter espaço ou enter para avançar para o fim(espaço)
// Listagens (modificar funcao printLista e dict de modo a aceitar um arg funcao printar em ficheiro e file, de modo a que se esses existam, seja também printado em ficheiro)
// testar mem usage e registar

int main(void) {
    limpar_terminal();
    colocarTerminalUTF8();
    data_atual();
    srand(time(NULL));

    Bdados *bd = (Bdados *)malloc(sizeof(Bdados));
    
    // Primeira iteração do programa
    if (faseInstalacao(CONFIG_TXT, '0') == 1) {
        inicializarBD(bd);
        if (!carregarDadosTxt(bd, NULL, NULL, NULL, NULL, NULL, NULL)) {
            // Tentar carregar dos ficheiros de backup
            // Talvez dar um aviso e dizer para colocar os ficheiros backup no diretório com o nome x
            // Libertar memória na função de carregar dados de acordo com o que foi alocado
            exit(EXIT_FAILURE);
        }
        // Abrir o ficheiro flag (não é aberto antes para evitar ter de o fechar, em caso de erro)
		(void) faseInstalacao(CONFIG_TXT, '1');
        (void) guardarDadosBin(bd, AUTOSAVE_BIN); // Guardar os dados em binário, caso o utilizador decida sair do programa forçadamente
    }
    else {
        // Carregar binário
        // Ler os ficheiros .bin no diretório e mostrar ao user para escolher
        // Caso não seja possível, usar um simples pedido do nome do ficheiro
        if (!carregarDadosBin(bd, AUTOSAVE_BIN)) {
            // Tentar carregar backups

            reset(NULL);
        }
    }
    
    the_architect(bd);
    (void) guardarDadosBin(bd, AUTOSAVE_BIN);
    freeTudo(bd); 
    printf("Guardei com sucesso!\n\n");
    return EXIT_SUCCESS;
}