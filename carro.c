#include "carro.h"
#include "validacoes.h"


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

void RegistarVeiculo(Bdados *bd){
    do{
        int codVeiculo, nif;
        char *matricula;
        char *marca;
        char *modelo;
        short ano;
    
        printf ("\n\tIntroduza as Informações:\n");
        do{
            printf ("\nCódigo do Veículo: ");
            scanf("%d", &codVeiculo);
        }while (validarCodVeiculo(codVeiculo) == 0);
        
        do{
            printf ("\nMatrícula:");
            matricula = lerLinhaTxt (stdin, NULL);
        }while (validarMatricula (matricula) == 0);
        
        do{
            printf ("\nMarca:");
            marca = lerLinhaTxt (stdin, NULL);
        }while (!validarMarca (marca));
    
        do {
            printf ("\nModelo:");
            modelo = lerLinhaTxt (stdin, NULL);
        }while (!validarMarca (modelo));
    
        do{
            printf ("\nAno do Veículo:");
            scanf ("%hd", &ano);
        }while (validarAnoCarro(ano) == 0);
        
        do{
            printf ("\nNIF do Dono:");
            scanf ("%d", &nif);
        }while (validarNif(nif) == 0);
    
        inserirCarroLido(bd, matricula, marca, modelo, ano, nif, codVeiculo);  
        
        free (matricula);
        free (marca);
        free (modelo);
        
        if (sim_nao("\nQuer introduzir outro elemento?") == 0) return;

    } while(1);
}

int ordenarAlfMarca (void *carro1, void *carro2){
    if (!carro1 || !carro2) return 0;
    Carro *x1 = (Carro*) carro1;
    Carro *x2 = (Carro*) carro2;
    if (strcmp(tolower(x1->marca),tolower(x2->marca)) == 1) return 1;
    return 0;
}

int ordenarAlfModelo (void *carro1, void *carro2){
    if (!carro1 || !carro2) return 0;
    Carro *x1 = (Carro*) carro1;
    Carro *x2 = (Carro*) carro2;
    if (strcmp(tolower(x1->modelo),tolower(x2->modelo)) == 1) return 1;
    return 0;
}

int ordenarAlfMatricula (void *carro1, void *carro2){
    if (!carro1 || !carro2) return 0;
    Carro *x1 = (Carro*) carro1;
    Carro *x2 = (Carro*) carro2;
    if (strcmp(x1->matricula, x2->matricula) == 1) return 1;
    return 0;
}

void listarVeiculos(Bdados *bd){

}