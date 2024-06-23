#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

// Definición de estructuras de datos
typedef enum { IZQUIERDA, DERECHA } Direccion;

typedef struct {
  int id;
  Direccion direccion;
} Auto;

// Declaración de variables globales
Auto puente[3] = {{0, IZQUIERDA}, {0, IZQUIERDA}, {0, IZQUIERDA}};
pthread_mutex_t semaforo_puente = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t semaforo_cambio = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cola_espera_izq = PTHREAD_COND_INITIALIZER;
pthread_cond_t cola_espera_der = PTHREAD_COND_INITIALIZER;
int contador_autos = 0;
pthread_mutex_t mutex_contador = PTHREAD_MUTEX_INITIALIZER;
int autos_en_espera_izq = 0;
int autos_en_espera_der = 0;
int autos_en_puente = 0;
int tiempo_simulacion = 0;

// Función para agregar un auto a la cola de espera correspondiente
void agregar_auto(Direccion direccion) {
  int id_auto;

  // Obtener un ID único para el auto
  pthread_mutex_lock(&mutex_contador);
  contador_autos++;
  id_auto = contador_autos;
  pthread_mutex_unlock(&mutex_contador);

  // Crear un nuevo auto con el ID y la dirección generados
  Auto auto_nuevo = {id_auto, direccion};

  // Agregar el auto a la cola de espera correspondiente
  if (direccion == IZQUIERDA) {
    pthread_mutex_lock(&semaforo_puente);
    autos_en_espera_izq++;
    pthread_cond_signal(&cola_espera_izq);
    pthread_mutex_unlock(&semaforo_puente);
  } else {
    pthread_mutex_lock(&semaforo_puente);
    autos_en_espera_der++;
    pthread_cond_signal(&cola_espera_der);
    pthread_mutex_unlock(&semaforo_puente);
  }

  printf("[%d] Auto %d agregado a la cola de espera (%s)\n", tiempo_simulacion, id_auto, direccion == IZQUIERDA ? "Izquierda" : "Derecha");
}

// Función para simular el cruce del puente por un auto
void cruzar_puente(int id, Direccion direccion) {
  int seccion;

  // Simular el cruce del puente sección por sección
  for (seccion = 0; seccion < 3; seccion++) {
    pthread_mutex_lock(&semaforo_puente);
    puente[seccion].id = id;
    puente[seccion].direccion = direccion;
    autos_en_puente++;
    printf("[%d] Auto %d cruzando el puente (sección %d, %s)\n", tiempo_simulacion, id, seccion + 1, direccion == IZQUIERDA ? "Izquierda" : "Derecha");
    pthread_mutex_unlock(&semaforo_puente);
    sleep(1);
  }

  // Auto ha salido del puente
  pthread_mutex_lock(&semaforo_puente);
  puente[2].id = 0;
  puente[2].direccion = IZQUIERDA;
  autos_en_puente--;
  printf("[%d] Auto %d ha salido del puente (%s)\n", tiempo_simulacion, id, direccion == IZQUIERDA ? "Izquierda" : "Derecha");

  // Gestionar cambio de turno si procede
  if ((autos_en_espera_der > 0 && direccion == IZQUIERDA) ||
      (autos_en_espera_izq > 0 && direccion == DERECHA)) {
    pthread_cond_signal(&semaforo_cambio);
  }

  pthread_mutex_unlock(&semaforo_puente);
}

// Función para controlar el cambio de turno entre sentidos
void gestionar_cambio_turno() {
  pthread_mutex_lock(&semaforo_cambio);

  // Si hay autos esperando en el otro extremo y ya hay autos cruzando, cambiar de turno
  if ((autos
