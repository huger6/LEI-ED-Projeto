#include "passagens.h"

int inserirPassagemLido(Bdados *bd, int idSensor, int codVeiculo, Data date, char tipoRegisto) {
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

    if (!addInicioLista(bd->passagens, (void *)pas)) return 0;

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