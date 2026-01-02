#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// =============================================
// 1. DEFINIR ARREGLOS Y VARIABLES GLOBALES
// =============================================

#define MAX_CLIENTES 100
#define MAX_CUENTAS 100
// =============================================
// 2. DEFINIR ESTRUCTURAS DE DATOS
// =============================================

// Estructura para Cuenta de Credito

////Se modifico el codiog y ahora guarda todos los datos en el archivo "datos_clientes.dat"///////////////////
typedef struct
{
    int num_cuenta;    // Numero unico de cuenta
    float adeudo;      // Monto actual que se debe
    float limite;      // Limite maximo de credito
    int meses_inicial; // Plazo: 3, 6 o 9 meses
} CuentaCredito;

// Estructura para Cliente
typedef struct
{
    int id_cliente;                     // ID unico del cliente
    char nombre[100];                   // Nombre completo
    char usuario[50];                   // Para login
    char password[50];                  // Para login
    CuentaCredito cuentas[MAX_CUENTAS]; ///////////integracion de la estructura de cuentas//////////////
    int ncuentas;                       // Numero de cuentas con las que cuenta el usuario//////
} Cliente;

Cliente lista_clientes[MAX_CLIENTES];     // Arreglo de clientes
CuentaCredito lista_cuentas[MAX_CUENTAS]; // Arreglo de cuentas

int total_clientes = 0; // Cuantos clientes hay registrados
int total_cuentas = 0;  // Cuantas cuentas hay registradas
int toc = 0;            /////////////////////////////////////////////////Variable para registrar cuentas

// =============================================
// 3. DEFINIR NOMBRES DE ARCHIVOS
// =============================================

const char *ARCHIVO_CLIENTES = "datos_clientes.dat";
const char *ARCHIVO_CUENTAS = "datos_cuentas.dat";
const char *REPORTE_CLIENTES = "reporte_clientes.txt";
const char *REPORTE_CUENTAS = "reporte_cuentas.txt";

// =============================================
// 4. FUNCIONES PARA MANEJO DE ARCHIVOS
// =============================================

// Funcion para GUARDAR clientes en archivo binario
void guardarClientesEnArchivo()
{
    FILE *archivo = fopen(ARCHIVO_CLIENTES, "wb");
    if (archivo == NULL)
    {
        printf("Error: No se pudo crear el archivo de clientes.\n");
        return;
    }

    // Primero guardamos cuantos clientes hay
    fwrite(&total_clientes, sizeof(int), 1, archivo);
    fwrite(&toc, sizeof(int), 1, archivo);

    // Luego guardamos cada cliente
    for (int i = 0; i < total_clientes; i++)
    {
        fwrite(&lista_clientes[i], sizeof(Cliente), 1, archivo);
    }
    // guardamos cada cuenta con la nueva variable
    for (int a = 0; a < toc; a++)
    {
        fwrite(&lista_cuentas[a], sizeof(Cliente), 1, archivo);
    }

    fclose(archivo);
    printf("Clientes guardados en '%s'\n", ARCHIVO_CLIENTES);
}

// Funcion para CARGAR clientes desde archivo binario
void cargarClientesDesdeArchivo()
{
    FILE *archivo = fopen(ARCHIVO_CLIENTES, "rb");
    if (archivo == NULL)
    {
        printf("Info: Archivo de clientes no existe. Se iniciara vacio.\n");
        total_clientes = 0;
        return;
    }

    // Primero leemos cuantos clientes hay
    fread(&total_clientes, sizeof(int), 1, archivo);
    fread(&toc, sizeof(int), 1, archivo);

    // Luego leemos cada cliente
    for (int i = 0; i < total_clientes; i++)
    {
        fread(&lista_clientes[i], sizeof(Cliente), 1, archivo);
    }
    ////cada cuenta con la nueva variable
    for (int a = 0; a < toc; a++)
    {
        fread(&lista_cuentas[a], sizeof(Cliente), 1, archivo);
    }

    fclose(archivo);
    printf("Clientes cargados desde '%s' (%d clientes)\n",
           ARCHIVO_CLIENTES, total_clientes);
}

