#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// =============================================
// 1. DEFINIR ARREGLOS Y VARIABLES GLOBALES
// =============================================

#define MAX_CLIENTES 100
#define MAX_CUENTAS 100

// Colores para la terminal.
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"

// =============================================
// 2. DEFINIR ESTRUCTURAS DE DATOS
// =============================================

/*
    Estructura para Cuenta de Credito
    Los datos se guardan en el archivo "datos_clientes.dat"
*/
typedef struct
{
    int num_cuenta;    // Numero unico de cuenta
    float adeudo;      // Monto actual que se debe
    float limite;      // Limite maximo de credito
    int meses_inicial; // Plazo: 3, 6 o 9 meses
} CuentaCredito;

/*
    Estructura para Clientes
    Los datos se guardan en el archivo "datos_clientes.dat"
*/
typedef struct
{
    int id_cliente;                     // ID unico del cliente
    char nombre[100];                   // Nombre completo
    char usuario[50];                   // Para login
    char password[50];                  // Para login
    CuentaCredito cuentas[MAX_CUENTAS]; // integracion de la estructura de cuentas de credito dentro de la estructura cliente
    int ncuentas;                       // Numero de cuentas con las que cuenta el usuario
} Cliente;

Cliente lista_clientes[MAX_CLIENTES]; // Arreglo de clientes

int total_clientes = 0; // Cuantos clientes hay registrados
int toc = 0;            // Variable para registrar cuentas (contador global para generar num_cuenta)

// =============================================
// 3. DEFINIR NOMBRES DE ARCHIVOS
// =============================================

const char *ARCHIVO_CLIENTES = "datos_clientes.dat";
const char *REPORTE_CLIENTES = "reporte_clientes.txt";
const char *REPORTE_CUENTAS = "reporte_cuentas.txt";

// =============================================
// 4. FUNCIONES AUXILIARES DE INTERFAZ
// =============================================

// Función para limpiar pantalla
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Función para pausar la ejecución
void pausar() {
    printf("\n%s%sPresione ENTER para continuar...%s", BOLD, CYAN, RESET);
    getchar();
    getchar();
}

// Función para limpiar el buffer de entrada
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Función para líneas de colores
void dibujarLinea(char caracter, int longitud) {
    int i;
    printf("%s", CYAN);
    for(i = 0; i < longitud; i++) {
        printf("%c", caracter);
    }
    printf("%s\n", RESET);
}

// Función para mostrar encabezado con título
void mostrarEncabezado(const char* titulo) {
    limpiarPantalla();
    printf("\n");
    dibujarLinea('=', 60);
    printf("%s%s%*s%s%*s%s\n", BOLD, BLUE, 
           (int)(30 - strlen(titulo)/2), "", titulo, 
           (int)(30 - strlen(titulo)/2), "", RESET);
    dibujarLinea('=', 60);
    printf("\n");
}

// Función para mostrar mensaje de éxito
void mostrarExito(const char* mensaje) {
    printf("\n%s%s✓ %s%s\n", BOLD, GREEN, mensaje, RESET);
}

// Función para mostrar mensaje de error
void mostrarError(const char* mensaje) {
    printf("\n%s%s✗ Error: %s%s\n", BOLD, RED, mensaje, RESET);
}

// Función para mostrar mensaje de advertencia
void mostrarAdvertencia(const char* mensaje) {
    printf("\n%s%s⚠ Advertencia: %s%s\n", BOLD, YELLOW, mensaje, RESET);
}

// Función para formatear montos monetarios
void imprimirMonto(float monto) {
    printf("$%,.2f", monto);
}

// =============================================
// 5. FUNCIONES PARA MANEJO DE ARCHIVOS
// =============================================

// Funcion para GUARDAR clientes en archivo binario
void guardarClientesEnArchivo()
{
    FILE *archivo;

    archivo = fopen(ARCHIVO_CLIENTES, "wb");
    if (archivo == NULL)
    {
        mostrarError("No se pudo crear el archivo de clientes.");
        return;
    }

    // Primero guardamos cuantos clientes hay
    fwrite(&total_clientes, sizeof(int), 1, archivo);
    fwrite(&toc, sizeof(int), 1, archivo);

    // Luego guardamos cada cliente
    {
        int i;
        for (i = 0; i < total_clientes; i++)
        {
            fwrite(&lista_clientes[i], sizeof(Cliente), 1, archivo);
        }
    }

    fclose(archivo);
}

// Funcion para CARGAR clientes desde archivo binario
void cargarClientesDesdeArchivo()
{
    FILE *archivo;

    archivo = fopen(ARCHIVO_CLIENTES, "rb");
    if (archivo == NULL)
    {
        // Primera vez que se ejecuta el programa
        total_clientes = 0;
        toc = 0;
        return;
    }

    // Primero leemos cuantos clientes hay
    fread(&total_clientes, sizeof(int), 1, archivo);
    fread(&toc, sizeof(int), 1, archivo);

    // Validaciones basicas para evitar lecturas fuera de rango si el archivo esta corrupto
    if (total_clientes < 0)
        total_clientes = 0;
    if (total_clientes > MAX_CLIENTES)
        total_clientes = MAX_CLIENTES;

    // Luego leemos cada cliente
    {
        int i;
        for (i = 0; i < total_clientes; i++)
        {
            fread(&lista_clientes[i], sizeof(Cliente), 1, archivo);
            // Aseguramos que ncuentas no exceda el maximo
            if (lista_clientes[i].ncuentas < 0)
                lista_clientes[i].ncuentas = 0;
            if (lista_clientes[i].ncuentas > MAX_CUENTAS)
                lista_clientes[i].ncuentas = MAX_CUENTAS;
        }
    }

    fclose(archivo);
}

