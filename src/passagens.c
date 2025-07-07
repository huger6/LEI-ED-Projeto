#include "passagens.h"
#include "bdados.h"
#include "validacoes.h"
#include "configs.h"

/**
 * @brief Aloca memória para a passagem 
 * 
 * @param bd Base de dados
 * @param idSensor Id do Sensor
 * @param codVeiculo Código do véiculo
 * @param date Data de passagem
 * @param tipoRegisto Tipo de registo
 * @return Passagem* Passagem ou NULL se erro
 */
Passagem *obterPassagem(int idSensor, Data date, char tipoRegisto) {
	Passagem *pas = (Passagem *)malloc(sizeof(Passagem));
	if (!pas) return NULL;

	//Id Sensor
	pas->idSensor = idSensor;
	pas->data = date;
	pas->tipoRegisto = tipoRegisto;

	return pas;
}

/**
 * @brief Insere uma viagem lida na base de dados
 * 
 * @param bd Base de dados
 * @param entrada Passagem de entrada
 * @param saida Passagem de saída
 * @param codVeiculo Código do veículo
 * @return int 1 se sucesso, 0 se erro
 */
int inserirViagemLido(Bdados *bd, Passagem *entrada, Passagem *saida, int codVeiculo) {
	if (!entrada || !saida) return 0;

	Viagem *v = (Viagem *)malloc(sizeof(Viagem));
	if (!v) {
		freePassagem(entrada);
		freePassagem(saida);
		return 0;
	}

	v->entrada = entrada;
	v->saida = saida;

	void *temp = (void *)&codVeiculo;
	Carro *ptrCarro = (Carro *)searchDict(bd->carrosCod, (void *)temp, compChaveCarroCod, compCodCarro, hashChaveCarroCod);
	if (!ptrCarro) {
		freePassagem(entrada);
		freePassagem(saida);
		free(v);
		return 0;
	}
	v->ptrCarro = ptrCarro;
	// Inserir o ponteiro de cada viagem no carro
	if (!v->ptrCarro->viagens) {
		v->ptrCarro->viagens = criarLista();
		if (!v->ptrCarro->viagens) {
			freePassagem(entrada);
			freePassagem(saida);
			free(v);
			return 0;
		}
	}
	getStatsViagem(bd, v);

	if (!addInicioLista(v->ptrCarro->viagens, (void *)v)) {
		freePassagem(entrada);
		freePassagem(saida);
		free(v);
		return 0;
	}

	if (!addInicioLista(bd->viagens, (void *)v)) {
		freePassagem(entrada);
		freePassagem(saida);
		free(v);
		return 0;
	}

	return 1;
}

/**
 * @brief Liberta a memória associada a uma passagem
 * 
 * @param passagem Passagem
 */
void freePassagem(void *passagem) {
	Passagem *obj = (Passagem *)passagem;
	free(obj);
}

/**
 * @brief Liberta a memória ocupada por uma viagem
 * 
 * @param viagem Viagem
 */
void freeViagem(void *viagem) {
	Viagem *obj = (Viagem *)viagem;
	freePassagem((void *)obj->entrada);
	freePassagem((void *)obj->saida);
	free(obj);
}

/**
 * @brief Guarda uma viagem em ficheiro binário
 * 
 * @param viagem Viagem
 * @param file Ficheiro binário, aberto
 */
void guardarViagemBin(void *viagem, FILE *file) {
	if (!viagem || !file) return;

	Viagem *x = (Viagem *)viagem;
	fwrite(&x->kms, sizeof(float), 1, file);
	fwrite(&x->tempo, sizeof(float), 1, file);
	fwrite(&x->velocidadeMedia, sizeof(float), 1, file);
	guardarPassagemBin((void *)x->entrada, file);
	guardarPassagemBin((void *)x->saida, file);
	fwrite(&x->ptrCarro->codVeiculo, sizeof(int), 1, file);
}

