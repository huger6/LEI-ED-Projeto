#ifndef UTEIS_HEADERS
#define UTEIS_HEADERS

#include <stdio.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <locale.h>
#include <time.h>

typedef struct {
    short ano, mes, dia, hora, minuto;
    float segundos;
} Data;

void limpar_buffer();
int verificar_e_limpar_buffer();
void limpar_terminal();
void pressione_enter();
void colocar_terminal_utf8();
int randomInt(int min, int max);



#endif