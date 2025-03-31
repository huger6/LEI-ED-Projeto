#include "validacoes.h"
#include "dados.h"


int validarNif(const int nif) 
{
    return 1;
}

char * validarNome(const char *nome)
{
    int comprimento = strlen(nome);
    
    //Verificar nome vazio
    if (!nome) return "\nO nome nao existe!\n"; //NULL != '\0'

    //Retirar o \n no final, se tiver, mas apenas se a string for apenas \n não contamos como válido
    if (nome[comprimento - 1] == '\n' && comprimento != 1)
    { 
        nome[comprimento - 1] = '\0';
        comprimento--;
    }
    // Verificar o primeiro e o último caractere
    if (nome[0] == ' ' || nome[comprimento - 1] == ' ') return "\nO nome tem espaços indevidos!\n";

    for (int i = 0; i < comprimento; i++) 
    {
        //Verificar se tem separador
        if (nome[i] == SEPARADOR) return ("\nO nome contém um caracter separador inválido (%c).\n", SEPARADOR);

        // Verificar se há dois espaços seguidos
        if (i < comprimento -1 && nome[i] == ' ' && nome[i+1] == ' ') return "\nO nome não pode conter espaços consecutivos!\n";

        //Verificar caracteres inválidos
        if (!iswalpha(nome[i]) && nome[i] != ' ' && nome[i] != '-') return "\nO Nome contém caracteres inválidos!\n";
        //iswalpha apenas retorna válido a-z e A-Z, logo as outras condições validam espaços e hifens
    }
    return NULL;
}

int validarCodPostal(const short zona, const short local)
{
    return 1;
}

int validarAnoCarro(const short ano)
{
    if (ano < 1885 || ano > DATA_ATUAL.ano) return 0;
    
    return 1;
}

int validarMatricula(const char *matricula)
{
    //Ver regex
    if ((isalpha(matricula[0]) && isalpha(matricula[1])) || (isdigit(matricula[0]) && isdigit(matricula[1])))
    {
        if (matricula[2] == '-' && matricula[5] == '-')
        {
            if ((isalpha(matricula[3]) && isalpha(matricula[4])) || (isdigit(matricula[3]) && isdigit(matricula[4])))
            {
                if ((isalpha(matricula[6]) && isalpha(matricula[7])) || (isdigit(matricula[6]) && isdigit(matricula[7])))
                {
                    if (matricula[8] == '\0') return 1;
                }
            }
        }
    }
    return 0;
}

char * validarMarca(const char *marca) {
    return NULL;
}

/**
 * @brief Valida o modelo do veículo
 * 
 * @param modelo Modelo
 * @return char* NULL caso válido, mensagem de erro caso contrário
 */
char * validarModelo(const char *modelo) {
    return NULL;
}

int validarCodVeiculo(const int codigo) {
    return 1;
}

int validarCodSensor(const int codSensor) {
    return 1;
}

int validarDistancia(const float distancia) {
    return 1;
}

int validarData(const Data date) {
    return 1;
}

int validarTipoRegisto(const char tipoRegisto) {
    return 1;
}