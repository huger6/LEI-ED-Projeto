//
//  ListaBiLigada.c
//  Ficha5_ListasBiligadas_Base_cDicas
//
//  Created by Jorge Loureiro on 30/04/2025.
//

#include "ListaBiLigada.h"

/* Ex. 2. Redefina a função ptELEMENTO criar_elemento() de modo a adequar
		  o modo de inicialização de um nodo para utilizar numa lista bi-ligada */
/**
 * @brief Função para criar um elemento da lista biligada
 *
 * @return Retorna E - ponteiro para o elemento criado.
 */
ptNODuplo CriarElemento(void)
{
	ptNODuplo E = (ptNODuplo) malloc(sizeof(NODuplo));
	E->info = (ptPALAVRA) malloc(sizeof(PALAVRA));
	E->seg = NULL;
	E->ant = NULL;							// O ponteiro para o elemento anterior também é colocado a NULL
	return E;
}

/**
 * @brief Função que liberta toda a memória associada directa ou indirectamente ao elemento especificado como parâmetro
 *
 * @param ele_libertar - elemento a libertar.
 */
void LibertarElemento(ptELE ele_libertar)
{
	free(ele_libertar->info);
	free(ele_libertar);
}

/**
 * @brief Função que cria um novo elemento já com o texto
 *
 * @return Retorna eleNovo - ponteiro para o elemento criado, já com o texto que lhe fica associado.
 */
ptNODuplo CriarElementoPreenchido(char *plv)
{
	ptELEMENTO eleNovo = CriarElemento();
	if (eleNovo)
		strcpy(eleNovo->info->texto, plv);
	return eleNovo;
}

/**
 * @brief Função que cria um novo elemento e lê a palavra a colocar no elemento criado
 *
 * @return Retorna eleNovo - ponteiro para o elemento criado.
 */
ptNODuplo LerElemento(void)
{
	ptELEMENTO eleNovo = CriarElemento();
	ptPALAVRA PAL = LerPalavra();
	eleNovo->info = PAL;
	return eleNovo;
}


/**
 * @brief Função que cria a Lista BiLigada (o "cabeçalho" da ListaBiligada)
 *
 * @return Retorna L - ponteiro para a Lista BiLigada criada.
 */
ptListaBiligada CriarListaBibligada(void)
{
	ptListaBiligada L = (ptListaBiligada)malloc(sizeof(ListaBiligada));
	L->numel = 0;
	L->inicio = NULL;
	return L;
}

/**
 * @brief Função que elimina todos os nós (elementos) da ListaBiligada e actualiza o "cabeçalho" respectivo, ficando como se a Lista BiLigada fosse criada novamente
 *
 * @param L - ponteiro para a Lista BiLigada.
 *
 */
void DestruirLBiligada(ptLISTA L)
{
	if (!L) {
		printf("\n A Lista não está definida...");
		return ;
	}
	if (!L->inicio) {
		printf("\n Não existem elementos na lista...\n");  // não há assim quiasquer nós a eliminar
		return ;
	}

	/*
	 Dicas:
		A função é semelhante à correspondente função da Lista. Assim:
		1. Basta ir procurar a correspondente função já criada para a Lista e ver o que falta acrescenar aqui.
	    Obs. Atenção que a função correspondente da Lista vai invocar outra função que vai libertar cada nó (elemento) da Lista
	 */

	// CODIGO AQUI
	NODuplo *aux = L->inicio;
	NODuplo *P;
	while (aux){
		P = aux;
		free (aux);
		aux = P->seg;
	}
	free(L);
	L->inicio = NULL;
	L->numel = 0;
	printf("Lista destruída com sucesso!\n");
}

/**
 * @brief Função que compara dois elementos, atendendo à palavra associada a cada um deles
 *
 * @param A - ponteiro para um dos elementos
 *
 * @param B - ponteiro para o outro elemento
 *
 * @return Retorna o valor lógico devolvido pela função CompararPalavras que é invocada, tendo como argumentos cada um dos elementos, ou seja:
 * - devolve um valor positivo se a palavra do elemento A for alfabeticamente maior do que a palavra do elemento B;
 * - devolve 0, se a palavra do elemento A for alfabeticamente igual à palavra do elemento B;
 * - devolve um valor negativo,se a palavra do elemento A for alfabeticamente nemor do que a palavra do elemento B;
 *
 */
