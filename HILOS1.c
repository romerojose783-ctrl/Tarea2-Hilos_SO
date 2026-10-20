#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_NUM 10000

// Variable compartida de control de turnos
// 0 = Turno del Hilo 1, 1 = Turno del Hilo 2
int turno = 0;

// Mutex y Variable de Condición para la sincronización
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

// Función que ejecutará el Hilo 1
void* rutina_hilo1(void* arg) {
    for (int i = 1; i <= MAX_NUM; i++) {
        pthread_mutex_lock(&mutex);
        
        // Esperar mientras no sea el turno del Hilo 1
        while (turno != 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        
        printf("El hilo 1 dice... %d\n", i);
        fflush(stdout); // Forzar impresión inmediata en pantalla
        
        // Cambiar el turno al Hilo 2
        turno = 1;
        
        // Despertar al otro hilo
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

// Función que ejecutará el Hilo 2
void* rutina_hilo2(void* arg) {
    for (int i = 1; i <= MAX_NUM; i++) {
        pthread_mutex_lock(&mutex);
        
        // Esperar mientras no sea el turno del Hilo 2
        while (turno != 1) {
            pthread_cond_wait(&cond, &mutex);
        }
        
        printf("El hilo 2 dice... %d\n", i);
        fflush(stdout); // Forzar impresión inmediata en pantalla
        
        // Cambiar el turno al Hilo 1
        turno = 0;
        
        // Despertar al otro hilo
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main() {
    pthread_t hilo1, hilo2;

    // Crear los dos hilos
    pthread_create(&hilo1, NULL, rutina_hilo1, NULL);
    pthread_create(&hilo2, NULL, rutina_hilo2, NULL);

    // Esperar a que ambos hilos terminen su ejecución
    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    // Destruir los recursos de sincronización
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Ambos hilos han terminado de contar hasta 10,000.\n");

    return 0;
}
