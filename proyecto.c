/*  Este programa es un CRUD que permite crear, visualizar, modificar y borrar elementos de una base de datos en formato CSV.
    Elaborado por: Sebastian Baloa, Osler Solano y Efraín Arreaza
//-----------------------------------------------------------------
    Revisiones:
    rev: 0, Fecha: 26-03-2024
    rev: 2, Fecha: 30-03-2024
*/

// -------------------------
//  Librerias
// -------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <ctype.h>

// -------------------------
//  Definiciones
// -------------------------

#define MAX_ID 4
#define MAX_NOMBRES 30
#define MAX_APELLIDOS 30
#define MAX_CARGO 20
#define MAX_NACIONALIDAD 20

// -------------------------
//  Estructuras
// -------------------------

// Definición de la estructura para las fechas:
// typedef struct Fechas
// {
//     int dia;
//     int mes;
//     int anno;
// } fecha;

// Definición de la estructura para los nombres y apellidos:
typedef struct NombreApellido
{
    char nombres[MAX_NOMBRES + 1];
    char apellidos[MAX_APELLIDOS + 1];
} nombres;

// Definición de la estructura del empleado
typedef struct datosEmpleado
{
    char idEmpleado[MAX_ID];
    nombres nombre;
    char cargo[MAX_CARGO + 1];
    char nacionalidad[MAX_NACIONALIDAD + 1];
} Empleado;

// Definición de la estructura del nodo de la lista enlazada
typedef struct Nodo
{
    Empleado empleado;      // Almacena la información del empleado
    struct Nodo *siguiente; // Apunta al siguiente nodo en la lista
} nodo;

// -------------------------
//  Variables globales
// -------------------------

//  Declaración de inicioLista
nodo *inicioLista = NULL;
/*  Logica:
-   Declarar primero un puntero que apunte al primer elemento de la lista enlazada.
-   El valor inicial sera NULL, para indicar que aun no existe la primera lista.
*/

int contadorEmpleados = 0;

unsigned int nroEmpleado = 0;

// -------------------------
//  Funciones
// -------------------------

/*  Interfaz de usuario:
    ------------------------ */
// funcion para mostrar el menu
void Mostrar_menu()
{
    printf("\n*** Menu *** \n");
    printf("1. Crear nuevo empleado \n");
    printf("2. Mostrar todos los empleados \n");
    printf("3. Actualizar informacion de empleado \n");
    printf("4. Eliminar empleado \n");
    printf("5. Exportar informacion de empleados a un archivo CSV \n");
    printf("6. Salir \n");
}

// funcion para agregar un nuevo empleado a la base de datos
void crearEmpleado()
{
    printf("\n ----> Menu \n");
    printf("         ----> Crear un nuevo empleado \n");
    printf("1. Ingresar datos manualmente \n");
    printf("2. Leer de archivo CSV \n");
}

/*  Manipulación de cadenas:
    ------------------------ */
// Función para eliminar el carácter de salto de línea ('\n') de una cadena
void eliminarSaltoDeLinea(char *cadena)
{
    int longitud = strlen(cadena); // Obtener la longitud de la cadena

    // Buscar el carácter de salto de línea ('\n') y reemplazarlo por un carácter nulo ('\0')
    for (int i = 0; i < longitud; i = i + 1)
    {
        if (cadena[i] == '\n')
        {
            cadena[i] = '\0';
            break; // Salir del bucle una vez que se ha eliminado el salto de línea
        }
    }
}

// Funcion para capitalizar la primera letra de todas las cadenas de caracteres
void capitalizarPrimeraLetra(char *cadena)
{
    int i = 0;
    int capitalizar = 1; // Variable para indicar si se debe capitalizar la próxima letra

    while (cadena[i] != '\0')
    {
        if (cadena[i] == ' ')
        {
            capitalizar = 1; // Si se encuentra un espacio, se debe capitalizar la próxima letra
        }
        else if (capitalizar)
        {
            if (cadena[i] >= 'a' && cadena[i] <= 'z') // Verificar si la letra es minuscula para capitalizar
            {
                cadena[i] = cadena[i] - 32; // Capitalizar la letra
                capitalizar = 0;            // Después de capitalizar, establecer a 0 para evitar capitalizar letras siguientes
            }
            else
            {
                capitalizar = 0; // Si la letra ya estaba capitalizada se establece capitalizar a 0 para evitar capitalizar letras siguientes
            }
        }
        else
        {
            if (cadena[i] >= 'A' && cadena[i] <= 'Z') // Verificar si la letra es mayuscula para convertir a minuscula
            {
                cadena[i] = cadena[i] + 32; // Convertir a minúscula
            }
        }
        i = i + 1;
    }
}