// Funcion para GENERAR REPORTE de clientes en texto
void generarReporteClientes()
{
    FILE *archivo;

    archivo = fopen(REPORTE_CLIENTES, "w");
    if (archivo == NULL)
    {
        mostrarError("No se pudo crear el reporte de clientes.");
        return;
    }

    fprintf(archivo, "=====================================\n");
    fprintf(archivo, "   REPORTE DE CLIENTES DEL BANCO    \n");
    fprintf(archivo, "=====================================\n");
    fprintf(archivo, "    Fecha: Sistema Bancario v1.0\n");
    fprintf(archivo, "=====================================\n\n");

    if (total_clientes == 0)
    {
        fprintf(archivo, "No hay clientes registrados.\n");
    }
    else
    {
        int i;
        fprintf(archivo, "Total de clientes registrados: %d\n\n", total_clientes);
        
        for (i = 0; i < total_clientes; i++)
        {
            fprintf(archivo, "┌─────────────────────────────────┐\n");
            fprintf(archivo, "│ Cliente #%03d                   │\n", i + 1);
            fprintf(archivo, "├─────────────────────────────────┤\n");
            fprintf(archivo, "│ ID:       %04d                  │\n", lista_clientes[i].id_cliente);
            fprintf(archivo, "│ Nombre:   %-20s │\n", lista_clientes[i].nombre);
            fprintf(archivo, "│ Usuario:  %-20s │\n", lista_clientes[i].usuario);
            fprintf(archivo, "│ Cuentas:  %02d                    │\n", lista_clientes[i].ncuentas);
            fprintf(archivo, "└─────────────────────────────────┘\n\n");
        }
    }

    fclose(archivo);
    mostrarExito("Reporte de clientes generado exitosamente.");
    printf("    Archivo: %s%s%s\n", YELLOW, REPORTE_CLIENTES, RESET);
}

// Funcion para GENERAR REPORTE de cuentas en texto
void generarReporteCuentas()
{
    FILE *archivo;
    int total_cuentas;
    int i;

    archivo = fopen(REPORTE_CUENTAS, "w");
    if (archivo == NULL)
    {
        mostrarError("No se pudo crear el reporte de cuentas.");
        return;
    }

    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "    REPORTE DE CUENTAS DE CREDITO        \n");
    fprintf(archivo, "==========================================\n");
    fprintf(archivo, "    Fecha: Sistema Bancario v1.0\n");
    fprintf(archivo, "==========================================\n\n");

    total_cuentas = 0;
    for (i = 0; i < total_clientes; i++)
        total_cuentas += lista_clientes[i].ncuentas;

    if (total_cuentas == 0)
    {
        fprintf(archivo, "No hay cuentas registradas.\n");
    }
    else
    {
        fprintf(archivo, "Total de cuentas activas: %d\n\n", total_cuentas);
        fprintf(archivo, "┌────────────┬────────────┬──────────────┬──────────────┬─────────┐\n");
        fprintf(archivo, "│   Cuenta   │ Cliente ID │    Adeudo    │    Límite    │  Meses  │\n");
        fprintf(archivo, "├────────────┼────────────┼──────────────┼──────────────┼─────────┤\n");

        for (i = 0; i < total_clientes; i++)
        {
            int j;
            for (j = 0; j < lista_clientes[i].ncuentas; j++)
            {
                CuentaCredito *cc;
                cc = &lista_clientes[i].cuentas[j];
                fprintf(archivo, "│ %10d │ %10d │ $%11.2f │ $%11.2f │ %7d │\n",
                        cc->num_cuenta,
                        lista_clientes[i].id_cliente,
                        cc->adeudo,
                        cc->limite,
                        cc->meses_inicial);
            }
        }
        fprintf(archivo, "└────────────┴────────────┴──────────────┴──────────────┴─────────┘\n");
    }

    fclose(archivo);
    mostrarExito("Reporte de cuentas generado exitosamente.");
    printf("    Archivo: %s%s%s\n", YELLOW, REPORTE_CUENTAS, RESET);
}

// =============================================
// 6. FUNCION PARA VISUALIZAR LAS ESTRUCTURAS
// =============================================

void visualizarEstructuras()
{
    mostrarEncabezado("VISUALIZAR ESTRUCTURAS DE DATOS");

    printf("%s%s1. ESTRUCTURA 'Cliente':%s\n", BOLD, BLUE, RESET);
    printf("   %s•%s id_cliente: Entero (identificador único)\n", GREEN, RESET);
    printf("   %s•%s nombre: Cadena de 100 caracteres\n", GREEN, RESET);
    printf("   %s•%s usuario: Cadena de 50 caracteres (para login)\n", GREEN, RESET);
    printf("   %s•%s password: Cadena de 50 caracteres (para login)\n", GREEN, RESET);
    printf("   %s•%s cuentas: Array de CuentaCredito\n", GREEN, RESET);
    printf("   %s•%s ncuentas: Número de cuentas activas\n\n", GREEN, RESET);

    printf("%s%s2. ESTRUCTURA 'CuentaCredito':%s\n", BOLD, BLUE, RESET);
    printf("   %s•%s num_cuenta: Entero (número único de cuenta)\n", GREEN, RESET);
    printf("   %s•%s adeudo: Flotante (monto actual adeudado)\n", GREEN, RESET);
    printf("   %s•%s limite: Flotante (límite máximo de crédito)\n", GREEN, RESET);
    printf("   %s•%s meses_inicial: Entero (3, 6 o 9 meses)\n\n", GREEN, RESET);

    printf("%s%s3. RELACIONES ENTRE DATOS:%s\n", BOLD, BLUE, RESET);
    printf("   %s→%s Un Cliente puede tener 0, 1 o más CuentasCredito\n", CYAN, RESET);
    printf("   %s→%s Cada CuentaCredito pertenece a UN solo Cliente\n", CYAN, RESET);
    printf("   %s→%s La relación se establece mediante arrays internos\n\n", CYAN, RESET);

    // Mostrar ejemplo si hay datos
    if (total_clientes > 0 && lista_clientes[0].ncuentas > 0)
    {
        printf("%s%s4. EJEMPLO DE DATOS REALES:%s\n", BOLD, YELLOW, RESET);
        dibujarLinea('-', 50);
        printf("   Cliente: ID=%d, Nombre=%s\n",
               lista_clientes[0].id_cliente, lista_clientes[0].nombre);
        printf("   Cuenta: Número=%d, Adeudo=", lista_clientes[0].cuentas[0].num_cuenta);
        imprimirMonto(lista_clientes[0].cuentas[0].adeudo);
        printf("\n");
    }
    
    pausar();
}

// =============================================
// 7. MENU CLIENTE
// =============================================

