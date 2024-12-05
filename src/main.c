#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000  // Tamaño de la matriz

// Definir la matriz A, L y U
double A[N][N];
double L[N][N] = {0};
double U[N][N];

// Estructura para pasar parámetros a los hilos
typedef struct {
    int row_start;  // Fila de inicio para la eliminación
    int row_end;    // Fila final para la eliminación
    int i;          // Fila actual
} LU_Thread_Data;

void generate_matrix(double matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (double)(rand() % 100);
        }
    }
}

void print_matrix(double matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Función de eliminación para una fila
void* gaussian_elimination(void* arg) {
    LU_Thread_Data* data = (LU_Thread_Data*) arg;
    int i = data->i;
    
    // Realizar la eliminación sobre las filas j = row_start a row_end
    for (int j = data->row_start; j < data->row_end; j++) {
        L[j][i] = A[j][i] / A[i][i];
        for (int k = i; k < N; k++) {
            A[j][k] -= L[j][i] * A[i][k];
        }
    }

    pthread_exit(NULL);
}

// Factorización LU
void lu_decomposition(int n_threads) {
    for (int i = 0; i < N; i++) {
        // Inicializamos la diagonal de L a 1
        L[i][i] = 1;

        // Creamos los hilos para cada iteración de eliminación
        pthread_t threads[n_threads];  // Hilos para paralelización

        // Dividir el trabajo entre hilos
        for (int j = i + 1; j < n_threads; j++) {
            LU_Thread_Data* data = (LU_Thread_Data*) malloc(sizeof(LU_Thread_Data));
            data->i = i;
            data->row_start = j;
            data->row_end = j + 1;

            // Crear hilo para realizar la eliminación en la fila j
            pthread_create(&threads[j], NULL, gaussian_elimination, (void*) data);
        }

        // Esperar que todos los hilos terminen
        for (int j = i + 1; j < N; j++) {
            pthread_join(threads[j], NULL);
        }
    }
}

int main(int argc, char** argv) {
    // Rellenar la matriz A con valores aleatorios
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 100;
        }
    }

    // Realizar la factorización LU
    lu_decomposition(atoi(argv[1]));

    // Imprimir una parte de las matrices L y U
    printf("Matriz L:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i <= j)
                printf("%f ", L[i][j]);
            else
                printf("0 ");
        }
        printf("\n");
    }

    printf("Matriz U:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (i <= j)
                printf("%f ", A[i][j]);
            else
                printf("0 ");
        }
        printf("\n");
    }

    return 0;
}