int CompararElementos(ptELE A, ptELE B)
{
	if (A == NULL && B == NULL)// Se ambos forem nulos, são considerados iguais
		return 0;
	else if (A == NULL || B == NULL)
		return -1;
	else // Comparação dos campos dos elementos. NOTA: vários campos requerem comparação campo a campo!!!
		return CompararPalavras(A->info, B->info);//compara texto do elemento A com o texto do elemento B
}

/* Ex. 3. 	Redefina a função void inserir_elemento_ordenado(ptLISTA L, ptELEMENTO ele_novo),
			de modo a suportar a atualização do apontador para o elemento anterior numa lista bi-ligada. */

/**
 * @brief Função que vai inserir de modo ordenado um novo elemento na lista BiLigada, atendendo à ordem alfabética da respectiva palavra associada
 *
 * @param L - ponteiro para a Lista BiLigada
 *
 * @param eleNovo - ponteiro para o elemento a inserir.
 *
 */
void InserirElementoOrdenadoBiligada(ptListaBiligada L, ptELEMENTO eleNovo)
{
	if (!L) return;

	if (L->inicio == NULL){ 	//lista vazia, inserir na 1.ª posição
		L->inicio = eleNovo;
	}
	else{						// se a lista não estiver vazia
		/*
		 Dicas:
			A função, naturalmente, deve ser bastante semelhante à correspondente da Lista. Assim:
			1. Ir procurar a função no projecto correspondente à utilização das Listas (ficha 2);
			2. Copiar para aqui o código da função que se perceber o que falta aqui, pois que algum dele já está aqui inserido;
			3. Como é dito na questão do enunciado, há que ter atenção que o ponteiro para o nó anterior deverá ser igualmente tratado;
			3.1. Para isso, importa perceber, antes de mais, se a lista está vazia. Caso seja o caso, bastará inserir o novo elemento como o 1.º.
				 Como se pode verificar, essa situação já está codificada na parte da função já disponibilizada.
			3.2. Agora, ter-se-á de verificar se o restante código copiado, consegue tratar as restantes situações.
				 Quando o novo elemento deve ser inserido:
				 a) antes do 1.º (caso em que act será o 1º);
				 b) entre o 1.º e qualquer outro, (act, sendo o 2.º, 3.º,..., último);
				 c) após o último.
				 Se for analisada a função correspondente da Lista simples copiada para aqui, percebe-se que nela,
				 deixa-se o act chegar a NULL, sendo essa a condição de saída, quando o novo elemento é para ser
				 inserido no fim, ou seja, não haver nenhum elemento cuja string associada seja maior do que
				 a associada ao novo elemento.
				 E disse-se também que o código a usar para inserir um novo elemento, quer fosse para inserir entre
				 b) o 1.º e 2.º, ou entre quaisquer outros 2 seguintes, ou c) mesmo no final, era o mesmo.
				 Analise-se o código da função da Lista quando se estava a tratar a inserção do novo elemento em qualquer
				 das situações anteriores (b) ou c)), que se apresenta seguidamente:
					ant->seg=ele_novo;
					ele_novo->seg=act;
				 Agora, há que tratar a situação da actualização do ponteiro para o elemento anterior. Então, o código
				 anterior teria se ser alterado, para tratar a situação b), para:
					ant->seg=ele_novo;
					ele_novo->seg=act;
					ele_novo->ant=ant;
					act->ant=ele_novo;
				 Ora, a situação estará resolvida para a situação b), mas para a c), a última linha já não vai poder
				 ser executada, pois act=NULL e, portanto, o elemento anterior ao novo último ficaria a NULL (pois, neste caso, act=NULL). Ou seja,
				 importa, agora, separar a funcionalidade b) da c). Para isso, tere-se-á de alterar a condição final do ciclo, para nunca
				 o permitir chegar a NULL e permitir separar a situação atrás sinalizada como b) e a c).
				 Ou seja:
				 3.2.1. Eliminar a condição de act=NULL;
				 3.2.2. Já não será necessário haver o ponteiro ant, pois cada elemento já tem essa informação;
				 3.2.3. Mas ter-se-á de poder discriminar entre a saída do ciclo na situação a) e b). Para isso:
					3.2.3.1. Para as situações b), p. ex., colocar av=2;
					3.2.3.2. para as situações c), p. ex., colocar av=0, mas o teste não poderá ser act == NULL, mas act-> ... == NULL;
				 3.3.1. Agora, vai tratar-se a situação a), que pode ser efectuada de forma semelhante;
				 3.3.2. Para as situaçõess tipo b), ou seja quando av=2 e, uma vez que já não há o ponteiro ant, ter-se-á que alterar
						o código para se usar apenas a informação relativa a act;
						Uma ajuda: ant->seg pode ser também act->ant->seg, certo? Para os restantes, encontrar soluções adequadas;
				 3.3.3. Já para a situação tipo c, há que ter em conta que act é o último elemento. Assim:
					3.3.3.1. Ter-se-á que actualizar o act->seg=???;
					3.3.3.2. Depois, o mesmo para os ponteiros do eleNovo.
		 */
		// CODIGO AQUI
	}
	L->numel++;
}

