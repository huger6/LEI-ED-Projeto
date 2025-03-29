#include "distancias.h"
#include "bd.h"

int inserirDistanciaLido(BaseDados *bd, int codSensor1, int codSensor2, float distancia) {
    if (!bd || !bd->distancias) return 0;

    Distancias * km = (Distancias *)malloc(sizeof(Distancias));
    if (!km) return 0;

    Sensor *sen1 = obterSensorPorId(bd, codSensor1);
    Sensor *sen2 = obterSensorPorId(bd, codSensor2);
    if (!sen1 || sen2) return 0; //Se os sensores não existem não pode haver distancias entre eles

    km->sensor1 = sen1;
    km->sensor2 = sen2;
    km->distancia = distancia;

    if (!addInicioLista(bd->distancias, (void *)km)) return 0;

    return 1;
}

int compararDistancias(void *dist1, void *dist2) {
    if (dist1 == NULL && dist2 == NULL) return 0;
    if (dist1 == NULL) return -1; //NULL < qualquer coisa
    if (dist2 == NULL) return 1;

    Distancias *x = (Distancias *)dist1;
    Distancias *y = (Distancias *)dist2;

    if (x->distancia > y->distancia) return 1;
    if (x->distancia < y->distancia) return -1;
    return 0;
}

Sensor * obterSensorPorId(BaseDados *bd, int codigo) {
    if (!bd || !bd->sensores || !bd->sensores->inicio) return NULL;

    No *p = bd->sensores->inicio;
    while(p) {
        if (p->info->codSensor == codigo) {
            return (Sensor *)p->info;
        }
    }
    return NULL;
}