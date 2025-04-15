#ifndef UTEIS_HEADERS
#define UTEIS_HEADERS

#include <stdio.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <locale.h>
#include <time.h>
#include <ctype.h>
#include <limits.h>

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
void colocar_terminal_utf8();
int randomInt(int min, int max);
void data_atual();
int sim_nao(char * mensagem);
char *normalizar_string(const char *str);
char *strlwrSafe(const char *str);
int stringToInt(const char * str, int * resultado);
int stringToShort(const char * str, short * resultado);
int stringToFloat(const char * str, float * resultado);
void converterPontoVirgulaDecimal(char *str);
char * lerLinhaTxt(FILE * ficheiro, int * n_linhas);
int converterCodPostal(const char *codPostal, short *zona, short *local);
int compararDatas(Data data1, Data data2);
char * converterParaData(const char *strData, Data * data);
float calcularIntervaloTempo(Data *data1, Data *data2);
int hashString(const char *str);


void medirTempo(void (*funcao()));



#endif