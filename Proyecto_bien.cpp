#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<windows.h>

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
typedef struct {
    int num_cuenta;         // Numero unico de cuenta
    float adeudo;           // Monto actual que se debe
    float limite;           // Limite maximo de credito
    int meses_inicial;      // Plazo: 3, 6 o 9 meses
} CuentaCredito;
// Estructura para Cliente
typedef struct {
    int id_cliente;          // ID unico del cliente
    char nombre[100];        // Nombre completo
    char usuario[50];        // Para login
    char password[50];       // Para login
    CuentaCredito cuentas[MAX_CUENTAS];///////////integracion de la estructura de cuentas//////////////
    int ncuentas; 			//Numero de cunetas con las que cuenta el usuario//////
} Cliente;

Cliente lista_clientes[MAX_CLIENTES];   // Arreglo de clientes
CuentaCredito lista_cuentas[MAX_CUENTAS]; // Arreglo de cuentas

int total_clientes = 0;     // Cuantos clientes hay registrados
int total_cuentas = 0;      // Cuantas cuentas hay registradas
int toc=0;					/////////////////////////////////////////////////Variable para registrar cuentas

// =============================================
// 3. DEFINIR NOMBRES DE ARCHIVOS
// =============================================

const char* ARCHIVO_CLIENTES = "datos_clientes.dat";
const char* ARCHIVO_CUENTAS = "datos_cuentas.dat";
const char* REPORTE_CLIENTES = "reporte_clientes.txt";
const char* REPORTE_CUENTAS = "reporte_cuentas.txt";

// =============================================
// 4. FUNCIONES PARA MANEJO DE ARCHIVOS
// =============================================

// Funcion para GUARDAR clientes en archivo binario
void guardarClientesEnArchivo() {
    FILE* archivo = fopen(ARCHIVO_CLIENTES, "wb");
    if (archivo == NULL) {
        printf("Error: No se pudo crear el archivo de clientes.\n");
        return;
    }
    
    // Primero guardamos cuantos clientes hay
    fwrite(&total_clientes, sizeof(int), 1, archivo);
    fwrite(&toc, sizeof(int),1,archivo);
    
    // Luego guardamos cada cliente
    for (int i = 0; i < total_clientes; i++) {
        fwrite(&lista_clientes[i], sizeof(Cliente), 1, archivo);
        
    }
    //guardamos cada cuenta con la nueva variable
        for (int a = 0; a < toc; a++) {
        fwrite(&lista_cuentas[a], sizeof(Cliente), 1, archivo);

    }
    
    fclose(archivo);
    printf("Clientes guardados en '%s'\n", ARCHIVO_CLIENTES);
}

// Funcion para CARGAR clientes desde archivo binario
void cargarClientesDesdeArchivo() {
    FILE* archivo = fopen(ARCHIVO_CLIENTES, "rb");
    if (archivo == NULL) {
        printf("Info: Archivo de clientes no existe. Se iniciara vacio.\n");
        total_clientes = 0;
        return;
    }
    
    // Primero leemos cuantos clientes hay
    fread(&total_clientes, sizeof(int), 1, archivo);
    fread(&toc, sizeof(int),1,archivo);
    
    // Luego leemos cada cliente
    for (int i = 0; i < total_clientes; i++) {
        fread(&lista_clientes[i], sizeof(Cliente), 1, archivo);
    }
    ////cada cuenta con la nueva variable
    for (int a = 0; a < toc; a++) {
        fread(&lista_cuentas[a], sizeof(Cliente), 1, archivo);

    }
    
    
    fclose(archivo);
    printf("Clientes cargados desde '%s' (%d clientes)\n", 
           ARCHIVO_CLIENTES, total_clientes);
}

// Funcion para GUARDAR cuentas en archivo binario//////////////////ahora ya no se usa esta funcion
void guardarCuentasEnArchivo() {
    FILE* archivo = fopen(ARCHIVO_CUENTAS, "wb");
    if (archivo == NULL) {
        printf("Error: No se pudo crear el archivo de cuentas.\n");
        return;
    }
    
    // Primero guardamos cuantas cuentas hay
    fwrite(&total_cuentas, sizeof(int), 1, archivo);
    
    // Luego guardamos cada cuenta
    for (int i = 0; i < total_cuentas; i++) {
        fwrite(&lista_cuentas[i], sizeof(CuentaCredito), 1, archivo);
    }
    
    fclose(archivo);
    printf("Cuentas guardadas en '%s'\n", ARCHIVO_CUENTAS);
}

