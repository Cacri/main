/*  Este programa es un CRUD que permite crear, visualizar, modificar y borrar elementos de una base de datos de excel. Cada uno de estos elementos será un
    aspirante para un cargo de tripulación de una embarcación.
    NOTA: (La jerarquía de cargos y el listado de cada uno de ellos está en la hoja del diccionario de variables).
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

#define MAX_NOMBRES 30
#define MAX_APELLIDOS 30
#define MAX_CARGO 20
#define MAX_NACIONALIDAD 20

// -------------------------
//  Estructuras
// -------------------------

// Definición de la estructura para las fechas:
typedef struct Fechas
{
    int dia;
    int mes;
    int anno;
} fecha;

// Definición de la estructura para los nombres y apellidos:
typedef struct NombreApellido
{
    char nombres[MAX_NOMBRES + 1];
    char apellidos[MAX_APELLIDOS + 1];
} nombres;

// Definición de la estructura del empleado
typedef struct datosEmpleado
{
    nombres nombre;
    char cargo[MAX_CARGO + 1];
    char nacionalidad[MAX_NACIONALIDAD + 1];
    // fecha fecha_nacimiento;
    // char lugar_nacimiento[15];
    // char pasaporte[15];
    // fecha fecha_vencimiento_pasaporte;
    // char libreta_marina[15];
} Empleado;

// Definición de la estructura del nodo de la lista enlazada
typedef struct Nodo
{
    Empleado empleado;      // Almacena la información del empleado
    struct Nodo *siguiente; // Apunta al siguiente nodo en la lista
} nodo;

// Definición del tipo booleano utilizando un tipo enumerado (enum)
typedef enum
{
    false = 0, // Valor entero 0 que representa "falso"
    true = 1   // Valor entero 1 que representa "verdadero"
} bool;        // Alias "bool" para este tipo enumerado

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
    for (int i = 0; i < longitud; i++)
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

// Función para mostrar la lista de empleados
void mostrarListaEmpleados(nodo *inicio)
{
    printf("\n ----> Menu \n");
    printf("         ----> Mostrar todos los empleados \n\n");
    printf("NRO: \tNombres: \t\tApellidos: \tCargo: \t\tNacionalidad: \n");

    nodo *actual = inicio;
    int pos = 1;
    while (actual != NULL)
    {
        // Imprimir la información del empleado del nodo actual
        printf("%-d \t%-s \t\t%-s \t\t%-s \t%-s \n", pos, actual->empleado.nombre.nombres, actual->empleado.nombre.apellidos,
               actual->empleado.cargo, actual->empleado.nacionalidad);
        actual = actual->siguiente;
        pos = pos + 1;
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
void liberarMemoriaLista(nodo *inicio)
{
    nodo *actual = inicio;
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
    fprintf(archivo, "NRO,Nombres,Apellidos,Cargo,Nacionalidad\n");

    // Escribir los datos de cada empleado en el archivo
    nodo *actual = listaEmpleados; // Cambiar a listaEmpleados
    int pos = 1;
    while (actual != NULL)
    {
        fprintf(archivo, "%d,%s,%s,%s,%s\n", pos, actual->empleado.nombre.nombres, actual->empleado.nombre.apellidos,
                actual->empleado.cargo, actual->empleado.nacionalidad);
        actual = actual->siguiente;
        pos = pos + 1;
    }

    fclose(archivo);
}

/*  Gestión de la base de datos:
    ------------------------ */
