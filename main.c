#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define VIP 800000
#define historial_texto "historial_de_compras.txt"
#define productos_binario "productos.bin"


typedef struct
{
    char calle[50];
    int altura;
    char piso[20];
} Direccion;

typedef struct
{
    Direccion direccion;
    char razon_social[50];
    char rubro[100];
    int cant_dias_sin_Comprar;
    float cantidad_facturacion;
    int cliente_vip;
} Clientes;

typedef struct
{
    char nombre[50];
    char descripcion[200];
    int stock;
    int vendidos;
    float precio;
} Productos;

struct Nodo_producto
{
    Productos datos;
    struct Nodo_producto *siguiente;
};

typedef struct
{
    struct Nodo_producto *ultimo;
    struct Nodo_producto *primero;
    int tam;
} Lista_enlazada_producto;

//Funciones
int imprimir_menu_principal();

void menu_admin(Lista_enlazada_producto * lista);
int imprimir_menu_admin();
void cargar_productos(Lista_enlazada_producto * lista);
void imprimir_producto( Productos producto, int i);


//Listas
void insertar_nodo_producto(Lista_enlazada_producto *lista, Productos *datos);

struct Nodo_producto *crear_nodo_producto(Productos *datos);
void listar_productos(Lista_enlazada_producto *lista);
void lista_productos_a_archivo(Lista_enlazada_producto *lista);
void archivo_a_lista_productos(Lista_enlazada_producto *lista);


void borrar_pantalla();
void frenar();
void cerrar_programa();
void invalida();
FILE * abrir_archivo(char * nombre_archivo, char * modo);
void vaciar_archivo(char *nombreArchivo);
int archivo_existe(char *nombreArchivo);

int main()
{
    int opcion;
    Lista_enlazada_producto *Lista_producto = (Lista_enlazada_producto *)calloc(1, sizeof(Lista_enlazada_producto));
    archivo_a_lista_productos(Lista_producto);
    do
    {
        borrar_pantalla();
        opcion = imprimir_menu_principal();
        switch (opcion)
        {
        case 1:
            //Menu clientes
            break;
        case 2:
            menu_admin(Lista_producto);
            break;
        case 3:
            cerrar_programa();
            break;
        default:
            invalida();
            break;
        }
    }while (opcion != 3);

    free(Lista_producto);
    return 0;
}


int imprimir_menu_principal()
{
    int opcion;
    printf("Menu de opciones:\n\n");
    printf("Opcion 1: Ingresar al menu de Clientes.\n");
    printf("Opcion 2: Ingresar al menu de Administradores\n");
    printf("Opcion 3: Salir del programa y cerrar la caja del dia.\n\n");
    printf("Ingresa la opcion deseada: ");
    fflush(stdin);
    scanf("%d", &opcion);
    return opcion;
}

void menu_admin(Lista_enlazada_producto * lista_datos_productos)
{
    int opcion;
    do
    {
        borrar_pantalla();
        opcion = imprimir_menu_admin();
        switch (opcion)
        {
            case 1:
            cargar_productos(lista_datos_productos);
            break;
            case 2:
            listar_productos(lista_datos_productos);
            break;
            case 3:
            //imprimir_historial_de_compras(texto);
            break;
            case 4:
            //imprimir_clientes(binario);
            break;
            case 5:
            //imprimir_clientes_vip(binario);
            break;
            case 6:
            //imprimir_clientes_sin_compras(binario);
            break;
            case 7:
            //printf("\n\nVolvemos al menu principal...\n\n");
            //tocar_tecla();
            break;
            default:
                invalida();
                break;
        }
    } while (opcion != 8);
}

int imprimir_menu_admin()
{
    int opcion;
    printf("\nMenu de opcions de los administradores:\n\n");
    printf("Opcion 1: Cargar Nuevos Productos.\n");
    printf("Opcion 2: Listar Productos.\n");
    printf("Opcion 3: Editar datos de algun cliente.\n");
    printf("Opcion 4: Imprimir historial de compras .\n");
    printf("Opcion 5: Imprimir lista de clientes.\n");
    printf("Opcion 6: Imprimir clientes vip.\n");
    printf("Opcion 7: Imprimir listado de las compras de los clientes.\n"); // cantidad de compras en X dias
    printf("Opcion 8: Volver al menu principal.\n\n");
    printf("Ingresa una opcion: ");
    fflush(stdin);
    scanf("%d", &opcion);
    return opcion;
}