/* Validación de entrada de datos:
    ------------------------*/
// Funcion para validar que no hayan numeros dentro de la cadena
int validarCaracteres(const char *str)
{
    // Valida si la cadena contiene caracteres no alfabéticos
    int i = 0;
    if (str[0] == ' ' || str[0] == '\0')
    {
        printf("Error: El nombre no puede estar vacio: %s \n", str);
        return 1; // Se encontró un carácter no alfabético
    }
    while (str[i] != '\0')
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z') || str[i] == ' ')
        {
            i = i + 1;
        }
        else
        {
            printf("Error: Solo se admiten caracteres alfabeticos: %s \n", str);
            return 1; // Se encontró un carácter no alfabético
        }
    }
    return 0; // Todos los caracteres son alfabéticos
}

/*  Manipulación de datos:
    ------------------------ */
// Función para agregar un nuevo empleado a la lista
void agregarEmpleado(const Empleado empleado)
{
    // Crear un nuevo nodo
    nodo *nuevoNodo = (nodo *)malloc(sizeof(nodo));
    if (nuevoNodo == NULL)
    {
        printf("Error: No se pudo asignar memoria para el nuevo nodo.\n");
        exit(1);
    }
    // Asignar la información del empleado al nuevo nodo
    nuevoNodo->empleado = empleado;
    nuevoNodo->siguiente = NULL;

    // Si la lista está vacía, el nuevo nodo será el inicio de la lista
    if (inicioLista == NULL)
    {
        inicioLista = nuevoNodo;
        return;
    }

    // Recorrer la lista hasta encontrar el último nodo
    nodo *actual = inicioLista;
    while (actual->siguiente != NULL)
    {                               // Mientras no lleguemos al último nodo
        actual = actual->siguiente; // Avanzamos al siguiente nodo
    }                               // Al salir del bucle, actual apunta al último nodo

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

// Función para eliminar un empleado de la lista
void eliminarEmpleado(nodo **cabeza, nodo *nodoAEliminar)
{
    // Si solo hay un elemento en la lista
    if ((*cabeza)->siguiente == NULL && nodoAEliminar == *cabeza)
    {
        free(nodoAEliminar);
        *cabeza = NULL;
        return;
    }

    // Caso especial: si el nodo a eliminar es el primer nodo de la lista
    if (*cabeza == nodoAEliminar)
    {
        *cabeza = nodoAEliminar->siguiente;
        free(nodoAEliminar);
    }

    // En caso de que el nodo a eliminar este en el medio
    nodo *temp = *cabeza;
    while (temp->siguiente != nodoAEliminar)
    {
        temp = temp->siguiente;
    }

    // Enlazar el nodo anterior directamente al siguiente nodo del nodo a eliminar
    temp->siguiente = nodoAEliminar->siguiente;

    // // Liberar la memoria ocupada por el nodo a eliminar
    free(nodoAEliminar);
}

// Función para mostrar la lista de empleados
void mostrarListaEmpleados(nodo *listaEmpleados)
{
    printf("\n ----> Menu \n");
    printf("         ----> Mostrar todos los empleados \n\n");
    printf("ID:     Nombres:       Apellidos:     Cargo:         Nacionalidad:\n");

    nodo *actual = listaEmpleados;
    while (actual != NULL)
    {
        // Imprimir la información del empleado del nodo actual
        printf("%-3s     %-15s%-15s%-15s%-15s\n", actual->empleado.idEmpleado, actual->empleado.nombre.nombres, actual->empleado.nombre.apellidos,
               actual->empleado.cargo, actual->empleado.nacionalidad);
        actual = actual->siguiente;
    }
}

// Funcion para contar empleados:
void contarEmpleados(int *contar)
{
    nodo *recorrerLista = inicioLista; // Suponiendo que 'inicioLista' es el puntero al inicio de la lista

    *contar = 0; // Inicializar el contador a 0

    // Recorrer la lista y contar empleados
    while (recorrerLista != NULL)
    {
        (*contar)++; // Incrementar el contador
        recorrerLista = recorrerLista->siguiente;
    }
}

// Función para liberar la memoria asignada para todos los nodos de la lista
void liberarMemoriaLista(nodo **inicio)
{
    nodo *actual = *inicio;
    while (actual != NULL)
    {
        nodo *temp = actual;        // Guardar una referencia al nodo actual
        actual = actual->siguiente; // Mover al siguiente nodo antes de liberar el actual
        free(temp);                 // Liberar la memoria del nodo actual
    }
}

/*  Entrada/Salida de archivos:
    ------------------------ */
// Función para guardar datos en archivo csv
void guardarDatosCSV(nodo *listaEmpleados)
{
    FILE *archivo;
    archivo = fopen("prueba.csv", "a"); // Cambiar a .csv
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo.\n");
        return;
    }

    // Escribir las cabeceras de las columnas
    fprintf(archivo, "ID,Nombres,Apellidos,Cargo,Nacionalidad\n");

    // Escribir los datos de cada empleado en el archivo
    nodo *actual = listaEmpleados; // Cambiar a listaEmpleados
    while (actual != NULL)
    {
        fprintf(archivo, "%s,%s,%s,%s,%s\n", actual->empleado.idEmpleado, actual->empleado.nombre.nombres, actual->empleado.nombre.apellidos,
                actual->empleado.cargo, actual->empleado.nacionalidad);
        actual = actual->siguiente;
    }

    fclose(archivo);
}

