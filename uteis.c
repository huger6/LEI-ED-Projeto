#include "uteis.h"

/* Limpa buffer do teclado
 *
 * @return void
 *         
 * @note Lê caracteres até encontrar \n ou EOF
 * @note Usar apenas se houver certeza de que há dados no buffer
 */
void limpar_buffer() {
    int lixo;
    while ((lixo = getchar()) != '\n' && lixo != EOF);
}

/* Verifica e limpa buffer do teclado
 *
 * @return int  1 se buffer só tem \n, 0 se tiver outros caracteres
 *         
 * @note Verifica primeiro caractere no buffer
 * @note Limpa sempre todo o buffer
 * @note Usar apenas se houver certeza de que há dados no buffer
 */
int verificar_e_limpar_buffer() {
    //Verificar o caracter que está no buffer
    char lixo = getchar();
    //Se não for o \n, ou seja, a entrada foi inválida
    if (lixo != '\n') {
        limpar_buffer(); //limpamos o buffer (pois ainda há o enter, pelo menos)
        return 0;
    }
    //Se estamos aqui, então o caracter é o \n, e já foi lido, logo não precisamos fazer nada
    return 1;
}

/* Limpa o terminal
 *
 * @return void
 *         
 * @note Multi-plataforma (Windows/Unix)
 */
void limpar_terminal() {
    #ifdef _WIN32 //_WIN32 é uma variável local automaticamente pre-definida pelo windows em todos os sistemas
        system("cls"); //Sistemas windows
    #else
        system("clear"); //Sistemas linux, macos, etc
    #endif
}

/* Pede ao utilizador para pressionar Enter
 *
 * @return void
 *         
 * @note Limpa buffer se necessário
 * @note Bloqueia até receber \n
 */
void pressione_enter() {
    printf("Pressione Enter para continuar.\n");
    while (getchar() != '\n');
}

/* Coloca terminal para UTF-8
 *
 * @return void
 *         
 * @note Windows: CP_UTF8 + locale pt para números
 * @note Unix: locale para tudo
 * @note Avisa se configuração falhar
 */
void colocar_terminal_utf8() {
    #ifdef _WIN32
        //SetConsoleOutputCP retorna 0 se houver um erro
        if ((SetConsoleOutputCP(CP_UTF8) == 0)||SetConsoleCP(CP_UTF8) == 0) {
            printf("Ocorreu um erro ao configurar o terminal do Windows para UTF-8.\n");
            printf("A aplicação irá continuar. Desformatação será visível. Para resolver, reinicie a aplicação.\n");
        }
	    setlocale(LC_NUMERIC, "Portuguese"); //Apenas afeta os números , ou seja, muda a notação de floats de '.' para ','
    #else
        setlocale(LC_ALL, "pt_PT.UTF-8");
    #endif
}

int randomInt(int min, int max) {
    return min + rand() % (max - min + 1);
}
