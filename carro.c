#include "carro.h"
#include "validacoes.h"
#include "bdados.h"
#include "uteis.h"
#include "structsGenericas.h"

/**
 * @brief Inserir um carro na base de dados
 * 
 * @param bd Base de dados
 * @param matricula Matrícula
 * @param marca Marca
 * @param modelo Modelo
 * @param ano Ano
 * @param nif Nif (0 caso não haja dono)
 * @param codVeiculo Código do veículo
 * @return int 0 se erro, 1 se sucesso
 * 
 * @note Não faz validações
 */
int inserirCarroLido(Bdados *bd, char *matricula, char *marca, char *modelo, short ano, int nif, int codVeiculo) {
    if (!bd) return 0;
    
    Carro *aut = (Carro *)malloc(sizeof(Carro));
    if (!aut) return 0;
    
    //Matrícula
    strcpy(aut->matricula, matricula);
    //Marca
    aut->marca = (char *)malloc(strlen(marca) * sizeof(char) + 1);
    if (!aut->marca) {
        free(aut);
        return 0;
    }
    strcpy(aut->marca, marca);
    //Modelo
    aut->modelo = (char *)malloc(strlen(modelo) * sizeof(char) + 1);
    if (!aut->modelo) {
        free(aut->marca);
        free(aut);
        return 0;
    }
    strcpy(aut->modelo, modelo);
    //Ano
    aut->ano = ano;
    //NIF (ptrPessoa)
    if (nif != 0) {
        void *temp = (void *)&nif;
        aut->ptrPessoa = (Dono *)searchDict(bd->donosNif, temp, compChaveDonoNif ,compCodDono, hashChaveCarroCod);
    }
    else {
        aut->ptrPessoa = NULL;
    }
    //Código Veículo
    aut->codVeiculo = codVeiculo;
    aut->viagens = NULL;
    
    //Associar o carro ao dono
    if (aut->ptrPessoa) {
		if (!aut->ptrPessoa->carros) {
            aut->ptrPessoa->carros = criarLista();
            if (!aut->ptrPessoa->carros) {
                free(aut->modelo);
                free(aut->marca);
                free(aut);
                return 0;
            }
        }
        if (!addInicioLista(aut->ptrPessoa->carros, (void *)aut)) {
            free(aut->modelo);
            free(aut->marca);
            free(aut);
            return 0;
        }
	}

    if (!appendToDict(bd->carrosCod, (void *)aut, compChaveCarroCod, criarChaveCarroCod, hashChaveCarroCod, freeCarro, freeChaveCarroCod)) {
        free(aut->modelo);
        free(aut->marca);
        free(aut);
        return 0;
    }

    if (!appendToDict(bd->carrosMarca, (void *)aut, compChaveCarroMarca, criarChaveCarroMarca, hashChaveCarroMarca, freeCarro, freeChaveCarroMarca)) {
        free(aut->modelo);
        free(aut->marca);
        free(aut);
        return 0;
    }
    
    return 1;
}

/**
 * @brief Compara 2 carros
 * 
 * @param carro1 Carro 1
 * @param carro2 Carro 2
 * @return int -1 se carro1 < carro2, 0 se iguais, 1 se carro1 > carro2
 */
int compararCarros(void *carro1, void *carro2) {
    if (carro1 == NULL && carro2 == NULL) return 0;
    if (carro1 == NULL) return -1; //NULL < qualquer coisa
    if (carro2 == NULL) return 1;

    Carro *x = (Carro *)carro1;
    Carro *y = (Carro *)carro2;

    if (x->codVeiculo > y->codVeiculo) return 1;
    if (x->codVeiculo < y->codVeiculo) return -1;
    return 0;
}

/**
 * @brief Compara o código de um veículo com um código
 * 
 * @param carro Carro
 * @param codigo Código (será convertido para int*)
 * @return int 0 se igual
 */
int compCodCarro(void *carro, void *codigo) {
    if (!carro || !codigo) return 1;

    Carro *x = (Carro *)carro;
    int *cod = (int *)codigo;
    if (x->codVeiculo == *cod) return 0;
    return 1;
}

/**
 * @brief Liberta a memória associada a um Carro
 * 
 * @param carro Carro
 */
void freeCarro(void *carro) {
    Carro *obj = (Carro *)carro;
    if (obj->marca) free(obj->marca);
    if (obj->modelo) free(obj->modelo);
    if (obj->viagens) {
        freeLista(obj->viagens, NULL);
    }
    free(obj);
}

/**
 * @brief Mostra um carro
 * 
 * @param carro Carro a mostrar
 * @param file Ficheiro
 */
void printCarro(void *carro, FILE *file) {
    if (!carro || !file) return;

    Carro *c = (Carro *)carro;
    
    fprintf(file, "Código do Veículo: %d\n", c->codVeiculo);
    fprintf(file, "Matrícula: %s\n", c->matricula);
    fprintf(file, "Marca: %s\n", c->marca ? c->marca : "n/a");
    fprintf(file, "Modelo: %s\n", c->modelo ? c->modelo : "n/a");
    fprintf(file, "Ano do Veículo: %d\n", c->ano);
    if (c->ptrPessoa) {
        fprintf(file, "NIF do Dono: %d\n", c->ptrPessoa->nif);
        fprintf(file, "Nome do Dono: %s\n", c->ptrPessoa->nome);
    }
    else {
        fprintf(file, "NIF do Dono: n/a\n");
        fprintf(file, "Nome do Dono: n/a\n");
    }
}

/**
 * @brief Guarda um elemento do tipo Carro num ficheiro binário
 * 
 * @param carro Carro
 * @param file Ficheiro binário
 */
void guardarCarroBin(void *carro, FILE *file) {
    if (!carro || !file) return;

    Carro *x = (Carro *)carro;
    fwrite(&x->ano, sizeof(short), 1, file);
    fwrite(&x->codVeiculo, sizeof(int), 1, file);
    fwrite(x->matricula, sizeof(x->matricula), 1, file);

    size_t tamanhoMarca = strlen(x->marca) + 1;
    fwrite(&tamanhoMarca, sizeof(size_t), 1, file);
    fwrite(x->marca, tamanhoMarca, 1, file);

    size_t tamanhoModelo = strlen(x->modelo) + 1;
    fwrite(&tamanhoModelo, sizeof(size_t), 1, file);
    fwrite(x->modelo, tamanhoModelo, 1, file);
    if (x->ptrPessoa) {
        fwrite(&x->ptrPessoa->nif, sizeof(int), 1, file);
    }
    else {
        int nif = 0; // caso o dono não exista ainda
        fwrite(&nif, sizeof(int), 1, file);
    }
}

/**
 * @brief Lê um carro para memória
 * 
 * @param file Ficheiro binário, aberto
 * @return void* Carro ou NULL se erro
 * 
 * @note Cria uma estrutura Dono para guardar o Nif, deve ser posteriormente eliminada e obtido o respetivo ponteiro
 */
