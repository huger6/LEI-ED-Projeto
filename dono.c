#include "dono.h"
#include "structsGenericas.h"
#include "constantes.h"
#include "bdados.h"
#include "validacoes.h"
#include "configs.h"

/**
 * @brief Introduz o dono na base de dados
 * 
 * @param bd Base de Dados
 * @param nome Nome do dono
 * @param nif NIF do dono
 * @param codigoPostal Cópigo postal do dono
 * @return int 1 se sucesso, 0 se erro
 */
int inserirDonoLido(Bdados *bd, char *nome, int nif, CodPostal codigoPostal) {
    if (!bd) return 0;

    Dono *dono = (Dono *)malloc(sizeof(Dono));
    if (!dono) return 0;
    //nif
    dono->nif = nif;
    //Nome
    dono->nome = (char *)malloc(strlen(nome) * sizeof(char) + 1);
    if (!dono->nome) {
        free(dono);
        return 0;
    }
    strcpy(dono->nome, nome);
    //Codigo Postal
    dono->codigoPostal.local = codigoPostal.local;
    dono->codigoPostal.zona = codigoPostal.zona;
    //Lista dos carros dos donos
    dono->carros = NULL;
    
    if (!appendToDict(bd->donosNif, (void *)dono, compChaveDonoNif, criarChaveDonoNif, hashChaveDonoNif, freeDono, freeChaveDonoNif)) {
        free(dono->nome);
        free(dono);
        return 0;
    }
    if (!appendToDict(bd->donosAlfabeticamente, (void *)dono, compChaveDonoAlfabeticamente, criarChaveDonoAlfabeticamente, hashChaveDonoAlfabeticamente, freeDono, freeChaveDonoAlfabeticamente)) {
        free(dono->nome);
        free(dono);
        return 0;
    }
        
    return 1;
}

/**
 * @brief Comparara donos pelo NIF
 * 
 * @param dono1 
 * @param dono2 
 * @return int -1 se dono1 < dono2, 0 se iguais, 1 se dono1 > dono2
 */
int compDonosNif(void *dono1, void *dono2) {
    if (dono1 == NULL && dono2 == NULL) return 0;
    if (dono1 == NULL) return -1; //NULL < qualquer coisa
    if (dono2 == NULL) return 1;

    Dono *x = (Dono *)dono1;
    Dono *y = (Dono *)dono2;

    if (x->nif > y->nif) return 1;
    if (y->nif < x->nif) return -1;
    return 0;
}

/**
 * @brief Compara os donos 
 * 
 * @param dono1 Dono 1
 * @param dono2 Dono 2
 * @return int -1 se dono1 < dono 2, 0 se iguais, 1 se dono1 > dono2
 */
int compDonosNome(void *dono1, void *dono2) { 
    if (dono1 == NULL && dono2 == NULL) return 0;
    if (dono1 == NULL) return -1; //NULL < qualquer coisa
    if (dono2 == NULL) return 1;

    Dono *x = (Dono *)dono1;
    Dono *y = (Dono *)dono2;
    char *xNorm = normString(x->nome);
    char *yNorm = normString(y->nome);

    int res = strcmp(xNorm, yNorm);

    free(xNorm);
    free(yNorm);
    
    return res;
}

/**
 * @brief Compara um dono(Nif) com um código
 * 
 * @param dono Dono
 * @param codigo Código
 * @return int 
 */
int compCodDono(void *dono, void *codigo) {
    if (!dono || !codigo) return 1;

    Dono *x = (Dono *)dono;
    int *cod = (int *)codigo;
    if (x->nif == *cod) return 0;
    return 1;
}

/**
 * @brief Compara 2 códigos postais
 * 
 * @param cod1 Código Postal 1
 * @param cod2 Código Postal 2
 * @return int 0 se iguais, 1 se diferentes
 */
int compararCodPostal(const CodPostal cod1, const CodPostal cod2) {
    if (cod1.zona == cod2.zona && cod1.local == cod2.local) return 0;
    return 1;
}

/**
 * @brief Liberta a memória de um dono
 * 
 * @param dono Dono
 */
void freeDono(void *dono) {
    Dono *obj = (Dono *)dono;
    if (obj->nome) free(obj->nome);
    if (obj->carros) {
        freeLista(obj->carros, NULL);
    }
    free(dono);
}

