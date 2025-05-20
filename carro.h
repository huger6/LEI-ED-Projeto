#ifndef CARRO_HEADERS
#define CARRO_HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dono.h"
#include "constantes.h"
#include "structsGenericas.h"

struct Bdados;

typedef struct {
    char matricula[MAX_MATRICULA + 1];
    char *marca;
    char *modelo;
    short ano;
    int codVeiculo; //PRIMARY KEY
    Dono *ptrPessoa;
    Lista *viagens;
} Carro;


int compararCarros(void *carro1, void *carro2);
int inserirCarroLido(struct Bdados *bd, char *matricula, char *marca, char *modelo, short ano, int nif, int codVeiculo);
int compCodCarro(void *carro, void *codigo);
void freeCarro(void *carro);
void printCarro(void *carro, FILE *file);
void guardarCarroBin(void *carro, FILE *file);
void *readCarroBin(FILE *file);
void guardarChaveCarroMarca(void *chaveMarca, FILE *file);
void *criarChaveCarroMarca(void *carro);
int hashChaveCarroMarca(void *chave);
void freeChaveCarroMarca(void *chave);
int compChaveCarroMarca(void *chave, void *chave2);
int compCarroMarca (void *carro1, void *carro2);
int compCarroMatricula(void *carro1, void *carro2);
int compCarroModelo(void *carro1, void *carro2);
void guardarChaveCarroCod(void *chave, FILE *file);
void *readChaveCarroCod(FILE *file);
void *criarChaveCarroCod(void *carro);
int hashChaveCarroCod(void *carro);
void freeChaveCarroCod(void *chave);
int compChaveCarroCod(void *chave, void *chave2);
void *criarChaveCarroMatricula(void *carro);
int hashChaveCarroMatricula(void *chave);
void freeChaveCarroMarca(void *chave);
int compChaveCarroMatricula(void *chave, void *chave2);
int compChaveCarroRankingInt(void *chave, void *chave2);
int compChaveCarroRankingFloat(void *chave, void *chave2);
void freeChaveCarroRankingInt(void *chave);
void freeChaveCarroRankingFloat(void *chave);
void printCarroXML(void *carro, int indentacao, FILE *file);
void printHeaderCarrosCSV(FILE *file);
void printCarroCSV(void *carro, FILE *file);
void printHeaderCarrosTXT(FILE *file);
void printCarroTXT(void *carro, FILE *file);
void printHeaderCarrosHTML(FILE *file);
void printCarroHTML(void *carro, FILE *file);
void printCarroRanking(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file);
void printCarroRankingTXT(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file);
void printCarroRankingCSV(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file);
void printMarcaRanking(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file);
void printMarcaRankingTXT(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file);
void printMarcaRankingCSV(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file);
void printHeaderCarroMaisInfracoes(FILE *file);
void printMaisInfracoes(void *compInfo, FILE *file);
void printHeaderCarroMaisInfracoesTXT(FILE *file);
void printHeaderCarroMaisInfracoesCSV(FILE *file);
void printHeaderCarroMaisKMS(FILE *file);
void printMaisKMS(void *compInfo, FILE *file);
void printMaisKMS_CSV(void *compInfo, FILE *file);
void printHeaderCarroMaisKMS_TXT(FILE *file);
void printHeaderCarroMaisKMS_CSV(FILE *file);
void printHeaderMarcaMaisKMS(FILE *file);
char *obterMarcaMaisComum(Dict *carrosMarca);
char *obterMarcaMaisVelocidadeMedia(struct Bdados *bd);
size_t memUsageCarro(void *carro);
size_t memUsageChaveCarroCod(void *chave);
size_t memUsageChaveCarroMarca(void *chave);
int obterCodVeiculoNovo(Dict *carrosCod);
int obterCodMaximoCarros(Dict *carrosCod);
void registarCarro(struct Bdados *bd);
void listarCarrosTodos(struct Bdados *bd);
void listarCarrosPorMatricula(struct Bdados *bd);
void listarCarrosPorMarca(struct Bdados *bd);
void listarCarrosPorModelo(struct Bdados *bd);
void listarCarrosPorPeriodoTempo(struct Bdados *bd);
void listarInfracoesPorPeriodoTempo(struct Bdados *bd);
void rankingInfracoes(struct Bdados *bd);
void rankingKMSPeriodoTempo(struct Bdados *bd);
void rankingKMSMarca(struct Bdados *bd);
void listarCarrosComInfracoes(struct Bdados *bd);


#endif