void *readCarroBin(FILE *file) {
    if (!file) return NULL;

    Carro *x = (Carro *)malloc(sizeof(Carro));
    if (!x) return NULL;

    x->viagens = NULL;

    fread(&x->ano, sizeof(short), 1, file);
    fread(&x->codVeiculo, sizeof(int), 1, file);
    fread(x->matricula, sizeof(x->matricula), 1, file);

    size_t tamanhoMarca;
    fread(&tamanhoMarca, sizeof(size_t), 1, file);
    x->marca = (char *)malloc(tamanhoMarca);
    if (!x->marca) {
        free(x);
        return NULL;
    }
    fread(x->marca, tamanhoMarca, 1, file);

    size_t tamanhoModelo;
    fread(&tamanhoModelo, sizeof(size_t), 1, file);
    x->modelo = (char *)malloc(tamanhoModelo);
    if (!x->modelo) {
        free(x->marca);
        free(x);
        return NULL;
    }
    fread(x->modelo, tamanhoModelo, 1, file);

    int nif = 0;
    fread(&nif, sizeof(int), 1, file);
    if (nif == 0) {
        x->ptrPessoa = NULL;
    }
    else {
        Dono *d = (Dono *)malloc(sizeof(Dono));
        if (!d) {
            free(x->marca);
            free(x->modelo);
            free(x);
            return NULL;
        }
        x->ptrPessoa = d;
        x->ptrPessoa->nif = nif;
    }
    
    return (void *)x;
}

// Chave por Marca

/**
 * @brief Guarda a chave do carro por marca em ficheiro binário
 * 
 * @param chaveMarca Chave
 * @param file Ficheiro binário, aberto
 */
void guardarChaveCarroMarca(void *chaveMarca, FILE *file) {
    if (!chaveMarca || !file) return;

    char *chave = (char *)chaveMarca;

    size_t comp = strlen(chave) + 1;
    fwrite(&comp, sizeof(size_t), 1, file);
    fwrite(chave, comp, 1, file);
}

/**
 * @brief Cria uma chave para o carro consoante a marca
 * 
 * @param carro Carro
 * @return void* Chave(tipo void) ou NULL se erro
 * 
 * @note Coloca a chave em minúscula
 */
void *criarChaveCarroMarca(void *carro) {
    if (!carro) return NULL;

    Carro *x = (Carro *)carro;

    return (void *)strlwrSafe(x->marca);
}

/**
 * @brief Função de hash para o carro por marca
 * 
 * @param chave Chave (marca)
 * @return int hash ou -1 se erro
 */
int hashChaveCarroMarca(void *chave) {
    if (!chave) return -1;

    char *key = (char *)chave;
    char *marcaNorm = normString(key);
    int hash = hashString(marcaNorm);
    free(marcaNorm); // Liberta a cópia criada por normString
    return hash;
}

/**
 * @brief Liberta a memória da chave dos carros por marca
 * 
 * @param chave Chave
 */
