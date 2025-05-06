//
//  Palavra.c
//  Ficha5_ListasBiligadas_Base_cDicas
//
//  Created by Jorge Loureiro on 30/04/2025.
//

#include "Palavra.h"

/**
 * @brief Função que permite ao utilizador especificar uma palavra
 *
 * @return Retorna P - ponteiro para a string com a palavra inserida pelo utilizador
 */
ptPALAVRA LerPalavra(void)
{
	ptPALAVRA P = (ptPALAVRA)malloc(sizeof(PALAVRA));
	if(!P) return NULL;
	printf("Digite uma palavra: ");
	scanf("%s", P->texto);
	return P;
}

/**
 * @brief Função que permite comprar 2 strings
 *
 * @param P1 - ponteiro para a 1.ª string a ser objecto dse comparação
 *
 * @param P2 - ponteiro para a 2.ª string (a ser comparada com a 1.ª)
 *
 * @return Se a string P1 e P2 existirem, é invocada a função strcmp da biblioteca string.h, sendo retornado o valor recebido dessa função; caso contrário, é retornado 0.
 *
 */
int CompararPalavras(PALAVRA *P1, PALAVRA *P2)
{
	if (P1 && P2)
		return strcmp(P1->texto, P2->texto);
	else
		return 0;
}

/**
 * @brief Função que vai apresentar no ecrã, uma palavra que lhe é passada pelo ponteiro P (por referência ao valor)
 *
 * @param P - ponteiro para a string a ser apresentada no ecrã
 *
 */

void MostrarPalavra(PALAVRA *P)
{
	if (P) printf("\t|%s|\n", P->texto);

}
