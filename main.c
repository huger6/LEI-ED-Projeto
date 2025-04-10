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

Para compilar MATEUS (versão antiga compilador):  gcc -Wall -Wextra -g -O0 -std=c2x -o "filename" main.c uteis.c validacoes.c sensores.c passagens.c menus.c listaGenerica.c dono.c distancias.c dados.c carro.c bdados.c

Para compilar em Windows, usar:
	gcc -Wall -Wextra -g -O0 -std=c23 -o (**FILENAME**) main.c uteis.c validacoes.c sensores.c passagens.c menus.c listaGenerica.c dono.c distancias.c dados.c carro.c bdados.c


	Testado com o compilador GGC em C23, no Windows 11 Home 23H2 (64bits)

------Ainda não foi testado em Linux------
Para compilar em Linux, usar:
	gcc -std=c2x -Wall -Wextra -o (**FILENAME**) main.c funcoes.c -D_XOPEN_SOURCE=700

	Garantir que estamos a usar gcc13 (C23) - Testado na versão 13.1.0
	Testado em Linux Ubuntu 20.04.6 LTS
*/

// TODO: Modificar separarParametros

int main(void) {
    limpar_terminal();
    colocar_terminal_utf8();
    data_atual();
    srand(time(NULL));

    Bdados *bd = (Bdados *)malloc(sizeof(Bdados));
    inicializarBD(bd);
    
    if (!carregarDadosTxt(bd, NULL, NULL, NULL, NULL, NULL, NULL)) {
        printf("Ocorreu um erro a carregar os dados para memória!\n");
        return EXIT_FAILURE;
    }

    the_architect(bd);

    freeTudo(bd);
    return EXIT_SUCCESS;
}