#include "carro.h"


int inserirCarroLido(Bdados *bd, char *matricula, char *marca, char *modelo, short ano, int nif, int codVeiculo) {
    if (!bd) return 0;
    
    Carro *aut = (Carro *)malloc(sizeof(Carro));
    if (!aut) return 0;
    
    //Matrícula
    strcpy(aut->matricula, matricula);
    //Marca
    aut->marca = (char *)malloc(strlen(marca) * sizeof(char) + 1);
    if (!aut->marca) {
        free(aut);
        return 0;
    }
    strcpy(aut->marca, marca);
    //Modelo
    aut->modelo = (char *)malloc(strlen(modelo) * sizeof(char) + 1);
    if (!aut->modelo) {
        free(aut->marca);
        free(aut);
        return 0;
    }
    strcpy(aut->modelo, modelo);
    //Ano
    aut->ano = ano;
    //NIF (ptrPessoa)
    aut->ptrPessoa = (Dono *)pesquisarPorChave(bd->donos, compCodDono, &nif);
    //Código Veículo
    aut->codVeiculo = codVeiculo;
    
    if (!addInicioLista(bd->carros, (void *)aut)) {
        free(aut->modelo);
        free(aut->marca);
        free(aut);
        return 0;
    }
    
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

int compCodVeiculo(void *carro, void *codigo) {
    if (!carro || !codigo) return 0;

    Carro *x = (Carro *)carro;
    int *cod = (int *)codigo;
    if (x->codVeiculo == *cod) return 1;
    return 0;
}

//Não se liberta o ponteiro para Dono certo?
void freeCarro(void *carro) {
    Carro *obj = (Carro *)carro;
    if (obj->marca) free(obj->marca);
    if (obj->modelo) free(obj->modelo);
    free(obj);
}

void mostrarCarro(void *carro) {
    if (!carro) return;

    Carro *x = (Carro*)carro;
    printf("\nCódigo do Veículo: %d", x->codVeiculo);
    printf("\nMatrícula: %s", x->matricula);
    printf("\nMarca: %s", x->marca);
    printf("\nModelo: %s", x->modelo);
    printf("\nAno do Veículo: %d", x->ano);
    printf("\nNome do Dono: %s", x->ptrPessoa->nome);
    printf("\nNIF do Dono: %d", x->ptrPessoa->nif);
}

void guardarCarroBin(void *carro, FILE *file) {
    if (!carro || !file) return;

    Carro *x = (Carro *)carro;
    fwrite(&x->ano, sizeof(short), 1, file);
    fwrite(&x->codVeiculo, sizeof(int), 1, file);
    fwrite(x->matricula, sizeof(x->matricula), 1, file);

    size_t tamanhoMarca = strlen(x->marca) + 1;
    fwrite(&tamanhoMarca, sizeof(size_t), 1, file);
    fwrite(x->marca, tamanhoMarca, 1, file);

    size_t tamanhoModelo = strlen(x->modelo) + 1;
    fwrite(&tamanhoModelo, sizeof(size_t), 1, file);
    fwrite(x->modelo, tamanhoModelo, 1, file);

    fwrite(&x->ptrPessoa->nif, sizeof(int), 1, file);
}