/**
 * @brief Lê uma viagem para memória
 * 
 * @param file Ficheiro binário, aberto
 * @return void* Viagem ou NULL se erro
 * 
 * @note Cria uma estrutura Carro para guardar o codVeiculo, deve ser libertada posteriormente e obtido o respetivo ponteiro
 */
void *readViagemBin(FILE *file) {
	if (!file) return NULL;

	Viagem *x = (Viagem *)malloc(sizeof(Viagem));
	if (!x) return NULL;

	fread(&x->kms, sizeof(float), 1, file);
	fread(&x->tempo, sizeof(float), 1, file);
	fread(&x->velocidadeMedia, sizeof(float), 1, file);
	x->entrada = (Passagem *)readPassagemBin(file);
	if (!x->entrada) {
		free(x);
		return NULL;
	}
	x->saida = (Passagem *)readPassagemBin(file);
	if (!x->saida) {
		freePassagem(x->entrada);
		free(x);
		return NULL;
	}
	x->ptrCarro = (Carro *)malloc(sizeof(Carro));
	if (!x->ptrCarro) {
		freePassagem(x->entrada);
		freePassagem(x->saida);
		free(x);
		return NULL;
	}
	fread(&x->ptrCarro->codVeiculo, sizeof(int), 1, file);

	return (void *)x;
}

/**
 * @brief Guarda a passagem em ficheiro binário
 * 
 * @param passagem Passagem
 * @param file Ficheiro binário, aberto
 */
void guardarPassagemBin(void *passagem, FILE *file) {
	if (!passagem || !file) return;

	Passagem *x = (Passagem *)passagem;

	fwrite(&x->data.ano, sizeof(short), 1, file);
	fwrite(&x->data.mes, sizeof(short), 1, file);
	fwrite(&x->data.dia, sizeof(short), 1, file);
	fwrite(&x->data.hora, sizeof(short), 1, file);
	fwrite(&x->data.min, sizeof(short), 1, file);
	fwrite(&x->data.seg, sizeof(float), 1, file);

	fwrite(&x->idSensor, sizeof(int), 1, file);
	fwrite(&x->tipoRegisto, sizeof(char), 1, file);
}

/**
 * @brief Lê uma passagem para memória
 * 
 * @param file Ficheiro binário, aberto
 * @return void* Passagem ou NULL se erro
 * 
 * @note Cria uma estrutura Carro para guardar o codVeiculo, deve ser libertada posteriormente e obtido o respetivo ponteiro
 */
void *readPassagemBin(FILE *file) {
	if (!file) return NULL;

	Passagem *x = (Passagem *)malloc(sizeof(Passagem));
	if (!x) return NULL;

	fread(&x->data.ano, sizeof(short), 1, file);
	fread(&x->data.mes, sizeof(short), 1, file);
	fread(&x->data.dia, sizeof(short), 1, file);
	fread(&x->data.hora, sizeof(short), 1, file);
	fread(&x->data.min, sizeof(short), 1, file);
	fread(&x->data.seg, sizeof(float), 1, file);

	fread(&x->idSensor, sizeof(int), 1, file);
	fread(&x->tipoRegisto, sizeof(char), 1, file);

	return (void *)x;
}

/**
 * @brief Obter e colocar na estrutura de Viagem os dados estatísticos da mesma
 * 
 * @param bd Base de dados
 * @param v Viagem 
 */
void getStatsViagem(Bdados *bd, Viagem *v) {
    v->kms = bd->distancias->matriz[(v->entrada->idSensor-1) * bd->distancias->nColunas + v->saida->idSensor-1];
    v->tempo = calcularIntervaloTempo(&v->entrada->data, &v->saida->data); //min
	if (v->tempo != 0) {
		v->velocidadeMedia = v->kms / (v->tempo / 60.0f);
	}
}

/**
 * @brief Mostra uma viagem
 * 
 * @param viagem Viagem a mostrar
 */
