#include "validacoes.h"
#include "dados.h"


int validarNif(const int nif) 
{
    return 1;
}

char * validarNome(char *nome)
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
    validarNome (marca);
    return NULL;
}

/**
 * @brief Valida o modelo do veículo
 * 
 * @param modelo Modelo
 * @return char* NULL caso válido, mensagem de erro caso contrário
 */
char * validarModelo(const char *modelo) {
    validarNome (modelo);
    return NULL;
}

int validarCodVeiculo(const int codigo) {
    if (codigo < 1) return 0;
    return 1;
}

int validarCodSensor(const int codSensor) {
    if (codSensor < 1) return 0;
    return 1;
}

int validarDistancia(const float distancia) {
    if (distancia < 0) return 0;
    return 1;
}

int validarData(const Data date, const char modo) {
    short dia = date.dia;
    short mes = date.mes;
    short ano = date.ano;
    short hora = date.hora;
    short min = date.min;
    float seg = date.seg;
    short dia_atual = DATA_ATUAL.dia;
    short mes_atual = DATA_ATUAL.mes;
    short ano_atual = DATA_ATUAL.ano;
    short hora_atual = DATA_ATUAL.hora;
    short min_atual = DATA_ATUAL.min;
    float seg_atual = DATA_ATUAL.seg;

    if ((ano < 1) || (dia < 1) || (mes < 1) || (mes > 12) || (hora < 0) || (hora > 24) || (min < 0) || (min > 60) || (seg < 0) || (seg > 60)){
        if (modo == '1') printf("\nERROR: Por favor, insira uma data válida!");
        return 0;
    }
    
    if (((seg > seg_atual) && (min > min_atual) && (hora > hora_atual) && (dia > dia_atual) && (mes >= mes_atual) && (ano >= ano_atual)) || (ano > ano_atual) ) {
        if (modo == '1') printf("\nERROR: Por favor, insira uma data válida!");
        return 0;
    }
    // validar o mes; validar a hora; validar os minutos; validar os segundos

    //Criamos um vetor com os dias de cada mes, fevereiro com 28 pois é o mais comum
    short dias_por_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    //Apenas criado para propóstios de informação ao user
    const char * nome_do_mes[] = {"janeiro", "fevereiro", "março", "abril", "maio", "junho", "julho", "agosto", "setembro", "outubro", "novembro", "dezembro"};

    //Verificar anos bissextos
    if (mes == 2 && ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))) {
        dias_por_mes[1] = 29;
    }

    if (dia > dias_por_mes[mes - 1]) {
        if (modo == '1') printf("O dia é inválido! O mês de %s tem apenas %hd dias.\n", nome_do_mes[mes - 1], dias_por_mes[mes -1]);
        return 0; 
    }
    return 1;
}

int validarTipoRegisto(const char tipoRegisto) {
    if (tipoRegisto == '0' || tipoRegisto == '1') return 1;
    return 0;
}