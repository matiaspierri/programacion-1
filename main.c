#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define VIP 800000
#define productos_binario "productos.bin"
#define clientes_binario "clientes.bin"

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

struct Nodo_cliente
{
    Clientes datos;
    struct Nodo_cliente *siguiente;
};

typedef struct
{
    struct Nodo_cliente *cabeza;
    struct Nodo_cliente *cola;
    int tam;
} Lista_enlazada_cliente;

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
    struct Nodo_producto *cabeza;
    struct Nodo_producto *cola;
    int tam;
} Lista_enlazada_producto;

//Funciones de imprimir menu
int imprimir_menu_principal();
int imprimir_menu_admin();
int imprimir_menu_admin_clientes();
int imprimir_menu_admin_productos();
int imprimir_menu_admin_facturacion();

//Funciones acciones de menu
void menu_admin(Lista_enlazada_cliente * lista_datos_clientes, Lista_enlazada_producto * lista_datos_productos);
void menu_cliente(Lista_enlazada_cliente * lista_datos_clientes, Lista_enlazada_producto * lista_datos_productos);
void menu_admin_clientes(Lista_enlazada_cliente * lista_datos_clientes);
void menu_admin_productos(Lista_enlazada_producto * lista_datos_productos);


//Funciones pantalla clientes
void cargar_clientes(Lista_enlazada_cliente * lista);
void ingresar_cliente(Clientes * cliente);
void listar_clientes(Lista_enlazada_cliente *lista);
void imprimir_cliente( Clientes cliente, int i);
void modificar_cliente(Lista_enlazada_cliente * lista);
int buscar_cliente(Lista_enlazada_cliente * lista, char * mensaje);
void eliminar_cliente(Lista_enlazada_cliente * lista);
void eliminar_cliente_cabeza(Lista_enlazada_cliente * lista);
void eliminar_cliente_cola(Lista_enlazada_cliente * lista);
void eliminar_cliente_medio(Lista_enlazada_cliente * lista, int id);
void listar_clientes_vip(Lista_enlazada_cliente *lista);
void listar_clientes_sin_compra(Lista_enlazada_cliente * lista);

//Funciones lista clientes
void insertar_nodo_cliente(Lista_enlazada_cliente *lista, Clientes *datos);
struct Nodo_cliente *crear_nodo_cliente(Clientes *datos);
void lista_clientes_a_archivo(Lista_enlazada_cliente *lista);
void archivo_a_lista_clientes(Lista_enlazada_cliente *lista);

//Funciones pantalla productos
void cargar_productos(Lista_enlazada_producto * lista);
void imprimir_producto( Productos producto, int i);
void listar_productos(Lista_enlazada_producto *lista);
void modificar_producto(Lista_enlazada_producto * lista);
void aumentar_stock(Lista_enlazada_producto * lista);
void ingresar_producto(Productos * producto);
int buscar_producto(Lista_enlazada_producto * lista, char * mensaje);
void eliminar_producto(Lista_enlazada_producto * lista);
void eliminar_producto_cabeza(Lista_enlazada_producto * lista);
void eliminar_producto_cola(Lista_enlazada_producto * lista);
void eliminar_producto_medio(Lista_enlazada_producto * lista, int id);

//Lista productos
void insertar_nodo_producto(Lista_enlazada_producto *lista, Productos *datos);
struct Nodo_producto *crear_nodo_producto(Productos *datos);
void lista_productos_a_archivo(Lista_enlazada_producto *lista);
void archivo_a_lista_productos(Lista_enlazada_producto *lista);

//Funciones generales
void borrar_pantalla();
void frenar();
void cerrar_programa();
void invalida();
FILE * abrir_archivo(char * nombre_archivo, char * modo);
void vaciar_archivo(char *nombreArchivo);
int archivo_existe(char *nombreArchivo);
void aumentar_dias_sin_compra(Lista_enlazada_cliente * lista);