// Funcion para GUARDAR cuentas en archivo binario//////////////////ahora ya no se usa esta funcion
void guardarCuentasEnArchivo()
{
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "wb");
    if (archivo == NULL)
    {
        printf("Error: No se pudo crear el archivo de cuentas.\n");
        return;
    }

    // Primero guardamos cuantas cuentas hay
    fwrite(&total_cuentas, sizeof(int), 1, archivo);

    // Luego guardamos cada cuenta
    for (int i = 0; i < total_cuentas; i++)
    {
        fwrite(&lista_cuentas[i], sizeof(CuentaCredito), 1, archivo);
    }

    fclose(archivo);
    printf("Cuentas guardadas en '%s'\n", ARCHIVO_CUENTAS);
}

// Funcion para CARGAR cuentas desde archivo binario//////////////no se usa esta funcion
void cargarCuentasDesdeArchivo()
{
    FILE *archivo = fopen(ARCHIVO_CUENTAS, "rb");
    if (archivo == NULL)
    {
        printf("Info: Archivo de cuentas no existe. Se iniciara vacio.\n");
        total_cuentas = 0;
        return;
    }

    // Primero leemos cuantas cuentas hay
    fread(&total_cuentas, sizeof(int), 1, archivo);

    // Luego leemos cada cuenta
    for (int i = 0; i < total_cuentas; i++)
    {
        fread(&lista_cuentas[i], sizeof(CuentaCredito), 1, archivo);
    }

    fclose(archivo);
    printf("Cuentas cargados desde '%s' (%d cuentas)\n",
           ARCHIVO_CUENTAS, total_cuentas);
}

// Funcion para GENERAR REPORTE de clientes en texto
void generarReporteClientes()
{
    FILE *archivo = fopen(REPORTE_CLIENTES, "w");
    if (archivo == NULL)
    {
        printf("Error: No se pudo crear el reporte de clientes.\n");
        return;
    }

    fprintf(archivo, "=== REPORTE DE CLIENTES DEL BANCO ===\n");
    fprintf(archivo, "======================================\n\n");

    if (total_clientes == 0)
    {
        fprintf(archivo, "No hay clientes registrados.\n");
    }
    else
    {
        for (int i = 0; i < total_clientes; i++)
        {
            fprintf(archivo, "Cliente #%d:\n", i + 1);
            fprintf(archivo, "  ID: %d\n", lista_clientes[i].id_cliente);
            fprintf(archivo, "  Nombre: %s\n", lista_clientes[i].nombre);
            fprintf(archivo, "  Usuario: %s\n", lista_clientes[i].usuario);
            fprintf(archivo, "  -----------------------------\n");
        }
    }

    fclose(archivo);
    printf("Reporte de clientes generado: '%s'\n", REPORTE_CLIENTES);
}

// Funcion para GENERAR REPORTE de cuentas en texto
void generarReporteCuentas()
{
    FILE *archivo = fopen(REPORTE_CUENTAS, "w");
    if (archivo == NULL)
    {
        printf("Error: No se pudo crear el reporte de cuentas.\n");
        return;
    }

    fprintf(archivo, "=== REPORTE DE CUENTAS DE CREDITO ===\n");
    fprintf(archivo, "======================================\n\n");

    if (total_cuentas == 0)
    {
        fprintf(archivo, "No hay cuentas registradas.\n");
    }
    else
    {
        fprintf(archivo, "%-10s %-10s %-12s %-12s %-15s\n",
                "Cuenta", "Dueno ID", "Adeudo", "Limite", "Meses");
        fprintf(archivo, "--------------------------------------------------\n");

        for (int i = 0; i < total_cuentas; i++)
        {
            fprintf(archivo, "%-10d $%-10.2f $%-10.2f %-15d\n",
                    lista_cuentas[i].num_cuenta,
                    lista_cuentas[i].adeudo,
                    lista_cuentas[i].limite,
                    lista_cuentas[i].meses_inicial);
        }
    }

    fclose(archivo);
    printf("Reporte de cuentas generado: '%s'\n", REPORTE_CUENTAS);
}