void menu_cliente()
{
    char usuario_input[50];
    char p[50];
    int op;
    int cntsel;
    int flag;
    int intentos = 3;
    int id = -1;
    int i;

    mostrarEncabezado("BANCA EN LÍNEA - ACCESO CLIENTE");

    // Solicitar nombre de usuario.
    printf("%sIngrese su nombre de usuario:%s ", BOLD, RESET);
    scanf("%49s", usuario_input);

    // Buscar el cliente por nombre de usuario
    for (i = 0; i < total_clientes; i++)
    {
        if (strcmp(lista_clientes[i].usuario, usuario_input) == 0)
        {
            id = i;
            break;
        }
    }

    // Validar si se encontró el usuario
    if (id == -1)
    {
        mostrarError("Usuario no encontrado.");
        pausar();
        return;
    }

    printf("\n%sBienvenido, %s%s%s\n\n", GREEN, BOLD, lista_clientes[id].nombre, RESET);

    // Solicitar contraseña con intentos limitados
    while (intentos > 0)
    {
        printf("%sIngrese su contraseña:%s ", BOLD, RESET);
        limpiarBuffer();
        scanf("%49s", p);

        if (strcmp(p, lista_clientes[id].password) == 0)
        {
            mostrarExito("Acceso autorizado");
            break;
        }
        else
        {
            intentos--;
            if (intentos > 0)
            {
                mostrarError("Contraseña incorrecta.");
                printf("    Intentos restantes: %s%d%s\n", YELLOW, intentos, RESET);
            }
            else
            {
                mostrarError("Demasiados intentos fallidos. Acceso bloqueado.");
                pausar();
                return;
            }
        }
    }

    op = 0;
    while (op != 4)
    {
        mostrarEncabezado("PORTAL DEL CLIENTE");
        
        printf("%sCliente:%s %s\n", BOLD, RESET, lista_clientes[id].nombre);
        printf("%sID:%s %04d\n", BOLD, RESET, lista_clientes[id].id_cliente);
        printf("%sCuentas activas:%s %d\n\n", BOLD, RESET, lista_clientes[id].ncuentas);
        
        dibujarLinea('-', 50);
        printf("\n%sMENÚ DE OPCIONES:%s\n\n", BOLD, RESET);
        printf("  %s[1]%s Registrar depósito\n", CYAN, RESET);
        printf("  %s[2]%s Registrar compra\n", CYAN, RESET);
        printf("  %s[3]%s Ver estado de cuentas\n", CYAN, RESET);
        printf("  %s[4]%s Cerrar sesión\n\n", RED, RESET);
        
        printf("%sSeleccione una opción:%s ", BOLD, RESET);
        scanf("%d", &op);

        switch (op)
        {
        case 1:
        {
            int i;
            float monto;

            mostrarEncabezado("REGISTRAR DEPÓSITO");
            
            if (lista_clientes[id].ncuentas == 0)
            {
                mostrarAdvertencia("No tiene cuentas registradas.");
                pausar();
                break;
            }

            printf("%sSUS CUENTAS DISPONIBLES:%s\n\n", BOLD, RESET);
            dibujarLinea('-', 50);
            
            for (i = 0; i < lista_clientes[id].ncuentas; i++)
            {
                printf("%sCuenta #%d%s\n", BOLD, i + 1, RESET);
                printf("  • Número de cuenta: %s%d%s\n", YELLOW, lista_clientes[id].cuentas[i].num_cuenta, RESET);
                printf("  • Adeudo actual: %s$%.2f%s\n", RED, lista_clientes[id].cuentas[i].adeudo, RESET);
                printf("  • Límite de crédito: %s$%.2f%s\n", GREEN, lista_clientes[id].cuentas[i].limite, RESET);
                dibujarLinea('-', 50);
            }

            printf("\n%sIngrese el número de cuenta:%s ", BOLD, RESET);
            scanf("%d", &cntsel);

            flag = 0;
            for (i = 0; i < lista_clientes[id].ncuentas; i++)
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
                    do
                    {
                        flag = 1;
                        printf("\n%sMonto del depósito: $%s", BOLD, RESET);
                        scanf("%f", &monto);

                        if (monto <= 0)
                        {
                            flag = 0;
                            mostrarError("El monto debe ser mayor a $0");
                        }
                    } while (flag == 0);

                    // Actualizar el adeudo
                    lista_clientes[id].cuentas[cntsel].adeudo -= monto;
                    
                    if (lista_clientes[id].cuentas[cntsel].adeudo < 0)
                        lista_clientes[id].cuentas[cntsel].adeudo = 0;

                    guardarClientesEnArchivo();

                    printf("\n");
                    dibujarLinea('=', 50);
                    mostrarExito("DEPÓSITO REGISTRADO EXITOSAMENTE");
                    printf("\n%sResumen de la transacción:%s\n", BOLD, RESET);
                    printf("  • Monto depositado: %s$%.2f%s\n", GREEN, monto, RESET);
                    printf("  • Nuevo adeudo: %s$%.2f%s\n", YELLOW, lista_clientes[id].cuentas[cntsel].adeudo, RESET);
                    printf("  • Cuenta: %s%d%s\n", CYAN, lista_clientes[id].cuentas[cntsel].num_cuenta, RESET);
                    dibujarLinea('=', 50);
                }
                else
                {
                    mostrarExito("La cuenta no tiene adeudos pendientes.");
                }
            }
            else
            {
                mostrarError("Número de cuenta no válido.");
            }
            pausar();
            break;
        }

        case 2:
        {
            int i;
            float monto_base;
            float interes;
            float meses;
            float adeudo;
            int confirmacion;

            mostrarEncabezado("REGISTRAR COMPRA A CRÉDITO");
            
            if (lista_clientes[id].ncuentas == 0)
            {
                mostrarAdvertencia("No tiene cuentas registradas.");
                pausar();
                break;
            }

            printf("%sSUS CUENTAS DISPONIBLES:%s\n\n", BOLD, RESET);
            dibujarLinea('-', 50);
            
            for (i = 0; i < lista_clientes[id].ncuentas; i++)
            {
                float disponible = lista_clientes[id].cuentas[i].limite - lista_clientes[id].cuentas[i].adeudo;
                printf("%sCuenta #%d%s\n", BOLD, i + 1, RESET);
                printf("  • Número de cuenta: %s%d%s\n", YELLOW, lista_clientes[id].cuentas[i].num_cuenta, RESET);
                printf("  • Adeudo actual: %s$%.2f%s\n", RED, lista_clientes[id].cuentas[i].adeudo, RESET);
                printf("  • Límite de crédito: %s$%.2f%s\n", GREEN, lista_clientes[id].cuentas[i].limite, RESET);
                printf("  • Crédito disponible: %s$%.2f%s\n", BLUE, disponible, RESET);
                dibujarLinea('-', 50);
            }

            printf("\n%sIngrese el número de cuenta:%s ", BOLD, RESET);
            scanf("%d", &cntsel);

            flag = 0;
            for (i = 0; i < lista_clientes[id].ncuentas; i++)
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
                do
                {
                    flag = 1;
                    printf("\n%sMonto de la compra: $%s", BOLD, RESET);
                    scanf("%f", &monto_base);

                    if (monto_base <= 0)
                    {
                        flag = 0;
                        mostrarError("El monto debe ser mayor a $0");
                    }
                } while (flag == 0);

                do
                {
                    flag = 1;
                    printf("\n%sPLAZOS DISPONIBLES:%s\n", BOLD, RESET);
                    printf("  %s[3]%s meses - Interés: 6%%\n", CYAN, RESET);
                    printf("  %s[6]%s meses - Interés: 12%%\n", CYAN, RESET);
                    printf("  %s[9]%s meses - Interés: 18%%\n", CYAN, RESET);
                    printf("\n%sSeleccione el plazo (3, 6 o 9):%s ", BOLD, RESET);
                    scanf("%f", &meses);

                    if (meses != 3 && meses != 6 && meses != 9)
                    {
                        flag = 0;
                        mostrarError("Plazo no válido. Debe ser 3, 6 o 9 meses.");
                    }
                } while (flag == 0);

                interes = meses * 0.02f;
                adeudo = monto_base * (1.0f + interes);

                if (lista_clientes[id].cuentas[cntsel].limite >= lista_clientes[id].cuentas[cntsel].adeudo + adeudo)
                {
                    printf("\n");
                    dibujarLinea('=', 50);
                    printf("%s%sRESUMEN DE LA COMPRA A CRÉDITO%s\n", BOLD, BLUE, RESET);
                    dibujarLinea('=', 50);
                    printf("\n");
                    printf("  %sMonto base:%s         $%.2f\n", BOLD, RESET, monto_base);
                    printf("  %sPlazo:%s              %d meses\n", BOLD, RESET, (int)meses);
                    printf("  %sTasa de interés:%s    %.0f%%\n", BOLD, RESET, interes * 100);
                    printf("  %sIntereses:%s          $%.2f\n", BOLD, RESET, monto_base * interes);
                    dibujarLinea('-', 50);
                    printf("  %sTOTAL A PAGAR:%s      %s$%.2f%s\n", BOLD, RESET, GREEN, adeudo, RESET);
                    dibujarLinea('-', 50);
                    printf("\n  %sEstado de la cuenta después de la compra:%s\n", BOLD, RESET);
                    printf("  • Nuevo adeudo:      %s$%.2f%s\n", YELLOW, lista_clientes[id].cuentas[cntsel].adeudo + adeudo, RESET);
                    printf("  • Límite de crédito: %s$%.2f%s\n", GREEN, lista_clientes[id].cuentas[cntsel].limite, RESET);
                    printf("  • Crédito restante:  %s$%.2f%s\n\n", BLUE, 
                           lista_clientes[id].cuentas[cntsel].limite - (lista_clientes[id].cuentas[cntsel].adeudo + adeudo), RESET);

                    printf("%s¿Confirmar la compra?%s\n", BOLD, RESET);
                    printf("  %s[1]%s Sí, confirmar\n", GREEN, RESET);
                    printf("  %s[2]%s No, cancelar\n\n", RED, RESET);
                    printf("%sSeleccione:%s ", BOLD, RESET);
                    scanf("%d", &confirmacion);

                    if (confirmacion != 1)
                    {
                        mostrarAdvertencia("Compra cancelada por el usuario.");
                        break;
                    }

                    lista_clientes[id].cuentas[cntsel].adeudo += adeudo;
                    lista_clientes[id].cuentas[cntsel].meses_inicial = (int)meses;

                    guardarClientesEnArchivo();
                    
                    mostrarExito("COMPRA REGISTRADA EXITOSAMENTE");
                    printf("    Cuenta No. %s%d%s actualizada\n", CYAN, 
                           lista_clientes[id].cuentas[cntsel].num_cuenta, RESET);
                }
                else
                {
                    mostrarError("El monto excede el límite de crédito disponible.");
                    printf("    Crédito disponible: %s$%.2f%s\n", YELLOW,
                           lista_clientes[id].cuentas[cntsel].limite - lista_clientes[id].cuentas[cntsel].adeudo, RESET);
                }
            }
            else
            {
                mostrarError("Número de cuenta no válido.");
            }
            pausar();
            break;
        }

        case 3:
        {
            int i;
            float total_adeudo = 0;
            float total_limite = 0;

            mostrarEncabezado("ESTADO DE CUENTAS");
            
            printf("%sCliente:%s %s\n", BOLD, RESET, lista_clientes[id].nombre);
            printf("%sUsuario:%s %s\n\n", BOLD, RESET, lista_clientes[id].usuario);

            if (lista_clientes[id].ncuentas == 0)
            {
                mostrarAdvertencia("No tiene cuentas registradas.");
                pausar();
                break;
            }

            dibujarLinea('=', 60);
            printf("%s%sDETALLE DE CUENTAS DE CRÉDITO%s\n", BOLD, BLUE, RESET);
            dibujarLinea('=', 60);
            
            for (i = 0; i < lista_clientes[id].ncuentas; i++)
            {
                float disponible = lista_clientes[id].cuentas[i].limite - lista_clientes[id].cuentas[i].adeudo;
                float porcentaje_uso = (lista_clientes[id].cuentas[i].adeudo / lista_clientes[id].cuentas[i].limite) * 100;
                
                printf("\n%s📊 CUENTA #%d%s\n", CYAN, i + 1, RESET);
                dibujarLinea('-', 40);
                printf("  %sNúmero de cuenta:%s     %d\n", BOLD, RESET, lista_clientes[id].cuentas[i].num_cuenta);
                printf("  %sAdeudo actual:%s        %s$%.2f%s\n", BOLD, RESET, 
                       RED, lista_clientes[id].cuentas[i].adeudo, RESET);
                printf("  %sLímite de crédito:%s    %s$%.2f%s\n", BOLD, RESET,
                       GREEN, lista_clientes[id].cuentas[i].limite, RESET);
                printf("  %sCrédito disponible:%s   %s$%.2f%s\n", BOLD, RESET,
                       BLUE, disponible, RESET);
                printf("  %sUso del crédito:%s      %.1f%%\n", BOLD, RESET, porcentaje_uso);
                
                // Barra de progreso visual del uso del crédito
                printf("  %sEstado:%s [", BOLD, RESET);
                int barras = (int)(porcentaje_uso / 5);
                int j;
                for (j = 0; j < 20; j++) {
                    if (j < barras) {
                        if (porcentaje_uso < 50) printf("%s█%s", GREEN, RESET);
                        else if (porcentaje_uso < 80) printf("%s█%s", YELLOW, RESET);
                        else printf("%s█%s", RED, RESET);
                    } else {
                        printf("░");
                    }
                }
                printf("] %.1f%%\n", porcentaje_uso);
                
                total_adeudo += lista_clientes[id].cuentas[i].adeudo;
                total_limite += lista_clientes[id].cuentas[i].limite;
            }
            
            printf("\n");
            dibujarLinea('=', 60);
            printf("%s%sRESUMEN TOTAL%s\n", BOLD, YELLOW, RESET);
            dibujarLinea('=', 60);
            printf("  %sTotal adeudado:%s       %s$%.2f%s\n", BOLD, RESET, RED, total_adeudo, RESET);
            printf("  %sLímite total:%s         %s$%.2f%s\n", BOLD, RESET, GREEN, total_limite, RESET);
            printf("  %sDisponible total:%s     %s$%.2f%s\n", BOLD, RESET, BLUE, total_limite - total_adeudo, RESET);
            dibujarLinea('=', 60);
            
            pausar();
            break;
        }

        case 4:
            mostrarExito("Sesión cerrada correctamente.");
            printf("    %sGracias por usar nuestros servicios.%s\n", CYAN, RESET);
            pausar();
            break;

        default:
            mostrarError("Opción no válida. Por favor, seleccione una opción del menú.");
            pausar();
            break;
        }
    }
}

