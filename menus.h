#ifndef MENUS_HEADERS
#define MENUS_HEADERS

#include <stdio.h>

#include "bdados.h"

char mostrarMenu(void (*escrever_menu)(), char min_opcao, char max_opcao);
int validacaoMenus(const char opcao, const char limInf, const char limSup);

void menuPrincipal();
void menuDonos();
void menuVeiculos();
void menuListagemVeiculos();
void menuPassagens();
void menuEstatisticas();
void menuVelocidadesMedias();
void menuInfracoes();
void menuAnaliseDados();
void menuExportacao();
void menuOpcoes();
void menuFormatosListagem();
void menuNomeFicheirosDados();
void menuGuiaUtilizacao();

void the_architect(Bdados *bd);
void processarMenuDonos(Bdados *bd);
void processarMenuVeiculos(Bdados *bd);
void processarMenuListagemVeiculos(Bdados *bd);
void processarMenuPassagens(Bdados *bd);
void processarMenuEstatisticas(Bdados *bd);
void processarMenuVelocidades(Bdados *bd);
void processarMenuInfracoes(Bdados *bd);
void processarMenuAnaliseDados(Bdados *bd);
void processarMenuExportacao(Bdados *bd);
void processarMenuOpcoes(Bdados *bd);




#endif