// =============================================
// 5. FUNCION PARA DEMOSTRAR LAS ESTRUCTURAS  - resumen de las variables implementadas -
// =============================================

void demostrarEstructuras()
{
    printf("\n=== DEMOSTRACION DE ESTRUCTURAS ===\n");

    // Mostrar informacion de la estructura Cliente
    printf("\n1. ESTRUCTURA 'Cliente':\n");
    printf("   - id_cliente: Entero (identificador unico)\n");
    printf("   - nombre: Cadena de 100 caracteres\n");
    printf("   - usuario: Cadena de 50 caracteres (para login)\n");
    printf("   - password: Cadena de 50 caracteres (para login)\n");

    // Mostrar informacion de la estructura CuentaCredito
    printf("\n2. ESTRUCTURA 'CuentaCredito':\n");
    printf("   - num_cuenta: Entero (numero unico de cuenta)\n");
    printf("   - id_dueno: Entero (RELACION con Cliente)\n");
    printf("   - adeudo: Flotante (monto actual adeudado)\n");
    printf("   - limite: Flotante (limite maximo de credito)\n");
    printf("   - meses_inicial: Entero (3, 6 o 9 meses)\n");

    // Mostrar relaciones
    printf("\n3. RELACIONES ENTRE DATOS:\n");
    printf("   - Un Cliente puede tener 0, 1 o mas CuentasCredito\n");
    printf("   - Cada CuentaCredito pertenece a UN solo Cliente\n");
    printf("   - La relacion se establece mediante 'id_dueno' en CuentaCredito\n");
    printf("     que apunta a 'id_cliente' en Cliente\n");

    // Mostrar ejemplo de relacion
    if (total_clientes > 0 && total_cuentas > 0)
    {
        printf("\n4. EJEMPLO DE RELACION:\n");
        printf("   Cliente: ID=%d, Nombre=%s\n",
               lista_clientes[0].id_cliente, lista_clientes[0].nombre);
        printf("   Tiene cuenta: Numero=%d, Adeudo=$%.2f\n",
               lista_cuentas[0].num_cuenta, lista_cuentas[0].adeudo);
    }
}

//////////////////////////////////Menu cliente/////////////////////////////////////////////////////////////////