// Funcion para buscar elementos en la base de datos
void buscarEmpleado(nodo *lista)
{
    while (1)
    {
        if (lista == NULL)
        {
            printf("Error: La lista de empleados está vacía.\n");
            Sleep(3);
            return; // Salir de la función, ya que no hay elementos para buscar
        }

        printf("Selecciona el criterio de búsqueda \n");
        printf("1. Numero de empleado \n");
        printf("2. Nombre del empleado \n");
        printf("3. Numero de pasaporte del empleado \n");
        printf("4. Volver al menu \n");

        printf("Seleccione una opcion: ");
        char opcion = getch();

        switch (opcion)
        {
        case '1':
            int numero;
            while (1)
            {
                system("cls");
                printf("Ingrese el numero del empleado que desea buscar: \n");

                scanf("%d", &numero);

                // Verificamos si la entrada es un numero entero
                if (scanf("%d", &numero) != 1)
                { // Si scanf() no puede leer un número entero (porque el usuario ingresó caracteres no numéricos),
                  // devolverá un valor distinto de 1, indicando un fallo en la lectura.
                    printf("Error: Entrada no válida.\n");
                    Sleep(3);
                }
                else
                {
                    break; // --> break para ciclo while
                }
            }

            for (int i = 0; i < numero - 1; i = i + 1)
            {
                // Verifica si el siguiente nodo no es nulo
                if (lista->siguiente != NULL)
                {
                    lista = lista->siguiente; // Avanza al siguiente nodo
                }
                else
                {
                    printf("Error: No se encontraron datos para el numero proporcionado en la base de datos. Asegurese de ingresar un numero valido.");
                    Sleep(3);
                    system("cls");
                }
            }
            return;

            break; // --> break para el case

        case '2':
            while (1)
            {
                printf("Ingrese el nombre del usuario que desea buscar: \n");
                char nombre[20];
                fgets(nombre, sizeof(nombre), stdin);
            }

        case '4':
            lista = NULL;
            return;
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
                            fgets(empleado.nombre.nombres, sizeof(empleado.nombre.nombres), stdin);

                            // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                            eliminarSaltoDeLinea(empleado.nombre.nombres);
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
                            else if (validarCaracteres(empleado.nombre.nombres) == 0)
                            {
                                break;
                            }
                        }

                        while (1)
                        {
                            // Limpiar el buffer de entrada
                            fflush(stdin);
                            printf("Apellidos de la persona: ");
                            fgets(empleado.nombre.apellidos, sizeof(empleado.nombre.apellidos), stdin);

                            // Verificar si el último carácter es un salto de línea y reemplazarlo con un carácter nulo
                            eliminarSaltoDeLinea(empleado.nombre.apellidos);
                            // Capitalizar la primera letra de cada nombre de la cadena
                            capitalizarPrimeraLetra(empleado.nombre.apellidos);
                            // Contamos la cantidad de caracters de la cadena
                            longitud = strlen(empleado.nombre.apellidos);

                            // Verificar si la entrada es una cadena vacía (solo Enter)
                            if (strcmp(empleado.nombre.nombres, "\n") == 0)
                            {
                                printf("Error: Debe ingresar al menos un nombre.\n");
                                goto inicio_nombre; // Volver al inicio del bucle para solicitar nuevamente la entrada
                            }
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
                        inicio_nombre:
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
                            fgets(empleado.cargo, sizeof(empleado.cargo), stdin);

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
                            fgets(empleado.nacionalidad, sizeof(empleado.nacionalidad), stdin);

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
                            printf("Guardando elementos...");
                            Sleep(5);
                            system("cls");
                            contarEmpleados(&contadorEmpleados);
                            printf("Se han annadido %d elementos a la base de datos exitosamente!", contadorEmpleados);
                        }
                        break;
                    inicio_opc_1:
                    }
                }
                else if (opc == '2')
                {
                    // Otras acciones para la opción '2' (si las hay)
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
                printf("Error: La lista de empleados está vacía.\n");
                Sleep(3);
                break; // Salir de la función, ya que no hay elementos para buscar
            }

            mostrarListaEmpleados(inicioLista);
            printf("Presiona cualquier tecla para volver al menu...\n");
            getch();
            system("cls");
            break;

        case '3':
            printf("\n ----> Menu \n");
            printf("         ----> Actualizar informacion de empleado \n\n");

            nodo *elemento = inicioLista;
            buscarEmpleado(elemento);

            if (elemento == NULL)
            {
                break;
            }

            system("cls");

            printf("- La busqueda coincide con el siguiente elemento: \n");
            printf("NRO: \tNombres: \t\tApellidos: \tCargo: \t\tNacionalidad: \n");
            // Imprimir la información del empleado del nodo elemento
            printf("%-s \t\t%-s \t\t%-s \t%-s \n", elemento->empleado.nombre.nombres, elemento->empleado.nombre.apellidos,
                   elemento->empleado.cargo, elemento->empleado.nacionalidad);

            printf("\n\n 1. Modificar nombres \n");
            printf("2. Modificar apellidos \n");
            printf("3. Modificar cargo \n");
            printf("4. Modificar nacionalidad \n");
            printf("5. Volver al menu \n");

            char a = getch();
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
            break;
        case '5':
            printf("Exportando datos...");
            guardarDatosCSV(inicioLista);
            Sleep(5);
            printf("Datos exportados correctamente");
            break;

        case '6':
            printf("Saliendo del sistema...");
            Sleep(5);
            liberarMemoriaLista(inicioLista);
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