/*  Gestión de la base de datos:
    ------------------------ */
// Funcion para buscar elementos en la base de datos
void buscarEmpleado(nodo *listaEmpleados)
{
    while (1)
    {
        printf("\nSelecciona el criterio de busqueda \n");
        printf("1. Numero de empleado \n");
        printf("2. Nombre del empleado \n");

        printf("Seleccione una opcion: ");
        char opcion = getch();

        switch (opcion)
        {
        case '1':
            char input[MAX_ID];
            unsigned int numero = 0;
            while (1)
            {
                system("cls");
                printf("Ingrese el numero del empleado que desea buscar: \n");

                // Leer la entrada del usuario como una cadena
                fgets(input, MAX_ID, stdin);

                // Convertir la cadena a un número entero
                numero = strtol(input, NULL, 10);

                // Verificar si la conversión fue exitosa
                if (numero == 0)
                {
                    printf("Error: Entrada no válida.\n");
                    Sleep(3);
                }
                else
                {
                    break; // Salir del bucle si la entrada es válida
                }
            }

            // Formatear el número como una cadena con tres dígitos y ceros a la izquierda
            char id[MAX_ID];
            sprintf(id, "%03d", numero);

            // Buscar el empleado con el ID especificado
            nodo *aux = listaEmpleados;
            while (aux != NULL)
            {
                if (!strcmp(id, aux->empleado.idEmpleado))
                {
                    printf("- La busqueda coincide con el siguiente elemento: \n");
                    printf("ID:     Nombres:       Apellidos:     Cargo:         Nacionalidad:\n");
                    // Imprimir la información del empleado del nodo elemento
                    printf("%-3s     %-15s%-15s%-15s%-15s\n", aux->empleado.idEmpleado, aux->empleado.nombre.nombres, aux->empleado.nombre.apellidos,
                           aux->empleado.cargo, aux->empleado.nacionalidad);
                    return; // Salir de la funcion después de encontrar el empleado
                }
                aux = aux->siguiente;
            }
            // Si no se encuentra el empleado
            printf("Error: No se encontraron datos para el numero proporcionado en la base de datos. Asegurese de ingresar un numero valido.");
            Sleep(1500);
            system("cls");
            break;

        case '2':
            while (1)
            {
                system("cls");
                printf("Ingrese el nombre del usuario que desea buscar:\n");
                char nombre[20];
                scanf("%s", nombre);
                capitalizarPrimeraLetra(nombre);

                // Variable auxiliar para recorrer la lista sin modificar listaEmpleados
                nodo *aux = listaEmpleados;

                while (aux != NULL)
                {
                    if (!strcmp(nombre, aux->empleado.nombre.nombres))
                    {
                        printf("- La busqueda coincide con el siguiente elemento: \n");
                        printf("ID:     Nombres:       Apellidos:     Cargo:         Nacionalidad:\n");
                        // Imprimir la información del empleado del nodo elemento
                        printf("%-3s     %-15s%-15s%-15s%-15s\n", aux->empleado.idEmpleado, aux->empleado.nombre.nombres, aux->empleado.nombre.apellidos,
                               aux->empleado.cargo, aux->empleado.nacionalidad);
                        return;
                    }
                    aux = aux->siguiente;
                }

                if (aux == NULL)
                {
                    printf("Error: No se encontraron datos para el nombre proporcionado en la base de datos. Asegurese de ingresar un nombre valido.");
                    Sleep(1500);
                    system("cls");
                }
            }
            break;

        default:
            system("cls");
            printf("Entrada invalida. Por favor, intentalo de nuevo\n");
            Sleep(5);
            break;
        }
    }
}

