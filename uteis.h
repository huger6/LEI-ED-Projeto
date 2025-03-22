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

typedef struct {
    short ano, mes, dia, hora, minuto;
    float segundos;
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
char * normalizar_string(char * str);
char * strlwrSafe(char * str);
int stringToInt(const char * str, int * resultado);
int stringToShort(const char * str, short * resultado);
int stringToFloat(const char * str, float * resultado);


void medirTempo(void (*funcao()));



#endif