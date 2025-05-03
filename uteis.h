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

extern char mostrarMenu(void (*escrever_menu)(), char min_opcao, char max_opcao);
extern void menuFormatosListagem();

typedef struct {
    short ano, mes, dia, hora, min;
    float seg;
} Data;

extern Data DATA_ATUAL;


void limpar_buffer();
int verificar_e_limpar_buffer();
void limpar_terminal();
void pressEnter();
int enter_espaco();
int randomInt(int min, int max);
void data_atual();
int sim_nao(char *mensagem);
char *normString(const char *str);
char *strlwrSafe(const char *str);
int stringToInt(const char *str, int *resultado);
int stringToShort(const char *str, short *resultado);
int stringToFloat(const char *str, float *resultado);
void converterPontoVirgulaDecimal(char *str);
void replaceStrObj(char *str, const char out, const char in);
char *lerLinhaTxt(FILE *ficheiro, int *n_linhas);
void pedirInt(int *num, char *mensagem, int (*validarInput)(int input));
void pedirShort(short *num, char *mensagem, int (*validarInput)(short input));
void pedirData(Data *data, char *mensagem);
void pedirPeriodoTempo(Data *inicio, Data *fim, char *mensagemInicial, char *mensagemFinal);
int converterCodPostal(const char *codPostal, short *zona, short *local);
int compararDatas(Data data1, Data data2);
char *converterParaData(const char *strData, Data *data);
float calcularIntervaloTempo(Data *data1, Data *data2);
int hashString(const char *str);
int deleteFile(const char *nome, const char modo);
double obterUsoMemoria();
void indent(int indentacao, FILE *file);
char *floatToStringPontoDecimal(float valor, int casasDecimais);
int validarNomeFicheiro(const char *filename);
FILE *abrirFicheiroComValidacao(const char *nome);
FILE *pedirListagemFicheiro(char *formatoSelecionado);




#endif