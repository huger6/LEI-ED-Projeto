#include "bdados.h"
#include "carro.h"
#include "distancias.h"
#include "dono.h"
#include "passagens.h"
#include "sensores.h"

/**
 * @brief Inicializa a base de dados criando as estruturas necessárias
 * 
 * @param bd Base de dados
 * @return int 1 se sucesso, 0 se erro
 */
int inicializarBD(Bdados *bd) {
    bd->carrosMarca = criarDict();
    bd->carrosCod = criarDict();
    bd->donosNif = criarDict();
    bd->donosAlfabeticamente = criarDict();
    bd->viagens = criarLista();
    bd->sensores = criarLista();
    inicializarMatrizDistancias(bd);
    if (!bd->carrosMarca || !bd->carrosCod|| !bd->distancias || !bd->distancias->matriz || !bd->donosNif ||
         !bd->donosAlfabeticamente	|| !bd->viagens || !bd->sensores) return 0;
    return 1;
}

/**
 * @brief Liberta toda a memória utilizada pelo programa
 * 
 * @param bd Base de dados
 */
void freeTudo(Bdados *bd) {
    if (!bd) return;
    printf("Memória antes de ser libertada: %.10f\n\n", obterUsoMemoria());

    freeDict(bd->carrosMarca, freeChaveCarroMarca, NULL);
    printf("Memória depois de libertar carrosMarca: %.10f\n\n", obterUsoMemoria());
    freeDict(bd->carrosCod, freeChaveCarroCod, freeCarro);
    printf("Memória depois de libertar carrosCod: %.10f\n\n", obterUsoMemoria());

    freeDict(bd->donosAlfabeticamente, freeChaveDonoAlfabeticamente, NULL);
    printf("Memória depois de libertar donosAlf: %.10f\n\n", obterUsoMemoria());
    freeDict(bd->donosNif, freeChaveDonoNif, freeDono);
    printf("Memória depois de libertar donosNif: %.10f\n\n", obterUsoMemoria());

    freeMatrizDistancias(bd->distancias);
    printf("Memória depois de libertar matriz: %.10f\n\n", obterUsoMemoria());

    freeLista(bd->viagens, freeViagem);
    printf("Memória depois de libertar viagens: %.10f\n\n", obterUsoMemoria());

    freeLista(bd->sensores, freeSensor);
    printf("Memória depois de libertar sensores: %.10f\n\n", obterUsoMemoria());

    free(bd);
    printf("Memória depois de libertar tudo: %.10f\n\n", obterUsoMemoria());
}

