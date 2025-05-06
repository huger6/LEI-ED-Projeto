//
//  main.c
//  Ficha5_ListasBiligadas_Base_cDicas
//
//  Created by Jorge Loureiro on 30/04/2025.
//

#include <locale.h>

#include "ListaBiLigada.h"

int menu_principal(void) {
	int opcao;
	printf("\n\n# MENU PRINCIPAL ------------------------------------------#");
	printf("\n| (1) Inserir um novo elemento na lista                    |");
	printf("\n| (2) Retirar um elemento (especificado pelo utilizador)   |");
	printf("\n| (3) Retirar o primeiro elemento da lista                 |");
	printf("\n| (4) Mostrar os elementos pela ordem actual               |");
	printf("\n| (5) Mostrar os elementos pela ordem inversa              |");
	printf("\n| (6) Listar n elementos anteriores a um elemento          |");
	printf("\n|   (7) Mostrar elemento actual                            |");
	printf("\n|   (8) Mostrar proximo elemento                           |");
	printf("\n|   (9) Mostrar elemento anterior                          |");
	printf("\n| (10) Mostrar o número de palavras na lista               |");
	printf("\n| (11) Inserir algumas palavras para teste...              |");
	printf("\n| -------------------------------------------------------- |");
	printf("\n|       (0) SAIR                                           |");
	printf("\n#----------------------------------------------------------#\n");
	fflush(stdin);
	do {
		printf("\n  Insira a opção : ");
		// fflush(stdin);
		scanf("%d",&opcao);
	} while (opcao<0 || opcao>11); // char????
	return opcao;
}
 
int main(void)
{
	setlocale(LC_ALL,"Portuguese");
	printf("Ficha n.º 5: Estruturas dinâmicas - Listas bi-ligadas!\n");
	ptELEMENTO act=NULL, ele_novo=NULL, ele_remover=NULL, ele_removido=NULL, ele_proc=NULL;
	ptLISTA lista = CriarListaBibligada();// Criar a lista
	for(;;) {
		//system("cls") para Windows; ou system("clear") para unix, linux ou MacOS;

		if (lista->numel == 0)
			act = NULL;
		if ((act == NULL) && (lista->numel > 0))
			act = lista->inicio;
		switch (menu_principal()) {
			case 1: // inserir elemento na lista
				ele_novo = LerElemento();
				InserirElementoOrdenadoBiligada(lista, ele_novo);
				break;
			case 2: //Retirar um elemento (especificado pelo utilizador)
				ele_remover = LerElemento();
				ele_removido = RemoverElementoBiligada(lista, ele_remover);

				if(ele_removido){
					/* CODIGO AQUI que deve:
						1. mostrar uma mensagem a indicar que o elemento foi removido;
						2. mostar a string associada ao elemento removido;
						3. libertar a memória relativa directa ou indirectamente ao elemento removido.
					 */
				}else
					/* CODIGO AQUI que deve:
						1. mostrar a mensagem de erro.
					 */
				break;
			case 3:
				/* Retirar o primeiro elemento da lista. Para isso:
					1. Invocar a função que permita retirar o 1.º elemento da lista;
					2. Se a operação em 1. tiver sucesso:
						2.1. mostrar uma mensagem a indicar que o elemento foi removido;
						2.2. mostar a string associada ao elemento removido;
						2.3. libertar a memória relativa directa ou indirectamente ao elemento removido.
					4. Se a operação em 1. não tiver sucesso:
						4.1. mostrar uma mensagem de erro.
				 */
				// CODIGO AQUI
				break;
			case 4:
				// Mostrar os elementos pela ordem actual
				if (lista->numel == 0)
					printf("Nao existem elementos na lista!\n");
				else
					MostrarBiligadaOrdenada(lista);
				break;
			case 5:
				/* Mostrar os elementos pela ordem inversa. Para isso:
				1. Verificar se existem dados a apresentar:
					1.1. se não houver, apresentar uma mensagem de erro;
					1.1. caso contrário, invocar a função apropriada que executa a funcionalidade.
				*/
				// CODIGO AQUI

				break;
			case 6:
				/* Listar os n elementos anteriores a um elemento especificado. Para isso:
				 1. Criar um nó de nome ele_proc (já declarado no início do main);
				 2. Invocar a função que vá ler os dados (a sting) para ser associado ao elemento a procurar (o nó criado);
				 3. Aceitar do utililizador o número de nós anteriores a serem apresentados, além do procurado;
				 4. Invocar a função apropriada que respinda a esta funcionalidade.
				 */
				// CODIGO AQUI
				
				break;
			case 7: // Mostrar elemento actual
				if (!act)
					printf("..."); // mostrar mensagem de erro
				else
				{
					printf("..."); // mostrar a palavra associada ao elemento actual
				}
				break;
			case 8: // Mostrar proximo elemento
				if (!act)
					printf("..."); // mostrar mensagem de erro
				else
				{
					// Testar se o próximo elemento ainda existe, ou seja, pelo menos,
					// se o próximo elemento é o último elemento da lista. Se assim for:
					printf("..."); // colocar em act o ponteiro para o próximo elemento e mostrar a palavra
								   // associada ao elemento actual (o próximo já lido).
					
					// Senão, mostrar mensagem de erro mostrando a palavra associada ao elemento actual,
					// indicando que já se está na última palavra.
				}
				break;
			case 9: // Mostrar elemento anterior
				if (!act)
					printf("..."); // mostrar mensagem de erro
				else
				{
					// testar se o elemento anterior ainda existe, ou seja se, pelo menos, o elemento anterior é o primeiro da lista - se assim for:
					printf("..."); // colocar em act o ponteiro para o elemento anterior e mostrar a string associada ao elemento actual (o anterior já lido)
					// senão, mostrar mensagem de erro mostrando a string associada ao elemento actual, indicando que já se está na primeira palavra
				}
				break;
			case 10: //Mostrar o número de palavras na lista
				printf("\nNumero palavras na lista: %d\n", NumElementosLista(lista));
				break;
			case 11:  // Inserir elementos já com palavras, para facilitar os testes
				ele_novo = CriarElementoPreenchido("verde");
				InserirElementoOrdenadoBiligada(lista, ele_novo);
				ele_novo = CriarElementoPreenchido("vermelho");
				InserirElementoOrdenadoBiligada(lista, ele_novo);
				ele_novo = CriarElementoPreenchido("azul");
				InserirElementoOrdenadoBiligada(lista, ele_novo);   
				ele_novo = CriarElementoPreenchido("Roxo"); // strcmp: atenção à ordem na tabela ASCII
				InserirElementoOrdenadoBiligada(lista, ele_novo);
				ele_novo = CriarElementoPreenchido("preto");
				InserirElementoOrdenadoBiligada(lista, ele_novo);
				ele_novo = CriarElementoPreenchido("amarelo");
				InserirElementoOrdenadoBiligada(lista, ele_novo);
				ele_novo = CriarElementoPreenchido("cor de rosa");
				InserirElementoOrdenadoBiligada(lista, ele_novo);
				ele_novo = CriarElementoPreenchido("verde claro");
				InserirElementoOrdenadoBiligada(lista, ele_novo);
				break;
			case 0:
				//libertar memoria de ele_remover
				DestruirLBiligada(lista);
				exit(0);    // fim do Programa
				break;
		}
		//system("pause");
	}
}