void menu_cliente()
{

    int id;
    char p[50];
    int op;
    int cntsel;
    int flag = 0;
    printf("========BIENVENIDO=======\n");
    printf("ingresa tu ID:\n");
    scanf("%d", &id);
    printf("Bienvenido %s \n", lista_clientes[id].usuario);
    printf("Ingresa tu contraseña: \n");
    int c; ///////////////////////////////////////////////////////////
    while ((c = getchar()) != '\n' && c != EOF)
        ; ////////////////////////////////Limpieza de buffer para que no se trabe jejejeje//////////////////////////////////////
    scanf("%s", p);
    printf("-%d, %s, %s-\n", strcmp(p, lista_clientes[id].password), lista_clientes[id].password, p);
    if (strcmp(p, lista_clientes[id].password) != 0) /////Verificar si la contrase�a es correcta//////////////
    {
        printf("Contraseña incorrecta cerrando el programa\n");
        exit(0);
    }
    while (op != 4)
    {
        printf("========BIENVENIDO=======\n");
        printf("Seleccione una opcion\n1- Registrar deposito \n2- Registrar compra \n3- Ver saldo de cuenta de credito \n4- Salir \n");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            printf("========Registrar depósito=======\n\n");
            printf("Cuentas del usuario %s:", lista_clientes[id].usuario);
            for (int i = 0; i < lista_clientes[id].ncuentas; i++)
            {
                printf("Cuenta n: %d \n", i + 1);
                printf("Num. cuenta: %d \n", lista_clientes[id].cuentas[i].num_cuenta);
                printf("Adeudo: %f\n", lista_clientes[id].cuentas[i].adeudo);
                printf("---------------------------");
            }

            printf("Selecciona la cuenta\n");
            scanf("%d", &cntsel);
            for (int i = 0; i < lista_clientes[id].ncuentas; i++)
            {
                if (cntsel == lista_clientes[id].cuentas[i].num_cuenta)
                {
                    flag = 1;
                    cntsel = i;
                    break;
                }
            }

            if (flag == 1)
            {
                if (lista_clientes[id].cuentas[cntsel].adeudo > 0)
                {
                    float monto;

                    do
                    {
                        flag = 1;
                        printf("Ingresa el monto del depósito:\n");
                        scanf("%f", &monto);

                        if (monto <= 0)
                        {
                            flag = 0;
                            printf("Error: El monto debe ser positivo\n");
                        }
                    } while (flag == 0);

                    lista_clientes[id].cuentas[cntsel].adeudo -= monto;

                    if (lista_clientes[id].cuentas[cntsel].adeudo < 0)
                    {
                        lista_clientes[id].cuentas[cntsel].adeudo = 0;
                    }
                    guardarClientesEnArchivo();

                    printf("Monto ingresado correctamente\n");
                    printf("El adeudo de la cuenta %d es ahora de: $%f\n", lista_clientes[id].cuentas[cntsel].num_cuenta, lista_clientes[id].cuentas[cntsel].adeudo);
                }
                else
                {
                    printf("La cuenta no tiene ningún adeudo\n");
                }
            }
            else
            {
                printf("Opción no válida\n");
            }
            break;
        case 2:
            printf("========Registrar depósito=======\n\n");
            printf("Cuentas del usuario %s:", lista_clientes[id].usuario);
            for (int i = 0; i < lista_clientes[id].ncuentas; i++)
            {
                printf("Cuenta n: %d \n", i + 1);
                printf("Num. cuenta: %d \n", lista_clientes[id].cuentas[i].num_cuenta);
                printf("Adeudo: %f\n", lista_clientes[id].cuentas[i].adeudo);
                printf("Limite de crédito: %f\n",lista_clientes[id].cuentas[i].limite);
                printf("---------------------------");
            }

            printf("Selecciona la cuenta\n");
            scanf("%d", &cntsel);
            for (int i = 0; i < lista_clientes[id].ncuentas; i++)
            {
                if (cntsel == lista_clientes[id].cuentas[i].num_cuenta)
                {
                    flag = 1;
                    cntsel = i;
                    break;
                }
            }

            if (flag == 1)
            {
                float monto_base;
                float interes;
                float meses;
                float adeudo;

                do
                {
                    flag = 1;
                    printf("Ingresa el monto base del crédito:\n");
                    scanf("%f", &monto_base);

                    if (monto_base <= 0)
                    {
                        flag = 0;
                        printf("Error: El monto debe ser positivo\n");
                    }
                } while (flag == 0);

                do
                {
                    flag = 1;
                    printf("Meses para pagar inicial (3, 6, 9):\n");
                    scanf("%f", &meses);

                    if (meses != 3 && meses != 6 && meses != 9)
                    {
                        flag = 0;
                        printf("Error: solo se puede ingresar 3, 6 o 9 meses\n");
                    }
                } while (flag == 0);

                interes = meses * 0.02;
                adeudo = monto_base * (1 + interes);

                if(lista_clientes[id].cuentas[cntsel].limite>=lista_clientes[id].cuentas[cntsel].adeudo+adeudo){

                    printf("=========================\n");
                    printf("========Resumen del crédito=======\n");
                    printf("Monto base: $%f\n", monto_base);
                    printf("Meses: %d meses\n", (int)meses);
                    printf("Interés (%d meses al 2%%): %d%%\n", (int)meses, (int)(interes * 100));
                    printf("Nuevo adeudo: $%f\n", lista_clientes[id].cuentas[cntsel].adeudo+adeudo);
                    printf("Limite del crédito: $%f\n", lista_clientes[id].cuentas[cntsel].limite);

                    printf("¿Desea continuar?\n");
                    printf("Si(1), No(2)\n");
                    scanf("%d", &op);

                    if (op != 1)
                    {
                        break;
                    }

                    
                    lista_clientes[id].cuentas[cntsel].adeudo += adeudo;
                    lista_clientes[id].cuentas[cntsel].meses_inicial = meses;

                    printf("Se ha establecido un nuevo adeudo de $%f a la cuenta No. %d\n", lista_clientes[id].cuentas[cntsel].adeudo, lista_clientes[id].cuentas[cntsel].num_cuenta);

                    guardarClientesEnArchivo();
                }else{
                    printf("Error: el monto no puede exceder el límite de crédito\n");
                }
            }
            else
            {
                printf("Opción no válida\n");
            }
            break;
        case 3:
            printf("========Ver saldo de cuentas de crédito=======\n\n");
            printf("Cuentas del usuario %s:", lista_clientes[id].usuario);
            printf("---------------------------");

            for (int i = 0; i < lista_clientes[id].ncuentas; i++)
            {
                printf("Cuenta n: %d \n", i + 1);
                printf("Num. cuenta: %d \n", lista_clientes[id].cuentas[i].num_cuenta);
                printf("Adeudo actual: %f\n", lista_clientes[id].cuentas[i].adeudo);
                printf("Limite de crédito: %f\n",lista_clientes[id].cuentas[i].limite);
                printf("Crédito disponible: %f\n",(lista_clientes[id].cuentas[i].limite-lista_clientes[id].cuentas[i].adeudo));
                printf("---------------------------");
            }
            
            break;
        case 4:
            break;
        default:
            printf("Opción no valida");
            break;
        }
    }
}

