#include "sensores.h"
#include "bdados.h"

/**
 * @brief Insere um sensor na base de dados
 * 
 * @param bd Base de dados
 * @param codSensor Código do sensor
 * @param designacao Designação do sensor
 * @param latitude 
 * @param longitude 
 * @return int 0 se erro, 1 se sucesso
 */
int inserirSensorLido(Bdados *bd, int codSensor, char *designacao, char *latitude, char *longitude) {
    if (!bd || !designacao || !latitude || !longitude) return 0;

    Sensor * sen = (Sensor *)malloc(sizeof(Sensor));
    if (!sen) return 0;
    //Codigo Sensor
    sen->codSensor = codSensor;
    //Designação
    sen->designacao = (char *)malloc(strlen(designacao) * sizeof(char) + 1);
    if (!sen->designacao) {
        free(sen);
        return 0;
    }
    strcpy(sen->designacao, designacao);
    //Latitude
    sen->latitude = (char *)malloc(strlen(latitude) * sizeof(char) + 1);
    if (!sen->latitude) {
        free(sen->designacao);
        free(sen);
        return 0;
    }
    strcpy(sen->latitude, latitude);
    //Longitude
    sen->longitude = (char *)malloc(strlen(longitude) * sizeof(char) + 1);
    if (!sen->longitude) {
        free(sen->designacao);
        free(sen->latitude);
        free(sen);
        return 0;
    }
    strcpy(sen->longitude, longitude);

    if (!addInicioLista(bd->sensores, (void *)sen)) {
        free(sen->designacao);
        free(sen->latitude);
        free(sen->longitude);
        free(sen);
        return 0;
    }

    return 1;
}

/**
 * @brief Compara 2 sensores
 * 
 * @param sensor1 
 * @param sensor2 
 * @return int -1 se sensor1 < sensor2, 0 se iguais, 1 se sensor1 > sensor2
 */
int compararSensores(void *sensor1, void *sensor2) {
    if (sensor1 == NULL && sensor2 == NULL) return 0;
    if (sensor1 == NULL) return -1; //NULL < qualquer coisa
    if (sensor2 == NULL) return 1;

    Sensor *x = (Sensor *)sensor1;
    Sensor *y = (Sensor *)sensor1;

    if (x->codSensor > y->codSensor) return 1;
    if (y->codSensor < x->codSensor) return -1;
    return 0;
}

/**
 * @brief Comparar um ID com um sensor(e respetivo ID)
 * 
 * @param sensor Sensor
 * @param idSensor ID
 * @return int 0 se iguais, 1 se diferentes ou erro
 */
int compIdSensor(void *sensor, void *idSensor) { 
    if (!sensor || !idSensor) return 1;

    Sensor *x = (Sensor *)sensor;
    int *id = (int *)idSensor;
    if (x->codSensor == *id) return 0;
    return 1;
}

/**
 * @brief Liberta toda a memória associada aos sensores
 * 
 * @param sensor Sensor a libertar
 */
void freeSensor(void *sensor) {
    Sensor *obj = (Sensor *)sensor;
    if (obj->designacao) free(obj->designacao);
    if (obj->latitude) free(obj->latitude);
    if (obj->longitude) free(obj->longitude);
    free(obj);
}

/**
 * @brief Mostra um sensor
 * 
 * @param sensor Sensor a mostrar
 */
void printSensor(void *sensor){
    if (!sensor) return;
    Sensor *x = (Sensor *) sensor;
    printf ("\nCódigo do Sensor: %d", x->codSensor);
    printf ("\nDesignação do Sensor: %s", x->designacao);
    printf ("\nLocalização do Sensor: %s\t%s", x->latitude, x->longitude);
}

/**
 * @brief Guarda um sensor em ficheiro binário
 * 
 * @param sensor Sensor
 * @param file Ficheiro binário, aberto
 */
void guardarSensorBin(void *sensor, FILE *file) {
    if (!sensor || !file) return;

    Sensor *x = (Sensor *)sensor;
    fwrite(&x->codSensor, sizeof(int), 1, file);

    size_t tamanhoDesignacao = strlen(x->designacao) + 1;
    fwrite(&tamanhoDesignacao, sizeof(size_t), 1, file);
    fwrite(x->designacao, tamanhoDesignacao, 1, file);

    size_t tamanhoLatitude = strlen(x->latitude) + 1;
    fwrite(&tamanhoLatitude, sizeof(size_t), 1, file);
    fwrite(x->latitude, tamanhoLatitude, 1, file);

    size_t tamanhoLongitude = strlen(x->longitude) + 1;
    fwrite(&tamanhoLongitude, sizeof(size_t), 1, file);
    fwrite(x->longitude, tamanhoLongitude, 1, file);
}