void freeChaveCarroMarca(void *chave) {
    if (!chave) return;

    char *key = (char *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos carros pela marca
 * 
 * @param chave Chave
 * @param carro Carro
 * @return int -1 se erro, 0 se iguais, 1 se diferente
 */
int compChaveCarroMarca(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    char *key = (char *)chave;
    char *key2 = (char *)chave2;

    if (strcmp(key, key2) == 0) {
        return 0;
    }
    return 1;
} 

// Chave por Código

/**
 * @brief Guarda a chave dos carros por código em ficheiro binário
 * 
 * @param chave Chave
 * @param file Ficheiro binário
 */
void guardarChaveCarroCod(void *chave, FILE *file) {
    if (!chave || !file) return;

    int *key = (int *)chave;

    fwrite(key, sizeof(int), 1, file);
}

/**
 * @brief Lê a chave do carro por código
 * 
 * @param file Ficheiro binário, aberto
 * @return void* chave ou NULL se erro
 */
void *readChaveCarroCod(FILE *file) {
    if (!file) return NULL;

    int *chave = (int *)malloc(sizeof(int));
    if (!chave) return NULL;
    fread(chave, sizeof(int), 1, file);
    
    return (void *)chave;
}

/**
 * @brief Cria uma chave para um carro pelo código
 * 
 * @param carro Carro
 * @return void* chave ou NULL se erro
 */
void *criarChaveCarroCod(void *carro) {
    if (!carro) return NULL;

    Carro *x = (Carro *)carro;
    int *chave = (int *)malloc(sizeof(int));
    *chave = x->codVeiculo;

    return (void *)chave;
}

/**
 * @brief Função de hash para carro por Cod
 * 
 * @param chave Chave
 * @return int -1 se erro ou a chave
 */
int hashChaveCarroCod(void *chave) {
    if (!chave) return -1;

    int *key = (int *)chave;
    return *key;
}

/**
 * @brief Liberta a memória alocada para a chave dos carros por código
 * 
 * @param chave Chave
 */
void freeChaveCarroCod(void *chave) {
    if (!chave) return;

    int *key = (int *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos carros pelo código
 * 
 * @param chave Chave
 * @param chave2 Chave 2
 * @return int -1 se erro, 0 se igual, 1 se diferente
 */
int compChaveCarroCod(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    int *key = (int *)chave;
    int *key2 = (int *)chave2;

    if (*key == *key2) return 0;
    return 1;
}

/**
 * @brief Cria uma chave para o carro por matrícula
 * 
 * @param carro Carro
 * @return void* Chave(tipo void) ou NULL se erro
 * 
 * @note Coloca a chave em minúscula
 */
void *criarChaveCarroMatricula(void *matricula) {
    if (!matricula) return NULL;

    Carro *x = (Carro *)matricula;

    return (void *)strlwrSafe(x->matricula);
}

/**
 * @brief Função de hash para o carro por matricula
 * 
 * @param chave Chave (matrícula)
 * @return int hash ou -1 se erro
 */
int hashChaveCarroMatricula(void *chave) {
    if (!chave) return -1;

    char *key = (char *)chave;
    char *matNorm = normString(key);
    int hash = hashString(matNorm);
    free(matNorm); // Liberta a cópia criada por normString
    return hash;
}

/**
 * @brief Liberta a memória da chave dos carros por matrícula
 * 
 * @param chave Chave
 */
void freeChaveCarroMatricula(void *chave) {
    if (!chave) return;

    char *key = (char *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos carros pela matricula
 * 
 * @param chave Chave
 * @param chave2 Chave
 * @return int -1 se erro, 0 se iguais, 1 se diferente
 */
int compChaveCarroMatricula(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    char *key = (char *)chave;
    char *key2 = (char *)chave2;

    if (strcmp(key, key2) == 0) {
        return 0;
    }
    return 1;
} 

/**
 * @brief Compara as chaves dos carros pelos rankings (ints)
 * 
 * @param chave Chave
 * @param carro Carro
 * @return int -1 se erro, 0 se igual, 1 se diferente
 * 
 * @note Ordena por ordem descrescente (mais primeiro)
 */
int compChaveCarroRankingInt(void *chave, void *chave2) {
    if (chave == NULL && chave2 == NULL) return 0;
    if (chave == NULL) return -1; //NULL < qualquer coisa
    if (chave2 == NULL) return 1;

    int *key = (int *)chave;
    int *key2 = (int *)chave2;

    if (*key < *key2) return 1;
    else if (*key > *key2) return -1;
    return 0;
}

/**
 * @brief Compara as chaves dos carros pelos rankings (floats)
 * 
 * @param chave Chave
 * @param carro Carro
 * @return int -1 se erro, 0 se igual, 1 se diferente
 * 
 * @note Ordena por ordem descrescente (mais primeiro)
 */
int compChaveCarroRankingFloat(void *chave, void *chave2) {
    if (chave == NULL && chave2 == NULL) return 0;
    if (chave == NULL) return -1; //NULL < qualquer coisa
    if (chave2 == NULL) return 1;

    float *key = (float *)chave;
    float *key2 = (float *)chave2;

    if (*key < *key2) return 1;
    else if (*key > *key2) return -1;
    return 0;
}

/**
 * @brief Liberta a memória alocada para uma chave de carro por ranking (ints)
 * 
 * @param chave Chave
 */
void freeChaveCarroRankingInt(void *chave) {
    if (!chave) return;

    int *key = (int *)chave;
    free(key);
}

/**
 * @brief Liberta a memória alocada para uma chave de carro por ranking
 * 
 * @param chave Chave
 */
void freeChaveCarroRankingFloat(void *chave) {
    if (!chave) return;

    float *key = (float *)chave;
    free(key);
}

/**
 * @brief Compara os carros pela marca
 * 
 * @param carro1 Carro 1
 * @param carro2 Carro 2
 * @return int -1 se carro1 < carro2, 0 se iguais, 1 se carro1 > carro2
 */
int compCarroMarca (void *carro1, void *carro2) {
    if (!carro1 || !carro2) return 0;
    if (!carro1) return -1;
    if (!carro2) return 1;

    Carro *x = (Carro *)carro1;
    Carro *y = (Carro *)carro2;
    char *xNorm = normString(x->marca);
    char *yNorm = normString(y->marca);

    int res = strcmp(xNorm, yNorm);

    free(xNorm);
    free(yNorm);

    return res;
}

/**
 * @brief Compara os carros pela matrícula
 * 
 * @param carro1 Carro 1
 * @param carro2 Carro 2
 * @return int -1 se carro1 < carro2, 0 se iguais, 1 se carro1 > carro2
 */
int compCarroMatricula(void *carro1, void *carro2) {
    if (!carro1 || !carro2) return 0;
    if (!carro1) return -1;
    if (!carro2) return 1;

    Carro *x = (Carro *)carro1;
    Carro *y = (Carro *)carro2;

    return stricmpSafe(x->matricula, y->matricula);
}

/**
 * @brief Compara os carros pelo modelo
 * 
 * @param carro1 Carro 1
 * @param carro2 Carro 2
 * @return int -1 se carro1 < carro2, 0 se iguais, 1 se carro1 > carro2
 */
int compCarroModelo(void *carro1, void *carro2) {
    if (!carro1 || !carro2) return 0;
    if (!carro1) return -1;
    if (!carro2) return 1;

    Carro *x = (Carro *)carro1;
    Carro *y = (Carro *)carro2;

    return stricmpSafe(x->modelo, y->modelo);
}

/**
 * @brief Mostra um carro em formato XML
 * 
 * @param carro Carro a mostrar
 * @param indentacao Indentação a usar no início
 * @param file Ficheiro to tipo .xml (ou .txt), aberto
 */
void printCarroXML(void *carro, int indentacao, FILE *file) {
    if (!carro || indentacao < 0 || !file) return;

    Carro *c = (Carro *)carro;
    
    indent(indentacao, file);
	fprintf(file, "<carro>\n");
	
    indent(indentacao + 1, file);
    fprintf(file, "<codVeiculo>%d</codVeiculo>\n", c->codVeiculo);
    indent(indentacao + 1, file);
    fprintf(file, "<matricula>%s</matricula>\n", c->matricula);
    indent(indentacao + 1, file);
    fprintf(file, "<ano>%d</ano>\n", c->ano);
    indent(indentacao + 1, file);
    fprintf(file, "<marca>%s</marca>\n", c->marca ? c->marca : "n/a");
    indent(indentacao + 1, file);
    fprintf(file, "<modelo>%s</modelo>\n", c->modelo ? c->modelo : "n/a");
    indent(indentacao + 1, file);
    if (c->ptrPessoa) fprintf(file, "<nifDono>%d</nifDono>\n", c->ptrPessoa->nif);
    else fprintf(file, "<nifDono>%s</nifDono>\n", "n/a");
	
	indent(indentacao, file);
	fprintf(file, "</carro>\n");
}

/**
 * @brief Escreve os headers dos carros em formato CSV
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderCarrosCSV(FILE *file) {
    if (!file) return;

    fprintf(file, "Código do veículo, Ano, Matrícula, Marca, Modelo, Nif Dono\n");
}

/**
 * @brief Mostra um carro em formato CSV
 * 
 * @param carro Carro a mostrar
 * @param file Ficheiro .csv, aberto
 */
void printCarroCSV(void *carro, FILE *file) {
    if (!carro || !file) return;

    Carro *c = (Carro *)carro;

    fprintf(file, "%d, %hd, %s, %s, %s, %d\n", c->codVeiculo, c->ano, c->matricula, c->marca ? c->marca : "n/a", 
        c->modelo ? c->modelo : "n/a", c->ptrPessoa ? c->ptrPessoa->nif : -1);
}

/**
 * @brief Escreve os headers dos carros em formato TXT (separado por tabs)
 * 
 * @param file Ficheiro .txt, aberto
 */
void printHeaderCarrosTXT(FILE *file) {
    if (!file) return;

    fprintf(file, "Código do veículo\tAno\tMatrícula\tMarca\tModelo\tNif Dono\n");
}

/**
 * @brief Mostra um carro em formato TXT (separado por tabs)
 * 
 * @param carro Carro a mostrar
 * @param file Ficheiro .txt, aberto
 */
void printCarroTXT(void *carro, FILE *file) {
    if (!carro || !file) return;

    Carro *c = (Carro *)carro;

    fprintf(file, "%d\t%hd\t%s\t%s\t%s\t%d\n", c->codVeiculo, c->ano, c->matricula, c->marca ? c->marca : "n/a", 
        c->modelo ? c->modelo : "n/a", c->ptrPessoa ? c->ptrPessoa->nif : -1);
}

/**
 * @brief Mostra o header da tabela dos carros ao exportar para HTML
 * 
 * @param file Ficheiro .html, aberto
 */
void printHeaderCarrosHTML(FILE *file) {
    if (!file) return;

    fprintf(file,
        "\t\t\t\t\t<tr>\n"
        "\t\t\t\t\t\t<th>CodVeiculo</th>\n"
        "\t\t\t\t\t\t<th>Ano</th>\n"
        "\t\t\t\t\t\t<th>Matrícula</th>\n"
        "\t\t\t\t\t\t<th>Marca</th>\n"
        "\t\t\t\t\t\t<th>Modelo</th>\n"
        "\t\t\t\t\t\t<th>NIF Dono</th>\n"
        "\t\t\t\t\t</tr>\n");
}

/**
 * @brief Mostra um carro em formato HTML
 * 
 * @param carro Carro 
 * @param file Ficheiro .html, aberto
 */
void printCarroHTML(void *carro, FILE *file) {
    if (!carro || !file) return;

    Carro *c = (Carro *)carro;

    fprintf(file,
        "\t\t\t\t\t<tr>\n"
        "\t\t\t\t\t\t<th>%d</th>\n"
        "\t\t\t\t\t\t<th>%d</th>\n"
        "\t\t\t\t\t\t<th>%s</th>\n"
        "\t\t\t\t\t\t<th>%s</th>\n"
        "\t\t\t\t\t\t<th>%s</th>\n"
        "\t\t\t\t\t\t<th>%d</th>\n"
        "\t\t\t\t\t</tr>\n", c->codVeiculo, c->ano, c->matricula, c->marca ? c->marca : "n/a", c->modelo ? c->modelo : "n/a", c->ptrPessoa ? c->ptrPessoa->nif : -1);
}

/**
 * @brief Mostra um carro de um ranking
 * 
 * @param no No do ranking
 * @param printHeaderCompObj Função para mostrar o header da compInfo
 * @param printCompObj Função para mostrar o compInfo
 * @param file Ficheiro onde mostrar
 */
void printCarroRanking(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file) {
    if (!no || !file) return;

    Carro *c = (Carro *)no->mainInfo;
    const int WIDTH_MATRICULA = 15;
    const int WIDTH_DONO = 30;
    const int WIDTH_MARCA = 20;
    
    // Prepara strings truncadas se necessário
    char donoStr[WIDTH_DONO + 1];
    char marcaStr[WIDTH_MARCA + 1];
    
    // Processa nome do dono
    const char* nomeDono = c->ptrPessoa ? c->ptrPessoa->nome : "n/a";
    if (strlen(nomeDono) > (size_t) WIDTH_DONO - 3) {
        strncpy(donoStr, nomeDono, WIDTH_DONO - 3);
        donoStr[WIDTH_DONO - 3] = '.';
        donoStr[WIDTH_DONO - 2] = '.';
        donoStr[WIDTH_DONO - 1] = '.';
        donoStr[WIDTH_DONO] = '\0';
    } else {
        strcpy(donoStr, nomeDono);
    }

    // Processa marca
    if (strlen(c->marca) > (size_t) WIDTH_MARCA - 3) {
        strncpy(marcaStr, c->marca, WIDTH_MARCA - 3);
        marcaStr[WIDTH_MARCA - 3] = '.';
        marcaStr[WIDTH_MARCA - 2] = '.';
        marcaStr[WIDTH_MARCA - 1] = '.';
        marcaStr[WIDTH_MARCA] = '\0';
    } else {
        strcpy(marcaStr, c->marca);
    }
    
    // Dados alinhados com as mesmas larguras
    fprintf(file, "%-*s\t%-*s\t%-*s", WIDTH_MATRICULA, c->matricula, WIDTH_DONO, donoStr, WIDTH_MARCA, marcaStr);
            
    if (printCompObj) {
        printCompObj(no->compInfo, file);
    } else {
        fprintf(file, "\n");
    }
}

/**
 * @brief Mostra um carro de um ranking em ficheiro TXT
 * 
 * @param no Nó a mostrar
 * @param printCompObj Função para mostrar no->compInfo
 * @param file Ficheiro .txt, aberto
 */
void printCarroRankingTXT(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file) {
    if (!no || !file) return;

    Carro *c = (Carro *)no->mainInfo;
    
    fprintf(file, "\t%s\t%s\t%s", c->matricula, c->ptrPessoa ? c->ptrPessoa->nome : "n/a", c->marca);
            
    if (printCompObj) {
        fprintf(file, "\t");
        printCompObj(no->compInfo, file);
    } else {
        fprintf(file, "\tn/a\n");
    }
}

/**
 * @brief Mostra um carro de um ranking em ficheiro CSV
 * 
 * @param no Nó a mostrar
 * @param printCompObj Função para mostrar no->compInfo
 * @param file Ficheiro .csv, aberto
 */
void printCarroRankingCSV(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file) {
    if (!no || !file) return;

    Carro *c = (Carro *)no->mainInfo;
    
    fprintf(file, ", %s, %s, %s", c->matricula, c->ptrPessoa ? c->ptrPessoa->nome : "n/a", c->marca);
            
    if (printCompObj) {
        fprintf(file, ", ");
        printCompObj(no->compInfo, file);
    } else {
        fprintf(file, ", n/a\n");
    }
}

/**
 * @brief Mostra uma marca de um ranking
 * 
 * @param no No do ranking
 * @param printHeaderCompObj Função para mostrar o header da compInfo
 * @param printCompObj Função para mostrar o compInfo
 * @param file Ficheiro onde mostrar
 */
void printMarcaRanking(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file) {
    if (!no || !file) return;

    Carro *c = (Carro *)no->mainInfo;
    const int WIDTH_MARCA = 20;
    
    // Processa marca
    char marcaStr[WIDTH_MARCA + 1];
    if (strlen(c->marca) > (size_t) WIDTH_MARCA - 3) {
        strncpy(marcaStr, c->marca, WIDTH_MARCA - 3);
        marcaStr[WIDTH_MARCA - 3] = '.';
        marcaStr[WIDTH_MARCA - 2] = '.';
        marcaStr[WIDTH_MARCA - 1] = '.';
        marcaStr[WIDTH_MARCA] = '\0';
    } else {
        strcpy(marcaStr, c->marca);
    }
    
    // Mostra apenas a marca
    fprintf(file, "%-*s", WIDTH_MARCA, marcaStr);
            
    if (printCompObj) {
        printCompObj(no->compInfo, file);
    } else {
        fprintf(file, "\n");
    }
}

/**
 * @brief Mostra uma marca de um ranking em ficheiro TXT
 * 
 * @param no Nó a mostrar
 * @param printCompObj Função para mostrar no->compInfo
 * @param file Ficheiro .txt, aberto
 */
void printMarcaRankingTXT(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file) {
    if (!no || !file) return;

    Carro *c = (Carro *)no->mainInfo;
    
    fprintf(file, "\t%s", c->marca ? c->marca : "n/a");
            
    if (printCompObj) {
        fprintf(file, "\t");
        printCompObj(no->compInfo, file);
    } else {
        fprintf(file, "\tn/a\n");
    }
}

/**
 * @brief Mostra uma marca de um ranking em ficheiro CSV
 * 
 * @param no Nó a mostrar
 * @param printCompObj Função para mostrar no->compInfo
 * @param file Ficheiro .csv, aberto
 */
void printMarcaRankingCSV(NoRankings *no, void (*printCompObj)(void *compInfo, FILE *file), FILE *file) {
    if (!no || !file) return;

    Carro *c = (Carro *)no->mainInfo;
    
    fprintf(file, ", %s", c->marca ? c->marca : "n/a");
            
    if (printCompObj) {
        fprintf(file, ", ");
        printCompObj(no->compInfo, file);
    } else {
        fprintf(file, ", n/a\n");
    }
}

/**
 * @brief Escreve os headers sobre o número de infrações
 * 
 * @param file Ficheiro
 */
void printHeaderCarroMaisInfracoes(FILE *file) {
    if (!file) return;

    fprintf(file, "\n\tMatrícula\tDono\t\t\t\tMarca\t\tNº de infrações\n");
}

/**
 * @brief Mostra os dados sobre as infrações de cada veículo
 * 
 * @param compInfo Infrações
 * @param file Ficheiro
 */
void printMaisInfracoes(void *compInfo, FILE *file) {
    if (!compInfo || !file) return;

    int *info = (int *)compInfo;

    fprintf(file, "%d\n", *info);
}

/**
 * @brief Mostra os headers para ficheiro .txt
 * 
 * @param file Ficheiro .txt, aberto
 */
void printHeaderCarroMaisInfracoesTXT(FILE *file) {
    if (!file) return;

    fprintf(file, "Matrícula\tDono\tMarca\tNº de infrações\n");
}

/**
 * @brief Mostra os headers para ficheiro .csv
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderCarroMaisInfracoesCSV(FILE *file) {
if (!file) return;

    fprintf(file, "Posição, Matrícula, Dono, Marca, Nº de infrações\n");
}

/**
 * @brief Escreve os headers sobre o número de kilometros 
 * 
 * @param file Ficheiro
 */
void printHeaderCarroMaisKMS(FILE *file) {
    if (!file) return;

    fprintf(file, "\n\tMatrícula\tDono\t\t\t\tMarca\t\tTotal de quilómetros(km) percorridos\n");
}

/**
 * @brief Mostra os dados sobre o total de quilómetros percorridos 
 * 
 * @param compInfo KMS
 * @param file Ficheiro aberto
 */
void printMaisKMS(void *compInfo, FILE *file) {
    if (!compInfo || !file) return;

    float *info = (float *)compInfo;

    fprintf(file, "%.2f\n", *info);
}

/**
 * @brief Mostra os dados sobre o total de quilómetros percorridos em CSV
 * 
 * @param compInfo KMS
 * @param file Ficheiro .csv, aberto
 */
void printMaisKMS_CSV(void *compInfo, FILE *file) {
    if (!compInfo || !file) return;

    float *info = (float *)compInfo;

    char *infoStr = floatToStringPontoDecimal(*info, 2);

    fprintf(file, "%s\n", infoStr ? infoStr : "n/a");

    if (infoStr) free(infoStr);
}

/**
 * @brief Mostra os headers para ficheiro .txt
 * 
 * @param file Ficheiro .txt, aberto
 */
void printHeaderCarroMaisKMS_TXT(FILE *file) {
    if (!file) return;

    fprintf(file, "Matrícula\tDono\tMarca\tTotal de quilómetros(km) percorridos\n");
}

/**
 * @brief Mostra os headers para ficheiro .csv
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderCarroMaisKMS_CSV(FILE *file) {
if (!file) return;

    fprintf(file, "Posição, Matrícula, Dono, Marca, Total de quilómetros(km) percorridos\n");
}

/**
 * @brief Escreve os headers sobre o número de kilometros
 * 
 * @param file Ficheiro
 */
void printHeaderMarcaMaisKMS(FILE *file) {
    if (!file) return;

    fprintf(file, "\n\tMarca\t\tTotal de quilómetros(km) percorridos\n");
}

/**
 * @brief Mostra os headers para ficheiro .txt
 * 
 * @param file Ficheiro .txt, aberto
 */
void printHeaderMarcaMaisKMS_TXT(FILE *file) {
    if (!file) return;

    fprintf(file, "Posição\tMarca\tTotal de quilómetros(km) percorridos\n");
}

/**
 * @brief Mostra os headers para ficheiro .csv
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderMarcaMaisKMS_CSV(FILE *file) {
if (!file) return;

    fprintf(file, "Posição, Marca, Total de quilómetros(km) percorridos\n");
}

/**
 * @brief Obtém a marca mais comum de um dicionário de carros ordenados em listas de marcas
 * 
 * @param carrosMarca Dicionário
 * @return char* Marca mais comum ou NULL se erro
 */
char *obterMarcaMaisComum(Dict *carrosMarca) {
    if (!carrosMarca) return NULL;

    Lista *maisElementos = NULL;
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = carrosMarca->tabela[i];
        if (p && p->dados) {
            if (!maisElementos) {
                maisElementos = p->dados;
            }
            else if (p->dados->nel > maisElementos->nel) {
                maisElementos = p->dados;
            }   
        }
    }

    if (!maisElementos || !maisElementos->inicio) return NULL;

    Carro *c = (Carro *)maisElementos->inicio->info;
    if (!c) return NULL;

    return c->marca;
}

/**
 * @brief Obtém a marca de carros cuja velocidade média seja maior
 * 
 * @param bd Base de dados 
 * @return char* Marca cuja velocidade média é  maior
 */
char *obterMarcaMaisVelocidadeMedia(Bdados *bd) {
    if (!bd) return NULL;

    char *marcaMaisRapida = NULL;
    float velocidadeMax = 0.0f;
    int contadorViagens = 0;

    float tempoTotal = 0.0f;
    float distanciaTotal = 0.0f;
    float velocidadeMedia = 0.0f;

    // Iterar por todas as marcas
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = bd->carrosMarca->tabela[i];
        // Para cada marca
        tempoTotal = 0.0f;
        distanciaTotal = 0.0f;
        velocidadeMedia = 0.0f;
        while(p) {
            if (p->dados) {
                // Para cada carro na marca
                No *m = p->dados->inicio;
                while (m) {
                    Carro *c = (Carro *)m->info;
                    
                    No *x = c->viagens->inicio;
                    // Para cada viagem
                    while(x) {
                        if (x->info) {
                            Viagem *v = (Viagem *)x->info;
                            
                            tempoTotal += v->tempo;
                            distanciaTotal += v->kms;
                            contadorViagens++;
                        }
                        x = x->prox;
                    }
                    m = m->prox;
                }
            }
            p = p->prox;
        }

        // Ver velocidade média e comparar
        if (tempoTotal > 0) {
            velocidadeMedia = distanciaTotal / (tempoTotal / 60.0f);
            pressEnter();
            if (velocidadeMedia > velocidadeMax) {
                p = bd->carrosMarca->tabela[i];
                velocidadeMax = velocidadeMedia;
                Carro *primeiroCarro = (Carro *)p->dados->inicio->info;
                marcaMaisRapida = primeiroCarro->marca;
            }
        }
    }
    printf("Contador de viagens: %d\n", contadorViagens);
    return marcaMaisRapida;
}

/**
 * @brief Memória ocupada por um carro
 * 
 * @param carro Carro
 * @return size_t Memória utilizada ou 0 se erro
 */
size_t memUsageCarro(void *carro) {
    if (!carro) return 0;

    Carro *c = (Carro *)carro;

    size_t mem = sizeof(Carro);
    mem += strlen(c->marca) + 1;
    mem += strlen(c->modelo) + 1;

    return mem;
}

/**
 * @brief Memória ocupada pela chave dos carros por código
 * 
 * @param chave Chave
 * @return size_t Memória ocupada ou 0 se erro
 */
size_t memUsageChaveCarroCod(void *chave) {
    if (!chave) return 0;

    int *key = (int *)chave;
    return sizeof(*key);
}

/**
 * @brief Memória ocupada pela chave dos carros por marca
 * 
 * @param chave Chave
 * @return size_t Memória ocupada ou 0 se erro
 */
size_t memUsageChaveCarroMarca(void *chave) {
    if (!chave) return 0;

    char *key = (char *)chave;

    size_t mem = strlen(key) + 1;
    return mem;
}

/**
 * @brief Obtém um código do veículo novo pronto a atribuir
 * 
 * @param carrosCod Dicionário dos carros por código
 * @return int Código ou -1 se erro
 * 
 * @note Verifica por gaps entre todos os códigos
 */
int obterCodVeiculoNovo(Dict *carrosCod) {
    if (!carrosCod) return -1;

    int codTentativa = 1;
    void *chave;

    while (1) {
        chave = (void *)&codTentativa;
        if (!searchDict(carrosCod, chave, compChaveCarroCod, compCodCarro, hashChaveCarroCod)) {
            return codTentativa;
        }
        codTentativa++;
    }

    return -1;
}

/** NÃO USADA
 * @brief Obtém o código máximo de um carro
 * 
 * @param carrosCod Dicionário dos carros
 * @return int -1 se erro ou máximo
 */
int obterCodMaximoCarros(Dict *carrosCod) {
    if (!carrosCod) return -1;

    int max = 0;
    
    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = carrosCod->tabela[i];
        while (p) {
            if (p->dados) {
                No *ptr = p->dados->inicio;
                while (ptr) {
                    Carro *c = (Carro *)ptr->info;
                    if (c->codVeiculo > max) {
                        max = c->codVeiculo;
                    }
                    ptr = ptr->prox;
                }
            }
            p = p->prox;
        }
    }
    
    return max;
}

/**
 * @brief Pedir as informações sobre o carro e inserir na base de dados
 * 
 * @param bd 
 */
void registarCarro(Bdados *bd) {
    if (!bd) return;
    
    do {
        limpar_terminal();
        int codVeiculo = 0;
        char *marca = NULL;
        char *modelo = NULL;
        char *matricula = NULL;
        short ano = 0;
        int nif = 0;
        // Código do veículo
        codVeiculo = obterCodVeiculoNovo(bd->carrosCod);
        if (codVeiculo == -1) continue;
        // Matrícula
        do {    
            printf("Insira a matrícula do veículo: ");
            matricula = lerLinhaTxt(stdin, NULL);
            if (!matricula) {
                printf("Erro a ler a matrícula!\n\n");
                pressEnter();
                continue;
            }
            if (!validarMatricula(matricula)) {
                free(matricula);
                printf("A matrícula é inválida!\n\n");
                pressEnter();
                continue;
            }
            break;
        } while(1);
        printf("\n");
        // Marca
        do {    
            printf("Insira a marca do veículo: ");
            marca = lerLinhaTxt(stdin, NULL);
            if (!marca) {
                printf("Erro a ler a marca!\n\n");
                pressEnter();
                continue;
            }
            char *mensagemErro = validarMarca(marca);
            if (mensagemErro) {
                printf("%s\n\n", mensagemErro);
                pressEnter();
                free(marca);
                continue;
            }
            break;
        } while(1);
        printf("\n");
        // Modelo
        do {    
            printf("Insira o modelo do veículo: ");
            modelo = lerLinhaTxt(stdin, NULL);
            if (!modelo) {
                printf("Erro a ler o modelo!\n\n");
                pressEnter();
                continue;
            }
            char *mensagemErro = validarModelo(modelo);
            if (mensagemErro) {
                printf("%s\n\n", mensagemErro);
                pressEnter();
                free(modelo);
                continue;
            }
            break;
        } while(1);
        printf("\n");
        // Ano
        pedirShort(&ano, "Insira o ano do veículo: ", validarAnoCarro);
        printf("\n");
        // Dono (NIF)
        do {
            pedirInt(&nif, "Insira o NIF do dono do veículo (ou 0 caso não tenha): ", NULL);
            if (nif != 0) {
                if (!validarNif(nif)) {
                    printf("O NIF é inválido!\n\n");
                    pressEnter();
                    continue;
                }
            }
            break;
        } while(1);
        printf("\n");
        
        // Inserir na bd
        if (!inserirCarroLido(bd, matricula, marca, modelo, ano, nif, codVeiculo)) {
            free(matricula);
            free(marca);
            free(modelo);
            printf("Ocorreu um erro a registar o dono em memória. Por favor tente novamente!\n\n");
            pressEnter();
            continue;
        }
        free(matricula);
        free(marca);
        free(modelo);

        if (!sim_nao("Quer inserir mais algum dono?")) break;
    } while(1);
}

/**
 * @brief Lista todos os carros
 * 
 * @param bd Base de dados
 */
void listarCarrosTodos(Bdados *bd) {
    if (!bd) return;
    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM];
    
    printDict(bd->carrosCod, printCarro, stdout, PAUSA_LISTAGEM);
    printf("\n----FIM DE LISTAGEM----\n");
    
    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printHeaderCarrosTXT(file);
            printDict(bd->carrosCod, printCarroTXT, file, 0);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printHeaderCarrosCSV(file);
            printDict(bd->carrosCod, printCarroCSV, file, 0);
        }
        fclose(file);
    }
    pressEnter();
}

