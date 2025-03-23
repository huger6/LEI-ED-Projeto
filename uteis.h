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
    short ano, mes, dia, hora, min;
    float seg;
} Data;

typedef struct {
    short zona; //Primeiros 4 dígitos
    short local; //Últimos 3 dígitos
} CodPostal;

/*

typedef struct {
    short graus;
    short min;
    short seg;
    char direcao; //N-S-O-E
} CoordenadasGMS;

typedef struct {
    char latitude[20]; //char * latitude;
    char longitude[20];
} Coord;

*/

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