////////////////////////////////Menu administrador////////////////////////////////////////////////////7

void menu_administrador()
{
    printf("========BIENVENIDO=======\n");
    int op;
    while (op != 5)
    {
        printf("========Administrador=======\n");
        printf("Seleccione una opcion\n1- Registrar cliente \n2- Registrar cuenta \n3-Ver clientes \n4- Ver cuentas \n5-Regresar al menu\n");

        scanf("%d", &op);
        int c; ///////////////////////////////////////////////////////////
        while ((c = getchar()) != '\n' && c != EOF)
            ; ////////////////////////////////Limpieza de buffer para que no se trabe jejejeje//////////////////////////////////////
        switch (op)
        {
            ///////////////////////////////////////Registrar clientes////////////////////////////////////////////////////////////
        case 1:
            Cliente c1;
            printf("========Registro de clientes=======\n\n");
            printf("Ingresa nombre del cliente completo\n");
            scanf("%s", c1.nombre);
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Ingresa su usuario\n");
            scanf("%s", c1.usuario);
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Ingresa su contraseña\n");
            scanf("%s", c1.password);
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Su id es: %d \n", total_clientes);
            lista_clientes[total_clientes].id_cliente = total_clientes;
            lista_clientes[total_clientes].ncuentas = 0;
            strcpy(lista_clientes[total_clientes].nombre, c1.nombre);
            strcpy(lista_clientes[total_clientes].usuario, c1.usuario);
            strcpy(lista_clientes[total_clientes].password, c1.password);
            total_clientes++;
            guardarClientesEnArchivo();
            break;
            /////////////////////////////////////Registrar cuentas///////////////////////////////////////////////////////////////////////
        case 2:
            int id;
            int d;
            printf("========Registro de cuentas=======\n\n");
            printf("Ingrese el id del cliente:\n");
            scanf("%d", &id);
            if (id > total_clientes)
            {
                printf("Ese usuario no existe\n");
                break;
            }
            printf("Usuario: %s \n", lista_clientes[id].usuario);
            printf("�Desea agregar una cuenta de credito al usuario?\n");
            printf("Si(1), No(2)\n");
            scanf("%d", &d);
            if (d == 1)
            {
                float monto_base;
                float interes;
                float meses;
                float adeudo;
                float limite;
                int flag;

                do
                {
                    flag = 1;
                    printf("Ingresa el monto base del crédito:\n");
                    scanf("%f", &monto_base);

                    if (monto_base <= 0)
                    {
                        flag = 0;
                        printf("Error: El monto debe ser positivo\n");
                    }
                } while (flag == 0);

                do
                {
                    flag = 1;
                    printf("Meses para pagar inicial (3, 6, 9):\n");
                    scanf("%f", &meses);

                    if (meses != 3 && meses != 6 && meses != 9)
                    {
                        flag = 0;
                        printf("Error: solo se puede ingresar 3, 6 o 9 meses\n");
                    }
                } while (flag == 0);

                do
                {
                    flag = 1;
                    printf("Ingresa límite del crédito:\n");
                    scanf("%f", &limite);

                    if (limite <= 0)
                    {
                        flag = 0;
                        printf("Error: El límite debe ser positivo\n");
                    }
                } while (flag == 0);

                interes = meses * 0.02;
                adeudo = monto_base * (1 + interes);

                printf("=========================\n");
                printf("========Resumen del crédito=======\n");
                printf("Monto base: $%f\n", monto_base);
                printf("Meses: %d meses\n", (int)meses);
                printf("Interés (%d meses al 2%%): %d%%\n", (int)meses, (int)(interes * 100));
                printf("Adeudo inicial total: $%f\n", adeudo);
                printf("Limite del crédito: $%f\n", limite);

                printf("¿Desea continuar?\n");
                printf("Si(1), No(2)\n");
                scanf("%d", &d);

                if (d != 1)
                {
                    break;
                }

                //////////////lista_clientes[id].ncuentas+=1;////// esta funcion se movi� unas lineas abajo para no desperdiciar memoria del arreglo :p
                lista_clientes[id].cuentas[lista_clientes[id].ncuentas].num_cuenta = 1000 + toc;
                lista_clientes[id].cuentas[lista_clientes[id].ncuentas].adeudo = adeudo;
                lista_clientes[id].cuentas[lista_clientes[id].ncuentas].meses_inicial = meses;
                lista_clientes[id].cuentas[lista_clientes[id].ncuentas].limite = limite;

                printf("Se a agredado al usuario: %s una cuenta de credito con numero: %d y un limite de $%f \n", lista_clientes[id].usuario, lista_clientes[id].cuentas[lista_clientes[id].ncuentas].num_cuenta, lista_clientes[id].cuentas[lista_clientes[id].ncuentas].limite);

                lista_clientes[id].ncuentas += 1;

                printf("El usuario cuenta con %d cuentas actualmente \n", lista_clientes[id].ncuentas);

                toc++;
                guardarClientesEnArchivo();
            }
            break;
            ////////////////////////////////////////Visualizar clientes//////////////////////////////////////////////////////////////////////////
        case 3:
            for (int i = 0; i < total_clientes; i++)
            {
                printf("cliente id %d \n", lista_clientes[i].id_cliente);
                printf("nombre %s \n", lista_clientes[i].nombre);
                printf("usuario %s \n", lista_clientes[i].usuario);
                printf("contrase�a %s \n", lista_clientes[i].password);
            }
            break;
            //////////////////////////////////////Visualizar cuentas de credito////////////////////////////////////////////////////////////////
        case 4:
            for (int i = 0; i < total_clientes; i++)
            {
                printf("cliente numero %d \n", lista_clientes[i].id_cliente + 1);
                printf("Usuario: %s \n", lista_clientes[i].usuario);
                printf("id: %d \n", lista_clientes[i].id_cliente);
                printf("Cuentas del cliente: %d \n", lista_clientes[i].ncuentas);
                /*for(int f=1;f<=lista_clientes[i].ncuentas;f++)
                    {
                        printf("Cuenta n: %d \n",f+1);
                        printf("Num . cuenta: %d \n",lista_clientes[i].cuentas[f].num_cuenta);  ////////Este for se modifico e
                                                                                                        simplemente iniciando desde 0 y no 1

                    }
                    */
                //////////////////////////////////versio correcta GG chavales//////////////////////////
                for (int f = 0; f < lista_clientes[i].ncuentas; f++)
                {
                    printf("Cuenta n: %d \n", f + 1);
                    printf("Num . cuenta: %d \n", lista_clientes[i].cuentas[f].num_cuenta);
                }
                //////////////////////////////////////////////////////////////
            }
            break;
        case 5:
            break;
        default:
            printf("Opci�n no disponible\n");
            break;
        }
    }
}