void printViagem(void *viagem, FILE *file) {
	if (!viagem) return;

	Viagem *v = (Viagem *)viagem;

	fprintf(file, "Código do veículo: %d\n", v->ptrCarro ? v->ptrCarro->codVeiculo : 0);
	fprintf(file, "Matrícula do veículo: %s\n", v->ptrCarro ? v->ptrCarro->matricula : "n/a");

	// Entrada
	fprintf(file, "Passagem de entrada:\n");
	fprintf(file, "\tSensor: %d\n", v->entrada->idSensor);
	fprintf(file, "\tData: %hd-%hd-%hdT%hd:%hd:%.3f\n", v->entrada->data.dia, v->entrada->data.mes, v->entrada->data.ano,
		v->entrada->data.hora, v->entrada->data.min, v->entrada->data.seg);
	// Saída
	fprintf(file, "Passagem de saída:\n");
	fprintf(file, "\tSensor: %d\n", v->saida->idSensor);
	fprintf(file, "\tData: %hd-%hd-%hdT%hd:%hd:%.3f\n", v->saida->data.dia, v->saida->data.mes, v->saida->data.ano,
		v->saida->data.hora, v->saida->data.min, v->saida->data.seg);

	fprintf(file, "Tempo decorrido: %.3f\n", v->tempo);
	fprintf(file, "Distância percorrida: %.2f\n", v->kms);
	fprintf(file, "Velocidade Média: %.2f\n", v->velocidadeMedia);
}

/**
 * @brief Mostra uma viagem em formato CSV
 * 
 * @param viagem Viagem a mostrar
 * @param indentacao Indentanção no início
 * @param file Ficheiro .csv, aberto
 */
void printViagemXML(void *viagem, int indentacao, FILE *file) {
	if (!viagem || indentacao < 0 || !file) return;

	Viagem *v = (Viagem *)viagem;

	indent(indentacao, file);
	fprintf(file, "<viagem>\n");
	//CodVeiculo
	indent(indentacao + 1, file);
	fprintf(file, "<codVeiculo>%d</codVeiculo>\n", v->ptrCarro->codVeiculo);
	//Entrada
	indent(indentacao + 1, file);
	fprintf(file, "<entrada>\n");
    indent(indentacao + 2, file);
    fprintf(file, "<idSensor>%d</idSensor>\n", v->entrada->idSensor);
    indent(indentacao + 2, file);
    fprintf(file, "<data>%hd-%hd-%hdT%hd:%hd:%f</data>\n", v->entrada->data.dia, v->entrada->data.mes, v->entrada->data.ano,
                v->entrada->data.hora, v->entrada->data.min, v->entrada->data.seg);
    indent(indentacao + 2, file);
    fprintf(file, "<tipoRegisto>%c</tipoRegisto>\n", v->entrada->tipoRegisto);
	indent(indentacao + 1, file);
	fprintf(file, "</entrada>\n");
	//Saída
	indent(indentacao + 1, file);
	fprintf(file, "<saida>\n");
    indent(indentacao + 2, file);
    fprintf(file, "<idSensor>%d</idSensor>\n", v->saida->idSensor);
    indent(indentacao + 2, file);
    fprintf(file, "<data>%hd-%hd-%hdT%hd:%hd:%.3f</data>\n", v->saida->data.dia, v->saida->data.mes, v->saida->data.ano,
                v->saida->data.hora, v->saida->data.min, v->saida->data.seg);
    indent(indentacao + 2, file);
    fprintf(file, "<tipoRegisto>%c</tipoRegisto>\n", v->saida->tipoRegisto);
	indent(indentacao + 1, file);
	fprintf(file, "</saida>\n");
	//Stats
	indent(indentacao + 1, file);
	fprintf(file, "<tempo>%.3fmin</tempo>\n", v->tempo);
	indent(indentacao + 1, file);
	fprintf(file, "<distancia>%.2fkm</distancia>\n", v->kms);
	indent(indentacao + 1, file);
	fprintf(file, "<velMedia>%.2fkm</velMedia>\n", v->kms);
	
	indent(indentacao, file);
	fprintf(file, "</viagem>\n");
}