/**
 * @brief Mostra um dono
 * 
 * @param dono Dono
 */
void printDono(void *dono, FILE *file) {
    if (!dono || !file) return;

    Dono  *d = (Dono *)dono;
    
    fprintf(file, "NIF: %d\n", d->nif);
    fprintf(file, "Nome: %s\n", d->nome ? d->nome : "n/a");
    fprintf(file, "Código Postal: %hd-%hd\n", d->codigoPostal.zona, d->codigoPostal.local);
}

/**
 * @brief Guarda um elemento do tipo Dono em ficheiro binário
 * 
 * @param obj Dono
 * @param file Ficheiro binário, aberto
 */
void guardarDonoBin(void *obj, FILE *file) {
    if (!obj || !file) return;

    Dono *x = (Dono *)obj;

    fwrite(&x->codigoPostal, sizeof(CodPostal), 1, file);
    fwrite(&x->nif, sizeof(int), 1, file);

    size_t tamanhoNome = strlen(x->nome) + 1;
    fwrite(&tamanhoNome, sizeof(size_t), 1, file);
    fwrite(x->nome, tamanhoNome, 1, file);
}

/**
 * @brief Lê um dono de um ficheiro binário
 * 
 * @param file Ficheiro binário, aberto
 * @return void* Dono ou NULL se erro
 */
void *readDonoBin(FILE *file) {
    if (!file) return NULL;

    Dono *x = (Dono *)malloc(sizeof(Dono));
    if (!x) return NULL;
    
    fread(&x->codigoPostal, sizeof(CodPostal), 1, file);
    fread(&x->nif, sizeof(int), 1, file);

    size_t tamanho;
    fread(&tamanho, sizeof(size_t), 1, file);

    x->nome = (char *)malloc(tamanho);
    fread(x->nome, tamanho, 1, file);

    x->carros = NULL;

    return (void *)x;
}

/**
 * @brief Cria uma chave para Donos por Nif
 * 
 * @param dono Dono
 * @return void* Chave ou NULL se erro
 */
void *criarChaveDonoNif(void *dono) {
    if (!dono) return NULL;

    Dono *x = (Dono *)dono;
    int *chaveNif = (int *)malloc(sizeof(int));
    if (!chaveNif) return NULL;

    *chaveNif = x->nif;
    return (void *)chaveNif;
}

/**
 * @brief Liberta a memória ocupada pela chave dos donos por Nif
 * 
 * @param chave Chave
 */
