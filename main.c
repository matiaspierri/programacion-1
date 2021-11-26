#include <stdio.h>
#include <stdlib.h>


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

void menu_admin();
int imprimir_menu_admin();
void cargar_productos(char * archivo);

void borrar_pantalla();
void frenar();
void cerrar_programa();
void invalida();
FILE * abrir_archivo(char * nombre_archivo, char * modo);

int main()
{
    int opcion;
    Lista_enlazada_producto *Lista_producto = (Lista_enlazada_producto *)calloc(1, sizeof(Lista_enlazada_producto));
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
            menu_admin();
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

void menu_admin()
{
    int opcion;
    do
    {
        borrar_pantalla();
        opcion = imprimir_menu_admin();
        switch (opcion)
        {
            case 1:
            cargar_productos(productos_binario);
            break;
            case 2:
            //editar_cliente(products);
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
    } while (opcion != 7);
}

int imprimir_menu_admin()
{
    int opcion;
    printf("\nMenu de opcions de los administradores:\n");
    printf("\nOpcion 1: Cargar Nuevos Productos.\n");
    printf("Opcion 2: Editar datos de algun cliente.\n");
    printf("Opcion 3: Imprimir historial de compras .\n");
    printf("Opcion 4: Imprimir lista de clientes.\n");
    printf("Opcion 5: Imprimir clientes vip.\n");
    printf("Opcion 6: Imprimir listado de las compras de los clientes.\n"); // cantidad de compras en X dias
    printf("Opcion 7: Volver al menu principal.\n\n");
    printf("Ingresa una opcion: ");
    fflush(stdin);
    scanf("%d", &opcion);
    return opcion;
}

void cargar_productos(char * archivo)
{
    FILE * prod = abrir_archivo(archivo, "r");
    char opcion = 'S';
    Productos * productos = malloc(sizeof(Productos));
    Productos * producto = productos;
    int contador = 1;

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
        printf("\n\nQuiere ingresar otro producto (S / N): ");
        fflush(stdin);
        scanf(" %c", &opcion);

        /*if (toupper(opcion) == 'S')
        {
            contador++;
            Productos * variable_temporal = realloc(productos, sizeof(Productos) * contador);

            if (variable_temporal == NULL)
            {
                printf("\n\nError al realocar memoria. Revise el programa....\n");
                touch_key();
                exit(-1);
            }

            productos = variable_temporal;
            producto = variable_temporal + contador - 1;
        }*/
    }

    fwrite(productos, sizeof(Productos), contador, prod);
    free(productos);
    fclose(prod);
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
