#include "passagens.h"

Passagem * obterPassagem(Bdados *bd, int idSensor, int codVeiculo, Data date, char tipoRegisto) {
    if (!bd || !bd->passagens) return 0;

    Passagem *pas = (Passagem *)malloc(sizeof(Passagem));
    if (!pas) return 0;

    //Id Sensor
    pas->idSensor = idSensor;
    //Ptr Carro (código veículo)
    Carro *ptrCarro = (Carro *)pesquisarPorChave(bd->carros, compCodPassagem, &codVeiculo);
    if (ptrCarro) {
        pas->veiculo = ptrCarro;
    }
    pas->data = date;
    pas->tipoRegisto = tipoRegisto;

    return pas;
}

/**
 * @brief Insere a viagem com a passagem de entrada e saída
 * 
 * @param bd Base de dados
 * @param v Viagem a colocar na bd
 * @param entrada Ponteiro para a passagem de entrada
 * @param saida Ponteiro para a passagem de saída
 * @return int 0 em caso de erro, -1 se inserido APENAS entrada, 1 se inserido APENAS saida, 2 caso inserido AMBOS
 */
int inserirViagemLido(Bdados *bd, Viagem *v, Passagem *entrada, Passagem *saida) {
    if (!bd || !bd->viagens || !v) return 0;
    if (!entrada && !saida) {
        return 0;
    }

    if (entrada && !saida) {
        v.entrada = entrada;
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

void guardarViagemBin(void *viagem, FILE *file) {
    if (!viagem || !file) return;

    Viagem *x = (Viagem *)viagem;
    guardarPassagemBin((void *)x->entrada, file);
    guardarPassagemBin((void *)x->saida, file);
}

void guardarPassagemBin(void *passagem, FILE *file) {
    if (!passagem || !file) return;

    Passagem *x = (Passagem *)passagem;
    fwrite(&x->data, sizeof(Data), 1, file);
    fwrite(&x->idSensor, sizeof(int), 1, file);
    fwrite(&x->tipoRegisto, sizeof(char), 1, file);
    fwrite(&x->veiculo->codVeiculo, sizeof(int), 1, file);
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