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

/**
 * @brief Insere a viagem com a passagem de entrada e saída
 * 
 * @param v Viagem a colocar na bd
 * @param entrada Ponteiro para a passagem de entrada
 * @param saida Ponteiro para a passagem de saída
 * @return int 0 em caso de erro, -1 se inserido APENAS entrada, 1 se inserido APENAS saida, 2 caso inserido AMBOS
 */
int inserirViagemLido(Viagem *v, Passagem *entrada, Passagem *saida) {
    if (!v) return 0;
    if (!entrada && !saida) {
        return 0;
    }

    if (entrada && !saida) {
        v->entrada = entrada;
        return -1;
    }
    if (saida && !entrada) {
        v->saida = saida;
        return 1;
    }
    if (entrada && saida) {
        v->entrada = entrada;
        v->saida = saida;
        return 2;
    }
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