int main()
{
    int opcion;
    Lista_enlazada_cliente *Lista_cliente = (Lista_enlazada_cliente *)calloc(1, sizeof(Lista_enlazada_cliente));
    Lista_enlazada_producto *Lista_producto = (Lista_enlazada_producto *)calloc(1, sizeof(Lista_enlazada_producto));
    archivo_a_lista_productos(Lista_producto);
    archivo_a_lista_clientes(Lista_cliente);
    do
    {
        borrar_pantalla();
        opcion = imprimir_menu_principal();
        switch (opcion)
        {
        case 1:
            menu_cliente(Lista_cliente, Lista_producto);
            break;
        case 2:
            menu_admin(Lista_cliente, Lista_producto);
            break;
        case 3:
            aumentar_dias_sin_compra(Lista_cliente);
            cerrar_programa();
            break;
        default:
            invalida();
            break;
        }
    }while (opcion != 3);
    free(Lista_cliente);
    free(Lista_producto);
    return 0;
}

void menu_cliente(Lista_enlazada_cliente * lista_datos_clientes, Lista_enlazada_producto * lista_datos_productos)
{
    if (lista_datos_clientes->tam == 0)
    {
        printf("\nNo hay Clientes.\n");
        printf("Ingrese Clientes para poder cargar compras\n");
        frenar();
        return;
    }
    if (lista_datos_productos->tam == 0)
    {
        printf("\nNo hay Productos\n");
        printf("Ingrese Productos para poder cargar compras\n");
        frenar();
        return;
    }
    else
    {
        int id_cliente, id_producto, cantidad;
        char respuesta='S';
        printf("Lista de Clientes: ");
        listar_clientes(lista_datos_clientes);
        id_cliente = buscar_cliente(lista_datos_clientes, "Ingrese el ID del cliente a Realizar compra: ");
        struct Nodo_cliente *nodo_actual_cliente = lista_datos_clientes->cabeza;
        for(int i=0; i<id_cliente ; i++)
        {
            if(i+1==id_cliente)
            {
                while(toupper(respuesta)=='S')
                {
                    borrar_pantalla();
                    imprimir_cliente(nodo_actual_cliente->datos, i);
                    listar_productos(lista_datos_productos);
                    id_producto = buscar_producto(lista_datos_productos, "Ingrese el ID del producto a comprar: ");
                    struct Nodo_producto *nodo_actual_producto = lista_datos_productos->cabeza;

                    for(int i=0; i<id_producto ; i++)
                    {
                        if(i+1==id_producto)
                        {
                            printf("\nCuantas Unidades desea comprar: ");
                            scanf("%d", &cantidad);
                            if( cantidad <= nodo_actual_producto->datos.stock )
                            {
                                nodo_actual_producto->datos.stock = nodo_actual_producto->datos.stock - cantidad;
                                nodo_actual_producto->datos.vendidos += cantidad;
                                nodo_actual_cliente->datos.cant_dias_sin_Comprar=0;
                                if(nodo_actual_cliente->datos.cantidad_facturacion > VIP)
                                {
                                    nodo_actual_cliente->datos.cliente_vip=1;
                                    nodo_actual_cliente->datos.cantidad_facturacion += ((float)cantidad * (nodo_actual_producto->datos.precio)) * 0.9;
                                }
                                else
                                {
                                    nodo_actual_cliente->datos.cantidad_facturacion += (float)cantidad * (nodo_actual_producto->datos.precio);
                                }
                                printf("\nProducto comprado correctamente\n");
                            }
                            else
                            {
                                printf("\nStock Insuficiente\n");
                            }
                            break;
                        }
                        else
                        {
                            nodo_actual_producto = nodo_actual_producto->siguiente;
                        }
                    }
                    printf("\nDesea realizar otra compra S/N: ");
                    fflush(stdin);
                    scanf(" %c", &respuesta);
                }
                break;
            }
            else
            {
                nodo_actual_cliente = nodo_actual_cliente->siguiente;
            }
        }
        lista_clientes_a_archivo(lista_datos_clientes);
        lista_productos_a_archivo(lista_datos_productos);
    }
}

void menu_admin(Lista_enlazada_cliente * lista_datos_clientes, Lista_enlazada_producto * lista_datos_productos)
{
    int opcion=0;
    while (opcion != 4)
    {
        borrar_pantalla();
        opcion = imprimir_menu_admin();
        switch (opcion)
        {
        case 1:
            menu_admin_clientes(lista_datos_clientes);
            break;
        case 2:
            menu_admin_productos(lista_datos_productos);
            break;
        case 3:

            break;
        case 4:
            printf("\nSaliendo del Menu de Administradores\n");
            frenar();
            break;
        default:
            invalida();
            break;
        }
    }
}

