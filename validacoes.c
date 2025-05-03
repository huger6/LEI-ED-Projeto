#include "validacoes.h"

/**
 * @brief Valida um NIF
 * 
 * @param nif Nif
 * @return int 1 se válido, 0 se inválido
 */
int validarNif(const int nif) 
{
    if (nif < 0) return 0;

    return 1;
}

/**
 * @brief Valida um nome
 * 
 * @param nome Nome
 * @return NULL se válido; retorna uma mensagem de erro se for inválido;
 */
char * validarNome(char *nome)
{
    int comprimento = strlen(nome);

    //Verificar nome vazio
    if (!nome) return "O nome nao existe!"; //NULL != '\0'

    //Retirar o \n no final, se tiver, mas apenas se a string for apenas \n não contamos como válido
    if (nome[comprimento - 1] == '\n' && comprimento != 1)
    { 
        nome[comprimento - 1] = '\0';
        comprimento--;
    }
    // Verificar o primeiro e o último caractere
    if (nome[0] == ' ' || nome[comprimento - 1] == ' ') return "\nO nome tem espaços indevidos!";

    for (int i = 0; i < comprimento; i++) 
    {
        //Verificar se tem separador
        if (nome[i] == SEPARADOR) {
            return "O nome contém um caracter separador inválido.";
        }
        
        // Verificar se há dois espaços seguidos
        if (i < comprimento -1 && nome[i] == ' ' && nome[i+1] == ' ') return "O nome não pode conter espaços consecutivos!";

        //Verificar caracteres inválidos
        if (!iswalpha(nome[i]) && nome[i] != ' ' && nome[i] != '-' && nome[i] != '?') return "O Nome contém caracteres inválidos!"; // ? para evitar excluir muitos dados
        //iswalpha apenas retorna válido a-z e A-Z, logo as outras condições validam espaços e hifens
    }
    return NULL;
}

int validarCodPostal(const short zona, const short local)
{
    return 1;
}

/**
 * @brief Validar ano do carro
 * 
 * @param ano Ano do carro
 * @return int 1,se válido; int 0, se inválido;
 */
int validarAnoCarro(const short ano)
{
    if (ano < 1885 || ano > DATA_ATUAL.ano) return 0;
    
    return 1;
}
/**
 * @brief Validar matricula de veículo
 * 
 * @param matricula Matricula de um veículo
 * @return int 1,se válido; int 0, se inválido; 
 */
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
/**
 * @brief Valida a marca do veículo
 * 
 * @param marca Marca
 * @return char* NULL caso válido, mensagem de erro caso contrário
 */
char * validarMarca(char *marca) {
    validarNome(marca);
    return NULL;
}

/**
 * @brief Valida o modelo do veículo
 * 
 * @param modelo Modelo
 * @return char* NULL caso válido, mensagem de erro caso contrário
 */
char * validarModelo(char *modelo) {
    validarNome(modelo);
    return NULL;
}

/**
 * @brief Validar código de veículo
 * 
 * @param codigo Código do veículo
 * @return int 0, se o Código do veículo for zero ou negativo, logo é inválido; int 1, se o código do veículo maior ou igual que 1, logo válido;
 */
int validarCodVeiculo(const int codigo) {
    if (codigo < 1) return 0;
    return 1;
}

/**
 * @brief Validar código de sensor
 * 
 * @param codSensor Código do Sensor
 * @return int 0, se o Código do sensor for zero ou negativo, logo é inválido; int 1, se o código do sensor maior ou igual que 1, logo válido;
 */
int validarCodSensor(const int codSensor) {
    if (codSensor < 1) return 0;
    return 1;
}

/**
 * @brief Validar distâncias
 * 
 * @param distancia Distância
 * @return int 0 se for negativa, logo é inválida; int 1 se for positiva, logo é válida;
 */
int validarDistancia(const float distancia) {
    if (distancia < 0) return 0;
    return 1;
}

/**
 * @brief validar data
 * 
 * @param date Data
 * @param modo escrever mensagem de erro se 1;
 * @return int 0 se for inválida; int 1 se válida;
 */
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

    // Validação básica dos campos
    if (ano < 1 || mes < 1 || mes > 12 || dia < 1 || 
        hora < 0 || hora >= 24 || min < 0 || min >= 60 || seg < 0 || seg >= 60) {
        if (modo == '1') printf("Por favor, insira uma data válida!\n");
        return 0;
    }

    // Validar se a data não está no futuro
    if ((ano > ano_atual) ||
        (ano == ano_atual && mes > mes_atual) ||
        (ano == ano_atual && mes == mes_atual && dia > dia_atual) ||
        (ano == ano_atual && mes == mes_atual && dia == dia_atual && hora > hora_atual) ||
        (ano == ano_atual && mes == mes_atual && dia == dia_atual && hora == hora_atual && min > min_atual) ||
        (ano == ano_atual && mes == mes_atual && dia == dia_atual && hora == hora_atual && min == min_atual && seg > seg_atual)) {
        if (modo == '1') printf("Por favor, insira uma data válida (não pode ser futura)!\n");
        return 0;
    }

    // Dias máximos de cada mês
    short dias_por_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Ajustar para ano bissexto (Fevereiro com 29 dias)
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        dias_por_mes[1] = 29;
    }

    // Validar o dia do mês
    if (dia > dias_por_mes[mes - 1]) {
        if (modo == '1') {
            const char *nome_do_mes[] = {
                "janeiro", "fevereiro", "março", "abril", "maio", "junho",
                "julho", "agosto", "setembro", "outubro", "novembro", "dezembro"
            };
            printf("Data inválida: O mês de %s tem apenas %hd dias.\n", nome_do_mes[mes - 1], dias_por_mes[mes - 1]);
        }
        return 0;
    }

    return 1;
}

/**
 * @brief Validar registo
 * 
 * @param tipoRegisto Registo
 * @return int 1,se válido; int 0, se inválido;
 */
int validarTipoRegisto(const char tipoRegisto) {
    if (tipoRegisto == '0' || tipoRegisto == '1') return 1;
    return 0;
}