/**
 * @brief Lista todos os carros por matrícula
 * 
 * @param bd Base de dados
 */
void listarCarrosPorMatricula(Bdados *bd) {
    if (!bd) return;
    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM];
    
    Lista *carrosMat = dictToLista(bd->carrosCod);
    mergeSortLista(carrosMat, compCarroMatricula);
    printLista(carrosMat, printCarro, stdout, PAUSA_LISTAGEM);
    printf("\n----FIM DE LISTAGEM----\n\n");
    
    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printHeaderCarrosTXT(file);
            printLista(carrosMat, printCarroTXT, file, 0);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printHeaderCarrosCSV(file);
            printLista(carrosMat, printCarroCSV, file, 0);
        }
        fclose(file);
    }
    freeLista(carrosMat, NULL);
    pressEnter();
}

/**
 * @brief Lista todos os carros por marca
 * 
 * @param bd Base de dados
 */
void listarCarrosPorMarca(Bdados *bd) {
    if (!bd) return;
    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM];
    
    printDict(bd->carrosMarca, printCarro, stdout, PAUSA_LISTAGEM);
    printf("\n----FIM DE LISTAGEM----\n");
    
    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printHeaderCarrosTXT(file);
            printDict(bd->carrosMarca, printCarroTXT, file, 0);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printHeaderCarrosCSV(file);
            printDict(bd->carrosMarca, printCarroCSV, file, 0);
        }
        fclose(file);
    }
    pressEnter();
}