void menu_admin_clientes(Lista_enlazada_cliente * lista_datos_clientes)
{
    int opcion=0;
    while (opcion != 7)
    {
        borrar_pantalla();
        opcion = imprimir_menu_admin_clientes();
        switch (opcion)
        {
        case 1:
            cargar_clientes(lista_datos_clientes);
            break;
        case 2:
            modificar_cliente(lista_datos_clientes);
            break;
        case 3:
            eliminar_cliente(lista_datos_clientes);
            break;
        case 4:
            listar_clientes(lista_datos_clientes);
            break;
        case 5:
            listar_clientes_vip(lista_datos_clientes);
            break;
        case 6:
            listar_clientes_sin_compra(lista_datos_clientes);
            break;
        case 7:
            printf("\nSaliendo del menu Admin de clientes\n");
            frenar();
            break;
        default:
            invalida();
            break;
        }
    }
}

void menu_admin_productos(Lista_enlazada_producto * lista_datos_productos)
{
    int opcion=0;
    while (opcion != 6)
    {
        borrar_pantalla();
        opcion = imprimir_menu_admin_productos();
        switch (opcion)
        {
        case 1:
            cargar_productos(lista_datos_productos);
            break;
        case 2:
            modificar_producto(lista_datos_productos);
            break;
        case 3:
            aumentar_stock(lista_datos_productos);
            break;
        case 4:
            eliminar_producto(lista_datos_productos);
            break;
        case 5:
            borrar_pantalla();
            listar_productos(lista_datos_productos);
            break;
        case 6:
            printf("\nSaliendo del menu Admin de Productos\n");
            frenar();
            break;
        default:
            invalida();
            break;
        }
    }
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

int imprimir_menu_admin()
{
    int opcion;
    printf("\nMenu de opciones de los administradores:\n\n");
    printf("Opcion 1: Clientes.\n");
    printf("Opcion 2: Productos.\n");
    printf("Opcion 3: Facturacion.\n");
    printf("Opcion 4: Volver al Menu de Inicio.\n");
    printf("Ingresa una opcion: ");
    fflush(stdin);
    scanf("%d", &opcion);
    return opcion;
}

int imprimir_menu_admin_clientes()
{
    int opcion;
    printf("\nClientes:\n\n");
    printf("Opcion 1: Cargar Clientes.\n");
    printf("Opcion 2: Modificar Cliente.\n");
    printf("Opcion 3: Borrar Cliente.\n");
    printf("Opcion 4: Listar Clientes.\n");
    printf("Opcion 5: Listar Clientes VIP.\n");
    printf("Opcion 6: Listar Clientes sin compras.\n");
    printf("Opcion 7: Volver al Menu admin.\n");
    printf("Ingresa una opcion: ");
    fflush(stdin);
    scanf("%d", &opcion);
    return opcion;
}

int imprimir_menu_admin_productos()
{
    int opcion;
    printf("\nProductos:\n\n");
    printf("Opcion 1: Cargar Productos.\n");
    printf("Opcion 2: Modificar Producto.\n");
    printf("Opcion 3: Aumentar Stock.\n");
    printf("Opcion 4: Borrar Producto.\n");
    printf("Opcion 5: Listar Productos.\n");
    printf("Opcion 6: Volver al Menu admin.\n");
    printf("Ingresa una opcion: ");
    fflush(stdin);
    scanf("%d", &opcion);
    return opcion;
}

int imprimir_menu_admin_facturacion()
{
    int opcion;
    printf("\nFacturacion:\n\n");
    printf("Opcion 1: Producto Mas Vendido.\n");
    printf("Opcion 2: Facturacion por Dia.\n");
    printf("Opcion 3: Facturacion de Hoy.\n");
    printf("Opcion 4: Facturacion Historica.\n");
    printf("Opcion 5: Volver al Menu admin.\n");
    printf("Ingresa una opcion: ");
    fflush(stdin);
    scanf("%d", &opcion);
    return opcion;
}

void cargar_clientes(Lista_enlazada_cliente * lista)
{
    char opcion = 'S';
    Clientes * cliente = malloc(sizeof(Clientes));

    while (toupper(opcion) == 'S')
    {
        borrar_pantalla();
        ingresar_cliente(cliente);
        cliente->cant_dias_sin_Comprar = -1;
        cliente->cantidad_facturacion = 0;
        cliente->cliente_vip = 0;
        insertar_nodo_cliente(lista, cliente);
        printf("\nQuiere ingresar otro Cliente (S / N): ");
        fflush(stdin);
        scanf(" %c", &opcion);
    }
    lista_clientes_a_archivo(lista);

}

void insertar_nodo_cliente(Lista_enlazada_cliente *lista, Clientes *datos)
{
    struct Nodo_cliente *nuevo_nodo = crear_nodo_cliente(datos);
    if (lista->tam == 0) // Primera Insercion
    {
        lista->cabeza = nuevo_nodo;
        lista->cola = nuevo_nodo;
    }
    else
    {
        lista->cola->siguiente = nuevo_nodo;
        lista->cola = nuevo_nodo;
    }
    lista->tam++;
    return;
}

struct Nodo_cliente *crear_nodo_cliente(Clientes *datos)
{
    struct Nodo_cliente *nuevo_nodo = (struct Nodo_cliente *)calloc(1, sizeof(struct Nodo_cliente));

    if (nuevo_nodo == NULL)
    {
        printf("Insuficiente Memoria");
        exit(-1);
    }

    nuevo_nodo->datos = *datos;
    nuevo_nodo->siguiente = NULL;

    return nuevo_nodo;
}

void ingresar_cliente(Clientes * cliente)
{
    printf("\nCargar datos del cliente:\n");
    printf("Razon social: ");
    fflush(stdin);
    gets(cliente->razon_social);
    printf("Rubro de la empresa: ");
    fflush(stdin);
    gets(cliente->rubro);
    printf("Direccion\n");
    printf("Calle: ");
    fflush(stdin);
    gets(cliente->direccion.calle);
    printf("Altura: ");
    scanf("%d", &cliente->direccion.altura);
    printf("Piso: ");
    fflush(stdin);
    gets(cliente->direccion.piso);
}

void listar_clientes(Lista_enlazada_cliente *lista)
{
    borrar_pantalla();
    if (lista->tam == 0)
    {
        printf("\nNo hay Clientes\n");
        frenar();
        return;
    }
    struct Nodo_cliente *nodo_actual = lista->cabeza;

    for (int i = 0; i < lista->tam; i++)
    {
        imprimir_cliente(nodo_actual->datos, i);
        //frenar();
        nodo_actual = nodo_actual->siguiente;
    }
    printf("\nFin de los clientes\n");
    frenar();
    return;
}

void imprimir_cliente( Clientes cliente, int i)
{
    printf("\nCliente N%d\n", i+1);
    printf("Razon Social: %s\n", cliente.razon_social);
    printf("Rubro: %s\n", cliente.rubro);
    printf("Cantidad de dias sin comprar: ");
    if(cliente.cant_dias_sin_Comprar==-1){ printf("Nunca compro\n"); } else { printf("%d\n", cliente.cant_dias_sin_Comprar); }
    printf("Cantidad Facturado: $%.2f\n", cliente.cantidad_facturacion);
    printf("Es VIP: ");
    if(cliente.cliente_vip==1) { printf("SI\n"); } else { printf("NO\n"); }
    printf("Direccion\n");
    printf("Calle: %s\n", cliente.direccion.calle);
    printf("Altura: %d\n", cliente.direccion.altura);
    printf("Piso: %s\n", cliente.direccion.piso);
}

void modificar_cliente(Lista_enlazada_cliente * lista)
{
    if (lista->tam == 0)
    {
        printf("\nNo hay Clientes\n");
        frenar();
        return;
    }
    else
    {
        int id;
        printf("Lista de Clientes: ");
        listar_clientes(lista);
        id = buscar_cliente(lista, "Ingrese el ID del cliente a editar: ");
        struct Nodo_cliente *nodo_actual = lista->cabeza;
        for(int i=0; i<id ; i++)
        {
            if(i+1==id)
            {
                imprimir_cliente(nodo_actual->datos, i);
                printf("\nIngrese los nuevos datos\n");
                ingresar_cliente(&nodo_actual->datos);
                lista_clientes_a_archivo(lista);
                printf("\Cliente actualizado correctamente\n");
                break;
            }
            else
            {
                nodo_actual = nodo_actual->siguiente;
            }
        }
        frenar();
    }
}

int buscar_cliente(Lista_enlazada_cliente * lista, char * mensaje)
{
    int id=lista->tam+1;
    while(id>(lista->tam))
    {
        printf("\n%s", mensaje);
        scanf("%d", &id);
        if(id>(lista->tam))
        {
            invalida();
        }
    }
    return id;
}

void eliminar_cliente(Lista_enlazada_cliente * lista)
{
    if (lista->tam == 0)
    {
        printf("\nNo hay Clientes\n");
        frenar();
        return;
    }
    else
    {
        int id;
        printf("Lista de Cliente: ");
        listar_clientes(lista);
        id = buscar_cliente(lista, "Ingrese el ID del Cliente a eliminar: ");
        if(id==1)
        {
            eliminar_cliente_cabeza(lista);
        }
        else if (id==lista->tam)
        {
            eliminar_cliente_cola(lista);
        }
        else
        {
            eliminar_cliente_medio(lista, id);
        }

        lista_clientes_a_archivo(lista);
        printf("\nCliente borrado correctamente");
        frenar();
    }
}

void eliminar_cliente_cabeza(Lista_enlazada_cliente * lista)
{
    struct Nodo_cliente *segundo_nodo = lista->cabeza->siguiente;
    lista->cabeza = segundo_nodo;
    lista->tam--;
    return;
}

void eliminar_cliente_cola(Lista_enlazada_cliente * lista)
{
    struct Nodo_cliente *nodo_actual = lista->cabeza;
    for (int i = 1; i < lista->tam; i++)
    {
        nodo_actual = nodo_actual->siguiente;
    }
    nodo_actual->siguiente = NULL;
    lista->cola = nodo_actual;
    lista->tam--;
    return;
}

void eliminar_cliente_medio(Lista_enlazada_cliente * lista, int id)
{
    struct Nodo_cliente *nodo_actual = lista->cabeza;
    struct Nodo_cliente *nodo_anterior = NULL;
    for (int i = 1; i < id; i++)
    {
        nodo_anterior = nodo_actual;
        nodo_actual = nodo_actual->siguiente;
    }
    nodo_anterior->siguiente = nodo_actual->siguiente;
    lista->tam--;
    return;
}

void listar_clientes_vip(Lista_enlazada_cliente *lista)
{
    int c=0;
    borrar_pantalla();
    if (lista->tam == 0)
    {
        printf("\nNo hay Clientes\n");
        frenar();
        return;
    }
    struct Nodo_cliente *nodo_actual = lista->cabeza;
    printf("\nLista clientes VIP\n");
    for (int i = 0; i < lista->tam; i++)
    {
        if(nodo_actual->datos.cliente_vip==1)
        {
            c=1;
            imprimir_cliente(nodo_actual->datos, i);
        }
        nodo_actual = nodo_actual->siguiente;
    }
    if(c==1)
    {
        printf("\nFin de los clientes\n");
    }
    else
    {
        printf("\nNo hay clientes VIP\n");
    }
    frenar();
    return;
}

void listar_clientes_sin_compra(Lista_enlazada_cliente *lista)
{
    int c=0;
    borrar_pantalla();
    if (lista->tam == 0)
    {
        printf("\nNo hay Clientes\n");
        frenar();
        return;
    }
    struct Nodo_cliente *nodo_actual = lista->cabeza;
    printf("\nLista clientes Sin Compras\n");
    for (int i = 0; i < lista->tam; i++)
    {
        if(nodo_actual->datos.cant_dias_sin_Comprar==-1)
        {
            c=1;
            imprimir_cliente(nodo_actual->datos, i);
        }
        nodo_actual = nodo_actual->siguiente;
    }
    if(c==1)
    {
        printf("\nFin de los clientes\n");
    }
    else
    {
        printf("\nNo hay clientes sin comprar\n");
    }
    frenar();
    return;
}

void lista_clientes_a_archivo(Lista_enlazada_cliente *lista)
{
    vaciar_archivo(clientes_binario);
    FILE * archivo = abrir_archivo(clientes_binario, "ab");
    struct Nodo_cliente *nodo_actual = lista->cabeza;
    for (int i = 0; i < lista->tam; i++)
    {
        fwrite(&nodo_actual->datos, sizeof(Clientes), 1, archivo);
        nodo_actual = nodo_actual->siguiente;
    }
    fclose(archivo);
}

void archivo_a_lista_clientes(Lista_enlazada_cliente *lista)
{
    if(archivo_existe(clientes_binario) == 1)
    {
        FILE *archivo=abrir_archivo(clientes_binario, "rb");
        Clientes *cliente = malloc(sizeof(Clientes));
        fseek(archivo, 0, SEEK_SET);
        fread(cliente, sizeof(Clientes), 1, archivo);
        while(!feof(archivo))
        {
            insertar_nodo_cliente(lista, cliente);
            fread(cliente, sizeof(Clientes), 1, archivo);
        }
        free(cliente);
    }
}

void cargar_productos(Lista_enlazada_producto * lista)
{
    char opcion = 'S';
    Productos * producto = malloc(sizeof(Productos));

    while (toupper(opcion) == 'S')
    {
        borrar_pantalla();
        ingresar_producto(producto);
        producto->vendidos=0;
        insertar_nodo_producto(lista, producto);
        printf("\n\nQuiere ingresar otro producto (S / N): ");
        fflush(stdin);
        scanf(" %c", &opcion);
    }
    free(producto);
    lista_productos_a_archivo(lista);
}

void ingresar_producto(Productos * producto)
{
    printf("Nombre del producto: ");
    fflush(stdin);
    gets(producto->nombre);
    printf("Descripcion del producto: ");
    fflush(stdin);
    gets(producto->descripcion);
    printf("Precio del producto: ");
    fflush(stdin);
    scanf("%f", &producto->precio);
    printf("Stock del producto: ");
    fflush(stdin);
    scanf("%d", &producto->stock);
}

void listar_productos(Lista_enlazada_producto *lista)
{
    printf("\nLista de Productos\n");
    if (lista->tam == 0)
    {
        printf("\nNo hay Productos\n");
        frenar();
        return;
    }
    struct Nodo_producto *nodo_actual = lista->cabeza;

    for (int i = 0; i < lista->tam; i++)
    {
        imprimir_producto(nodo_actual->datos, i);
        //frenar();
        nodo_actual = nodo_actual->siguiente;
    }
    printf("\nFin de los productos\n");
    frenar();
    return;
}

void imprimir_producto( Productos producto, int i)
{
    printf("\nProducto N%d", i+1);
    printf("\nProducto: %s", producto.nombre);
    printf("\nDescripcion: %s", producto.descripcion);
    printf("\nStock: %d", producto.stock);
    printf("\nVendidos: %d", producto.vendidos);
    printf("\nPrecio: %.2f\n", producto.precio);
}

void modificar_producto(Lista_enlazada_producto * lista)
{
    if (lista->tam == 0)
    {
        printf("\nNo hay Productos\n");
        frenar();
        return;
    }
    else
    {
        int id;
        borrar_pantalla();
        listar_productos(lista);
        id = buscar_producto(lista, "Ingrese el ID del producto a editar: ");
        struct Nodo_producto *nodo_actual = lista->cabeza;
        for(int i=0; i<id ; i++)
        {
            if(i+1==id)
            {
                imprimir_producto(nodo_actual->datos, i);
                printf("\nIngrese los nuevos datos\n");
                ingresar_producto(&nodo_actual->datos);
                lista_productos_a_archivo(lista);
                printf("\nProducto actualizado correctamente\n");
                break;
            }
            else
            {
                nodo_actual = nodo_actual->siguiente;
            }
        }
        frenar();
    }
}

void aumentar_stock(Lista_enlazada_producto * lista)
{
    if (lista->tam == 0)
    {
        printf("\nNo hay Productos\n");
        frenar();
        return;
    }
    else
    {
        int id, unidades;
        borrar_pantalla();
        listar_productos(lista);
        id = buscar_producto(lista, "Ingrese el ID del producto a aumentar stock: ");
        struct Nodo_producto *nodo_actual = lista->cabeza;
        for(int i=0; i<id ; i++)
        {
            if(i+1==id)
            {
                printf("Cuantas unidades desea aumentar en el Stock: ");
                scanf("%d", &unidades);
                nodo_actual->datos.stock+=unidades;
                lista_productos_a_archivo(lista);
                printf("\nStock actualizado correctamente\n");
                break;
            }
            else
            {
                nodo_actual = nodo_actual->siguiente;
            }
        }
        frenar();
    }
}

void eliminar_producto(Lista_enlazada_producto * lista)
{
    if (lista->tam == 0)
    {
        printf("\nNo hay Productos\n");
        frenar();
        return;
    }
    else
    {
        int id;
        borrar_pantalla();
        listar_productos(lista);
        id = buscar_producto(lista, "Ingrese el ID del producto a eliminar: ");
        if(id==1)
        {
            eliminar_producto_cabeza(lista);
        }
        else if (id==lista->tam)
        {
            eliminar_producto_cola(lista);
        }
        else
        {
            eliminar_producto_medio(lista, id);
        }

        lista_productos_a_archivo(lista);
        printf("\nProducto borrado correctamente");
        frenar();
    }
}

void eliminar_producto_cabeza(Lista_enlazada_producto * lista)
{
    struct Nodo_producto *segundo_nodo = lista->cabeza->siguiente;

    lista->cabeza = segundo_nodo;
    lista->tam--;
    return;
}

void eliminar_producto_cola(Lista_enlazada_producto * lista)
{
    struct Nodo_producto *nodo_actual = lista->cabeza;
    for (int i = 1; i < lista->tam; i++)
    {
        nodo_actual = nodo_actual->siguiente;
    }
    nodo_actual->siguiente = NULL;
    lista->cola = nodo_actual;
    lista->tam--;
    return;
}

void eliminar_producto_medio(Lista_enlazada_producto * lista, int id)
{
    struct Nodo_producto *nodo_actual = lista->cabeza;
    struct Nodo_producto *nodo_anterior = NULL;
    for (int i = 1; i < id; i++)
    {
        nodo_anterior = nodo_actual;
        nodo_actual = nodo_actual->siguiente;
    }
    nodo_anterior->siguiente = nodo_actual->siguiente;
    lista->tam--;
    return;
}

int buscar_producto(Lista_enlazada_producto * lista, char * mensaje)
{
    int id=lista->tam+1;
    while(id>(lista->tam))
    {
        printf("\n%s", mensaje);
        scanf("%d", &id);
        if(id>(lista->tam))
        {
            invalida();
        }
    }
    return id;
}

void insertar_nodo_producto(Lista_enlazada_producto *lista, Productos *datos)
{
    struct Nodo_producto *nuevo_nodo = crear_nodo_producto(datos);
    if (lista->tam == 0) // Primera Insercion
    {
        lista->cabeza = nuevo_nodo;
        lista->cola = nuevo_nodo;
    }
    else
    {
        lista->cola->siguiente = nuevo_nodo;
        lista->cola = nuevo_nodo;
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

void borrar_pantalla()
{
    system("cls");
}

void frenar()
{
    printf("\n");
    system("pause");
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
    struct Nodo_producto *nodo_actual = lista->cabeza;
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

void aumentar_dias_sin_compra(Lista_enlazada_cliente * lista)
{
    struct Nodo_cliente *nodo_actual = lista->cabeza;
    for (int i = 0; i < lista->tam; i++)
    {
        if(nodo_actual->datos.cant_dias_sin_Comprar!=-1)
        {
            nodo_actual->datos.cant_dias_sin_Comprar++;
        }
        nodo_actual = nodo_actual->siguiente;
    }
    lista_clientes_a_archivo(lista);
    return;
}