/* Exercício 4. Defina a função ptELEMENTO remover_elemento(ptLISTA L, ptELEMENTO ele_remover) de modo a suportar a atualização
                do apontador para o elemento anterior numa lista bi-ligada */

/**
 * @brief Função que vai remover um elemento da Lista BiLigada
 *
 * @param L - ponteiro para a Lista BiLigada
 *
 * @param eleRemover - ponteiro para o elemento que se pretende remover. Obviamente, vai procurar-se um elemento com a palavra associada a este elemento, sendo esse o removido.
 *
 */
ptELEMENTO RemoverElementoBiligada(ptLISTA L, ptELEMENTO eleRemover)
{
	if (!L || !L->inicio) return NULL;

	/*
	 Dicas:
		Mais uma vez, a função deve ser bastante semelhante à correspondente da Lista. Assim:
		1. Ir procurar a função no projecto correspondente à utilização das Listas (ficha 2);
		2. Copiar para aqui o código da função que se perceber que falta aqui, pois que algum dele já está aqui inserido;
		3. Como é dito na questão do enunciado, só há que ter atenção que o ponteiro para o nó anterior deverá ser
		   igualmente tratado e, mais uma vez, o ponteiro ant não é necessário;
		   3.1. Para isso, importa perceber que se o nó a remover for o 1.º, naturalmente, o ponteiro para o nó
				anterior deverá ficar a ...
		   3.2.Depois, há que tratar a situação do nó a remover ser qualquer um, ecxcepto o 1.º. Assim:
			   3.2.1. Na linha ant->seg=act->seg;, como já não há o ponteiro ant, ant->seg pode reescrever-se como act->ant->...
			   3.2.2. Quanto à actualização do nó seguinte ao actual (que vai ficar a seguir ao anterior ao actual),
					  ou seja o act->seg->ant, só faz sentido se o nó a remover não for o último. Assim:
					  3.2.2.1. Actualizar o act->seg->ant=..., só se act->seg for ???
		4. Se o elemento não for encontrado, naturalmente, será retornado NULL, como já acontecia para o caso da Lista simples.
	 */
	// CODIGO AQUI
	
	return NULL;
}

/* Exercício 5. Defina a função que permita remover o 1.º elemento da ListaBiLigada (usando a correspondente função da Lista), de modo a suportar a atualização do apontador
				para o elemento anterior numa lista bi-ligada. */
/**
 * @brief Função que vai remover o 1.º elemento da Lista BiLigada
 *
 * @param L - ponteiro para a Lista BiLigada
 *
 */
ptELEMENTO RemoverPrimeiroBiligada(ptLISTA L) {
	if (!L) return NULL;
	/*
	 Dicas:
		Mais uma vez, a função deve ser bastante semelhante à correspondente da Lista. Assim:
		1. Ir procurar a função no projecto correspondente à utilização das Listas (ficha 2);
		2. Copiar para aqui o código da função que se perceber que falta aqui, pois que algum dele
		   já está aqui inserido;
		3. Como é dito na questão do enunciado, só há que ter atenção que o ponteiro para o nó anterior
		   deverá ser igualmente tratado;
		   3.1. Para isso, importa perceber que se o 1.º elemento for removido, o 2.º elemento
				passa a ser o primeiro e, então o início da lista passa a ser o 2.º no, o que já está
				a ser feito na função correspondente da Lista;
		   3.2. Quanto ao anterior do novo nó inicial deverá ser...
	 */
	// CODIGO AQUI
	
	return NULL;
}