// =============================================
// 8. MENU ADMINISTRADOR
// =============================================

void menu_administrador()
{
    int op;
    char password_admin[50];
    int intentos = 3;

    mostrarEncabezado("ACCESO ADMINISTRATIVO");

    // Solicitar contraseña de administrador
    printf("%s⚠ ÁREA RESTRINGIDA%s\n\n", RED, RESET);
    
    while (intentos > 0)
    {
        printf("%sIngrese la contraseña de administrador:%s ", BOLD, RESET);
        scanf("%49s", password_admin);
        
        // Contraseña de administrador por defecto: "admin123"
        if (strcmp(password_admin, "admin123") == 0)
        {
            mostrarExito("Acceso administrativo autorizado");
            break;
        }
        else
        {
            intentos--;
            if (intentos > 0)
            {
                mostrarError("Contraseña incorrecta.");
                printf("    Intentos restantes: %s%d%s\n", YELLOW, intentos, RESET);
            }
            else
            {
                mostrarError("Demasiados intentos fallidos. Acceso bloqueado.");
                pausar();
                return;
            }
        }
    }

    op = 0;
    while (op != 8)
    {
        mostrarEncabezado("PANEL DE ADMINISTRACIÓN");
        
        printf("%sESTADÍSTICAS DEL SISTEMA:%s\n", BOLD, RESET);
        printf("  • Clientes registrados: %s%d%s\n", GREEN, total_clientes, RESET);
        printf("  • Cuentas totales: %s%d%s\n\n", BLUE, toc, RESET);
        
        dibujarLinea('-', 50);
        printf("\n%sMENÚ DE OPCIONES:%s\n\n", BOLD, RESET);
        printf("  %s[1]%s Registrar nuevo cliente\n", CYAN, RESET);
        printf("  %s[2]%s Registrar nueva cuenta\n", CYAN, RESET);
        printf("  %s[3]%s Ver listado de clientes\n", CYAN, RESET);
        printf("  %s[4]%s Ver listado de cuentas\n", CYAN, RESET);
        printf("  %s[5]%s Eliminar cliente\n", CYAN, RESET);
        printf("  %s[6]%s Eliminar cuenta\n", CYAN, RESET);
        printf("  %s[7]%s Generar reportes\n", CYAN, RESET);
        printf("  %s[8]%s Salir del panel administrativo\n\n", RED, RESET);

        printf("%sSeleccione una opción:%s ", BOLD, RESET);
        scanf("%d", &op);
        limpiarBuffer();

        switch (op)
        {
        case 1:
        {
            Cliente c1;

            mostrarEncabezado("REGISTRO DE NUEVO CLIENTE");

            if (total_clientes >= MAX_CLIENTES)
            {
                mostrarError("Se ha alcanzado el límite máximo de clientes.");
                pausar();
                break;
            }

            printf("%sCOMPLETE LOS SIGUIENTES DATOS:%s\n\n", BOLD, RESET);
            
            printf("  %sNombre completo:%s ", BOLD, RESET);
            fgets(c1.nombre, 100, stdin);
            c1.nombre[strcspn(c1.nombre, "\n")] = 0;

            printf("  %sNombre de usuario:%s ", BOLD, RESET);
            scanf("%49s", c1.usuario);

            printf("  %sContraseña:%s ", BOLD, RESET);
            scanf("%49s", c1.password);

            // Asignar datos al nuevo cliente
            lista_clientes[total_clientes].id_cliente = total_clientes;
            lista_clientes[total_clientes].ncuentas = 0;
            strcpy(lista_clientes[total_clientes].nombre, c1.nombre);
            strcpy(lista_clientes[total_clientes].usuario, c1.usuario);
            strcpy(lista_clientes[total_clientes].password, c1.password);

            printf("\n");
            dibujarLinea('=', 50);
            mostrarExito("CLIENTE REGISTRADO EXITOSAMENTE");
            printf("\n%sDatos del nuevo cliente:%s\n", BOLD, RESET);
            printf("  • ID asignado: %s%04d%s\n", GREEN, total_clientes, RESET);
            printf("  • Nombre: %s%s%s\n", CYAN, c1.nombre, RESET);
            printf("  • Usuario: %s%s%s\n", CYAN, c1.usuario, RESET);
            dibujarLinea('=', 50);

            total_clientes++;
            guardarClientesEnArchivo();
            
            pausar();
            break;
        }

        case 2:
        {
            int id;
            int confirmacion;
            float monto_base;
            float interes;
            float meses;
            float adeudo;
            float limite;
            int flag;

            mostrarEncabezado("REGISTRO DE NUEVA CUENTA");

            printf("%sIngrese el ID del cliente:%s ", BOLD, RESET);
            scanf("%d", &id);

            if (id < 0 || id >= total_clientes)
            {
                mostrarError("ID de cliente no válido.");
                pausar();
                break;
            }

            printf("\n%sCliente seleccionado:%s %s\n", BOLD, RESET, lista_clientes[id].nombre);
            printf("%sUsuario:%s %s\n", BOLD, RESET, lista_clientes[id].usuario);
            printf("%sCuentas actuales:%s %d\n\n", BOLD, RESET, lista_clientes[id].ncuentas);

            if (lista_clientes[id].ncuentas >= MAX_CUENTAS)
            {
                mostrarError("El cliente ha alcanzado el límite máximo de cuentas.");
                pausar();
                break;
            }

            printf("%s¿Desea agregar una cuenta de crédito?%s\n", BOLD, RESET);
            printf("  %s[1]%s Sí, continuar\n", GREEN, RESET);
            printf("  %s[2]%s No, cancelar\n\n", RED, RESET);
            printf("%sSeleccione:%s ", BOLD, RESET);
            scanf("%d", &confirmacion);

            if (confirmacion != 1)
                break;

            printf("\n%sCONFIGURACIÓN DE LA CUENTA:%s\n\n", BOLD, RESET);

            do
            {
                flag = 1;
                printf("  %sMonto inicial del crédito: $%s", BOLD, RESET);
                scanf("%f", &monto_base);

                if (monto_base <= 0)
                {
                    flag = 0;
                    mostrarError("El monto debe ser mayor a $0");
                }
            } while (flag == 0);

            do
            {
                flag = 1;
                printf("\n%sPLAZOS DISPONIBLES:%s\n", BOLD, RESET);
                printf("  %s[3]%s meses - Interés: 6%%\n", CYAN, RESET);
                printf("  %s[6]%s meses - Interés: 12%%\n", CYAN, RESET);
                printf("  %s[9]%s meses - Interés: 18%%\n", CYAN, RESET);
                printf("\n  %sPlazo inicial (3, 6 o 9):%s ", BOLD, RESET);
                scanf("%f", &meses);

                if (meses != 3 && meses != 6 && meses != 9)
                {
                    flag = 0;
                    mostrarError("Plazo no válido.");
                }
            } while (flag == 0);

            do
            {
                flag = 1;
                printf("\n  %sLímite de crédito: $%s", BOLD, RESET);
                scanf("%f", &limite);

                if (limite <= 0)
                {
                    flag = 0;
                    mostrarError("El límite debe ser mayor a $0");
                }
                else if (limite < monto_base)
                {
                    flag = 0;
                    mostrarError("El límite no puede ser menor al monto inicial");
                }
            } while (flag == 0);

            interes = meses * 0.02f;
            adeudo = monto_base * (1.0f + interes);

            printf("\n");
            dibujarLinea('=', 50);
            printf("%s%sRESUMEN DE LA CUENTA%s\n", BOLD, BLUE, RESET);
            dibujarLinea('=', 50);
            printf("\n");
            printf("  %sMonto base:%s         $%.2f\n", BOLD, RESET, monto_base);
            printf("  %sPlazo:%s              %d meses\n", BOLD, RESET, (int)meses);
            printf("  %sInterés total:%s      %.0f%%\n", BOLD, RESET, interes * 100);
            printf("  %sAdeudo inicial:%s     $%.2f\n", BOLD, RESET, adeudo);
            printf("  %sLímite de crédito:%s  $%.2f\n", BOLD, RESET, limite);
            dibujarLinea('=', 50);

            printf("\n%s¿Confirmar creación de cuenta?%s\n", BOLD, RESET);
            printf("  %s[1]%s Sí, crear cuenta\n", GREEN, RESET);
            printf("  %s[2]%s No, cancelar\n\n", RED, RESET);
            printf("%sSeleccione:%s ", BOLD, RESET);
            scanf("%d", &confirmacion);

            if (confirmacion != 1)
            {
                mostrarAdvertencia("Creación de cuenta cancelada.");
                pausar();
                break;
            }

            // Crear la cuenta
            lista_clientes[id].cuentas[lista_clientes[id].ncuentas].num_cuenta = 1000 + toc;
            lista_clientes[id].cuentas[lista_clientes[id].ncuentas].adeudo = adeudo;
            lista_clientes[id].cuentas[lista_clientes[id].ncuentas].meses_inicial = (int)meses;
            lista_clientes[id].cuentas[lista_clientes[id].ncuentas].limite = limite;

            printf("\n");
            mostrarExito("CUENTA CREADA EXITOSAMENTE");
            printf("  • Cliente: %s%s%s\n", CYAN, lista_clientes[id].nombre, RESET);
            printf("  • Número de cuenta: %s%d%s\n", GREEN, 1000 + toc, RESET);
            printf("  • Límite de crédito: %s$%.2f%s\n", YELLOW, limite, RESET);

            lista_clientes[id].ncuentas++;
            toc++;
            guardarClientesEnArchivo();
            
            pausar();
            break;
        }

        case 3:
        {
            int i;
            
            mostrarEncabezado("LISTADO DE CLIENTES");

            if (total_clientes == 0)
            {
                mostrarAdvertencia("No hay clientes registrados en el sistema.");
                pausar();
                break;
            }

            printf("%sTotal de clientes:%s %d\n\n", BOLD, RESET, total_clientes);
            
            for (i = 0; i < total_clientes; i++)
            {
                printf("%s╔══════════════════════════════════════════╗%s\n", CYAN, RESET);
                printf("%s║%s %sCLIENTE #%03d%s                             %s║%s\n", 
                       CYAN, RESET, BOLD, i + 1, RESET, CYAN, RESET);
                printf("%s╠══════════════════════════════════════════╣%s\n", CYAN, RESET);
                printf("%s║%s ID:         %s%-28d%s %s║%s\n", 
                       CYAN, RESET, YELLOW, lista_clientes[i].id_cliente, RESET, CYAN, RESET);
                printf("%s║%s Nombre:     %s%-28s%s %s║%s\n", 
                       CYAN, RESET, GREEN, lista_clientes[i].nombre, RESET, CYAN, RESET);
                printf("%s║%s Usuario:    %s%-28s%s %s║%s\n", 
                       CYAN, RESET, BLUE, lista_clientes[i].usuario, RESET, CYAN, RESET);
                printf("%s║%s Contraseña: %s%-28s%s %s║%s\n", 
                       CYAN, RESET, RED, "********", RESET, CYAN, RESET);
                printf("%s║%s Cuentas:    %s%-28d%s %s║%s\n", 
                       CYAN, RESET, YELLOW, lista_clientes[i].ncuentas, RESET, CYAN, RESET);
                printf("%s╚══════════════════════════════════════════╝%s\n\n", CYAN, RESET);
            }
            
            pausar();
            break;
        }

        case 4:
        {
            int i, total_cuentas_sistema = 0;
            float total_adeudos = 0, total_limites = 0;
            
            mostrarEncabezado("LISTADO DE CUENTAS");

            for (i = 0; i < total_clientes; i++)
            {
                if (lista_clientes[i].ncuentas > 0)
                {
                    int j;
                    
                    printf("\n%s════════════════════════════════════════════%s\n", BLUE, RESET);
                    printf("%s%sCLIENTE: %s%s\n", BOLD, CYAN, lista_clientes[i].nombre, RESET);
                    printf("ID: %s%04d%s | Usuario: %s%s%s\n", 
                           YELLOW, lista_clientes[i].id_cliente, RESET,
                           GREEN, lista_clientes[i].usuario, RESET);
                    printf("%s════════════════════════════════════════════%s\n", BLUE, RESET);
                    
                    for (j = 0; j < lista_clientes[i].ncuentas; j++)
                    {
                        float disponible = lista_clientes[i].cuentas[j].limite - lista_clientes[i].cuentas[j].adeudo;
                        
                        printf("\n  %sCuenta #%d%s\n", BOLD, j + 1, RESET);
                        printf("  ├─ Número:      %s%d%s\n", CYAN, lista_clientes[i].cuentas[j].num_cuenta, RESET);
                        printf("  ├─ Adeudo:      %s$%.2f%s\n", RED, lista_clientes[i].cuentas[j].adeudo, RESET);
                        printf("  ├─ Límite:      %s$%.2f%s\n", GREEN, lista_clientes[i].cuentas[j].limite, RESET);
                        printf("  ├─ Disponible:  %s$%.2f%s\n", BLUE, disponible, RESET);
                        printf("  └─ Plazo:       %s%d meses%s\n", YELLOW, lista_clientes[i].cuentas[j].meses_inicial, RESET);
                        
                        total_cuentas_sistema++;
                        total_adeudos += lista_clientes[i].cuentas[j].adeudo;
                        total_limites += lista_clientes[i].cuentas[j].limite;
                    }
                }
            }
            
            if (total_cuentas_sistema == 0)
            {
                mostrarAdvertencia("No hay cuentas registradas en el sistema.");
            }
            else
            {
                printf("\n");
                dibujarLinea('=', 50);
                printf("%s%sESTADÍSTICAS GENERALES%s\n", BOLD, YELLOW, RESET);
                dibujarLinea('=', 50);
                printf("  Total de cuentas:     %s%d%s\n", GREEN, total_cuentas_sistema, RESET);
                printf("  Adeudo total:         %s$%.2f%s\n", RED, total_adeudos, RESET);
                printf("  Límites totales:      %s$%.2f%s\n", GREEN, total_limites, RESET);
                printf("  Disponible total:     %s$%.2f%s\n", BLUE, total_limites - total_adeudos, RESET);
                dibujarLinea('=', 50);
            }
            
            pausar();
            break;
        }

        case 5:
        {
            int id_eliminar;
            int confirmacion;
            int i;

            mostrarEncabezado("ELIMINAR CLIENTE");

            if (total_clientes == 0)
            {
                mostrarAdvertencia("No hay clientes registrados.");
                pausar();
                break;
            }

            printf("%sLISTA DE CLIENTES:%s\n\n", BOLD, RESET);
            for (i = 0; i < total_clientes; i++)
            {
                printf("  %s[ID: %04d]%s %s - Usuario: %s\n", 
                       YELLOW, lista_clientes[i].id_cliente, RESET,
                       lista_clientes[i].nombre, lista_clientes[i].usuario);
            }

            printf("\n%sIngrese el ID del cliente a eliminar:%s ", BOLD, RESET);
            scanf("%d", &id_eliminar);

            if (id_eliminar < 0 || id_eliminar >= total_clientes)
            {
                mostrarError("ID de cliente no válido.");
                pausar();
                break;
            }

            printf("\n%s⚠ ATENCIÓN:%s Esta acción eliminará:\n", RED, RESET);
            printf("  • Cliente: %s%s%s\n", CYAN, lista_clientes[id_eliminar].nombre, RESET);
            printf("  • Usuario: %s%s%s\n", CYAN, lista_clientes[id_eliminar].usuario, RESET);
            printf("  • Todas sus %s%d%s cuentas asociadas\n", YELLOW, lista_clientes[id_eliminar].ncuentas, RESET);

            printf("\n%s¿Está seguro de eliminar este cliente?%s\n", BOLD, RESET);
            printf("  %s[1]%s Sí, eliminar\n", RED, RESET);
            printf("  %s[2]%s No, cancelar\n\n", GREEN, RESET);
            printf("%sSeleccione:%s ", BOLD, RESET);
            scanf("%d", &confirmacion);

            if (confirmacion != 1)
            {
                mostrarAdvertencia("Eliminación cancelada.");
                pausar();
                break;
            }

            // Mover todos los clientes posteriores una posición hacia atrás
            for (i = id_eliminar; i < total_clientes - 1; i++)
            {
                lista_clientes[i] = lista_clientes[i + 1];
                lista_clientes[i].id_cliente = i; // Actualizar ID
            }

            total_clientes--;
            guardarClientesEnArchivo();

            mostrarExito("Cliente eliminado exitosamente.");
            pausar();
            break;
        }

        case 6:
        {
            int id_cliente;
            int num_cuenta_eliminar;
            int confirmacion;
            int i, j;
            int cuenta_encontrada = 0;

            mostrarEncabezado("ELIMINAR CUENTA");

            printf("%sIngrese el ID del cliente propietario:%s ", BOLD, RESET);
            scanf("%d", &id_cliente);

            if (id_cliente < 0 || id_cliente >= total_clientes)
            {
                mostrarError("ID de cliente no válido.");
                pausar();
                break;
            }

            if (lista_clientes[id_cliente].ncuentas == 0)
            {
                mostrarAdvertencia("Este cliente no tiene cuentas.");
                pausar();
                break;
            }

            printf("\n%sCUENTAS DEL CLIENTE %s:%s\n\n", BOLD, lista_clientes[id_cliente].nombre, RESET);
            for (i = 0; i < lista_clientes[id_cliente].ncuentas; i++)
            {
                printf("  %s[Cuenta #%d]%s\n", CYAN, i + 1, RESET);
                printf("    • Número: %s%d%s\n", YELLOW, lista_clientes[id_cliente].cuentas[i].num_cuenta, RESET);
                printf("    • Adeudo: %s$%.2f%s\n", RED, lista_clientes[id_cliente].cuentas[i].adeudo, RESET);
                printf("    • Límite: %s$%.2f%s\n\n", GREEN, lista_clientes[id_cliente].cuentas[i].limite, RESET);
            }

            printf("%sIngrese el número de cuenta a eliminar:%s ", BOLD, RESET);
            scanf("%d", &num_cuenta_eliminar);

            // Buscar la cuenta
            for (i = 0; i < lista_clientes[id_cliente].ncuentas; i++)
            {
                if (lista_clientes[id_cliente].cuentas[i].num_cuenta == num_cuenta_eliminar)
                {
                    cuenta_encontrada = 1;

                    printf("\n%s⚠ ATENCIÓN:%s Esta acción eliminará la cuenta:\n", RED, RESET);
                    printf("  • Número: %s%d%s\n", YELLOW, num_cuenta_eliminar, RESET);
                    printf("  • Adeudo: %s$%.2f%s\n", RED, lista_clientes[id_cliente].cuentas[i].adeudo, RESET);

                    printf("\n%s¿Está seguro de eliminar esta cuenta?%s\n", BOLD, RESET);
                    printf("  %s[1]%s Sí, eliminar\n", RED, RESET);
                    printf("  %s[2]%s No, cancelar\n\n", GREEN, RESET);
                    printf("%sSeleccione:%s ", BOLD, RESET);
                    scanf("%d", &confirmacion);

                    if (confirmacion == 1)
                    {
                        // Mover las cuentas posteriores una posición hacia atrás
                        for (j = i; j < lista_clientes[id_cliente].ncuentas - 1; j++)
                        {
                            lista_clientes[id_cliente].cuentas[j] = lista_clientes[id_cliente].cuentas[j + 1];
                        }
                        lista_clientes[id_cliente].ncuentas--;
                        guardarClientesEnArchivo();
                        mostrarExito("Cuenta eliminada exitosamente.");
                    }
                    else
                    {
                        mostrarAdvertencia("Eliminación cancelada.");
                    }
                    break;
                }
            }

            if (!cuenta_encontrada)
            {
                mostrarError("Número de cuenta no encontrado.");
            }

            pausar();
            break;
        }

        case 7:
        {
            mostrarEncabezado("GENERACIÓN DE REPORTES");
            
            printf("%sGenerando reportes del sistema...%s\n\n", BOLD, RESET);
            
            generarReporteClientes();
            generarReporteCuentas();
            
            printf("\n%sReportes generados exitosamente:%s\n", BOLD, RESET);
            printf("  %s✓%s %s%s%s\n", GREEN, RESET, YELLOW, REPORTE_CLIENTES, RESET);
            printf("  %s✓%s %s%s%s\n", GREEN, RESET, YELLOW, REPORTE_CUENTAS, RESET);
            
            pausar();
            break;
        }

        case 8:
            mostrarExito("Saliendo del panel administrativo...");
            pausar();
            break;

        default:
            mostrarError("Opción no válida. Seleccione una opción del menú.");
            pausar();
            break;
        }
    }
}

