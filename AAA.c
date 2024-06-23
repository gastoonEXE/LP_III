#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estructura para un nodo de la cola
typedef struct Nodo {
    pthread_t hilo;
    struct Nodo* siguiente;
} Nodo;

// Estructura para la cola
typedef struct {
    Nodo* frente;
    Nodo* final;
} Cola;

// Inicializar una cola vacía
void inicializarCola(Cola* cola) {
    cola->frente = NULL;
    cola->final = NULL;
}

// Encolar un hilo en la cola
void encolar(Cola* cola, pthread_t hilo) {
    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
    nuevoNodo->hilo = hilo;
    nuevoNodo->siguiente = NULL;

    if (cola->frente == NULL) {
        cola->frente = nuevoNodo;
        cola->final = nuevoNodo;
    } else {
        cola->final->siguiente = nuevoNodo;
        cola->final = nuevoNodo;
    }
}

// Desencolar un hilo de la cola
pthread_t desencolar(Cola* cola) {
    if (cola->frente == NULL) {
        // La cola está vacía
        return 0; // O algún valor que indique error
    }

    Nodo* nodoDesencolado = cola->frente;
    pthread_t hilo = nodoDesencolado->hilo;

    cola->frente = cola->frente->siguiente;
    free(nodoDesencolado);

    return hilo;
}

// Función que ejecuta un hilo
void* ejecutarHilo(void* arg) {
    // Aquí va la lógica específica del hilo
    printf("Hilo ejecutado\n");
    return NULL;
}

int main() {
    Cola colaIzq, colaDer;
    inicializarCola(&colaIzq);
    inicializarCola(&colaDer);

    // Encolar hilos (simulación)
    encolar(&colaIzq, pthread_self());
    encolar(&colaDer, pthread_self());

    // Desencolar y ejecutar hilos
    pthread_t hiloIzq = desencolar(&colaIzq);
    pthread_t hiloDer = desencolar(&colaDer);

    pthread_create(&hiloIzq, NULL, ejecutarHilo, NULL);
    pthread_create(&hiloDer, NULL, ejecutarHilo, NULL);

    pthread_join(hiloIzq, NULL);
    pthread_join(hiloDer, NULL);

    return 0;
}
