#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define VIP 800000
#define productos_binario "productos.bin"
#define clientes_binario "clientes.bin"
#define tickets_binario "tickets.bin"
#define fecha_binario "fecha.bin"

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

typedef struct
{
    int anio;
    int mes;
    int dia;
    int hora;
    int minuto;
    int segundo;
}Fecha;

typedef struct
{
    char cliente[100];
    char producto[100];
    Fecha fecha;
    float monto;
    int pago_vip;
}Tickets;

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
void menu_admin_facturacion(Lista_enlazada_producto * lista_datos_producto);

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
void listar_clientes_sin_compra_fecha(Lista_enlazada_cliente * lista);
void listar_clientes_sin_compra(Lista_enlazada_cliente *lista);


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

//Funciones pantalla Facturacion
void producto_mas_vendido(Lista_enlazada_producto * lista);
void facturacion_hoy();
void facturacion_por_dia();
void facturacion_historica();


//Funciones generales
void borrar_pantalla();
void frenar();
void cerrar_programa();
void invalida();
FILE * abrir_archivo(char * nombre_archivo, char * modo);

int archivo_existe(char *nombreArchivo);
void aumentar_dias_sin_compra(Lista_enlazada_cliente * lista);
void imprimir_ticket(Tickets * datos);
int diferencia_entre_fecha( Fecha * fecha1, Fecha * fecha2);


int main()
{
    int opcion;

    // Creo las listas enlazadas para guardar clientes y productos
    Lista_enlazada_cliente *Lista_cliente = (Lista_enlazada_cliente *)calloc(1, sizeof(Lista_enlazada_cliente));
    Lista_enlazada_producto *Lista_producto = (Lista_enlazada_producto *)calloc(1, sizeof(Lista_enlazada_producto));
    
    // Leo los archivos binarios y cargo las listas
    archivo_a_lista_productos(Lista_producto);
    archivo_a_lista_clientes(Lista_cliente);

    do
    {
        // Accedo a la fecha de la PC y aumento la fecha
        aumentar_dias_sin_compra(Lista_cliente);


        borrar_pantalla();
        
        // Muestro al usuario el menu principal y guardo su opcion
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
            cerrar_programa();
            break;
        default:
            invalida();
            break;
        }
    }while (opcion != 3);

    // Libero memoria
    free(Lista_cliente);
    free(Lista_producto);
    return 0;
}

