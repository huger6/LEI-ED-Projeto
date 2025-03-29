#include "carro.h"
#include "bd.h"


int inserirCarroLido(BaseDados *bd, char *matricula, char *marca, char *modelo, short ano, int nif, int codVeiculo) {
    if (!bd) return 0;
    
    Carro *aut = (Carro *)malloc(sizeof(Carro));
    if (!aut) return 0;
    
    //Matrícula
    aut->matricula = (char *)malloc(strlen(matricula) * sizeof(char) + 1);
    if (aut->matricula) {
        free(aut);
        return 0;
    }
    strcpy(aut->matricula, matricula);
    //Marca
    aut->marca = (char *)malloc(strlen(marca) * sizeof(char) + 1);
    if (!aut->marca) {
        free(aut->matricula);
        free(aut);
        return 0;
    }
    strcpy(aut->marca, marca);
    //Modelo
    aut->modelo = (char *)malloc(strlen(modelo) * sizeof(char) + 1);
    if (!aut->modelo) {
        free(aut->matricula);
        free(aut->marca);
        free(aut);
        return 0;
    }
    strcpy(aut->modelo, modelo);
    //Ano
    aut->ano = ano;
    //NIF (ptrPessoa)
    aut->ptrPessoa = obterDonoCarro();
    //Código Veículo
    aut->codVeiculo = codVeiculo;
    
    if (!addInicioLista(bd->carros, (void *)aut)) return 0;
    
    return 1;
}

int compararCarros(void *carro1, void *carro2) {
    if (carro1 == NULL && carro2 == NULL) return 0;
    if (carro1 == NULL) return -1; //NULL < qualquer coisa
    if (carro2 == NULL) return 1;

    Carro *x = (Carro *)carro1;
    Carro *y = (Carro *)carro2;

    if (x->codVeiculo > y->codVeiculo) return 1;
    if (x->codVeiculo < y->codVeiculo) return -1;
    return 0;
}

Dono * obterDonoCarro(BaseDados *bd, int nif) {
    if (!bd || !bd->donos || !bd->donos->inicio) return NULL;

    No *p = bd->donos->inicio;
    while(p) {
        if (p->info->nif == nif) {
            return (Dono *)p->info;
        }
    }
    return NULL;
}

