//
//  ListaBiLigada.h
//  Ficha5_ListasBiligadas_Base_cDicas
//
//  Created by Jorge Loureiro on 30/04/2025.
//

#ifndef ListaBiLigada_h
#define ListaBiLigada_h

#include "Palavra.h"

#include <stdio.h>
#include <stdlib.h>



//estruturas

/* Exercício 1
 * O nó da estrutura BiLigada é semelhante ao da Lista, apenas difere do facto de ter um ponteiro adicional para o nó anterior,
 * permitindo assim "navegar" não só para a "frente", atrvés do ponteiro seg, mas também para o anterior, através do novo ponteiro ant.
 */
typedef struct no_duplo
{
	ptPALAVRA info;                                   	// Ponteiro para a estrutura Palavra
	struct no_duplo *seg;								// Ponteiro para o próximo elemento da lista dupl. ligada
	struct no_duplo *ant;								// Ponteiro para o elemento anterior da lista dupl. ligada
														// (diferença em relação ao nó da lista)
}NODuplo, *ptNODuplo, Elemento, *ptELEMENTO, *ptELE;

/*
 * Já o "cabeçalho" da Lista BiLigada é igual ao da Lista.
 */
typedef struct
{
	char lingua[MAX_LINGUA];
	int numel;
	ptNODuplo inicio;
}ListaBiligada, *ptListaBiligada, *ptLISTA;

// prototipos de funções
ptNODuplo CriarElemento(void); // ex2
void LibertarElemento(ptELE ele_libertar);
ptNODuplo CriarElementoPreenchido(char *plv); // extra
ptNODuplo LerElemento(void); // extra
ptListaBiligada CriarListaBibligada(void);
void DestruirLBiligada(ptLISTA L); //extra
int CompararElementos(ptELE A, ptELE B); //extra
void InserirElementoOrdenadoBiligada(ptListaBiligada L, ptELEMENTO eleNovo); // ex3
ptELEMENTO RemoverElementoBiligada(ptLISTA L, ptELEMENTO eleRemover); // ex4
ptELEMENTO RemoverPrimeiroBiligada(ptLISTA L); //ex5
void MostrarElemento(ptELE eleMostrar); //extra
void MostrarBiligadaOrdenada(ptLISTA L);//ex 6
void MostrarBiligadaInversa(ptLISTA L); //ex7
ptELEMENTO PesquisarLBiligada(ptLISTA L, ptNODuplo elePesquisar); // extra
void MostrarNAnterioresBiligada(ptLISTA L, ptELEMENTO elePesquisar, int n); //ex8
int NumElementosLista(ptLISTA L); //extra

#endif /* ListaBiLigada_h */