//////////////////////////////////////////////Menu principal//////////////////////////////////////////////////////////////

void menu_principal()
{
    int opcion;

    while (opcion != 3)
    {
        printf("========BIENVENIDO=======\n");
        printf("=========================\n");
        printf("Seleccione una opcion\n1- Administracion \n2- Cliente \n3-Salir\n");
        scanf("%d", &opcion);
        switch (opcion)
        {
        case 1:
            menu_administrador();
            break;
        case 2:
            menu_cliente();
            break;
        case 3:
            //////////////////////////////////////Recordad guardar chavales//////////////////////////////////////////////////
            exit(0);
            break;
        default:
            printf("Opcion no disponible \n");
            break;
        }
    }
}

int main(void)
{
    printf("========================================\n");
    printf("  SISTEMA BANCARIO - AVANCE 2\n");
    printf("  Responsable: Carlos\n");
    printf("  Fecha: 25-27 Diciembre 2025\n");
    printf("========================================\n");

    // =========================================
    // PASO 1: Cargar datos existentes
    // =========================================
    printf("\n1. CARGANDO DATOS DESDE ARCHIVOS...\n");
    cargarClientesDesdeArchivo();
    cargarCuentasDesdeArchivo();

    // =========================================
    // PASO 2: Agregar datos de prueba si no hay
    // =========================================
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // =========================================
    // PASO 3: Demostrar las estructuras
    // =========================================
    demostrarEstructuras();

    // =========================================
    // PASO 4: Guardar datos en archivos
    // =========================================
    printf("\n5. GUARDANDO DATOS EN ARCHIVOS...\n");
    guardarClientesEnArchivo();
    guardarCuentasEnArchivo();

    // =========================================
    // PASO 5: Generar reportes en texto
    // =========================================
    printf("\n6. GENERANDO REPORTES EN TEXTO...\n");
    generarReporteClientes();
    generarReporteCuentas();

    // =========================================
    // PASO 6: Mostrar resumen
    // =========================================
    printf("\n============================''============\n");
    printf("  RESUMEN DEL AVANCE 2 - COMPLETADO\n");
    printf("========================================\n");
    printf("Estructuras de datos definidas: 2\n");
    printf("Relaciones establecidas: 1 (Cliente-Cuenta)\n");
    printf("Archivos implementados: 4\n");
    printf("Funciones de archivo creadas: 6\n");
    printf("Datos de prueba cargados: Si\n");
    printf("Reportes generados: Si\n");
    printf("\nArchivos creados:\n");
    printf("  - %s (datos binarios clientes)\n", ARCHIVO_CLIENTES);
    printf("  - %s (datos binarios cuentas)\n", ARCHIVO_CUENTAS);
    printf("  - %s (reporte texto clientes)\n", REPORTE_CLIENTES);
    printf("  - %s (reporte texto cuentas)\n", REPORTE_CUENTAS);
    printf("\n========================================\n");

    printf("\n Hola Avance 3");

    menu_principal();
    return 0;
}