/**
 * @brief Função que vai mostar a palavra associada ao elemento passado como parâmetro
 *
 * @param eleMostrar - elemento cuja palavra associada vai ser apresentada no ecrã.
 *
 */
void MostrarElemento(ptELE eleMostrar)
{
	MostrarPalavra(eleMostrar->info);
}

/* Exercício 6. Defina uma função que mostre os elementos os elementos pela ordem actual. */
/**
 * @brief Função que vai mostar as palavras associadas a todos os elementos da Lista BiLigada
 *
 * @param L - ponteiro para a Lista BiLigada.
 *
 */
void MostrarBiligadaOrdenada(ptLISTA L)
{
	if (!L) {
		printf("\nNão está definida a lista...");
		return ;
	}
	if (!L->inicio) {
		printf("\n Não existem elementos na lista...\n");
		return ;
	}

	printf("\nPalavras na lista:\n");
	ptELE p = L->inicio;
	while(p){
		MostrarElemento(p);
		printf("\n-----------------------------\n");
		p = p->seg;
	}
}

/* Exercício 7. Defina uma função que mostre os elementos da Lista Duplamente Ligada pela ordem inversa, tirando partido do ponteiro para o elemento anterior.*/
/**
 * @brief Função que vai mostar as palavras associadas a todos os elementos da Lista BiLigada, mas por ordem algabeticamente descrescente, ou seja, iniciando nas palavras com 1.ª letra Z e terminando nas iniciadas em A
 *
 * @param L - ponteiro para a Lista BiLigada.
 *
 */
void MostrarBiligadaInversa(ptLISTA L)
{
	if (!L) {
		printf("\n Nao está definida a lista...");
		return ;
	}
	if (!L->inicio) {
		printf("\n Nao existem elementos na lista...\n");
		return ;
	}
	/*
	 Dicas:
		Mais uma vez, a função deve ser bastante semelhante à correspondente da Lista. Assim:
		1. Ir procurar a função no projecto correspondente, quando se tratas das Listas (ficha 2);
		2. O códido, tal como está no Projecto relativo à utilização das Listas funcionará também aqui, porque a
		   Lista Duplamente Ligada faz tudo o que faz a Lista simples. Neste caso, pode perfeitamente ler os
		   elementos na ordem em que estão na Lista, ou seja, 1.º, 2.º, ..., último.
		   Como se poderá verificar no código, no caso das Listas e, para relembrar, como só é permitido inciar
		   a leitura no 1.º nó e ir lendo os seguintes, foram apresentadas 2 soluções:
		   a) Na 1.ª, era usada uma função recursiva que iniciava a leitura no 1.º nó e, antes de mostar a
			  string associada ao nó no ecrã, invocava novamente a função e, assim, ia lendo os sucessivos nós,
			  mas deixando em suspenso a respectiva apresentação no ecrã.
			  Quando já não houvesse mais nós a ler, o processo recursivo terminava e, então, a string do último
			  nó lido era apresentada e, depois, a correspondente ao nó anterior e assim sucessivamente,
			  até terminar no primeiro;
		   b) Na 2.ª, era criado um vector de nós iguais a cada nó da Listae com um número de elementos (nós)
			  igual ao da Lista. Depois, eram lidos os sucessivos nós e copiados para cada um dos elementos de
			  índice sucessivamente crescentes do vector. Finalmente, como a lista já tinha sido integralmente
			  para o vector, bastava iniciar a leitura do vector pelo último elemento e apresentar a string
			  associada ao nó lá armazenado (que seria o último) e, depois, processeguir o processo com
			  a leitura do elemento anterior e apresentar a string e assim  sucessivamente, até se chegar ao 1.º.
		3. Agora, pode aproveitar-se a nova funcionalidade das Listas Duplamente Ligadas: depois de ir
		   até ao último nó, apresentar a palavra associada e, depois, ir lendo sucessivamente os anteriores,
		   apresentando no ecrã a string associada, até chegar-se ao 1.º e, depois chegando-se a NULL,
		   a operação termina.
		   É aplicando este algoritmo que se proceda à alteração da função copiada. É certo que ficará bastante
		   mais simples do que qualquer das 2 criadas para o caso das Listas simples. Eis uma vantagem das
		   Listas Duplamente Ligadas face às Listas Simplesmente Ligadas.
	 */

	// CODIGO AQUI
	NODuplo *P = L->inicio;
	NODuplo *Q;
	while (P){
		Q = P;
		P = Q->seg;
	}
	while (Q){
		printf("INFO: %s", Q->info->texto);
		Q = Q->ant;
	}


}