/**
 * @brief Escreve os headers das viagens em formato CSV
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderViagensCSV(FILE *file) {
	if (!file) return;

	fprintf (file, "Código do Veículo, Entrada-sensor, Entrada-data, Entrada-tipo, Saída-sensor, Saída-data, Saída-tipo, Tempo, Distancia, Velocidade Média\n");
}

/**
 * @brief Mostra uma viagem em formato CSV
 * 
 * @param viagem Viagem a mostrar
 * @param file Ficheiro .csv, aberto
 */
void printViagemCSV(void *viagem, FILE *file) {
	if ( !viagem || !file) return;

	Viagem *v = (Viagem *)viagem;

	char *segEntrada = floatToStringPontoDecimal(v->entrada->data.seg, 3);
	char *segSaida = floatToStringPontoDecimal(v->saida->data.seg, 3);
	char *tempoStr = floatToStringPontoDecimal(v->tempo, 3);
	char *kmsStr = floatToStringPontoDecimal(v->kms, 2);
	char *velMediaStr = floatToStringPontoDecimal(v->velocidadeMedia, 2);

	fprintf(file,"%d, %d, %hd-%hd-%hdT%hd:%hd:%s, %c, %d, %hd-%hd-%hdT%hd:%hd:%s, %c, %s, %s, %s\n",
		v->ptrCarro ? v->ptrCarro->codVeiculo : -1, v->entrada->idSensor, v->entrada->data.dia, v->entrada->data.mes, v->entrada->data.ano,
		v->entrada->data.hora, v->entrada->data.min, segEntrada ? segEntrada : "n/a", v->entrada->tipoRegisto, 
		v->saida->idSensor, v->saida->data.dia, v->saida->data.mes, v->saida->data.ano, v->saida->data.hora, v->saida->data.min, 
		segSaida ? segSaida : "n/a", v->saida->tipoRegisto, tempoStr ? tempoStr : "n/a", kmsStr ? kmsStr : "n/a", velMediaStr ? velMediaStr : "n/a");

	if (segEntrada) free(segEntrada);
	if (segSaida) free(segSaida);
	if (tempoStr) free(tempoStr);
	if (kmsStr) free(kmsStr);
	if (velMediaStr) free(velMediaStr);
}

/**
 * @brief Escreve os headers das viagens em formato CSV
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderViagensTXT(FILE *file) {
	if (!file) return;

	fprintf (file, "Código do Veículo\tEntrada-sensor\tEntrada-data\tEntrada-tipo\tSaída-sensor\tSaída-data\tSaída-tipo\tTempo\tDistancia\tVelocidade Média\n");
}

/**
 * @brief Mostra uma viagem em formato CSV
 * 
 * @param viagem Viagem a mostrar
 * @param file Ficheiro .csv, aberto
 */
void printViagemTXT(void *viagem, FILE *file) {
	if ( !viagem || !file) return;

	Viagem *v = (Viagem *)viagem;

	fprintf(file,"%d\t%d\t%hd-%hd-%hdT%hd:%hd:%.3f\t%c\t%d\t%hd-%hd-%hdT%hd:%hd:%.3f\t%c\t%.3f\t%.2f\t%.2f\n",
		v->ptrCarro ? v->ptrCarro->codVeiculo : -1, v->entrada->idSensor, v->entrada->data.dia, v->entrada->data.mes, v->entrada->data.ano,
		v->entrada->data.hora, v->entrada->data.min, v->entrada->data.seg, v->entrada->tipoRegisto, 
		v->saida->idSensor, v->saida->data.dia, v->saida->data.mes, v->saida->data.ano, v->saida->data.hora, v->saida->data.min, 
		v->saida->data.seg, v->saida->tipoRegisto, v->tempo, v->kms, v->velocidadeMedia);
}

