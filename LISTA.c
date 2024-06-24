#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Estructura para representar un auto (nodo de la lista)
typedef struct Auto {
    pthread_t hilo;
    char id[7]; // "autoNN"
    char direccion; // 'I' para izquierda, 'D' para derecha, F si no esta iniciada
    int estado; // 0 al 2 para posicion en el puente, 4 para modo espera, 3 para ser eliminado
    struct Auto* siguiente;
    struct Auto* anterior; 
} Auto;

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

    nuevoAuto->estado = 4;
    nuevoAuto->hilo = hiloA;
    // Asignar la dirección
    nuevoAuto->direccion = direccion[0]; // Tomamos el primer carácter

    nuevoAuto->siguiente = NULL;
    nuevoAuto->anterior = NULL;
    return nuevoAuto;
}

// int main() {
//     ListaDobleEnlazada miLista;
//     inicializarLista(&miLista);

//     // Crear algunos autos y agregarlos a la lista
//     Auto* auto1 = inicializarAuto("I");
//     Auto* auto2 = inicializarAuto("D");
//     Auto* auto3 = inicializarAuto("I");
//     //Auto* auto1 = (Auto*)malloc(sizeof(Auto));
    
    
//     // Inicializa los campos de auto1 según tus necesidades
//     insertarAlFinal(&miLista, auto1);
//     insertarAlFinal(&miLista, auto2);
//     insertarAlFinal(&miLista, auto3);

//     // Imprimir la lista
//     printf("Lista de autos:\n");
//     imprimirLista(&miLista);

//     // Buscar un nodo por ID
//     const char* idBuscado = "auto01";
//     Auto* nodoEncontrado = buscarNodoPorID(&miLista, idBuscado);
//     if (nodoEncontrado != NULL) {
//         printf("Nodo encontrado: ID: %s, Dirección: %c\n", nodoEncontrado->id, nodoEncontrado->direccion);
//     } else {
//         printf("Nodo con ID %s no encontrado.\n", idBuscado);
//     }

//     // Eliminar un nodo (por ejemplo, auto1)
//     eliminarNodo(&miLista, auto1);
    
//     printf("\nLista de autos:\n");
//     imprimirLista(&miLista);
//     // Liberar memoria (si es necesario)
//     // ...

//     return 0;
// }

// Imprimir el puente
void imprimirPuente() 
{
    char fle[2];
    char dir[10];
    char id_0[7];

    if (ArrPuente[0].direccion == "I")
    {
        strcpy(fle,">>");
        strcpy(dir,">>>>>>>>>>");
        strcpy(id_0, ArrPuente[0].id);
    }
    else if (ArrPuente[0].direccion == "D")
    {
        strcpy(fle,"<<");
        strcpy(dir,"<<<<<<<<<<");
        strcpy(id_0, ArrPuente[0].id);
    }
    else
    {
        strcpy(fle,"==");
        strcpy(dir,"==========");
        strcpy(id_0,"======");      
    }


    {

    }
    
    printf("\n==================================================================\n");
    printf("================%s======>>>>>>>>>>=======>>>>>>>>>>=======\n", &dir);
    printf("================%s%s%s======>>%s>>=======>>%s>>=======\n",&id_0,  );
    printf("================%s======>>>>>>>>>>=======>>>>>>>>>>=======\n", &dir);
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
    Auto *actual = (Auto *)ptr;  /* una variable que apunta a un auto */
    
    

    if (estaVacio(ArrPuente) == 1)
    {
        actual->estado = 0;
        ArrPuente[0] = *actual;
        contador=contador+1;

        // Buscar un nodo por ID
        const char* idBuscado = actual->id;
        Auto* nodoEncontrado = buscarNodoPorID(&miLista, idBuscado);
        if (nodoEncontrado != NULL) {
            printf("Nodo encontrado: ID: %s, Dirección: %c\n", nodoEncontrado->id, nodoEncontrado->direccion);
        } else {
            printf("Nodo con ID %s no encontrado.\n", idBuscado);
        }
        // Eliminar un nodo encontrado
        eliminarNodo(&miLista, nodoEncontrado); 
    }
    else
    {
        if (ArrPuente[0].direccion == actual->direccion)
        {
            if (contador <= 4 )
            {
                actual->estado = 0;
                ArrPuente[2] = ArrPuente[1];
                ArrPuente[1] = ArrPuente[0];
                ArrPuente[0] = *actual;
                contador=contador+1;
            }
            
            ArrPuente[0].estado == 0;
        }
        else
        {

        }
        
    }
    


    // /* aca se ejecutan las instrucciones */
    // printf("direccion %c ID %s \n", actual->direccion, actual->id);

    printf("el semforo esta asi: \n");
    for (size_t i = 0; i < 3; i++)
    {
        if(ArrPuente[i].direccion != 'I' && ArrPuente[i].direccion != 'D')
        {
            printf("NULL-");
        }
        else
        {
            printf("| %s |",ArrPuente[i].id);
        }
        
    }
    // printf("\n");
    // //pthread_exit(0); /* exit */
} 

    // Auto* actual = lista->frente;
    // //while (actual != NULL) {
    //     if(actual->direccion == 'I')
    //     {
    //         printf("=> %s \n", actual->id);

/*#####################################------------- MAIN  -------------#####################################*/
int main() {
    // Auto autos[100];
    // int contador = 0;

    
    
    inicializarLista(&miLista);

    
    Auto* auto1; // Crear algunos autos y agregarlos a la lista

                auto1 = inicializarAuto("I");
                insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("D");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("I");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("D");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("I");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("D");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("D");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("D");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("D");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("D");
                // insertarAlFinal(&miLista, auto1);
                // auto1 = inicializarAuto("D");
                // insertarAlFinal(&miLista, auto1);
                


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
            // Iniciar simulación

            Auto* actual = miLista.frente;
            //printf("este es el primer auto => %s \n", actual->id);
            
            pthread_create(&actual->hilo, NULL, (void *) &funcion, actual);
            pthread_join(actual->hilo, NULL);
            imprimirPuente();
            // while (actual != NULL) {
            //     pthread_create(&actual->hilo, NULL, (void *) &funcion, actual);
            //     actual = actual->siguiente;
            //     // if(actual->direccion == 'I')
            //     // {
            //     //     printf("=> %s \n", actual->id);
            //     //     actual = actual->siguiente;
            //     // }
            //     // else
            //     // {
            //     //     printf("<= %s \n", actual->id);
            //     //     actual = actual->siguiente;
            //     // }
            // }






            printf("Simulación iniciada.\n");
            //break;
        }
    }

    return 0;
}