/* ERRO AQUI
void exportarListaCarros(FILE *f, Lista *carros) {
    if (!carros || !carros->inicio) {
        fprintf(f, "  <carros />\n");
        return;
    }

    fprintf(f, "  <carros>\n");
    No *n = carros->inicio;
    while (n) {
        Carro *c = (Carro *)n->info;
        fprintf(f, "    <carro>\n");
        fprintf(f, "      <matricula>%s</matricula>\n", c->matricula);
        fprintf(f, "      <marca>%s</marca>\n", c->marca ? c->marca : "Desconhecida");
        fprintf(f, "      <modelo>%s</modelo>\n", c->modelo ? c->modelo : "Desconhecido");
        fprintf(f, "      <ano>%d</ano>\n", c->ano);
        fprintf(f, "      <codVeiculo>%d</codVeiculo>\n", c->codVeiculo);
        if (c->ptrPessoa)
            fprintf(f, "      <nifDono>%d</nifDono>\n", c->ptrPessoa->nif);
        else
            fprintf(f, "      <nifDono />\n");
        fprintf(f, "    </carro>\n");
        n = n->prox;
    }
    fprintf(f, "  </carros>\n");
}

void exportarListaDonos(FILE *f, Lista *donos) {
    if (!donos || !donos->inicio) {
        fprintf(f, "  <donos />\n");
        return;
    }

    fprintf(f, "  <donos>\n");
    No *n = donos->inicio;
    while (n) {
        Dono *d = (Dono *)n->info;
        fprintf(f, "    <dono>\n");
        fprintf(f, "      <nif>%d</nif>\n", d->nif);
        fprintf(f, "      <nome>%s</nome>\n", d->nome ? d->nome : "Desconhecido");
        fprintf(f, "      <codigoPostal>%04d-%03d</codigoPostal>\n", d->codigoPostal.local, d->codigoPostal.zona);
        fprintf(f, "    </dono>\n");
        n = n->prox;
    }
    fprintf(f, "  </donos>\n");
}

void exportarListaPassagens(FILE *f, Lista *passagens) {
    if (!passagens || !passagens->inicio) {
        fprintf(f, "  <passagens />\n");
        return;
    }

    fprintf(f, "  <passagens>\n");
    No *n = passagens->inicio;
    while (n) {
        Passagem *p = (Passagem *)n->info;
        fprintf(f, "    <passagem>\n");
        fprintf(f, "      <idSensor>%d</idSensor>\n", p->idSensor);
        fprintf(f, "      <matricula>%s</matricula>\n", p->veiculo ? p->veiculo->matricula : "");
        fprintf(f, "      <data>%02d-%02d-%04d</data>\n", p->data.dia, p->data.mes, p->data.ano);
        fprintf(f, "      <hora>%02d:%02d:%02d</hora>\n", p->data.hora, p->data.min, p->data.seg);
        if (p->tipoRegisto == '0')
            fprintf(f, "      <tipo>Entrada</tipo>\n");
        else if (p->tipoRegisto == '1')
            fprintf(f, "      <tipo>Saida</tipo>\n");
        else
            fprintf(f, "      <tipo>Desconhecido</tipo>\n");
        fprintf(f, "    </passagem>\n");
        n = n->prox;
    }
    fprintf(f, "  </passagens>\n");
}

void exportarListaSensores(FILE *f, Lista *sensores) {
    if (!sensores || !sensores->inicio) {
        fprintf(f, "  <sensores />\n");
        return;
    }

    fprintf(f, "  <sensores>\n");
    No *n = sensores->inicio;
    while (n) {
        Sensor *s = (Sensor *)n->info;
        fprintf(f, "    <sensor>\n");
        fprintf(f, "      <codSensor>%d</codSensor>\n", s->codSensor);
        fprintf(f, "      <nomeSensor>%s</nomeSensor>\n", s->designacao ? s->designacao : "Desconhecido");
        fprintf(f, "      <latitude>%s</latitude>\n", s->latitude ? s->latitude : "N/A");
        fprintf(f, "      <longitude>%s</longitude>\n", s->longitude ? s->longitude : "N/A");
        fprintf(f, "    </sensor>\n");
        n = n->prox;
    }
    fprintf(f, "  </sensores>\n");
}

void exportarListaDistancias(FILE *f, Lista *distancias) {
    if (!distancias || !distancias->inicio) {
        fprintf(f, "  <distancias />\n");
        return;
    }

    fprintf(f, "  <distancias>\n");
    No *n = distancias->inicio;
    while (n) {
        Distancias *d = (Distancias *)n->info;
        fprintf(f, "    <distancia>\n");
        fprintf(f, "      <sensor1>%s</sensor1>\n", d->sensor1 && d->sensor1->designacao ? d->sensor1->designacao : "Desconhecido");
        fprintf(f, "      <sensor2>%s</sensor2>\n", d->sensor2 && d->sensor2->designacao ? d->sensor2->designacao : "Desconhecido");
        fprintf(f, "      <valor>%.2f</valor>\n", d->distancia);
        fprintf(f, "    </distancia>\n");
        n = n->prox;
    }
    fprintf(f, "  </distancias>\n");
}

void exportarBDadosXML(FILE *f, Bdados *bd) {
    fprintf(f, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    fprintf(f, "<baseDados>\n");

    exportarListaCarros(f, bd->carros);
    exportarListaDonos(f, bd->donos);
    exportarListaPassagens(f, bd->passagens);
    exportarListaSensores(f, bd->sensores);
    exportarListaDistancias(f, bd->distancias);

    fprintf(f, "</baseDados>\n");
}

*/