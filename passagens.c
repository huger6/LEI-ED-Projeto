#include "passagens.h"
#include "bdados.h"	

int inserirPassagemLido(Bdados *bd, int idSensor, int codVeiculo, Data date, char tipoRegisto) {
    if (!bd || !bd->passagens) return 0;

    Passagem * pas = (Passagem *)malloc(Passagem);
    if (!pas) return 0;

    //Id Sensor
    pas->idSensor = idSensor;
    //Ptr Carro (código veículo)
    Carro * ptrCarro = obterCarroPorId(bd, codVeiculo);
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

Carro * obterCarroPorId(Bdados *bd, int codigo) {
    if (!bd || !bd->carros || !bd->carros->inicio) return 0;

    No *p = bd->carros->inicio;
    while(p) {
        if (p->info->codVeiculo == codigo) {
            return (Carro *)p->info;
        }
    }
    return NULL;
}

void freePassagem(void *passagem) {
    Passagem *obj = (Passagem *)passagem;
    free(obj);
}