/**
 * @brief Mostra o header da tabela das viagens ao exportar para HTML
 * 
 * @param file Ficheiro .html, aberto
 */
void printHeaderViagensHTML(FILE *file) {
    if (!file) return;

    fprintf(file,
        "\t\t\t\t\t<tr>\n"
        "\t\t\t\t\t\t<th>codVeículo</th>\n"
        "\t\t\t\t\t\t<th>Entrada-sensor</th>\n"
        "\t\t\t\t\t\t<th>Entrada-tipo</th>\n"
        "\t\t\t\t\t\t<th>Saída-sensor</th>\n"
        "\t\t\t\t\t\t<th>Saída-data</th>\n"
        "\t\t\t\t\t\t<th>Saída-tipo</th>\n"
        "\t\t\t\t\t\t<th>Tempo</th>\n"
        "\t\t\t\t\t\t<th>Distância</th>\n"
        "\t\t\t\t\t\t<th>Velocidade Média</th>\n"
        "\t\t\t\t\t</tr>\n");
}

/**
 * @brief Mostra uma viagem em formato HTML
 * 
 * @param viagem Viagem
 * @param file Ficheiro .html, aberto
 */
void printViagemHTML(void *viagem, FILE *file) {
    if (!viagem || !file) return;

    Viagem *v = (Viagem *)viagem;

    fprintf(file,
        "\t\t\t\t\t<tr>\n"
        "\t\t\t\t\t\t<th>%d</th>\n"
        "\t\t\t\t\t\t<th>%d</th>\n"
        "\t\t\t\t\t\t<th>%hd-%hd-%hdT%hd:%hd:%.3f</th>\n"
        "\t\t\t\t\t\t<th>%c</th>\n"
        "\t\t\t\t\t\t<th>%d</th>\n"
        "\t\t\t\t\t\t<th>%hd-%hd-%hdT%hd:%hd:%.3f</th>\n"
        "\t\t\t\t\t\t<th>%c</th>\n"
        "\t\t\t\t\t\t<th>%.3f</th>\n"
        "\t\t\t\t\t\t<th>%.2f</th>\n"
        "\t\t\t\t\t\t<th>%.2f</th>\n"
        "\t\t\t\t\t</tr>\n",
        v->ptrCarro ? v->ptrCarro->codVeiculo : -1, v->entrada->idSensor, v->entrada->data.dia, v->entrada->data.mes, v->entrada->data.ano,
		v->entrada->data.hora, v->entrada->data.min, v->entrada->data.seg, v->entrada->tipoRegisto, 
		v->saida->idSensor, v->saida->data.dia, v->saida->data.mes, v->saida->data.ano, v->saida->data.hora, v->saida->data.min, 
		v->saida->data.seg, v->saida->tipoRegisto, v->tempo, v->kms, v->velocidadeMedia);
}

/**
 * @brief Memória ocupada por uma passagem
 * 
 * @param passagem Passagem
 * @return size_t Memória ocupada ou 0 se erro
 */
size_t memUsagePassagem(void *passagem) {
	if (!passagem) return 0;

	size_t mem = sizeof(Passagem);
	return mem;
}

/**
 * @brief Memória ocupada por uma viagem
 * 
 * @param viagem Viagem
 * @return size_t Memória ocupada ou 0 se erro
 */
size_t memUsageViagem(void *viagem) {
	if (!viagem) return 0;

	Viagem *v = (Viagem *)viagem;

	size_t mem = sizeof(Viagem);

	mem += memUsagePassagem((void *)v->entrada);
	mem += memUsagePassagem((void *)v->saida);

	return mem;
}

/**
 * @brief Pede os dados e regista na base de dados
 * 
 * @param bd Base de dados
 */