// Menu de cliente
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
        int id_cliente, id_producto, cantidad=0, contador = 1;
        float facturacion_por_venta = 0;
        char respuesta='S';
        printf("Lista de Clientes: ");
        listar_clientes(lista_datos_clientes);
        id_cliente = buscar_cliente(lista_datos_clientes, "Ingrese el ID del cliente a Realizar compra: ");
        struct Nodo_cliente *nodo_actual_cliente = lista_datos_clientes->cabeza;

        // Iniciamos al compra
        for(int i=0; i<id_cliente ; i++)
        {
            if(i+1==id_cliente)
            {
                Tickets * tickets = malloc(sizeof(Tickets));
                Tickets * ticket = tickets;
                FILE * archivo = abrir_archivo(tickets_binario, "ab");

                while(toupper(respuesta)=='S')
                {
                    borrar_pantalla();
                    imprimir_cliente(nodo_actual_cliente->datos, i);
                    
                    // Mostramos los productos para comprar
                    listar_productos(lista_datos_productos);

                    // Guardamos la eleccion del cliente
                    id_producto = buscar_producto(lista_datos_productos, "Ingrese el ID del producto a comprar: ");
                    struct Nodo_producto *nodo_actual_producto = lista_datos_productos->cabeza;

                    for(int i=0; i<id_producto ; i++)
                    {
                        if(i+1==id_producto)
                        {
                            do
                            {
                                // Preguntamos cuantas unidades desea comprar para saber si tenemos stock
                                printf("\nCuantas Unidades desea comprar: ");
                                scanf("%d", &cantidad);
                                if(cantidad>nodo_actual_producto->datos.stock)
                                {
                                    printf("\nStock Insuficiente\n");
                                }
                            }while( cantidad > nodo_actual_producto->datos.stock );
                            

                            // Guardamos la venta 
                            nodo_actual_producto->datos.stock = nodo_actual_producto->datos.stock - cantidad;
                            nodo_actual_producto->datos.vendidos += cantidad;
                            nodo_actual_cliente->datos.cant_dias_sin_Comprar=0;

                            // Si el cliente es VIP le ofrecemos el descuento correspondiente y calculamos
                            // el monto total
                            if(nodo_actual_cliente->datos.cantidad_facturacion > VIP)
                            {
                                nodo_actual_cliente->datos.cliente_vip=1;
                                facturacion_por_venta = ((float)cantidad * (nodo_actual_producto->datos.precio)) * 0.9;
                            }
                            else
                            {
                                facturacion_por_venta = (float)cantidad * (nodo_actual_producto->datos.precio);
                            }
                            nodo_actual_cliente->datos.cantidad_facturacion += facturacion_por_venta;
                            printf("\nProducto comprado correctamente\n");

                            // Guardamos la fecha y hora local
                            time_t t;
                            struct tm *info_tiempo;
                            time(&t);
                            info_tiempo = localtime(&t);

                            // Copiamos dentro del ticket el producto y cliente
                            strcpy(ticket->cliente, nodo_actual_cliente->datos.razon_social);
                            strcpy(ticket->producto, nodo_actual_producto->datos.nombre);

                            // Guardamos el monto
                            ticket->monto = facturacion_por_venta;
                            ticket->pago_vip = nodo_actual_cliente->datos.cliente_vip;
                            
                            // Guardamos la fecha
                            ticket->fecha.anio = info_tiempo->tm_year+1900;
                            ticket->fecha.mes = info_tiempo->tm_mon+1;
                            ticket->fecha.dia = info_tiempo->tm_mday;
                            ticket->fecha.hora = info_tiempo->tm_hour;
                            ticket->fecha.minuto = info_tiempo->tm_min;
                            ticket->fecha.segundo = info_tiempo->tm_sec;

                            printf("\n**********Su ticket**********\n");
                            imprimir_ticket(ticket);
                        }
                        else
                        {
                            nodo_actual_producto = nodo_actual_producto->siguiente;
                        }
                    }

                    printf("\nDesea realizar otra compra S/N: ");
                    fflush(stdin);
                    scanf(" %c", &respuesta);

                    if(toupper(respuesta)=='S')
                    {
                        contador++;
                        Tickets * variable_temporal = realloc(tickets, sizeof(Tickets) * contador);
                        if (variable_temporal == NULL)
                        {
                            printf("\n\nError al realocar memoria. Revise el programa.\n\n");
                            frenar();
                            exit(-1);
                        }
                        tickets = variable_temporal;
                        ticket = variable_temporal + contador - 1;
                    }
                }
                fwrite(tickets , sizeof(Tickets), contador, archivo);
                free(tickets);
                fclose(archivo);
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

// Funcionar para hacer un print de un ticket
void imprimir_ticket(Tickets * datos)
{
    printf("\nNombre del Cliente: %s\n", datos->cliente);
    printf("Producto: %s\n", datos->producto);
    printf("Monto: %.2f\n", datos->monto);
    printf("Pago con descuento: ");
    if(datos->pago_vip==1){printf("SI\n");}else{printf("NO\n");}
    printf("Fecha: %04d/%02d/%02d\n", datos->fecha.anio, datos->fecha.mes, datos->fecha.dia);
    printf("Hora: %02d/%02d/%02d\n", datos->fecha.hora, datos->fecha.minuto, datos->fecha.segundo);
}

// Menu genreral
void menu_admin(Lista_enlazada_cliente * lista_datos_clientes, Lista_enlazada_producto * lista_datos_productos)
{
    int opcion=0;
    while (opcion != 4)
    {
        aumentar_dias_sin_compra(lista_datos_clientes);
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
            menu_admin_facturacion(lista_datos_productos);
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

// Menu principal de cleintes
void menu_admin_clientes(Lista_enlazada_cliente * lista_datos_clientes)
{
    int opcion=0;
    while (opcion != 7)
    {
        aumentar_dias_sin_compra(lista_datos_clientes);
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
            listar_clientes_sin_compra_fecha(lista_datos_clientes);
            break;
        case 7:
            listar_clientes_sin_compra(lista_datos_clientes);
            break;

        case 8:
            printf("\nSaliendo del menu Admin de clientes\n");
            frenar();
            break;
        default:
            invalida();
            break;
        }
    }
}

// Menu principal de productos
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

// Menu principal de facturacion
void menu_admin_facturacion(Lista_enlazada_producto * lista_datos_producto)
{
    int opcion=0;
    while (opcion != 5)
    {
        borrar_pantalla();
        opcion = imprimir_menu_admin_facturacion();
        switch (opcion)
        {
        case 1:
            producto_mas_vendido(lista_datos_producto);
            break;
        case 2:
            facturacion_por_dia();
            break;
        case 3:
            facturacion_hoy();
            break;
        case 4:
            facturacion_historica();
            break;
        case 5:
            printf("\nSaliendo del menu Admin de Facturacion\n");
            frenar();
            break;
        default:
            invalida();
            break;
        }
    }
}

// Imprimos el menu y retornamos la opcion elegida
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

// Imprimos el menu y retornamos la opcion elegida
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

// Imprimos el menu y retornamos la opcion elegida
int imprimir_menu_admin_clientes()
{
    int opcion;
    printf("\nClientes:\n\n");
    printf("Opcion 1: Cargar Clientes.\n");
    printf("Opcion 2: Modificar Cliente.\n");
    printf("Opcion 3: Borrar Cliente.\n");
    printf("Opcion 4: Listar Clientes.\n");
    printf("Opcion 5: Listar Clientes VIP.\n");
    printf("Opcion 6: Listar Clientes sin compras segun fecha.\n");
    printf("Opcion 7: Listar Clientes sin compras.\n");
    printf("Opcion 8: Volver al Menu admin.\n");
    printf("Ingresa una opcion: ");
    fflush(stdin);
    scanf("%d", &opcion);
    return opcion;
}

// Imprimos el menu y retornamos la opcion elegida
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

// Imprimos el menu y retornamos la opcion elegida
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

/*
    Ingresamos los datos del cliente hasta que el usuario
    nos diga que no desea cargar nada mas
*/
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

// Insertamos el nodo dentro de la lista 
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

// Creamos un nuevo nodo y guaradmos los datos de la estructura cliente dentro
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

// Guardamos los datos del cliente
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

// Funcion para recorrer los clientes y hacer un print
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


// Funcion para hacer un print del cliente
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

/*
    Ya que ID==Posicion en lista, recorremos la lista hasta llegar al
    ID que inserto el usuario
*/
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

/* 
    Buscamos el cliente que el usuario quiere borrar y 
    dependiendo de donde se encuentre en la lista decidimos como borrarlo
*/
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

/*
    Recorremos la lista de cliente y mostramos en pantalla
    los que tengan facturacion == 800.000
*/
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


/* 
    Listamos los clientes que no tienen comprar segun
    la cantidad de dias que quiere ver el usuario
*/
void listar_clientes_sin_compra_fecha(Lista_enlazada_cliente *lista)
{
    int c=0, dias;
    borrar_pantalla();
    if (lista->tam == 0)
    {
        printf("\nNo hay Clientes\n");
        frenar();
        return;
    }
    struct Nodo_cliente *nodo_actual = lista->cabeza;
    printf("Ingrese la cantidad minima dias sin comprar a buscar: ");
    scanf("%d", &dias);
    printf("\nLista clientes Sin Comprar\n");
    for (int i = 0; i < lista->tam; i++)
    {
        if(nodo_actual->datos.cant_dias_sin_Comprar>dias)
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

/* 
    Recorremos la lista de clientes y vamos guardando la estructura
    de cada cliente, sin guardar el nodo
*/
void lista_clientes_a_archivo(Lista_enlazada_cliente *lista)
{
    
    FILE * archivo = abrir_archivo(clientes_binario, "wb");
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
    // Abro el archivo dew clientes
    if(archivo_existe(clientes_binario) == 1)
    {

        /*
            Recorro el archivo de clientes y voy guardando el dato de cada
            estructura para luego insertarla en la lista
        */

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

/* 
    Pedimos al usuario los datos del producto y luego
    lo insertamos en un nodo de la lista
    En caso de que el usuario quiera, repetimos este proceso
    para cargar todos los productos necesarios
*/
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

// Solicitamos los datos del producto
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

// Recorremos la lista de productos y la imprimimos
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

// Funcion para hacer un print de producto
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
    // Verifico si no hay productos
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

        // Pregunto al usuario que producto quiere modificar
        id = buscar_producto(lista, "Ingrese el ID del producto a editar: ");
        struct Nodo_producto *nodo_actual = lista->cabeza;

        // Recorro la lista hasta llegar al producto
        for(int i=0; i<id ; i++)
        {
            if(i+1==id)
            {
                // Muestro el producto
                imprimir_producto(nodo_actual->datos, i);
                
                printf("\nIngrese los nuevos datos\n");
                ingresar_producto(&nodo_actual->datos);
                
                // Persisto los datos en el archivo
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
    // Verifico que haya productos
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
        
        // Guardo que producto se quiere modificar
        id = buscar_producto(lista, "Ingrese el ID del producto a aumentar stock: ");
        

        // Recorro la lista hasta llegar al producto seleccionado
        struct Nodo_producto *nodo_actual = lista->cabeza;
        for(int i=0; i<id ; i++)
        {
            if(i+1==id)
            {
                printf("\nCuantas unidades desea aumentar en el Stock: ");
                scanf("%d", &unidades);

                // Aumento el stock y lo persisto en el archivo
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
    // Verifico si hay productos en la lista
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

        // Notese que ID = Posicion en la lista

        // Si queremos eliminar la cabeza 
        if(id==1)
        {
            eliminar_producto_cabeza(lista);
        
        }
        // Si queremos eliminar la cola
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

// Funcion para eliminar un nodo en caso de estar en la cabeza
void eliminar_producto_cabeza(Lista_enlazada_producto * lista)
{
    struct Nodo_producto *segundo_nodo = lista->cabeza->siguiente;

    lista->cabeza = segundo_nodo;
    lista->tam--;
    return;
}


// Funcion para eliminar un nodo en caso de estar en la cola
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


// Funcion para eliminar un nodo en caso de estar en el medio
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
    // Creo un nodo con los datos
    struct Nodo_producto *nuevo_nodo = crear_nodo_producto(datos);
    
    
    /* Realizo la insercion del nodo*/
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

    // Aumento el tamaño de la lista
    lista->tam++;
    return;
}

struct Nodo_producto *crear_nodo_producto(Productos *datos)
{

    // Creo el nuevo nodo
    struct Nodo_producto *nuevo_nodo = (struct Nodo_producto *)calloc(1, sizeof(struct Nodo_producto));

    if (nuevo_nodo == NULL)
    {
        printf("Insuficiente Memoria");
        exit(-1);
    }
    
    // Guardo los datos en el nuevo nodo 
    nuevo_nodo->datos = *datos;
    nuevo_nodo->siguiente = NULL;

    // Devuelvo el nodo
    return nuevo_nodo;  
}

void producto_mas_vendido(Lista_enlazada_producto * lista)
{
    borrar_pantalla();
    int max=0, id=0, mayor=0;
    
    // Verifico si hay productos
    if (lista->tam == 0)
    {
        printf("\nNo hay Productos\n");
        frenar();
        return;
    }
    struct Nodo_producto *nodo_actual = lista->cabeza;

    // Recorro la lista de productos
    for (int i = 0; i < lista->tam; i++)
    {

        // Verifico si el valor de vendidos del nodo actual es mayor a mi maximo
        if( max<nodo_actual->datos.vendidos && nodo_actual->datos.vendidos!=0 )
        {
            mayor=1;
            id=i;

            // Guardo el nuevo maximo
            max=nodo_actual->datos.vendidos;
        }
        nodo_actual = nodo_actual->siguiente;
    }

    struct Nodo_producto *nodo_actual2 = lista->cabeza;

    
    if(mayor==1)
    {
        for (int j = 0; j < id; j++)
        {
            nodo_actual2 = nodo_actual2->siguiente;
        }
        printf("\nEl producto mas vendido es \n");
        imprimir_producto(nodo_actual2->datos, id);
    }
    else
    {
        printf("\nNo se han realizado ventas\n");
    }

    free(nodo_actual2);
    free(nodo_actual);
    frenar();
    return;
}

void facturacion_historica()
{
    // Si existe el archivo de tickets
    if(archivo_existe(tickets_binario) == 1)
    {
        borrar_pantalla();
        int i=0;
        float facturacion=0;

        // Recorremos el archivo de tickets
        FILE *archivo=abrir_archivo(tickets_binario, "rb");
        Tickets *ticket = malloc(sizeof(Tickets));
        fseek(archivo, 0, SEEK_SET);
        fread(ticket, sizeof(Tickets), 1, archivo);
        while(!feof(archivo))
        {
            i++;
            printf("\nTicket N%d\n", i);
            imprimir_ticket(ticket);

            // sumamos la facturacion
            facturacion+=ticket->monto;
            fread(ticket, sizeof(Tickets), 1, archivo);
        }

        // Imprimimos la facturacion historica
        printf("\nLa facturacion historica de la empresa es: %.2f\n", facturacion);
        frenar();
        free(ticket);
    } else
    {
        printf("\nNo hay compras registradas\n");
        frenar();
    }
}

void facturacion_por_dia()
{
    // Si existe el binario de tickets
    if(archivo_existe(tickets_binario) == 1)
    {
        borrar_pantalla();
        int i=0, dia, mes, anio;
        float facturacion=0;

        // Pedimos al usuario a partir de que fecha queremos calcular
        printf("\nIngrese el anio: ");
        scanf("%d", &anio);
        printf("\nIngrese el mes: ");
        scanf("%d", &mes);
        printf("\nIngrese el dia: ");
        scanf("%d", &dia);


        // Recorremos el archivo de tickets
        FILE *archivo=abrir_archivo(tickets_binario, "rb");
        Tickets *ticket = malloc(sizeof(Tickets));
        fseek(archivo, 0, SEEK_SET);
        fread(ticket, sizeof(Tickets), 1, archivo);
        while(!feof(archivo))
        {
            i++;

            // Si la fecha del ticket concuerda con lo que pidio el usuario
            if(ticket->fecha.anio==anio && ticket->fecha.mes==mes && ticket->fecha.dia==dia)
            {
                printf("\nTicket N%d\n", i);
                imprimir_ticket(ticket);
                facturacion+=ticket->monto;
            }
            fread(ticket, sizeof(Tickets), 1, archivo);
        }
        printf("\nLa facturacion del dia %d/%02d/%02d de la empresa es: %.2f\n", anio, mes, dia, facturacion);
        frenar();
        free(ticket);
    } else
    {
        printf("\nNo hay compras registradas\n");
        frenar();
    }
}

void facturacion_hoy()
{  
    // Si existe el binario de tickets
    if(archivo_existe(tickets_binario) == 1)
    {
        borrar_pantalla();
        int i=0;
        float facturacion=0;
        time_t t;
        struct tm *info_tiempo;
        time(&t);

        // Obtenemos la fecha de hoy
        info_tiempo = localtime(&t);
        FILE *archivo=abrir_archivo(tickets_binario, "rb");


        // Empezamos a recorrer el binario de tickets
        Tickets *ticket = malloc(sizeof(Tickets));
        fseek(archivo, 0, SEEK_SET);
        fread(ticket, sizeof(Tickets), 1, archivo);
        while(!feof(archivo))
        {
            i++;

            // Si la fecha del ticket coincide con el dia de hoy
            if(ticket->fecha.anio==info_tiempo->tm_year+1900 && ticket->fecha.mes==info_tiempo->tm_mon+1 && ticket->fecha.dia==info_tiempo->tm_mday)
            {
                // Imprimos el ticket
                printf("\nTicket N%d\n", i);
                imprimir_ticket(ticket);

                // Sumamos el monto al ticket
                facturacion+=ticket->monto;
            }
            fread(ticket, sizeof(Tickets), 1, archivo);
        }

        // Imprimimos la facturacion 
        printf("\nLa facturacion de hoy %d/%02d/%02d de la empresa es: %.2f\n", info_tiempo->tm_year+1900, info_tiempo->tm_mon+1, info_tiempo->tm_mday, facturacion);
        frenar();
        free(ticket);
    } else
    {
        printf("\nNo hay compras registradas\n");
        frenar();
    }
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
    // Cerramos el programar
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
    
    
    FILE * archivo = abrir_archivo(productos_binario, "wb");
    struct Nodo_producto *nodo_actual = lista->cabeza;

    // Recorremos la lista
    for (int i = 0; i < lista->tam; i++)
    {
        // guardamos los datos del nodo(sin el nodo)
        fwrite(&nodo_actual->datos, sizeof(Productos), 1, archivo);

        // Avanzamos al siguiente nodo 
        nodo_actual = nodo_actual->siguiente;
    }
    fclose(archivo);
}

void archivo_a_lista_productos(Lista_enlazada_producto *lista)
{
    // Si existe el archivo binario con lista de productos
    if(archivo_existe(productos_binario) == 1)
    {

        FILE *archivo=abrir_archivo(productos_binario, "rb");
        Productos *producto = malloc(sizeof(Productos));
        fseek(archivo, 0, SEEK_SET);
        
        // Leemos el producto del archivo binario
        fread(producto, sizeof(Productos), 1, archivo);
        while(!feof(archivo))
        {
            // Insertamos ese producto en la lista
            insertar_nodo_producto(lista, producto);
            fread(producto, sizeof(Productos), 1, archivo);
        }
        free(producto);
    }
}


int archivo_existe(char *nombreArchivo)
{
    /* 
        Verificamos si es posible abrir el archivo,
        devolvemos 1 o 0 para identificarlo(simil booleano)
    */
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
    int dias = 0;
    if(archivo_existe(fecha_binario))
    {
        /* 
            Si tenemos un archivo binario con fecha procedemos a abrirlo
            y calcular la diferencia de fechas
        */
        FILE * archivo = abrir_archivo(fecha_binario, "r+b");
        Fecha *fecha1 = malloc(sizeof(Fecha));
        fseek(archivo, 0, SEEK_SET);
        fread(fecha1, sizeof(Fecha), 1, archivo);
        Fecha *fecha2 = malloc(sizeof(Fecha));
        time_t t;
        struct tm *info_tiempo;
        time(&t);
        info_tiempo = localtime(&t);
        fecha2->anio = info_tiempo->tm_year+1900;
        fecha2->mes = info_tiempo->tm_mon+1;
        fecha2->dia = info_tiempo->tm_mday;
        fecha2->hora = info_tiempo->tm_hour;
        fecha2->minuto = info_tiempo->tm_min;
        fecha2->segundo = info_tiempo->tm_sec;

        dias=diferencia_entre_fecha( fecha1, fecha2);

        rewind(archivo);
        fwrite(fecha2, sizeof(Fecha), 1, archivo);
        free(fecha1);
        free(fecha2);
        fclose(archivo);
    }
    else
    {
        /* 
            Si no tenemos un archivo binario con fecha 
            procedemos a generarlo para comparar si cambio de dia 
            o no
        */
        FILE * archivo = abrir_archivo(fecha_binario, "wb");
        Fecha *fecha1 = malloc(sizeof(Fecha));

        time_t t;
        struct tm *info_tiempo;
        time(&t);
        info_tiempo = localtime(&t);
        fecha1->anio = info_tiempo->tm_year+1900;
        fecha1->mes = info_tiempo->tm_mon+1;
        fecha1->dia = info_tiempo->tm_mday;
        fecha1->hora = info_tiempo->tm_hour;
        fecha1->minuto = info_tiempo->tm_min;
        fecha1->segundo = info_tiempo->tm_sec;

        fwrite(fecha1, sizeof(Fecha), 1, archivo);
        fclose(archivo);
    }

    
    if(fabs(dias)>0)
    {
        /*
            Si la diferencia entre dias es mayor a 0, es decir que paso un dia, procedemos 
            a aumentar la cantidad de dias sin comprar del cliente
        */
        struct Nodo_cliente *nodo_actual = lista->cabeza;
        for (int i = 0; i < lista->tam; i++)
        {
            if(nodo_actual->datos.cant_dias_sin_Comprar!=-1)
            {
                nodo_actual->datos.cant_dias_sin_Comprar+=fabs(dias);
            }
            nodo_actual = nodo_actual->siguiente;
        }
        lista_clientes_a_archivo(lista);
        return;
    }
}


int diferencia_entre_fecha( Fecha * fecha1, Fecha * fecha2)
{
    int bisiesto[12]={31,29,31,30,31,30,31,31,30,31,30,31};
	int normal[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    int i,j;
	int dias1,dias2;
	dias1=dias2=0;

    /* 
        Realizamos todas las tranformaciones necesarias
        para pasar las fechas a dias y guardamos ese valor
    */
	for(i=1; i<fecha1->anio;i++)
	{
        if((i%4==0&&i%400!=0)||(i%100==0))
        {
            dias1++;
        }
	}
	int dia_mes1=0;
	if( (fecha1->anio%4==0 && fecha1->anio%400!=0) || (fecha1->anio%100==0) )
	{
        for(int j=0;j<fecha1->mes;j++)
        {
            dia_mes1+=bisiesto[j];
        }
	}
	else
	{
		for(j=0;j<fecha1->mes;j++)
        {
            dia_mes1+=normal[j];
        }
	}
	dias1=(fecha1->anio-1)*365+dias1+dia_mes1+fecha1->dia;


    /* 
        Realizamos todas las tranformaciones necesarias
        para pasar las fechas a dias y guardamos ese valor
    */

	for( i=1; i<fecha2->anio;i++)
	{
        if((i%4==0&&i%400!=0)||(i%100==0))
        {
            dias2++;
        }
	}
	int dia_mes2=0;
	if((fecha2->anio%4==0&&fecha2->anio%400!=0)||(fecha2->anio%100==0))
	{
        for( j=0;j<fecha2->mes;j++)
        {
            dia_mes2=dia_mes2+bisiesto[j];
        }
	}
	else
    {
        for(j=0;j<fecha2->mes;j++)
        {
            dia_mes2=dia_mes2+normal[j];
        }
	}
	dias2=(fecha2->anio-1)*365+dias2+dia_mes2+fecha2->dia;

    // Devuelvo la diferencia de dias entre las dos fechas
	return dias2-dias1;
}

void listar_clientes_sin_compra(Lista_enlazada_cliente *lista){
    int c=0;
    borrar_pantalla();
    if (lista->tam == 0)
    {
        printf("\nNo hay Clientes\n");
        frenar();
        return;
    }
    struct Nodo_cliente *nodo_actual = lista->cabeza;
    printf("\nLista clientes sin comprar\n");
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
        printf("\nNo hay clientes sin compras\n");
    }
    frenar();
    return;

}