#ifndef UTEIS_HEADERS
#define UTEIS_HEADERS

#include <stdio.h>
#include <stdlib.h>
#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
    #include <psapi.h>
#else 
    #include <sys/resource.h>
#endif
#include <locale.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>
#include <string.h>

#include "constantes.h"

typedef struct {
    short ano, mes, dia, hora, min;
    float seg;
} Data;

extern Data DATA_ATUAL;


void limpar_buffer();
int verificar_e_limpar_buffer();
void limpar_terminal();
void pressione_enter();
int randomInt(int min, int max);
void data_atual();
int sim_nao(char * mensagem);
char *normString(const char *str);
char *strlwrSafe(const char *str);
int stringToInt(const char * str, int * resultado);
int stringToShort(const char * str, short * resultado);
int stringToFloat(const char * str, float * resultado);
void converterPontoVirgulaDecimal(char *str);
char * lerLinhaTxt(FILE * ficheiro, int * n_linhas);
int converterCodPostal(const char *codPostal, short *zona, short *local);
int compararDatas(Data data1, Data data2);
char *converterParaData(const char *strData, Data *data);
float calcularIntervaloTempo(Data *data1, Data *data2);
int hashString(const char *str);
int deleteFile(const char *nome, const char modo);
double obterUsoMemoria();




#endif