/* Exercício Extra */
/**
 * @brief Função que vai pesquisar um elemento, através da palavra associada
 *
 * @param L - ponteiro para a Lista BiLigada;
 *
 * @param elePesquisar - o elemento, cujo nome associado se pretende encontrar na lista duplamente ligada.
 *
 * @return Retorna o ponteiro do elemento encontrado com o nome associado a elePesquisar, se algum elementos com esse nome for encontrado, ou NULL, se nenhum for encontrado.
 *
 */
ptELEMENTO PesquisarLBiligada(ptLISTA L, ptNODuplo elePesquisar)
{
	ptELEMENTO p = L->inicio;
	while (p)
	{
		if (CompararElementos(p,  elePesquisar) == 0)
			return p;
		p = p->seg;
	}
	return NULL;
}

/* Exercício 8 - Defina uma função que mostre os dados dos n nós que antecedem uma dada palavra de uma lista duplamente ligada. */
/**
 * @brief Função que vai mostrar o nome associado aos n (o 3.º parâmetro da função) elementos anteriores ao nome associado ao elemento cujo ponteiro é o 2.º parâmetro
 *
 * @param L - ponteiro para a Lista BiLigada;
 *
 * @param elePesquisar - o elemento, cujo nome associado se pretende encontrar na lista duplamente ligada que, se for encontrado, será o 1.º a ser mostrado, seguido dos nomes associados aos n elementos anteriores
 *
 * @param n - número de elementos anteriores àquele cujo nome é igual ao nome associado ao ponteiro que é o 2.º parâmetro, cujos nomes vão ser apresentados.
 *
 */
void MostrarNAnterioresBiligada(ptLISTA L, ptELEMENTO elePesquisar, int n)
{
	if (!L) {
		printf("\n Nao est· definida a lista...");
		return ;
	}
	if (!L->inicio) {
		printf("\n Nao existem elementos na lista...\n");
		return ;
	}
	/*
	 Dicas:
		Neste caso, não há nenhuma função com funcionalidade semelhante à que é aqui solicitada.
		No entanto, pode-se tentar encontrar o algoritmo que permita responder à funcionalidade requerida.
		Ele poderá ser descrito, informalmente, da seguinte forma:
		a1. Procurar o elmento a pesquisar e, para isto:
			Como já existe uma função que faz a pesquisa de uma elemento (a função PesquisarLBiligada),
			bastará invocar esta função para receber o nó da Lista BiLigada com o nome associado igual
			ao elePesquisar ou NULL, se não existir nenhum nó associado a esse nome;
		a2. Na função principal, depois de invocada a função anterior e recebido o ponteiro:
			a2.1. Caso o ponteiro recebido não seja nulo (significando que o dado a procurar existe):
				  a2.1.1. Bastará mostrar a sting associada ao elemento encontrado;
				  a2.1.2. Continuar a ler os n nós anteriores apresentando a string associada a cada no.
						  Para isto, será natural usar um ciclo, obviamente, e a funcionalidade nova de
						  poderem ler-se os sucessivos nós anteriores.
			a2.2. Caso o ponteirio recebido seja nulo, mostrar mensagem de erro apropriada.
	 */
	// CODIGO AQUI

}

/* Exercício Extra */
/**
 * @brief Função que vai retornar o número de elementos que estão na Lista BiLigada
 *
 * @param L - ponteiro para a Lista BiLigada.
 *
 */
int NumElementosLista(ptLISTA L)
{
	if (!L ||!L->inicio) return 0;
	// return L->numel;
	int n = 0;
	ptELEMENTO p = L->inicio;

	while (p) {
		n++;
		p = p->seg;
	}
	return n;
}