/**
 * @brief Lista todos os carros por modelo
 * 
 * @param bd Base de dados
 */
void listarCarrosPorModelo(Bdados *bd) {
    if (!bd) return;
    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM] = {0};
    
    Lista *carrosMod = dictToLista(bd->carrosCod);
    mergeSortLista(carrosMod, compCarroModelo);
    printLista(carrosMod, printCarro, stdout, PAUSA_LISTAGEM);
    printf("\n----FIM DE LISTAGEM----\n");
    
    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printHeaderCarrosTXT(file);
            printLista(carrosMod, printCarroTXT, file, 0);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printHeaderCarrosCSV(file);
            printLista(carrosMod, printCarroCSV, file, 0);
        }
        fclose(file);
    }
    freeLista(carrosMod, NULL);
    pressEnter();
}

/**
 * @brief Lista os carros quer circularam durante um determinado período de tempo
 * 
 * @param bd Base de dados
 */
void listarCarrosPorPeriodoTempo(Bdados *bd) {
    if (!bd) return;
    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM] = {0};
    
    int count = 0;
    Data inicio = {0,0,0,0,0,0.0f};
    Data fim = {0,0,0,0,0,0.0f};
    pedirPeriodoTempo(&inicio, &fim, "Insira a data inicial: ", "Insira a data final: ");

    No *p = bd->viagens->inicio;
    int sair = 0; // flag
    int countTotal = 0;
    while(p && sair == 0) {
        Viagem *v = (Viagem *)p->info;
        if (compararDatas(fim, v->entrada->data) >= 0 && compararDatas(inicio, v->saida->data) <= 0) {
            count++;
            printViagem(p->info, stdout);
            if (count % PAUSA_LISTAGEM == 0) {
                printf("\n");
                int opcao = enter_espaco_esc();
                switch (opcao) {
                    case 0:
                        break;
                    case 1:
                        while(countTotal < bd->viagens->nel - PAUSA_LISTAGEM) {
                            p = p->prox;
                            countTotal++;
                        }
                        break;
                    case 2:
                        sair = 1;
                        break;
                    default:
                        break;
                }
            }
            printf("\n");
        }
        countTotal++;
        p = p->prox;
    }
    printf("\n----FIM DE LISTAGEM----\n");

    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printHeaderViagensTXT(file);
            No *p = bd->viagens->inicio;
            while(p) {
                Viagem *v = (Viagem *)p->info;
                if (compararDatas(fim, v->entrada->data) >= 0 && compararDatas(inicio, v->saida->data) <= 0) {
                    printViagemTXT(p->info, file);
                }
                p = p->prox;
            }
        }
        else if (strcmp(formato, ".csv") == 0) {
            printHeaderViagensCSV(file);
            No *p = bd->viagens->inicio;
            while(p) {
                Viagem *v = (Viagem *)p->info;
                if (compararDatas(fim, v->entrada->data) >= 0 && compararDatas(inicio, v->saida->data) <= 0) {
                    printViagemCSV(p->info, file);
                }
                p = p->prox;
            }
        }
        fclose(file);
    }
    pressEnter();
}