void freeChaveDonoNif(void *chave) {
    if (!chave) return;

    int *key = (int *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos donos por Nif
 * 
 * @param chave Chave
 * @param dono Dono
 * @return int 0 se igual, 1 se diferente 
 */
int compChaveDonoNif(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    int *key = (int *)chave;
    int *key2 = (int *)chave2;

    if (*key == *key2) return 0;
    return 1;
} 

/**
 * @brief Função de hash para chave do tipo NIF 
 * 
 * @param chave Dono
 * @return int Nif do dono ou -1 se erro
 */
int hashChaveDonoNif(void *chave) {
    if (!chave) return -1;

    int *key = (int *)chave;

    return *key;
}

/**
 * @brief Cria uma chave para donos alfabeticamente
 * 
 * @param dono Dono
 * @return void* Chave ou NULL se erro
 */
void *criarChaveDonoAlfabeticamente(void *dono) {
    if (!dono) return NULL;

    Dono *x = (Dono *)dono;
    char *chaveAlf = malloc(sizeof(char));
    if (!chaveAlf) return NULL;

    *chaveAlf = tolower(x->nome[0]);
    return (void *)chaveAlf;
}

/**
 * @brief Liberta a memória da chave dos donos por ordem alfabética
 * 
 * @param chave Chave
 */
void freeChaveDonoAlfabeticamente(void *chave) {
    if (!chave) return;

    char *key = (char *)chave;
    free(key);
}

/**
 * @brief Compara as chaves dos donos alfabeticamente
 * 
 * @param chave Chave 1
 * @param chave2 Chave 2
 * @return int -1 se erro, 0 se iguais, 1 se diferentes
 */
int compChaveDonoAlfabeticamente(void *chave, void *chave2) {
    if (!chave || !chave2) return -1;

    char *key = (char *)chave;
    char *key2 = (char *)chave2;

    if (*key == *key2) return 0;
    return 1;
} 

/**
 * @brief Guarda a chave do dict dos donos por ordem alfabética
 * 
 * @param chave Chave
 * @param file Ficheiro binário
 */
void guardarChaveDonoAlfabeticamente(void *chave, FILE *file) {
    if (!chave || !file) return;

    char *key = (char *)chave;
    size_t comp = strlen(key) + 1;
    fwrite(&comp, sizeof(size_t), 1, file);
    fwrite(key, comp, 1, file);
}

/**
 * @brief Função de hash para o dono por ordem alfabética
 * 
 * @param chave 
 * @return int 
 */
int hashChaveDonoAlfabeticamente(void *chave) {
    if (!chave) return -1;

    char *letra = (char *)chave;
    
    return tolower(*letra) - 'a';
}

/**
 * @brief Mostra um dono em formato XML
 * 
 * @param dono Dono a mostrar
 * @param indentacao Indentação no início
 * @param file Ficheiro .xml (ou .txt), aberto
 */
void printDonoXML(void *dono, int indentacao, FILE *file) {
    if (!dono || indentacao < 0 || !file) return;

    Dono *d = (Dono *)dono;

    indent(indentacao, file);
	fprintf(file, "<dono>\n");
	
    indent(indentacao + 1, file);
    fprintf(file, "<nif>%d</nif>\n", d->nif);
    indent(indentacao + 1, file);
    fprintf(file, "<codPostal>%hd-%hd</codPostal>\n", d->codigoPostal.zona, d->codigoPostal.local);
    indent(indentacao + 1, file);
    fprintf(file, "<nome>%s</nome>\n", d->nome);
	
	indent(indentacao, file);
	fprintf(file, "</dono>\n");
}

/**
 * @brief Escreve os headers em formato CSV para o Dono
 * 
 * @param file Ficheiro .csv onde escrever, aberto
 */
void printHeaderDonosCSV(FILE *file) {
    if (!file) return;
    
    fprintf(file, "Nif, Nome, Código Postal\n");
}

/**
 * @brief Mostra o dono em formato CSV
 * 
 * @param dono Dono a mostrar
 * @param file Ficheiro .csv, aberto
 */
void printDonoCSV(void *dono, FILE *file) {
    if (!dono || !file) return;

    Dono *d = (Dono *)dono;

    fprintf(file, "%d, %s, %hd-%hd\n", d->nif, d->nome ? d->nome : "n/a", d->codigoPostal.zona, d->codigoPostal.local);
}

/**
 * @brief Mostra os donos junto às suas velocidades médias
 * 
 * @param dono Dono
 * @param velocidadeMedia Velocidade média 
 * @param file Ficheiro .csv, aberto
 */
void printDonoVelocidadesCSV(Dono *dono, float velocidadeMedia, FILE *file) {
    if (!dono || velocidadeMedia < 0 || !file) return;

    char *velMediaStr = floatToStringPontoDecimal(velocidadeMedia, 2);

    fprintf(file, "%d\t%s\t%s\n", dono->nif, dono->nome ? dono->nome : "n/a", velMediaStr);

    if (velMediaStr) free(velMediaStr);
}

/**
 * @brief Escreve os headers em formato TXT para o Dono
 * 
 * @param file Ficheiro .txt onde escrever, aberto
 */
void printHeaderDonosTXT(FILE *file) {
    if (!file) return;
    
    fprintf(file, "Nif\tNome\tCódigo Postal\n");
}

/**
 * @brief Mostra o dono em formato TXT
 * 
 * @param dono Dono a mostrar
 * @param file Ficheiro .txt, aberto
 */
void printDonoTXT(void *dono, FILE *file) {
    if (!dono || !file) return;

    Dono *d = (Dono *)dono;

    fprintf(file, "%d\t%s\t%hd-%hd\n", d->nif, d->nome ? d->nome : "n/a", d->codigoPostal.zona, d->codigoPostal.local);
}

/**
 * @brief Mostra o header da tabela dos donos ao exportar para HTML
 * 
 * @param file Ficheiro .html, aberto
 */
void printHeaderDonosHTML(FILE *file) {
    if (!file) return;

    fprintf(file,
        "\t\t\t\t\t<tr>\n"
        "\t\t\t\t\t\t<th>NIF</th>\n"
        "\t\t\t\t\t\t<th>Nome</th>\n"
        "\t\t\t\t\t\t<th>Código Postal</th>\n"
        "\t\t\t\t\t</tr>\n");
}

/**
 * @brief Mostra um dono em formato HTML
 * 
 * @param dono Dono
 * @param file Ficheiro .html, aberto
 */
void printDonoHTML(void *dono, FILE *file) {
    if (!dono || !file) return;

    Dono *d = (Dono *)dono;

    fprintf(file,
        "\t\t\t\t\t<tr>\n"
        "\t\t\t\t\t\t<th>%d</th>\n"
        "\t\t\t\t\t\t<th>%s</th>\n"
        "\t\t\t\t\t\t<th>%hd-%hd</th>\n"
        "\t\t\t\t\t</tr>\n",
        d->nif, d->nome ? d->nome : "n/a", d->codigoPostal.zona, d->codigoPostal.local);
}

/**
 * @brief Mostra os donos junto às suas velocidades médias
 * 
 * @param dono Dono
 * @param velocidadeMedia Velocidade média 
 * @param file Ficheiro .txt, aberto
 */
void printDonoVelocidadesTXT(Dono *dono, float velocidadeMedia, FILE *file) {
    if (!dono || velocidadeMedia < 0 || !file) return;

    fprintf(file, "%d\t%s\t%.2f\n", dono->nif, dono->nome ? dono->nome : "n/a", velocidadeMedia);
}

/**
 * @brief Memória usada por um dono
 * 
 * @param dono Dono
 * @return size_t Memória usada pelo dono ou 0 se erro
 */
size_t memUsageDono(void *dono) {
    if (!dono) return 0;

    Dono *d = (Dono *)dono;

    size_t mem = sizeof(Dono);
    mem += strlen(d->nome) + 1;

    return mem;
}

/**
 * @brief Memória utilizada pela chave dos donos por código
 * 
 * @param chave Chave
 * @return size_t Memória utilizada ou 0 se erro
 */
size_t memUsageChaveDonoCod(void *chave) {
    if (!chave) return 0;

    int *key = (int *)chave;
    return sizeof(*key);
}

/**
 * @brief Memória utilizada pela chave dos donos alfabeticamente
 * 
 * @param chave Chave
 * @return size_t Memória utilizada ou 0 se erro
 */
size_t memUsageChaveDonoAlfabeticamente(void *chave) {
    if (!chave) return 0;

    return sizeof(char);
}

/**
 * @brief Pede os dados sobre o dono e insere na base de dados
 * 
 * @param bd Base de dados
 */
void registarDono(Bdados *bd) {
    if (!bd) return;
    
    do {
        limpar_terminal();
        int nif = 0;
        char *nome = NULL;
        CodPostal cod = {0,0};
        // NIF
        do {
            pedirInt(&nif, "Insira o NIF do dono: ", validarNif);
            printf("\n");
            // Validar se já existe
            void *temp = (void *)&nif;
            Dono *nifUsado = (Dono *)searchDict(bd->donosNif, temp, compChaveDonoNif ,compCodDono, hashChaveCarroCod);
            if (nifUsado) {
                printf("O nif \"%d\" já existe!\n\n", nif);
                pressEnter();
                continue;
            }   
            break;
        } while(1);
        // Nome
        do {    
            printf("Insira o nome do dono: ");
            nome = lerLinhaTxt(stdin, NULL);
            if (!nome) {
                printf("Erro a ler o nome!\n\n");
                pressEnter();
                continue;
            }
            char *mensagemErro = validarNome(nome);
            if (mensagemErro) {
                printf("%s\n\n", mensagemErro);
                pressEnter();
                free(nome);
                continue;
            }
            break;
        } while(1);
        printf("\n");
        // Codigo Postal
        do {
            printf("Insira o código postal no formato XXXX-XXX: ");
            char *codPostal = lerLinhaTxt(stdin, NULL);
            if (!codPostal) {
                printf("Erro a ler o código postal!\n\n");
                pressEnter();
                continue;
            }
            if (!converterCodPostal(codPostal, &cod.zona, &cod.local)) {
                free(codPostal);
                printf("Erro a ler o código postal!\n\n");
                pressEnter();
                continue;
            }
            free(codPostal);

            if (!validarCodPostal(cod.zona, cod.local)) {
                printf("Código postal inválido\n\n");
                pressEnter();
                continue;
            }
            break;
        } while(1);
        printf("\n");

        if (!inserirDonoLido(bd, nome, nif, cod)) {
            free(nome);
            printf("Ocorreu um erro a registar o dono em memória. Por favor tente novamente!\n\n");
            pressEnter();
            continue;
        }
        free(nome);
        
        if (!sim_nao("Quer inserir mais algum dono?")) break;
    } while(1);
}

/**
 * @brief Obtém o condutor com a maior velocidade média
 * 
 * @param bd Base de dados
 * @return Dono* Dono ou NULL se erro
 */
Dono *obterCondutorMaisVelocidadeMedia(Bdados *bd) {
    if (!bd) return NULL;

    Dono *donoMaisRapido = NULL;
    float velocidadeMax = 0.0f;
    float tempoTotal = 0.0f;
    float distanciaTotal = 0.0f;
    float velocidadeMedia = 0.0f;

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = bd->donosNif->tabela[i];

        tempoTotal = 0.0f;
        distanciaTotal = 0.0f;
        velocidadeMedia = 0.0f;
        while(p) {
            if (p->dados) {
                No *m = p->dados->inicio;
                while(m) {
                    Dono *d = (Dono *)m->info;

                    if (d->carros) {
                        No *x = d->carros->inicio;
                        while(x) {
                            Carro *c = (Carro *)x->info;
                            if (c->viagens) {
                                No *l = c->viagens->inicio;
                                while(l) {
                                    Viagem *v = (Viagem *)l->info;
                                    
                                    tempoTotal += v->tempo;
                                    distanciaTotal += v->kms;
                                    l = l->prox; //viagem
                                }
                            }
                            x = x->prox; //carro
                        }
                    }
                    if (tempoTotal > 0) {
                        velocidadeMedia = distanciaTotal / (tempoTotal / 60.0f);
                        if (velocidadeMedia > velocidadeMax) {
                            p = bd->donosNif->tabela[i];
                            velocidadeMax = velocidadeMedia;
                            donoMaisRapido = d;
                        }
                    }
                    m = m->prox; //dono
                }
            }
            p = p->prox;
        }
    }

    return donoMaisRapido;
}

/**
 * @brief Lista todos os donos ordenados pelo NIF
 * 
 * @param bd Base de dados
 */
void listarDonosNIF(Bdados *bd) {
    if (!bd) return;

    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM];
    
    Lista *donosNifOrd = dictToLista(bd->donosNif);
    mergeSortLista(donosNifOrd, compDonosNif);
    printLista(donosNifOrd, printDono, stdout, pausaListagem);
    printf("\n----FIM DE LISTAGEM----\n");
    
    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printHeaderDonosTXT(file);
            printLista(donosNifOrd, printDonoTXT, file, 0);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printHeaderDonosCSV(file);
            printLista(donosNifOrd, printDonoCSV, file, 0);
        }
        fclose(file);
    }
    freeLista(donosNifOrd, NULL);
    pressEnter();
}

