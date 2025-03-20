#ifndef MENUS_HEADERS
#define MENUS_HEADERS

#include <stdio.h>

char mostrar_menu(void (*escrever_menu)(), char min_opcao, char max_opcao);
void menu_principal();

#endif