// =============================================
// 9. MENU PRINCIPAL
// =============================================

void menu_principal()
{
    int opcion = 0;
    
    while (opcion != 4)
    {
        mostrarEncabezado("SISTEMA BANCARIO v2.0");
        
        printf("%s", CYAN);
        printf("    ╔════════════════════════════╗\n");
        printf("    ║       BANCO DIGITAL        ║\n");
        printf("    ║   Su banco de confianza    ║\n");
        printf("    ╚════════════════════════════╝\n");
        printf("%s\n", RESET);
        
        printf("%sMENÚ PRINCIPAL:%s\n\n", BOLD, RESET);
        printf("  %s[1]%s  Acceso Administrativo\n", CYAN, RESET);
        printf("  %s[2]%s  Portal del Cliente\n", CYAN, RESET);
        printf("  %s[3]%s  Ver Estructuras de Datos\n", CYAN, RESET);
        printf("  %s[4]%s  Salir del Sistema\n\n", RED, RESET);
        
        printf("%sSeleccione una opción:%s ", BOLD, RESET);
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
            visualizarEstructuras();
            break;
            
        case 4:
            mostrarEncabezado("CERRANDO SISTEMA");
            
            printf("%sGuardando datos...%s\n", BOLD, RESET);
            guardarClientesEnArchivo();
            
            printf("\n%s", GREEN);
            printf("    ╔════════════════════════════════╗\n");
            printf("    ║   Gracias por usar nuestro     ║\n");
            printf("    ║        Sistema Bancario        ║\n");
            printf("    ║         ¡Hasta pronto!         ║\n");
            printf("    ╚════════════════════════════════╝\n");
            printf("%s\n", RESET);
            
            exit(0);
            break;
            
        default:
            mostrarError("Opción no válida. Por favor seleccione una opción del menú.");
            pausar();
            break;
        }
    }
}