/**
 * @brief Lista as infrações de um dado período de tempo
 * 
 * @param bd Base de dados
 */
void listarInfracoesPorPeriodoTempo(Bdados *bd) {
    if (!bd) return;

    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM] = {0};
    
    Data inicio = {0,0,0,0,0,0.0f};
    Data fim = {0,0,0,0,0,0.0f};
    pedirPeriodoTempo(&inicio, &fim, "Insira a data inicial: ", "Insira a data final: ");

    Ranking *r = criarRanking();
    if (!r) {
        printf("Ocorreu um erro inesperado! Por favor tente novamente mais tarde!\n");
        pressEnter();
    }

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = bd->carrosCod->tabela[i];
        while(p) {
            if (p->dados) {
                No *m = p->dados->inicio;
                while(m) {
                    Carro *c = (Carro *)m->info;

                    int infracoes = 0;

                    if (c->viagens) {
                        No *l = c->viagens->inicio;
                        while(l) {
                            Viagem *v = (Viagem *)l->info;
                            
                            if (compararDatas(fim, v->entrada->data) >= 0 && compararDatas(inicio, v->saida->data) <= 0) {
                                if (v->velocidadeMedia > MAX_VELOCIDADE_AE || v->velocidadeMedia < MIN_VELOCIDADE_AE) {
                                    infracoes++;
                                }
                            }
                            l = l->prox;
                        }
                    }
                    if (infracoes > 0) {
                        int *infr = (int *)malloc(sizeof(int));
                        *infr = infracoes;

                        void *temp = (void *)infr;
                        addToRanking(r, (void *)c, temp);
                    }
                    m = m->prox;
                }
            }
            p = p->prox;
        }
    }

    mergeSortRanking(r, compChaveCarroRankingInt);

    printRanking(r, printCarroRanking, printHeaderCarroMaisInfracoes, printMaisInfracoes, stdout, PAUSA_LISTAGEM);

    printf("\n----FIM DE LISTAGEM----\n");

    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printRanking(r, printCarroRankingTXT, printHeaderCarroMaisInfracoesTXT, printMaisInfracoes, file, PAUSA_LISTAGEM);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printRanking(r, printCarroRankingCSV, printHeaderCarroMaisInfracoesCSV, printMaisInfracoes, file, PAUSA_LISTAGEM);
        }
        fclose(file);
    }
    freeRanking(r, NULL, freeChaveCarroRankingInt);
    pressEnter();
}