/**
 * @brief Lista todos os donos ordenados por ordem alfabética
 * 
 * @param bd Base de dados
 */
void listarDonosAlfabeticamente(Bdados *bd) {
    if (!bd) return;

    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM];
    
    printDict(bd->donosAlfabeticamente, printDono, stdout, pausaListagem);
    printf("\n----FIM DE LISTAGEM----\n");
    
    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            printHeaderDonosTXT(file);
            printDict(bd->donosAlfabeticamente, printDonoTXT, file, 0);
        }
        else if (strcmp(formato, ".csv") == 0) {
            printHeaderDonosCSV(file);
            printDict(bd->donosAlfabeticamente, printDonoCSV, file, 0);
        }
        fclose(file);
    }
    pressEnter();
}

/**
 * @brief Lista os donos com as suas respetivsas velocidades médias (não ordenado)
 * 
 * @param bd Base de dados
 */
void listarDonosVelocidadesMedias(Bdados *bd) {
    if (!bd) return;

    limpar_terminal();
    FILE *file = NULL;
    char formato[TAMANHO_FORMATO_LISTAGEM];

    int tempo, distancia;
    float velocidadeMedia;

    int count = 0;

    for (int i = 0; i < TAMANHO_TABELA_HASH && listagemFlag == 0; i++) {
        NoHashing *p = bd->donosNif->tabela[i];

        while(p) {
            if (p->dados) {
                No *m = p->dados->inicio;
                while(m && listagemFlag == 0) {
                    Dono *d = (Dono *)m->info;
                    
                    tempo = 0;
                    distancia = 0;
                    velocidadeMedia = 0;

                    // Procura todas as viagens de cada carro
                    if (d->carros) {
                        No *x = d->carros->inicio;
                        while(x) {
                            Carro *c = (Carro *)x->info;
                            if (c->viagens) {
                                No *l = c->viagens->inicio;
                                while(l) {
                                    Viagem *v = (Viagem *)l->info;
                                    
                                    tempo += v->tempo;
                                    distancia += v->kms;
                                    l = l->prox; //viagem
                                }
                            }
                            x = x->prox; //carro
                        }
                    }
                    if (tempo > 0) {
                        velocidadeMedia = distancia / (tempo / 60.0f);
                        Dono *dono = (Dono *)p->dados->inicio->info;
                        printf("Nome: %s\n", dono->nome);
                        printf("Velocidade Média: %.2f\n\n", velocidadeMedia);
                        count++;
                        if (count % pausaListagem == 0) {
                            printf("\n");
                            int opcao = enter_espaco_esc();
                            switch (opcao) {
                                case 0:
                                    break;
                                case 1:
                                    while(count < bd->donosNif->nelDict - pausaListagem || !p) { //!p importante
                                        if (!p) {
                                            p = bd->donosNif->tabela[++i];
                                        } 
                                        else {
                                            p = p->prox;
                                            count++; // apenas se incrementa se houver p
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
                    }
                    m = m->prox; //dono
                }
            }
            p = p->prox;
        }
    }
    
    printf("\n----FIM DE LISTAGEM----\n\n");

    if (listagemFlag == 1) {
        listagemFlag = 0;
    }
    
    file = pedirListagemFicheiro(formato);
    if (file) {
        if (strcmp(formato, ".txt") == 0) {
            fprintf(file, "Nif\tNome\tVelocidade media\n");
            for (int i = 0; i < TAMANHO_TABELA_HASH && listagemFlag == 0; i++) {
                NoHashing *p = bd->donosNif->tabela[i];

                while(p) {
                    if (p->dados) {
                        No *m = p->dados->inicio;
                        while(m && listagemFlag == 0) {
                            Dono *d = (Dono *)m->info;
                            
                            tempo = 0;
                            distancia = 0;
                            velocidadeMedia = 0;

                            // Procura todas as viagens de cada carro
                            if (d->carros) {
                                No *x = d->carros->inicio;
                                while(x) {
                                    Carro *c = (Carro *)x->info;
                                    if (c->viagens) {
                                        No *l = c->viagens->inicio;
                                        while(l) {
                                            Viagem *v = (Viagem *)l->info;
                                            
                                            tempo += v->tempo;
                                            distancia += v->kms;
                                            l = l->prox; //viagem
                                        }
                                    }
                                    x = x->prox; //carro
                                }
                            }
                            if (tempo > 0) {
                                velocidadeMedia = distancia / (tempo / 60.0f);
                                Dono *dono = (Dono *)p->dados->inicio->info;
                                printDonoVelocidadesTXT(dono, velocidadeMedia, file);
                                count++;
                            }
                            m = m->prox; //dono
                        }
                    }
                    p = p->prox;
                }
            }
        }
        else if (strcmp(formato, ".csv") == 0) {
            fprintf(file, "Nif, Nome, Velocidade media\n");
            for (int i = 0; i < TAMANHO_TABELA_HASH && listagemFlag == 0; i++) {
                NoHashing *p = bd->donosNif->tabela[i];

                while(p) {
                    if (p->dados) {
                        No *m = p->dados->inicio;
                        while(m && listagemFlag == 0) {
                            Dono *d = (Dono *)m->info;
                            
                            tempo = 0;
                            distancia = 0;
                            velocidadeMedia = 0;

                            // Procura todas as viagens de cada carro
                            if (d->carros) {
                                No *x = d->carros->inicio;
                                while(x) {
                                    Carro *c = (Carro *)x->info;
                                    if (c->viagens) {
                                        No *l = c->viagens->inicio;
                                        while(l) {
                                            Viagem *v = (Viagem *)l->info;
                                            
                                            tempo += v->tempo;
                                            distancia += v->kms;
                                            l = l->prox; //viagem
                                        }
                                    }
                                    x = x->prox; //carro
                                }
                            }
                            if (tempo > 0) {
                                velocidadeMedia = distancia / (tempo / 60.0f);
                                Dono *dono = (Dono *)p->dados->inicio->info;
                                printDonoVelocidadesCSV(dono, velocidadeMedia, file);
                                count++;
                            }
                            m = m->prox; //dono
                        }
                    }
                    p = p->prox;
                }
            }
        }
        fclose(file);
    }
    pressEnter();
}

/**
 * @brief Pede um código postal e mostra as velocidades médias associadas a esse código postal
 * 
 * @param bd Base de dados
 */
void velocidadeMediaPorCodPostal(Bdados *bd) {
    if (!bd) return;
    
    limpar_terminal();

    CodPostal chave = {0,0};
    // Pedir código postal
    do {
        printf("Insira o código postal no formato XXXX-XXX: ");
        char *codPostal = lerLinhaTxt(stdin, NULL);
        if (!codPostal) {
            printf("Erro a ler o código postal!\n\n");
            pressEnter();
            continue;
        }
        converterCodPostal(codPostal, &chave.zona, &chave.local);
        free(codPostal);

        if (!validarCodPostal(chave.zona, chave.local)) {
            printf("Código postal inválido\n\n");
            pressEnter();
            continue;
        }
        break;
    } while(1);
    printf("\n");
    
    // Filtrar os donos com os códigos postais para uma lista
    Lista *donosCods = criarLista();
    if (!donosCods) {
        printf("Ocorreu um erro inesperado. Tente novamente mais tarde!\n");
        pressEnter();
        return;
    }

    for (int i = 0; i < TAMANHO_TABELA_HASH; i++) {
        NoHashing *p = bd->donosNif->tabela[i];
        while(p) {
            if (p->dados) {
                No *l = p->dados->inicio;
                while(l) {
                    Dono *d = (Dono *)l->info;
                    if (compararCodPostal(d->codigoPostal, chave) == 0) {
                        (void) addInicioLista(donosCods, l->info);
                    }
                    l = l->prox;
                }
            }
            p = p->prox;
        }
    }

    // Percorrer a lista e encontrar a velocidade média
    No *p = donosCods->inicio;
    if (!p) {
        printf("Não foram encontrados donos com o código postal \"%hd-%hd\"\n", chave.zona, chave.local);
        freeLista(donosCods, NULL);
        pressEnter();
        return;
    }
    int tempo = 0;
    int distancia = 0;
    float velocidadeMedia = 0.0f;

    while(p) {
        Dono *d = (Dono *)p->info;
        if (d->carros) {
            No *m = d->carros->inicio;
            while(m) {
                Carro *c = (Carro *)m->info;
                if (c->viagens) {
                    No *l = c->viagens->inicio;
                    while(l) {
                        Viagem *v = (Viagem *)l->info;

                        tempo += v->tempo;
                        distancia += v->kms;
                        l = l->prox;
                    }
                }
                m = m->prox;
            }
        }
        p = p->prox;
    }
    if (tempo > 0) {
        velocidadeMedia = distancia / (tempo / 60.0f);
    }
    else {
        printf("Não há dados sobre quaisquer viagens efetuadas sobre os donos com o código postal \"%hd-%hd\"!\n", chave.zona, chave.local);
        freeLista(donosCods, NULL);
        pressEnter();
        return;
    }

    printf("A velocidade média associada ao código postal \"%hd-%hd\" é de %.2fkm/h!\n\n", chave.zona, chave.local, velocidadeMedia);
    freeLista(donosCods, NULL);
    pressEnter();
}