// =============================================
// 10. FUNCIÓN MAIN
// =============================================

int main(void)
{
    // Configurar la codificación UTF-8 para Windows
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
    // =========================================
    // PASO 1: Inicializar el sistema
    // =========================================
    mostrarEncabezado("INICIALIZANDO SISTEMA");
    
    printf("%sCargando datos...%s\n\n", BOLD, RESET);
    
    // =========================================
    // PASO 2: Cargar datos existentes
    // =========================================
    cargarClientesDesdeArchivo();
    
    if (total_clientes == 0)
    {
        printf("%sℹ Primera ejecución detectada.%s\n", YELLOW, RESET);
        printf("  Creando estructura de datos inicial...\n");
        
        // Crear un cliente de prueba
        strcpy(lista_clientes[0].nombre, "Usuario Demo");
        strcpy(lista_clientes[0].usuario, "demo");
        strcpy(lista_clientes[0].password, "1234");
        lista_clientes[0].id_cliente = 0;
        lista_clientes[0].ncuentas = 0;
        total_clientes = 1;
        
        guardarClientesEnArchivo();
        
        printf("\n%s✓ Cliente demo creado:%s\n", GREEN, RESET);
        printf("    Usuario: %sdemo%s\n", CYAN, RESET);
        printf("    Contraseña: %s1234%s\n", CYAN, RESET);
        printf("    ID: %s0%s\n", CYAN, RESET);
    }
    else
    {
        printf("%s✓ Datos cargados exitosamente%s\n", GREEN, RESET);
        printf("    Clientes en sistema: %s%d%s\n", CYAN, total_clientes, RESET);
    }
    
    printf("\n%s✓ Sistema inicializado correctamente%s\n", GREEN, RESET);
    
    printf("\n%sNOTA:%s Contraseña de administrador: %sadmin123%s\n", YELLOW, RESET, CYAN, RESET);
    
    pausar();
    
    // =========================================
    // PASO 3: Iniciar el menú principal
    // =========================================
    menu_principal();
    
    return 0;
}