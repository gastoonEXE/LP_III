#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

// Estructura para representar un auto (nodo de la lista)
typedef struct Auto {
    pthread_t hilo;
    char id[7]; // "autoNN"
    char direccion; // 'I' para izquierda, 'D' para derecha, F si no esta iniciada
    int estado; // 0 al 2 para posicion en el puente, 4 para modo espera, 3 para ser eliminado
    struct Auto* siguiente;
    struct Auto* anterior; 
} Auto;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

Auto EpacioVacio = {0,"E", 'F', 4, NULL, NULL};

Auto ArrPuente[3]={
    {0,"E", 'F', 4, NULL, NULL},
    {0,"E", 'F', 4, NULL, NULL},
    {0,"E", 'F', 4, NULL, NULL}
}; //el arreglo que representa el puente

int contador = 0;

// Estructura para la lista doblemente enlazada
typedef struct {
    Auto* frente;
    Auto* final;
} ListaDobleEnlazada;

ListaDobleEnlazada miLista;     // INICIALIZAMOS UNA LISTA ENLAZADA QUE CONTENDRA TODOS NUESTROS AUTOS



// Inicializar una lista vacía
void inicializarLista(ListaDobleEnlazada* lista) {
    lista->frente = NULL;
    lista->final = NULL;
}

// Insertar un auto al final de la lista
void insertarAlFinal(ListaDobleEnlazada* lista, Auto* nuevoAuto) {
    if (lista->frente == NULL) {
        lista->frente = nuevoAuto;
        lista->final = nuevoAuto;
    } else {
        nuevoAuto->anterior = lista->final;
        lista->final->siguiente = nuevoAuto;
        lista->final = nuevoAuto;
    }
}

// Eliminar un nodo específico (auto) de la lista
void eliminarNodo(ListaDobleEnlazada* lista, Auto* autoAEliminar) {
    if (autoAEliminar == NULL) {
        return; // No se encontró el nodo
    }

    if (autoAEliminar->anterior != NULL) {
        autoAEliminar->anterior->siguiente = autoAEliminar->siguiente;
    } else {
        lista->frente = autoAEliminar->siguiente;
    }

    if (autoAEliminar->siguiente != NULL) {
        autoAEliminar->siguiente->anterior = autoAEliminar->anterior;
    } else {
        lista->final = autoAEliminar->anterior;
    }

    free(autoAEliminar);
}

// Imprimir la lista
void imprimirLista(const ListaDobleEnlazada* lista) 
{
    Auto* actual = lista->frente;
    while (actual != NULL) {
        if(actual->direccion == 'I')
        {
            printf("=> %s \n", actual->id);
            actual = actual->siguiente;
        }
        else
        {
            printf("<= %s \n", actual->id);
            actual = actual->siguiente;
        }

    }
}

// Recorrer la lista (siguiente y anterior)
void recorrerLista(const ListaDobleEnlazada* lista) {
    // Implementa según tus necesidades
}

