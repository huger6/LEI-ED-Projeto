#include "passagens.h"
#include "bdados.h"

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

	getStatsViagem(bd, v);

	if (!addInicioLista(bd->viagens, (void *)v)) {
		freePassagem(entrada);
		freePassagem(saida);
		free(v);
		return 0;
	}

	return 1;
}

int compararPassagens(void *passagem1, void *passagem2) {
	if (passagem1 == NULL && passagem2 == NULL) return 0;
	if (passagem1 == NULL) return -1; //NULL < qualquer coisa
	if (passagem2 == NULL) return 1;

	Passagem *x = (Passagem *)passagem1;
	Passagem *y = (Passagem *)passagem2;

	return (compararDatas(x->data, y->data));
}

int compCodPassagem(void *passagem, void *codigo) {
	if (!passagem || !codigo) return 0;

	Passagem *x = (Passagem *)passagem;
	int *cod = (int *)codigo;

	if (x->idSensor == *cod) return 1;
	return 0;
}

void freePassagem(void *passagem) {
	Passagem *obj = (Passagem *)passagem;
	free(obj);
}

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

void guardarPassagemBin(void *passagem, FILE *file) {
	if (!passagem || !file) return;

	Passagem *x = (Passagem *)passagem;
	fwrite(&x->data, sizeof(Data), 1, file);
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

	fread(&x->data, sizeof(Data), 1, file);
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
    v->kms = bd->distancias->matriz[v->entrada->idSensor * bd->distancias->nColunas + v->saida->idSensor];
    v->tempo = calcularIntervaloTempo(&v->entrada->data, &v->saida->data); //min
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
	
	indent(indentacao, file);
	fprintf(file, "</viagem>\n");
}

/*
void mostrarPassagem(void *passagem){
	if (!passagem) return;

	Passagem *x = (Passagem*) passagem;
	printf("\nID de Passagem: %d", x->idSensor);
	printf("\nMatricula do veículo registado pelo sensor: %s", x->veiculo->matricula);
	printf("\nData: %d/%d/%d", x->data.dia, x->data.mes, x->data.ano);
	printf("\nHora: %d:%d e %d seg", x->data.hora, x->data.min, x->data.seg);
	if (strcmp(x->tipoRegisto, '1') == 0){
		printf("\nTipo de Registo: Saída");
	}
	if (strcmp(x->tipoRegisto, '0') == 0){
		printf("\nTipo de Registo: Entrada");
	}
}
*/

/**
 * @brief Escreve os headers das viagens em formato CSV
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderViagensCSV(FILE *file) {
	if (!file) return;

	fprintf (file, "Código do Veículo, Entrada-sensor, Entrada-data, Entrada-tipo, Saída-sensor, Saída-data, Saída-tipo, Tempo, Distancia\n");
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

	fprintf(file,"%d, %d, %hd-%hd-%hdT%hd:%hd:%s, %c, %d, %hd-%hd-%hdT%hd:%hd:%s, %c, %s, %s\n",
		v->ptrCarro ? v->ptrCarro->codVeiculo : -1, v->entrada->idSensor, v->entrada->data.dia, v->entrada->data.mes, v->entrada->data.ano,
		v->entrada->data.hora, v->entrada->data.min, segEntrada ? segEntrada : "n/a", v->entrada->tipoRegisto, 
		v->saida->idSensor, v->saida->data.dia, v->saida->data.mes, v->saida->data.ano, v->saida->data.hora, v->saida->data.min, 
		segSaida ? segSaida : "n/a", v->saida->tipoRegisto, tempoStr ? tempoStr : "n/a", kmsStr ? kmsStr : "n/a");

	if (segEntrada) free(segEntrada);
	if (segSaida) free(segSaida);
	if (tempoStr) free(tempoStr);
	if (kmsStr) free(kmsStr);
}

size_t memoriaOcupadaViagem(void *viagem){
	if (!viagem) return 0;
	Viagem *aux = (Viagem*) viagem;
}