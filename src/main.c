#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define WINDOW_SIZE 4096  // Tamaño de la ventana de búsqueda
#define LOOKAHEAD_SIZE 16 // Tamaño de la ventana de anticipación

// Estructura para cada hilo
typedef struct {
    char *data;
    size_t start;
    size_t end;
    FILE *output_file;
} thread_data_t;

// Estructura para almacenar las tuplas (offset, longitud, símbolo)
typedef struct {
    int offset;
    int length;
    char symbol;
} lz77_tuple;

// Función que comprime una sección del archivo
void* compress_section(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    char *input_data = data->data;
    size_t start = data->start;
    size_t end = data->end;
    FILE *output_file = data->output_file;
    
    // Implementación básica de la compresión LZ77
    for (size_t i = start; i < end; i++) {
        int offset = -1, length = 0;
        char symbol = input_data[i];

        // Buscar coincidencias en la ventana de búsqueda
        for (int j = (i - WINDOW_SIZE > 0 ? i - WINDOW_SIZE : 0); j < i; j++) {
            int match_length = 0;
            while (input_data[j + match_length] == input_data[i + match_length] && match_length < LOOKAHEAD_SIZE) {
                match_length++;
            }

            if (match_length > length) {
                length = match_length;
                offset = i - j;
            }
        }

        // Escribir tupla (offset, longitud, símbolo) en el archivo comprimido
        fwrite(&offset, sizeof(int), 1, output_file);
        fwrite(&length, sizeof(int), 1, output_file);
        fwrite(&symbol, sizeof(char), 1, output_file);
    }

    return NULL;
}

// Función principal
int main(int argc, char** argv) {
    // Abrir el archivo de entrada
    FILE *input_file = fopen("large_input.txt", "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Obtener el tamaño del archivo
    fseek(input_file, 0, SEEK_END);
    size_t file_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Leer todo el contenido del archivo en un buffer
    char *data = malloc(file_size);
    fread(data, sizeof(char), file_size, input_file);
    fclose(input_file);

    // Abrir el archivo de salida comprimido
    FILE *output_file = fopen("compressed_output.lz77", "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        return 1;
    }

    
    int NUM_THREADS = atoi(argv[1]);

    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];

    size_t block_size = file_size / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].data = data;
        thread_data[i].start = i * block_size;
        thread_data[i].end = (i == NUM_THREADS - 1) ? file_size : (i + 1) * block_size;
        thread_data[i].output_file = output_file;

        pthread_create(&threads[i], NULL, compress_section, &thread_data[i]);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Cerrar el archivo de salida y liberar recursos
    fclose(output_file);
    free(data);

    printf("Compresión completada.\n");
    return 0;
}