/**
 * @brief Ranking das infrações
 * 
 * @param bd Base de dados
 */
void rankingInfracoes(Bdados *bd) {
    if (!bd) return;

    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM] = {0};

    Ranking *r = criarRanking();
    if (!r) {
        printf("Ocorreu um erro inesperado! Por favor tente novamente mais tarde!\n");
        pressEnter();
    }

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = bd->carrosCod->tabela[i];
        while(p) {
            if (p->dados) {
                No *m = p->dados->inicio;
                while(m) {
                    Carro *c = (Carro *)m->info;

                    int infracoes = 0;

                    if (c->viagens) {
                        No *l = c->viagens->inicio;
                        while(l) {
                            Viagem *v = (Viagem *)l->info;
                            
                            if (v->velocidadeMedia > MAX_VELOCIDADE_AE || v->velocidadeMedia < MIN_VELOCIDADE_AE) {
                                infracoes++;
                            }
                            l = l->prox;
                        }
                    }
                    if (infracoes > 0) {
                        int *infr = (int *)malloc(sizeof(int));
                        *infr = infracoes;

                        void *temp = (void *)infr;
                        addToRanking(r, (void *)c, temp);
                    }
                    m = m->prox;
                }
            }
            p = p->prox;
        }
    }

    mergeSortRanking(r, compChaveCarroRankingInt);

    printRanking(r, printCarroRanking, printHeaderCarroMaisInfracoes, printMaisInfracoes, stdout, PAUSA_LISTAGEM);

    printf("\n----FIM DE LISTAGEM----\n");

    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printRanking(r, printCarroRankingTXT, printHeaderCarroMaisInfracoesTXT, printMaisInfracoes, file, PAUSA_LISTAGEM);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printRanking(r, printCarroRankingCSV, printHeaderCarroMaisInfracoesCSV, printMaisInfracoes, file, PAUSA_LISTAGEM);
        }
        fclose(file);
    }
    freeRanking(r, NULL, freeChaveCarroRankingInt);
    pressEnter();
}

