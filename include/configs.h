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
extern int backupsON;
extern int pausaListagem;

extern char *donosFilename;
extern char *carrosFilename;
extern char *sensoresFilename;
extern char *distanciasFilename;
extern char *passagensFilename;

extern char *donosExportacaoFilename;
extern char *carrosExportacaoFilename;
extern char *sensoresExportacaoFilename;
extern char *distanciasExportacaoFilename;
extern char *viagensExportacaoFilename;
extern char *databaseExportacaoXML;

void colocarTerminalUTF8();
int faseInstalacao(const char *flag, const char abrir);
void reset(struct Bdados *bd);
void setPausaListagem();
void setFilenames();
void setExportacaoFilenames();
void freeFilenames();
void freeExportacaoFilenames();
size_t memUsageVarGlobais();
void autosave(struct Bdados *bd);


#endif