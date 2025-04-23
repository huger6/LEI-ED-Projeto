#include "listagem.h"

/*
FILE *pedirListagem() {
    const char *formatos[] = {".txt", ".csv"};
    short opcao;
    char *filename;
    FILE *file = NULL;

    if (sim_nao("Deseja guardar a listagem num ficheiro?")) {
        opcao = (short) mostrarMenu(menuFormatosListagem, '0', '2') - '0';
        if (opcao == 0) {
            limpar_terminal();
            return NULL; //Sair
        }
        opcao--; //Fazemos a opcao ser igual ao indice de formatos
        strcpy(formato_selecionado, formatos[opcao]); 

        //Nome do ficheiro fica à escolha do utilizador
        do {
            printf("Nome do ficheiro a guardar: ");
            nome_ficheiro = ler_linha_txt(stdin, NULL);
            if (validar_nome_ficheiro(nome_ficheiro)) break;
            else free(nome_ficheiro);
        } while(1);

        //Garantir que a string a que vamos concatenar o nome da extensão do ficheiro tem tamanho suficiente para isso
        char * temp = realloc(nome_ficheiro, strlen(nome_ficheiro) + strlen(formatos[opcao]) + 1);
        if (!temp) { 
            free(nome_ficheiro);
            printf("Erro ao alocar memória. Por favor tente novamente mais tarde.\n");
            return NULL;
        }
        nome_ficheiro = temp;

        //strcat dá append na 2º string ao fim da 1ª.
        strcat(nome_ficheiro, formatos[opcao]);
        ficheiro = validar_ficheiro_e_abrir(nome_ficheiro);
        if (!ficheiro) {
            free(nome_ficheiro);
            return NULL; //Caso seja inválido
        }
        limpar_terminal();
        printf("O ficheiro \"%s\" foi aberto com sucesso!\n", nome_ficheiro);
        return ficheiro;
    }
    return NULL;
}

*/