void registarViagem(Bdados *bd) {
	if (!bd) return;

	do {
		limpar_terminal();
		char *matricula = NULL;
		Carro *c = NULL;
		Passagem *entrada = NULL;
		Passagem *saida = NULL;
		// Matrícula
		do {    
            printf("Insira a matrícula do veículo: ");
            matricula = lerLinhaTxt(stdin, NULL);
            if (!matricula) {
                printf("Erro ao ler a matrícula!\n\n");
                pressEnter();
                continue;
            }
            if (!validarMatricula(matricula)) {
                free(matricula);
                printf("A matrícula é inválida!\n\n");
                pressEnter();
                continue;
            }
            void *matTemp = (void *)matricula;
            c = (Carro *)searchDict(bd->carrosMat, matTemp, compChaveCarroMatricula, compCarroMatricula, hashChaveCarroMatricula);
            if (!c) {
                free(matricula);
                printf("Não foi encontrado nenhum carro com esta matrícula!\n\n");
                pressEnter();
                continue;
            }
            break;
        } while(1);
		printf("\n");

		// Entrada
		do {
			printf("Por favor insira a passagem de entrada: \n");
			entrada = pedirPassagem(bd);
			if (!entrada) {
				printf("Ocorreu um erro a processar a passagem!\n");
				pressEnter();
				continue;
			}
			entrada->tipoRegisto = '0';
			break;
		} while(1);
		printf("\n");

		// Saida
		do {
			printf("Por favor insira a passagem de saída: \n");
			saida = pedirPassagem(bd);
			if (!saida) {
				printf("Ocorreu um erro a processar a passagem!\n");
				pressEnter();
				continue;
			}
			saida->tipoRegisto = '1';
			break;
		} while(1);
		printf("\n");

		// Validar mesmo ID Sensor
		if (entrada->idSensor == saida->idSensor) {
			printf("Os códigos dos sensores são iguais!\n");
			pressEnter();
			freePassagem((void *)entrada);
			freePassagem((void *)saida);
			continue;
		}

		// Validar as duas passagens em conjunto
		if (compararDatas(entrada->data, saida->data) == 1) {
			printf("A data da passagem de saída é inválida!\n");
			pressEnter();
			freePassagem((void *)entrada);
			freePassagem((void *)saida);
			continue;
		}
		
		// Inserir na bd
		if (!inserirViagemLido(bd, entrada, saida, c->codVeiculo)) {
			printf("Ocorreu um erro a registar a viagem em memória. Por favor tente novamente!\n\n");
			pressEnter();
			continue;
		}

		if (!sim_nao("Quer inserir mais alguma viagem?")) break;
	} while(1);
}

/**
 * @brief Pede uma passagem ao utilizador
 * 
 * @return Passagem* Passagem ou NULL se erro
 */
Passagem *pedirPassagem(Bdados *bd) {
	Passagem *p = (Passagem *)malloc(sizeof(Passagem));
	if (!p) return NULL;

	int idSensor = 0;
	Data date = {0,0,0,0,0,0.0f};
	do {
		pedirInt(&idSensor, "Insira o ID do sensor: ", validarCodSensor);
		void *chave = (void *)&idSensor;
		if (!searchLista(bd->sensores, compIdSensor, chave)) {
			printf("O código do sensor não existe!\n");
			pressEnter();
		}
		break;
	} while(1);

	pedirData(&date, NULL);
	
	p->idSensor = idSensor;
	p->data = date;

	return p;
}

/**
 * @brief Lista todas as viagens
 * 
 * @param bd 
 */
void listarViagensTodas(Bdados *bd) {
    if (!bd) return;
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM];
    
    printLista(bd->viagens, printViagem, stdout, pausaListagem);
    printf("\n----FIM DE LISTAGEM----\n");
    
    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printHeaderViagensTXT(file);
            printLista(bd->viagens, printViagemTXT, file, 0);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printHeaderViagensCSV(file);
            printLista(bd->viagens, printViagemCSV, file, 0);
        }
		fclose(file);
    }
    pressEnter();
}