// Buscar un nodo específico por ID
Auto* buscarNodoPorID(const ListaDobleEnlazada* lista, const char* idBuscado) {
    Auto* actual = lista->frente;
    while (actual != NULL) {
        if (strcmp(actual->id, idBuscado) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL; // No se encontró el nodo
}

Auto* inicializarAuto(const char* direccion) { 
    static int contador = 1; // Contador autoincremental
    pthread_t hiloA = 0; //
    Auto* nuevoAuto = (Auto*)malloc(sizeof(Auto));


    // Crear el ID autoincremental (autoNN)
    snprintf(nuevoAuto->id, sizeof(nuevoAuto->id), "auto%02d", contador);
    contador++;

    nuevoAuto->estado = 4; // inicializa en estado de espera
    nuevoAuto->hilo = hiloA;
    // Asignar la dirección
    nuevoAuto->direccion = direccion[0]; // Tomamos el primer carácter

    nuevoAuto->siguiente = NULL;
    nuevoAuto->anterior = NULL;
    return nuevoAuto;
}

// Imprimir el puente
void imprimirPuente() 
{
    char fle_0[3], fle_1[3], fle_2[3];
    char dir_0[11], dir_1[11], dir_2[11];
    char id_0[7], id_1[7], id_2[7];

    if (ArrPuente[0].direccion == 'I')
    {
        strcpy(fle_0,">>");
        strcpy(dir_0,">>>>>>>>>>");
        strcpy(id_0, ArrPuente[0].id);
    }
    else if (ArrPuente[0].direccion == 'D')
    {
        strcpy(fle_0,"<<");
        strcpy(dir_0,"<<<<<<<<<<");
        strcpy(id_0, ArrPuente[0].id);
    }
    else
    {
        strcpy(fle_0,"==");
        strcpy(dir_0,"==========");
        strcpy(id_0,"======");      
    }

    if (ArrPuente[1].direccion == 'I')
    {
        strcpy(fle_1,">>");
        strcpy(dir_1,">>>>>>>>>>");
        strcpy(id_1, ArrPuente[1].id);
    }
    else if (ArrPuente[1].direccion == 'D')
    {
        strcpy(fle_1,"<<");
        strcpy(dir_1,"<<<<<<<<<<");
        strcpy(id_1, ArrPuente[1].id);
    }
    else
    {
        strcpy(fle_1,"==");
        strcpy(dir_1,"==========");
        strcpy(id_1,"======");      
    }

    if (ArrPuente[2].direccion == 'I')
    {
        strcpy(fle_2,">>");
        strcpy(dir_2,">>>>>>>>>>");
        strcpy(id_2, ArrPuente[2].id);
    }
    else if (ArrPuente[2].direccion == 'D')
    {
        strcpy(fle_2,"<<");
        strcpy(dir_2,"<<<<<<<<<<");
        strcpy(id_2, ArrPuente[2].id);
    }
    else
    {
        strcpy(fle_2,"==");
        strcpy(dir_2,"==========");
        strcpy(id_2,"======");      
    }

    
    printf("\n==================================================================\n");
    printf("================%s======%s=======%s=======\n", dir_0, dir_1, dir_2);
    printf("================%s%s%s======%s%s%s=======%s%s%s=======\n",fle_0, id_0,fle_0,fle_1, id_1,fle_1,fle_2, id_2,fle_2);
    printf("================%s======%s=======%s=======\n", dir_0, dir_1, dir_2);
    printf("==================================================================\n");
}


// Función para verificar si el vector ArrPuente está vacío
int estaVacio(const Auto arr[]) {
    for (int i = 0; i < 3; ++i) {
        //printf("= %d =",arr[i].estado);
        if (arr[i].estado >= 0 && arr[i].estado <= 2 ) {
            
            return 0; // No está vacío
        }
    }
    return 1; // Está vacío
}


/*#####################################------------- FUNCIONES DE HILOS  -------------#####################################*/
void funcion ( void *ptr )
{
    
    // // cuando 
    // //de izquierda a derecha
    // // mientras el estado esta en espera hace un 

    Auto *actual = (Auto *)ptr;  /* una variable que apunta a un auto */
    

    int actual_estado = 4;

    printf("la direccion de este hilo es: %c", actual->direccion);

    if (actual->direccion == 'I')
    {
        while (actual_estado == 4)
        {
            
            pthread_mutex_lock(&mutex);

            if (estaVacio(ArrPuente) == 1)
            {
                actual->estado = 0;
                ArrPuente[0] = *actual;
                contador=1;
                // Buscar un nodo por ID
                const char* idBuscado = actual->id;
                Auto* nodoEncontrado = buscarNodoPorID(&miLista, idBuscado);
                eliminarNodo(&miLista, nodoEncontrado);
                actual_estado = 0;
                imprimirPuente();
                imprimirLista(&miLista);
            }
            else
            {
                if (ArrPuente[0].direccion == actual->direccion)
                {
                    if (contador <= 4 )
                    {
                        
                        actual->estado = 0;
                        ArrPuente[1].estado = 2;
                        ArrPuente[0].estado = 1;

                        ArrPuente[2] = ArrPuente[1];
                        ArrPuente[1] = ArrPuente[0];
                        ArrPuente[0] = *actual;
                        contador=contador+1;
                        // Buscar un nodo por ID
                        const char* idBuscado = actual->id;
                        Auto* nodoEncontrado = buscarNodoPorID(&miLista, idBuscado);
                        eliminarNodo(&miLista, nodoEncontrado); 
                        actual_estado = 0;
                        imprimirPuente();
                        imprimirLista(&miLista);
                    }
                    else 
                    {
                        ArrPuente[2] = ArrPuente[1];
                        ArrPuente[1] = ArrPuente[0];
                        ArrPuente[0] = EpacioVacio;
                        imprimirPuente();
                        imprimirLista(&miLista); 
                        //contador=contador+1;
                    }
                    
                    //ArrPuente[0].estado == 0;
                }
                else
                {
                    ArrPuente[2] = ArrPuente[1];
                    ArrPuente[1] = ArrPuente[0];
                    ArrPuente[0] = EpacioVacio;
                    imprimirPuente();
                    imprimirLista(&miLista);
                }
                
            }
            pthread_mutex_unlock(&mutex);
            sleep(2);
        }
    }
    else if (actual->direccion == 'D')
    {
        pthread_mutex_lock(&mutex);
        while (actual_estado == 4)
        {
            if (estaVacio(ArrPuente) == 1)
            {
                actual->estado = 2;
                ArrPuente[2] = *actual;
                contador=1;
                // Buscar un nodo por ID
                const char* idBuscado = actual->id;
                Auto* nodoEncontrado = buscarNodoPorID(&miLista, idBuscado);
                eliminarNodo(&miLista, nodoEncontrado);
                actual_estado = 2;
                imprimirPuente();
                imprimirLista(&miLista); 
            }
            else
            {
                if (ArrPuente[2].direccion == actual->direccion)
                {
                    if (contador <= 4 )
                    {
                        
                        actual->estado = 2;
                        
                        ArrPuente[2].estado = 1;
                        ArrPuente[1].estado = 0;

                        ArrPuente[0] = ArrPuente[1];
                        ArrPuente[1] = ArrPuente[2];
                        ArrPuente[2] = *actual;                       

                        contador=contador+1;
                        // Buscar un nodo por ID
                        const char* idBuscado = actual->id;
                        Auto* nodoEncontrado = buscarNodoPorID(&miLista, idBuscado);
                        eliminarNodo(&miLista, nodoEncontrado); 
                        actual_estado = 2;
                        imprimirPuente();
                        imprimirLista(&miLista);
                    }
                    else 
                    {
                        ArrPuente[2] = EpacioVacio;
                        ArrPuente[1] = ArrPuente[2];
                        ArrPuente[0] = ArrPuente[1];
                        imprimirPuente();
                        imprimirLista(&miLista);    
                        //contador=contador+1;
                    }
                    
                    //ArrPuente[0].estado == 0;
                }
                else
                {
                    ArrPuente[2] = EpacioVacio;
                    ArrPuente[1] = ArrPuente[2];
                    ArrPuente[0] = ArrPuente[1];
                    imprimirPuente();
                    imprimirLista(&miLista);
                }
                
            }
            sleep(2);
        }
        pthread_mutex_unlock(&mutex);
    }
    //printf("\n- CERRANDO HILO , %ld -\n", pthread_self());
    pthread_exit(0); /* exit thread */
} 

/*#####################################------------- MAIN  -------------#####################################*/
int main() {



    inicializarLista(&miLista);
    
    Auto* auto1; // Crear algunos autos y agregarlos a la lista

                auto1 = inicializarAuto("I");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("I");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("I");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("D");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("I");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("D");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("D");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("D");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("D");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("D");
                insertarAlFinal(&miLista, auto1);
                auto1 = inicializarAuto("D");
                insertarAlFinal(&miLista, auto1);
                


    char comando[10];
    while (1) { // en este while se ejecutan los programas

        printf("Ingrese un comando (car izq, car der, status, start): ");
        scanf("%s", comando);

        if (strcmp(comando, "car") == 0) {
            char direccion[4];
            scanf("%s", direccion);

            if (strcmp(direccion, "izq") == 0) {
                auto1 = inicializarAuto("I");
                insertarAlFinal(&miLista, auto1);
                //agregarAuto(autos, &contador, 'I');
            } else if (strcmp(direccion, "der") == 0) {
                auto1 = inicializarAuto("D");
                insertarAlFinal(&miLista, auto1);
                //agregarAuto(autos, &contador, 'D');
            }
        } else if (strcmp(comando, "status") == 0) {
            // Mostrar estado de los autos
            printf("\nLista de autos:\n");
            imprimirLista(&miLista);

        } else if (strcmp(comando, "start") == 0) {

            Auto* actual = miLista.frente;
            Auto* siguiente = actual->siguiente;


            while (actual->siguiente != NULL) {
                
                //sleep(1);

                siguiente = actual->siguiente; //almacena el siguiente auto
                
                
                pthread_create(&actual->hilo, NULL, (void *) &funcion, actual); // elimina el auto actual
                pthread_join(actual->hilo, NULL);
                
                //eliminarNodo(&miLista, actual); 
                //imprimirPuente();
                //imprimirLista(&miLista);
                
                actual = siguiente; //actual ahora almacena el siguiente auto

                if (actual->siguiente == NULL)
                {
                    pthread_create(&actual->hilo, NULL, (void *) &funcion, actual); // elimina el auto actual
                    pthread_join(actual->hilo, NULL);
                    //eliminarNodo(&miLista, actual); 
                    //imprimirPuente();
                    //imprimirLista(&miLista);
                }
                
            }

        }
    }

    return 0;
}

