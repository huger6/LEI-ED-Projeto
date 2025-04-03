#ifndef MENUS_HEADERS
#define MENUS_HEADERS

#include <stdio.h>

#include "bdados.h"

char mostrar_menu(void (*escrever_menu)(), char min_opcao, char max_opcao);
void validacao_menus(short *valido, const char opcao, const char limInf, const char limSup);

void menu_principal();
void menu_registos();
void menu_listagens();
void menu_trafego();
void menu_infracoes();
void menu_velocidades();

void the_architect(Bdados *bd);



#endif