// Funcion para CARGAR cuentas desde archivo binario//////////////no se usa esta funcion
void cargarCuentasDesdeArchivo() {
    FILE* archivo = fopen(ARCHIVO_CUENTAS, "rb");
    if (archivo == NULL) {
        printf("Info: Archivo de cuentas no existe. Se iniciara vacio.\n");
        total_cuentas = 0;
        return;
    }
    
    // Primero leemos cuantas cuentas hay
    fread(&total_cuentas, sizeof(int), 1, archivo);
    
    // Luego leemos cada cuenta
    for (int i = 0; i < total_cuentas; i++) {
        fread(&lista_cuentas[i], sizeof(CuentaCredito), 1, archivo);
    }
    
    fclose(archivo);
    printf("Cuentas cargados desde '%s' (%d cuentas)\n", 
           ARCHIVO_CUENTAS, total_cuentas);
}

// Funcion para GENERAR REPORTE de clientes en texto
void generarReporteClientes() {
    FILE* archivo = fopen(REPORTE_CLIENTES, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo crear el reporte de clientes.\n");
        return;
    }
    
    fprintf(archivo, "=== REPORTE DE CLIENTES DEL BANCO ===\n");
    fprintf(archivo, "======================================\n\n");
    
    if (total_clientes == 0) {
        fprintf(archivo, "No hay clientes registrados.\n");
    } else {
        for (int i = 0; i < total_clientes; i++) {
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
void generarReporteCuentas() {
    FILE* archivo = fopen(REPORTE_CUENTAS, "w");
    if (archivo == NULL) {
        printf("Error: No se pudo crear el reporte de cuentas.\n");
        return;
    }
    
    fprintf(archivo, "=== REPORTE DE CUENTAS DE CREDITO ===\n");
    fprintf(archivo, "======================================\n\n");
    
    if (total_cuentas == 0) {
        fprintf(archivo, "No hay cuentas registradas.\n");
    } else {
        fprintf(archivo, "%-10s %-10s %-12s %-12s %-15s\n", 
                "Cuenta", "Dueno ID", "Adeudo", "Limite", "Meses");
        fprintf(archivo, "--------------------------------------------------\n");
        
        for (int i = 0; i < total_cuentas; i++) {
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

void demostrarEstructuras() {
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
    if (total_clientes > 0 && total_cuentas > 0) {
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
	printf("========BIENVENIDO=======\n");
	printf("ingresa tu ID:\n");
	scanf("%d",&id);
	printf("Bienvenido %s \n",lista_clientes[id].usuario);
	printf("Ingresa tu contraseña: \n");
	int c;///////////////////////////////////////////////////////////
while ((c = getchar()) != '\n' && c != EOF);////////////////////////////////Limpieza de buffer para que no se trabe jejejeje//////////////////////////////////////
	gets(p);	
	if(strcmp(p,lista_clientes[id].password) !=0) /////Verificar si la contraseña es correcta//////////////
	{
		printf("Contraseña incorrecta cerrando el programa");
		exit(0);
	}
	while(op!=4)
	{
			printf("========BIENVENIDO=======\n");
			printf("Seleccione una opcion\n1- Registrar deposito \n2- Registrar compra \n3-Ver saldo de cuenta de credito \n4- Salir \n");
			scanf("%d",&op);
	switch(op)
	{
		case 1:
			printf("Usuario %s",lista_clientes[id].usuario); //////////////Deposito
			printf("Registrar deposito:");
			break;
		case 2:
			printf("Usuario %s",lista_clientes[id].usuario);/////////////Compra
			printf("Registrar compra \n");
			break;
		case 3:
			printf("Usuario %s",lista_clientes[id].usuario);////////////Saldo de cuenta
			printf("Ver saldo de cuenta de credito");
			break;
		case 4:
			break;
		default:
			printf("opcion no valida");
			break;
	}
		
	}

}


////////////////////////////////Menu administrador////////////////////////////////////////////////////7


void menu_administrador()
{
		printf("========BIENVENIDO=======\n");
	int op;
	while(op!=5)
	{
	printf("========Administrador=======\n");
	printf("Seleccione una opcion\n1- Registrar cliente \n2- Registrar cuenta \n3-Ver clientes \n4- Ver cuentas \n5-Regresar al menu\n");

			scanf("%d",&op);
int c;///////////////////////////////////////////////////////////
while ((c = getchar()) != '\n' && c != EOF);////////////////////////////////Limpieza de buffer para que no se trabe jejejeje//////////////////////////////////////
	switch(op)
	{
///////////////////////////////////////Registrar clientes////////////////////////////////////////////////////////////
		case 1:
			Cliente c1;
			printf("Ingresa nombre del cliente completo\n");
			gets(c1.nombre);
			printf("Ingresa su usuario\n");
			gets(c1.usuario);
			printf("Ingresa su contraseña\n");
			gets(c1.password);
			printf("Su id es: %d \n",total_clientes);
			lista_clientes[total_clientes].id_cliente=total_clientes;
			lista_clientes[total_clientes].ncuentas=0;
			strcpy(lista_clientes[total_clientes].nombre,c1.nombre);
			strcpy(lista_clientes[total_clientes].usuario,c1.usuario);
			strcpy(lista_clientes[total_clientes].password,c1.password);
				total_clientes++;
				guardarClientesEnArchivo();
			break;
/////////////////////////////////////Registrar cuentas///////////////////////////////////////////////////////////////////////
		case 2:
			int id;
			int d;
			printf("Ingrese el id del cliente:");
			scanf("%d",&id);
			if(id>total_clientes)
			{
				printf("Ese usuario no existe");
				break;
			}
			printf("Usuario: %s \n",lista_clientes[id].usuario);
			printf("¿Desea agregar una cuenta de credito al usuario?\n");
			printf("Si(1), No(2)\n");
			scanf("%d",&d);
			if(d==1)
			{
				//////////////lista_clientes[id].ncuentas+=1;////// esta funcion se movió unas lineas abajo para no desperdiciar memoria del arreglo :p
				lista_clientes[id].cuentas[lista_clientes[id].ncuentas].num_cuenta= 1000 + toc;
				lista_clientes[id].cuentas[lista_clientes[id].ncuentas].limite=10000;

				printf("Se a agredado al usuario: %s una cuneta de credito con numero: %d y un limite de 10,000 \n",lista_clientes[id].usuario, lista_clientes[id].cuentas[lista_clientes[id].ncuentas].num_cuenta);
				
				lista_clientes[id].ncuentas+=1;
				
				printf("El usuario cuenta con %d cuentas actualmente \n",lista_clientes[id].ncuentas);
				
			toc++;
			guardarClientesEnArchivo();
		}
			break;
////////////////////////////////////////Visualizar clientes//////////////////////////////////////////////////////////////////////////
		case 3:
			for(int i=0;i<total_clientes;i++)
			{
				printf("cliente id %d \n",lista_clientes[i].id_cliente);
				printf("nombre %s \n",lista_clientes[i].nombre);
				printf("usuario %s \n",lista_clientes[i].usuario);
				printf("contraseña %s \n",lista_clientes[i].password);	
			}
			break;
//////////////////////////////////////Visualizar cuentas de credito////////////////////////////////////////////////////////////////
		case 4:
				for(int i=0;i<total_clientes;i++)
			{
				printf("cliente numero %d \n",lista_clientes[i].id_cliente+1);
				printf("Usuario: %s \n",lista_clientes[i].usuario);
				printf("id: %d \n",lista_clientes[i].id_cliente);
				printf("Cuentas del cliente: %d \n",lista_clientes[i].ncuentas);
				/*for(int f=1;f<=lista_clientes[i].ncuentas;f++)
					{
						printf("Cuenta n: %d \n",f+1);
						printf("Num . cuenta: %d \n",lista_clientes[i].cuentas[f].num_cuenta);  ////////Este for se modifico e
																										simplemente iniciando desde 0 y no 1 
						
					}
					*/
					//////////////////////////////////versio correcta GG chavales//////////////////////////
					for(int f=0;f<lista_clientes[i].ncuentas;f++)
					{
						printf("Cuenta n: %d \n",f+1);
						printf("Num . cuenta: %d \n",lista_clientes[i].cuentas[f].num_cuenta);
						
					}
				//////////////////////////////////////////////////////////////		
			}
			break;
		case 5:
			break;
		default:
			printf("Opción no disponible");
			break;
	}
	}
}


//////////////////////////////////////////////Menu principal//////////////////////////////////////////////////////////////


void menu_principal()
{
	int opcion;

	while(opcion!=3)
	{
	printf("========BIENVENIDO=======\n");
	printf("=========================\n");
	printf("Seleccione una opcion\n1- Administracion \n2- Cliente \n3-Salir\n");
	scanf("%d",&opcion);
	switch(opcion)
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
    
    menu_principal();
    return 0;	
}