// -------------------------
//  Función principal
// -------------------------

int main()
{

    Empleado empleado;
    nodo *recorrerLista;
    recorrerLista = inicioLista;

    do
    {
        Mostrar_menu();
        printf("Seleccione una opcion: ");
        char opc = getch();

        system("cls");
        switch (opc)
        {
        case '1':
            do
            {
                crearEmpleado();
                printf("Seleccione una opcion: ");
                char a = getch();
                // Limpiar el buffer de entrada
                fflush(stdin);

                if (a == '1')
                {
                    int longitud;
                    system("cls");
                    printf("\n ----> Menu \n");
                    printf("         ----> Crear empleado \n");
                    printf("                 ----> Ingresar datos manualmente \n\n");

                    while (1)
                    {
                        printf("Ingrese los datos del empleado: \n\n");
                        while (1)
                        {
                            // Limpiar el buffer de entrada
                            fflush(stdin);
                            printf("Nombres de la persona: ");
                            scanf("%s", empleado.nombre.nombres);

                            // Capitalizar la primera letra de cada nombre de la cadena
                            capitalizarPrimeraLetra(empleado.nombre.nombres);
                            // Contar la cantidad de caractéres de la cadena
                            longitud = strlen(empleado.nombre.nombres);

                            // Verificamos si sobrepasa el maximo establecido
                            if (longitud > MAX_NOMBRES)
                            {
                                printf("Error: Ha excedido el limite de %d caracteres.\n", MAX_NOMBRES);
                                Sleep(3);
                            }
                            else if (!validarCaracteres(empleado.nombre.nombres))
                            {
                                break;
                            }
                        }

                        while (1)
                        {
                            // Limpiar el buffer de entrada
                            fflush(stdin);
                            printf("Apellidos de la persona: ");
                            scanf("%s", empleado.nombre.apellidos);

                            // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                            eliminarSaltoDeLinea(empleado.nombre.apellidos);
                            // Capitalizar la primera letra de cada nombre de la cadena
                            capitalizarPrimeraLetra(empleado.nombre.apellidos);
                            // Contamos la cantidad de caracters de la cadena
                            longitud = strlen(empleado.nombre.apellidos);

                            // Verificamos si sobrepasa el maximo establecido
                            if (longitud > MAX_APELLIDOS)
                            {
                                printf("Error: Ha excedido el limite de %d caracteres.\n", MAX_APELLIDOS);
                                Sleep(3);
                            }
                            else if (!validarCaracteres(empleado.nombre.apellidos))
                            {
                                break;
                            }
                        }

                        while (recorrerLista != NULL)
                        {
                            if (strcmp(recorrerLista->empleado.nombre.nombres, empleado.nombre.nombres) == 0 ||
                                strcmp(recorrerLista->empleado.nombre.apellidos,
                                       empleado.nombre.apellidos) != 1)
                            {
                                printf("Error: El empleado '%s' ya existe en la lista.\n", empleado.nombre.nombres);
                                Sleep(3);
                                goto inicio_opc_1; // Volver al inicio del bucle para solicitar nuevamente los datos del empleado
                            }
                            recorrerLista = recorrerLista->siguiente; // Avanzar al siguiente nodo
                        }

                        while (1)
                        {
                            // Limpiar el buffer de entrada
                            fflush(stdin);
                            printf("Cargo de la persona: ");
                            scanf("%s", empleado.cargo);

                            // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                            eliminarSaltoDeLinea(empleado.cargo);
                            // Capitalizar la primera letra de cada nombre de la cadena
                            capitalizarPrimeraLetra(empleado.nombre.nombres);
                            // Contamos la cantidad de caracters de la cadena
                            longitud = strlen(empleado.cargo);

                            // Verificamos si sobrepasa el maximo establecido
                            if (longitud > MAX_CARGO)
                            {
                                printf("Error: Ha excedido el limite de %d caracteres.\n", MAX_CARGO);
                                Sleep(3);
                            }
                            else if (!validarCaracteres(empleado.cargo))
                            {
                                break;
                            }
                        }

                        while (1)
                        {
                            // Limpiar el buffer de entrada
                            fflush(stdin);
                            printf("Nacionalidad de la persona: ");
                            scanf("%s", empleado.nacionalidad);

                            // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                            eliminarSaltoDeLinea(empleado.nacionalidad);
                            // Capitalizar la primera letra de cada nombre de la cadena
                            capitalizarPrimeraLetra(empleado.nombre.nombres);
                            // Contamos la cantidad de caracters de la cadena
                            longitud = strlen(empleado.nacionalidad);

                            // Verificamos si sobrepasa el maximo establecido
                            if (longitud > MAX_NACIONALIDAD)
                            {
                                printf("Error: Ha excedido el limite de %d caracteres.\n", MAX_NACIONALIDAD);
                                Sleep(3);
                            }
                            else if (!validarCaracteres(empleado.nacionalidad))
                            {
                                break;
                            }
                        }
                        // Limpiar el buffer de entrada
                        fflush(stdin);

                        nroEmpleado = nroEmpleado + 1;
                        sprintf(empleado.idEmpleado, "%03d", nroEmpleado);

                        agregarEmpleado(empleado);
                        printf("Elemento annadido exitosamente! \n");

                        printf("Presione 'A' para annadir otro empleado o cualquier tecla para finalizar: ");
                        a = getch();

                        if (a == 'A' || a == 'a')
                        {
                            system("cls");
                            goto inicio_opc_1;
                        }
                        else
                        {
                            printf("Guardando elementos... ");
                            Sleep(1500);
                            system("cls");
                            contarEmpleados(&contadorEmpleados);
                            printf("Se han annadido %d elementos a la base de datos exitosamente!", contadorEmpleados);
                            Sleep(1500);
                            system("cls");
                        }
                        break;
                    inicio_opc_1:
                    }
                }
                else if (opc == '2')
                {
                    printf("\n\nProximamente...\n\n");
                    Sleep(1500);
                    system("cls");
                    break;
                }
                else
                {
                    system("cls");
                    printf("Entrada invalida. Por favor, intentalo de nuevo\n");
                    Sleep(5);
                    goto inicio_case_1;
                }
                break;
            inicio_case_1:
            } while (1);
            break;

        case '2':
            if (inicioLista == NULL)
            {
                printf("Error: La lista de empleados esta vacia.\n");
                Sleep(1500);
                break; // Salir de la función, ya que no hay elementos para buscar
            }

            mostrarListaEmpleados(inicioLista);
            printf("Presiona cualquier tecla para volver al menu...\n");
            getch();
            system("cls");
            break;

        case '3':
            if (inicioLista == NULL)
            {
                printf("Error: La lista de empleados esta vacia.\n");
                Sleep(3);
                break; // Salir de la función, ya que no hay elementos para buscar
            }

            nodo *elemento = inicioLista;
            while (1)
            {
                system("cls");
                printf("\n ----> Menu \n");
                printf("         ----> Actualizar informacion de empleado \n\n");

                buscarEmpleado(elemento);

                printf("\nContinuar (1), Volver al menu (2), Cualquier tecla para volver a buscar\n");
                char opcion = getch();

                if (opcion == '1')
                {
                    // Salir del bucle y continuar con el programa
                    system("cls");
                    break;
                }
                else if (opcion == '2')
                {
                    // Volver al menú principal
                    goto volver;
                }
                else
                {
                    // Volver a buscar otro empleado
                    // No se necesita ninguna acción adicional, ya que el programa continuará en el siguiente ciclo del bucle while
                }
            }

            printf("\n ----> Menu \n");
            printf("         ----> Actualizar informacion de empleado \n\n");
            printf("NRO: \tNombres: \tApellidos: \tCargo: \tNacionalidad: \n");
            printf("%-s \t\t%-s \t\t%-s \t%-s \n", elemento->empleado.nombre.nombres, elemento->empleado.nombre.apellidos,
                   elemento->empleado.cargo, elemento->empleado.nacionalidad);
            printf("\n\n1. Modificar nombres \n");
            printf("2. Modificar apellidos \n");
            printf("3. Modificar cargo \n");
            printf("4. Modificar nacionalidad \n");
            printf("5. Volver al menu \n");

            char a = getch();
            // Limpiar el buffer de entrada
            fflush(stdin);

            int longitud;
            switch (a)
            {
            case '1':
                while (1)
                {
                    printf("Ingrese el nuevo nombre: ");
                    fgets(elemento->empleado.nombre.nombres, sizeof(elemento->empleado.nombre.nombres), stdin);
                    // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                    eliminarSaltoDeLinea(elemento->empleado.nombre.apellidos);
                    // Capitalizar la primera letra de cada nombre de la cadena
                    capitalizarPrimeraLetra(elemento->empleado.nombre.apellidos);
                    // Contamos la cantidad de caracters de la cadena
                    longitud = strlen(elemento->empleado.nombre.apellidos);
                    // Verificamos si sobrepasa el maximo establecido
                    if (longitud > MAX_APELLIDOS)
                    {
                        printf("Error: Ha excedido el limite de %d caracteres.\n", MAX_APELLIDOS);
                        Sleep(3);
                        system("cls");
                    }
                    else if (!validarCaracteres(empleado.nombre.nombres))
                    {
                        break;
                    }
                }
                break;

            case '2':
                while (1)
                {
                    printf("Ingrese el nuevo apellido: ");
                    fgets(elemento->empleado.nombre.apellidos, sizeof(elemento->empleado.nombre.apellidos), stdin);
                    // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                    eliminarSaltoDeLinea(elemento->empleado.nombre.apellidos);
                    // Capitalizar la primera letra de cada nombre de la cadena
                    capitalizarPrimeraLetra(elemento->empleado.nombre.apellidos);
                    // Contamos la cantidad de caracters de la cadena
                    longitud = strlen(elemento->empleado.nombre.apellidos);
                    // Verificamos si sobrepasa el maximo establecido
                    if (longitud > MAX_APELLIDOS)
                    {
                        printf("Error: Ha excedido el limite de %d caracteres.\n", MAX_APELLIDOS);
                        Sleep(3);
                        system("cls");
                    }
                    else if (!validarCaracteres(empleado.nombre.nombres))
                    {
                        break;
                    }
                }
                break;

            case '3':
                while (1)
                {
                    printf("Ingrese el nuevo cargo: ");
                    fgets(elemento->empleado.cargo, sizeof(elemento->empleado.cargo), stdin);
                    // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                    eliminarSaltoDeLinea(elemento->empleado.cargo);
                    // Capitalizar la primera letra de cada nombre de la cadena
                    capitalizarPrimeraLetra(elemento->empleado.cargo);
                    // Contamos la cantidad de caracters de la cadena
                    longitud = strlen(elemento->empleado.cargo);
                    // Verificamos si sobrepasa el maximo establecido
                    if (longitud > MAX_CARGO)
                    {
                        printf("Error: Ha excedido el limite de %d caracteres.\n", MAX_CARGO);
                        Sleep(3);
                        system("cls");
                    }
                    else if (!validarCaracteres(empleado.nombre.nombres))
                    {
                        break;
                    }
                }
                break;

            case '4':
                while (1)
                {
                    printf("Ingrese la nueva nacionalidad: ");
                    fgets(elemento->empleado.nacionalidad, sizeof(elemento->empleado.nacionalidad), stdin);
                    // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                    eliminarSaltoDeLinea(elemento->empleado.nacionalidad);
                    // Capitalizar la primera letra de cada nombre de la cadena
                    capitalizarPrimeraLetra(elemento->empleado.nacionalidad);
                    // Contamos la cantidad de caracters de la cadena
                    longitud = strlen(elemento->empleado.nacionalidad);
                    // Verificamos si sobrepasa el maximo establecido
                    if (longitud > MAX_NACIONALIDAD)
                    {
                        printf("Error: Ha excedido el limite de %d caracteres.\n", MAX_NACIONALIDAD);
                        Sleep(3);
                        system("cls");
                    }
                    else if (!validarCaracteres(empleado.nombre.nombres))
                    {
                        break;
                    }
                }
                break;

            case '5':
                printf("Volviendo al menu...");
                Sleep(5);
                system("cls");
                break;
            default:
                system("cls");
                printf("Entrada invalida. Por favor, intentalo de nuevo\n");
                Sleep(5);
                break;
            }
        volver:
            break;

        case '4':
            if (inicioLista == NULL)
            {
                printf("Error: La lista de empleados esta vacia.\n");
                Sleep(3);
                break; // Salir de la función, ya que no hay elementos para buscar
            }

            elemento = inicioLista;

            while (1)
            {
                system("cls");
                printf("\n ----> Menu \n");
                printf("         ----> Borrar empleado \n\n");

                buscarEmpleado(elemento);

                printf("\nContinuar (1), Volver al menu (2), Cualquier tecla para volver a buscar\n");
                char opcion = getch();

                if (opcion == '1')
                {
                    // Salir del bucle y continuar con el programa
                    system("cls");
                    break;
                }
                else if (opcion == '2')
                {
                    // Volver al menú principal
                    goto volver2;
                }
                else
                {
                    // Volver a buscar otro empleado
                    // No se necesita ninguna acción adicional, ya que el programa continuará en el siguiente ciclo del bucle while
                }
            }
            printf("\n ----> Menu \n");
            printf("         ----> Borrar empleado \n\n");
            printf("NRO: \tNombres: \tApellidos: \tCargo: \tNacionalidad: \n");
            printf("%-s \t\t%-s \t\t%-s \t%-s \n", elemento->empleado.nombre.nombres, elemento->empleado.nombre.apellidos,
                   elemento->empleado.cargo, elemento->empleado.nacionalidad);

            printf("\nEliminar empleado (1), Volver al menu (2)");

            a = getch();

            switch (a)
            {
            case '1':
                char c;
                printf("Seguro que desea eliminar al empleado con ID %s? (s/n): ", elemento->empleado.idEmpleado);
                scanf(" %c", &c);

                if (c == 's' || c == 'S')
                {
                    eliminarEmpleado(&inicioLista, elemento);
                    printf("La eliminacion del empleado con ID %s ha sido realizada exitosamente.\n", elemento->empleado.idEmpleado);
                    Sleep(1500);
                    system("cls");
                }
                else
                {
                    printf("La eliminacion del empleado con ID %s ha sido cancelada.\n", elemento->empleado.idEmpleado);
                    Sleep(1500);
                    system("cls");
                }
                break;

            case '2':

            default:
                system("cls");
                printf("Entrada invalida. Por favor, intentalo de nuevo\n");
                Sleep(5);
                break;
            }
        volver2:
            break;

        case '5':
            printf("Exportando datos...");
            guardarDatosCSV(inicioLista);
            Sleep(1500);
            printf("Datos exportados correctamente");
            break;

        case '6':
            printf("Saliendo del sistema...");
            Sleep(5);
            liberarMemoriaLista(&inicioLista);
            return 0;

        default:
            system("cls");
            printf("Entrada invalida. Por favor, intentalo de nuevo\n");
            Sleep(5);
            break;
        }
    } while (1);

    printf("Presiona una tecla para salir...\n");
    getchar(); // Espera hasta que se presione una tecla
    return 0;
}