/**
 * @brief Lê um sensor para memória
 * 
 * @param file Ficheiro binário, aberto
 * @return void* Sensor ou NULL se erro
 */
void *readSensorBin(FILE *file) {
    if (!file) return NULL;

    Sensor *x = (Sensor *)malloc(sizeof(Sensor));
    if (!x) return NULL;

    fread(&x->codSensor, sizeof(int), 1, file);

    size_t tamanhoDesignacao;
    fread(&tamanhoDesignacao, sizeof(size_t), 1, file);
    x->designacao = (char *)malloc(tamanhoDesignacao);
    if (!x->designacao) {
        free(x);
        return NULL;
    }
    fread(x->designacao, tamanhoDesignacao, 1, file);

    size_t tamanhoLatitude;
    fread(&tamanhoLatitude, sizeof(size_t), 1, file);
    x->latitude = (char *)malloc(tamanhoLatitude);
    if (!x->latitude) {
        free(x->designacao);
        free(x);
        return NULL;
    }
    fread(x->latitude, tamanhoLatitude, 1, file);

    size_t tamanhoLongitude;
    fread(&tamanhoLongitude, sizeof(size_t), 1, file);
    x->longitude = (char *)malloc(tamanhoLongitude);
    if (!x->longitude) {
        free(x->designacao);
        free(x->latitude);
        free(x);
        return NULL;
    }
    fread(x->longitude, tamanhoLongitude, 1, file);

    return (void *)x;
}

/**
 * @brief Mostra um sensor em formato XML
 * 
 * @param sensor Sensor a mostrar
 * @param indentacao Indentanção a mostrar na primeira tag
 * @param file Ficheiro .xml (ou .txt), aberto
 */
void printSensorXML(void *sensor, int indentacao, FILE *file) {
    if (!sensor || indentacao < 0 || !file) return;

    Sensor *s = (Sensor *)sensor;

    indent(indentacao, file);
	fprintf(file, "<sensor>\n");
	
    indent(indentacao + 1, file);
    fprintf(file, "<idSensor>%d</idSensor>\n", s->codSensor);
    indent(indentacao + 1, file);
    fprintf(file, "<designacao>%s</designacao>\n", s->designacao ? s->designacao : "n/a");
    indent(indentacao + 1, file);
    fprintf(file, "<latitude>%s</latitude>\n", s->latitude ? s->latitude : "n/a");
    indent(indentacao + 1, file);
    fprintf(file, "<longitude>%s</longitude>\n", s->longitude ? s->longitude : "n/a");
	
	indent(indentacao, file);
	fprintf(file, "</sensor>\n");
}

/**
 * @brief Mostra o header dos sensores
 * 
 * @param file Ficheiro .csv, aberto
 */
void printHeaderSensoresCSV(FILE *file) {
    if (!file) return;

    fprintf(file, "Código Sensor, Designação, Latitude, Longitude\n");
}

/**
 * @brief Mostra o sensor em formato CSV
 * 
 * @param sensor Sensor a mostrar
 * @param file Ficheiro .csv, aberto
 */
void printSensorCSV(void *sensor, FILE *file) {
    if (!sensor || !file) return;

    Sensor *s = (Sensor *)sensor;

    // Aspas para escapar as virgulas
    fprintf(file, "%d, %s, \"%s\", \"%s\"\n", s->codSensor, s->designacao, s->latitude, s->longitude); 
}

/**
 * @brief Memória usada por um sensor
 * 
 * @param sensor Sensor
 * @return size_t Memória usada ou 0 se erro
 */
size_t memUsageSensor(void *sensor) {
    if (!sensor) return 0;

    Sensor *s = (Sensor *)sensor;

    size_t mem = sizeof(Sensor);
    mem += strlen(s->designacao) + 1;
    mem += strlen(s->latitude) + 1;
    mem += strlen(s->longitude) + 1;

    return mem;
}