void cargar_productos(Lista_enlazada_producto * lista)
{
    char opcion = 'S';
    Productos * producto = malloc(sizeof(Productos));

    while (toupper(opcion) == 'S')
    {
        borrar_pantalla();
        printf("Nombre del producto: ");
        fflush(stdin);
        gets(producto->nombre);
        printf("\nDescripcion del producto: ");
        fflush(stdin);
        gets(producto->descripcion);
        printf("\nPrecio del producto: ");
        fflush(stdin);
        scanf("%f", &producto->precio);
        printf("\nStock del producto: ");
        fflush(stdin);
        scanf("%d", &producto->stock);
        producto->vendidos=0;

        insertar_nodo_producto(lista, producto);

        printf("\n\nQuiere ingresar otro producto (S / N): ");
        fflush(stdin);
        scanf(" %c", &opcion);
    }
    lista_productos_a_archivo(lista);
}

void insertar_nodo_producto(Lista_enlazada_producto *lista, Productos *datos)
{
    struct Nodo_producto *nuevo_nodo = crear_nodo_producto(datos);
    if (lista->tam == 0) // Primera Insercion
    {
        lista->ultimo = nuevo_nodo;
        lista->primero = nuevo_nodo;
    }
    else
    {
        nuevo_nodo->siguiente = lista->ultimo;
        lista->ultimo = nuevo_nodo;
    }
    lista->tam++;
    return;
}

struct Nodo_producto *crear_nodo_producto(Productos *datos)
{
    struct Nodo_producto *nuevo_nodo = (struct Nodo_producto *)calloc(1, sizeof(struct Nodo_producto));

    if (nuevo_nodo == NULL)
    {
        printf("Insuficiente Memoria");
        exit(-1);
    }

    nuevo_nodo->datos = *datos;
    nuevo_nodo->siguiente = NULL;

    return nuevo_nodo;
}

void listar_productos(Lista_enlazada_producto *lista)
{
    borrar_pantalla();
    if (lista->tam == 0)
    {
        printf("\nNo hay Productos\n");
        frenar();
        return;
    }
    struct Nodo_producto *nodo_actual = lista->ultimo;

    for (int i = 0; i < lista->tam; i++)
    {
        imprimir_producto(nodo_actual->datos, i);
        nodo_actual = nodo_actual->siguiente;
    }
    printf("\nFin de los productos\n");
    frenar();
    borrar_pantalla();
    return;
}

void imprimir_producto( Productos producto, int i)
{
    printf("\n\nProducto N%d\n", i+1);
    printf("\nProducto: %s", producto.nombre);
    printf("\nDescripcion: %s", producto.descripcion);
    printf("\nStock: %d", producto.stock);
    printf("\nVendidos: %d", producto.vendidos);
    printf("\nPrecio: %.2f\n", producto.precio);
    frenar();
}

void borrar_pantalla()
{
    system("cls");
}

void frenar()
{
    printf("\n");
    system("pause");
    printf("\n");
}

void cerrar_programa()
{
    borrar_pantalla();
    printf("Usted a cerrado el programa, lo que implica que cerro la caja del local.\n");
    printf("Muchas gracias!!!\nNos vemos pronto....\n");
    frenar();
}

void invalida()
{
    printf("\nOpcion invalida. Intente nuevamente.\n");
    frenar();
}

FILE * abrir_archivo(char * nombre_archivo, char * modo)
{
    FILE *archivo=fopen(nombre_archivo, modo);
    if(archivo==NULL)
    {
        printf("\nERROR AL ABRIR EL ARCHIVO");
        exit(-1);
    }
    return archivo;
}

void lista_productos_a_archivo(Lista_enlazada_producto *lista)
{
    vaciar_archivo(productos_binario);
    FILE * archivo = abrir_archivo(productos_binario, "ab");
    struct Nodo_producto *nodo_actual = lista->ultimo;
    Productos prod;
    for (int i = 0; i < lista->tam; i++)
    {
        fwrite(&nodo_actual->datos, sizeof(Productos), 1, archivo);
        nodo_actual = nodo_actual->siguiente;
    }
    fclose(archivo);
}

void archivo_a_lista_productos(Lista_enlazada_producto *lista)
{
    if(archivo_existe(productos_binario) == 1)
    {
        FILE *archivo=abrir_archivo(productos_binario, "rb");
        Productos *producto = malloc(sizeof(Productos));
        fseek(archivo, 0, SEEK_SET);
        fread(producto, sizeof(Productos), 1, archivo);
        while(!feof(archivo))
        {
            insertar_nodo_producto(lista, producto);
            fread(producto, sizeof(Productos), 1, archivo);
        }
        free(producto);
    }
}

void vaciar_archivo(char *nombreArchivo)
{
    FILE *archivo=abrir_archivo(nombreArchivo, "w");
    fclose(archivo);
}

int archivo_existe(char *nombreArchivo)
{
    FILE *file;
    if (file = fopen(nombreArchivo, "r"))
    {
        fclose(file);
        return 1;
    }
    else
    {
        return 0;
    }
}
