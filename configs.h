#ifndef CONFIGS_HEADERS
#define CONFIGS_HEADERS

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#include <locale.h>

struct Bdados;

extern int autosaveON;

extern char *donosFilename;
extern char *carrosFilename;
extern char *sensoresFilename;
extern char *distanciasFilename;
extern char *passagensFilename;

void colocarTerminalUTF8();
int faseInstalacao(const char *flag, const char abrir);
void reset(struct Bdados *bd);
void setFilenames();
void freeFilenames();
size_t memUsageVarGlobais();


#endif