/**
 * @brief Ranking do total de quilómetros percorridos por cada veículo num dado período de tempo
 * 
 * @param bd Base de dados
 */
void rankingKMSPeriodoTempo(Bdados *bd) {
    if (!bd) return;

    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM] = {0};
    
    Data inicio = {0,0,0,0,0,0.0f};
    Data fim = {0,0,0,0,0,0.0f};
    pedirPeriodoTempo(&inicio, &fim, "Insira a data inicial: ", "Insira a data final: ");

    Ranking *r = criarRanking();
    if (!r) {
        printf("Ocorreu um erro inesperado! Por favor tente novamente mais tarde!\n");
        pressEnter();
    }

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = bd->carrosCod->tabela[i];
        while(p) {
            if (p->dados) {
                No *m = p->dados->inicio;
                while(m) {
                    Carro *c = (Carro *)m->info;

                    float kmsPercorridos = 0;

                    if (c->viagens) {
                        No *l = c->viagens->inicio;
                        while(l) {
                            Viagem *v = (Viagem *)l->info;
                            
                            if (compararDatas(fim, v->entrada->data) >= 0 && compararDatas(inicio, v->saida->data) <= 0) {
                                kmsPercorridos += v->kms;
                            }
                            l = l->prox;
                        }
                    }
                    if (kmsPercorridos > 0) {
                        float *kms = (float *)malloc(sizeof(float));
                        *kms = kmsPercorridos;

                        void *temp = (void *)kms;
                        addToRanking(r, (void *)c, temp);
                    }
                    m = m->prox;
                }
            }
            p = p->prox;
        }
    }

    mergeSortRanking(r, compChaveCarroRankingFloat);

    printRanking(r, printCarroRanking, printHeaderCarroMaisKMS, printMaisKMS, stdout, PAUSA_LISTAGEM);

    printf("\n----FIM DE LISTAGEM----\n");

    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printRanking(r, printCarroRankingTXT, printHeaderCarroMaisKMS_TXT, printMaisKMS, file, PAUSA_LISTAGEM);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printRanking(r, printCarroRankingCSV, printHeaderCarroMaisKMS_CSV, printMaisKMS_CSV, file, PAUSA_LISTAGEM);
        }
        fclose(file);
    }
    freeRanking(r, NULL, freeChaveCarroRankingFloat);
    pressEnter();
}

/**
 * @brief Ranking do total de quilómetros percorridos por cada marca
 * 
 * @param bd Base de dados
 */
void rankingKMSMarca(Bdados *bd) {
    if (!bd) return;

    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM] = {0};

    Ranking *r = criarRanking();
    if (!r) {
        printf("Ocorreu um erro inesperado! Por favor tente novamente mais tarde!\n");
        pressEnter();
    }

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = bd->carrosMarca->tabela[i];
        while(p) {
            if (p->dados) {
                No *m = p->dados->inicio;
                float kmsPercorridos = 0;
                while(m) {
                    Carro *c = (Carro *)m->info;

                    if (c->viagens) {
                        No *l = c->viagens->inicio;
                        while(l) {
                            Viagem *v = (Viagem *)l->info;
                            
                            kmsPercorridos += v->kms;
                            l = l->prox;
                        }
                    }
                    m = m->prox;
                }
                if (kmsPercorridos > 0) {
                    m = p->dados->inicio;
                    float *kms = (float *)malloc(sizeof(float));
                    *kms = kmsPercorridos;

                    void *temp = (void *)kms;
                    addToRanking(r, m->info, temp); // colocamos um dos carros (obtemos a marca por ele)
                }
            }
            p = p->prox;
        }
    }

    mergeSortRanking(r, compChaveCarroRankingFloat);

    printRanking(r, printMarcaRanking, printHeaderMarcaMaisKMS, printMaisKMS, stdout, PAUSA_LISTAGEM);

    printf("\n----FIM DE LISTAGEM----\n");

    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printRanking(r, printMarcaRankingTXT, printHeaderMarcaMaisKMS_TXT, printMaisKMS, file, PAUSA_LISTAGEM);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printRanking(r, printMarcaRankingCSV, printHeaderMarcaMaisKMS_CSV, printMaisKMS_CSV, file, PAUSA_LISTAGEM);
        }
        fclose(file);
    }
    freeRanking(r, NULL, freeChaveCarroRankingFloat);
    pressEnter();
}

/**
 * @brief Lista os carros com infrações
 * 
 * @param bd Base de dados
 */
void listarCarrosComInfracoes(Bdados *bd) {
    if (!bd) return;
    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM] = {0};

    int count = 0;

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = bd->carrosCod->tabela[i];
        while(p && listagemFlag == 0) {
            if (p->dados) {
                No *l = p->dados->inicio;
                while(l && listagemFlag == 0) {
                    Carro *c = (Carro *)l->info;
                    if (c->viagens) {
                        No *m = c->viagens->inicio;
                        while(m && listagemFlag == 0) {
                            Viagem *v = (Viagem *)m->info;
                            if (v->velocidadeMedia > MAX_VELOCIDADE_AE || v->velocidadeMedia < MIN_VELOCIDADE_AE) {
                                printf("Matrícula: %s\n\n", c->matricula);
                                count++;
                                if (count % PAUSA_LISTAGEM == 0) {
                                    printf("\n");
                                    int opcao = enter_espaco_esc();
                                    switch (opcao) {
                                        case 0:
                                            break;
                                        case 1:
                                            while(count < bd->carrosCod->nelDict - PAUSA_LISTAGEM || !p) {
                                                if (!p) {
                                                    p = bd->carrosCod->tabela[++i];
                                                }
                                                else {
                                                    p = p->prox;
                                                    count++;
                                                }
                                            }
                                            break;
                                        case 2:
                                            listagemFlag = 1;
                                            break;
                                        default:
                                            break;
                                    }
                                }
                                break;
                            }
                            m = m->prox;
                        }
                    }
                    l = l->prox;
                }
            }
            p = p->prox;
        }
    }
    printf("\n----FIM DE LISTAGEM----\n");
    
    if (listagemFlag == 1) {
        listagemFlag = 0;
    }

    file = pedirListagemFicheiro(formato);
    if (file) {
        fprintf(file, "Matrícula\n");
        for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
            NoHashing *p = bd->carrosCod->tabela[i];
            while(p && listagemFlag == 0) {
                if (p->dados) {
                    No *l = p->dados->inicio;
                    while(l && listagemFlag == 0) {
                        Carro *c = (Carro *)l->info;
                        if (c->viagens) {
                            No *m = c->viagens->inicio;
                            while(m && listagemFlag == 0) {
                                Viagem *v = (Viagem *)m->info;
                                if (v->velocidadeMedia > MAX_VELOCIDADE_AE || v->velocidadeMedia < MIN_VELOCIDADE_AE) {
                                    fprintf(file, "%s\n", c->matricula);
                                    break;
                                }
                                m = m->prox;
                            }
                        }
                        l = l->prox;
                    }
                }
                p = p->prox;
            }
        }
        fclose(file);
    }
    pressEnter();
}

