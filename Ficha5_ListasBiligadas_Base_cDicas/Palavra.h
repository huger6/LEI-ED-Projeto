//
//  Palavra.h
//  Ficha5_ListasBiligadas_Base_cDicas
//
//  Created by Jorge Loureiro on 30/04/2025.
//

#ifndef Palavra_h
#define Palavra_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_PALAVRA 100 // Tamanho máximo para o nome
#define MAX_LINGUA 3
#define LINGUA "PT"

//estruturas de dados
typedef struct Palavra
{
	char texto[MAX_PALAVRA];
} *ptPALAVRA, PALAVRA;

//prototipos de funções
ptPALAVRA LerPalavra(void);
int CompararPalavras(PALAVRA *P1, PALAVRA *P2);
void MostrarPalavra(PALAVRA *P);

#endif /* Palavra_h */
