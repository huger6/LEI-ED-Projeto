#ifndef MENUS_HEADERS
#define MENUS_HEADERS

#include <stdio.h>

char mostrar_menu(void (*escrever_menu)(), char min_opcao, char max_opcao);
void menu_principal();
void validacao_menus(short * valido, const char opcao, const char limInf, const char limSup);


#endif