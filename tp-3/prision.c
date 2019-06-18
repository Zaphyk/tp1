#include "prision.h"
#include "conversion.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NOMBRE 200
#define MAX_NOMBRE_LIBERADOS 20

typedef struct preso {
	char nombre[MAX_NOMBRE];
	unsigned int edad;
	char conducta;
	unsigned int pabellon;
	unsigned int celda;
	char fecha[MAX_FECHA];
	int maldicion_realizada;
} preso_t;

typedef struct procesador_comando {
	int tipo;
	int (*ejecutor)(char**);
} procesador_comando_t;

int procesar_perdonables(char** parametros);
int procesar_liberar(char** parametros);
int procesar_actualizar(char** parametros);
int procesar_mostrar_liberados(char** parametros);
int procesar_ayuda(char** parametros);

const procesador_comando_t PROCESADORES[] =
{
	(procesador_comando_t) {COMANDO_PERDONABLES, &procesar_perdonables},
	(procesador_comando_t) {COMANDO_LIBERAR, &procesar_liberar},
	(procesador_comando_t) {COMANDO_ACTUALIZAR, &procesar_actualizar},
	(procesador_comando_t) {COMANDO_MOSTRAR_LIBERADOS, &procesar_mostrar_liberados},
	(procesador_comando_t) {COMANDO_AYUDA, &procesar_ayuda}
};

const char* ARCHIVO_CRUCIO = "crucio.dat";
const char* ARCHIVO_IMPERIUS = "imperius.dat";
const char* FORMATO_ARCHIVO_LIBERADOS = "liberados_%s.txt";
const char* LIBERADOS_FORMATO = "%s\n";

void ejecutar_comando(int tipo, char** parametros)
{
	procesador_comando_t procesador;
	for(int i = 0; i < TOTAL_COMANDOS; ++i)
	{
		if(PROCESADORES[i].tipo == tipo)
			procesador = PROCESADORES[i];
	}
	int resultado = (*(procesador.ejecutor))(parametros);
	if (resultado != 0) printf("Error al ejecutar comando.\n");
}

size_t leer_preso(preso_t* preso, FILE* archivo)
{
	return fread(preso, sizeof(preso_t), 1, archivo);
}

void escribir_preso(preso_t* preso, FILE* archivo)
{
	fwrite(preso, sizeof(preso_t), 1, archivo);
}

int procesar_perdonables(char** parametros)
{
	char* nombre_archivo = parametros[0];
	FILE* crucio = fopen(ARCHIVO_CRUCIO, "r");
	FILE* imperius = fopen(ARCHIVO_IMPERIUS, "r");
	FILE* perdonables = fopen(nombre_archivo, "w");
	if (crucio == NULL || imperius == NULL || perdonables == NULL)
	{
		if(crucio != NULL) fclose(crucio);
		if(imperius != NULL) fclose(imperius);
		if(perdonables != NULL) fclose(perdonables);
		return -1;
	}

	preso_t preso_crucio;
	preso_t preso_imperius;
	size_t leidos_crucio = leer_preso(&preso_crucio, crucio);
	size_t leidos_imperius = leer_preso(&preso_imperius, imperius);

	do {
		if (strcmp(preso_crucio.nombre, preso_imperius.nombre) < 0)
		{
			escribir_preso(&preso_crucio, perdonables);
			leidos_crucio = leer_preso(&preso_crucio, crucio);
		}
		else if(strcmp(preso_crucio.nombre, preso_imperius.nombre) > 0)
		{
			escribir_preso(&preso_imperius, perdonables);
			leidos_imperius = leer_preso(&preso_imperius, imperius);
		}
		else
		{
			leidos_crucio = leer_preso(&preso_crucio, crucio);
			leidos_imperius = leer_preso(&preso_imperius, imperius);
		}
	} while(leidos_imperius > 0 && leidos_crucio > 0);

	while(leidos_crucio > 0)
	{
		leidos_crucio = leer_preso(&preso_crucio, crucio);
		escribir_preso(&preso_crucio, perdonables);
	}

	while(leidos_imperius > 0)
	{
		leidos_imperius = leer_preso(&preso_imperius, imperius);
		escribir_preso(&preso_crucio, perdonables);
	}

	fclose(crucio);
	fclose(imperius);
	fclose(perdonables);
	return 0;
}

bool existe_archivo(char* nombre)
{
	FILE* archivo = fopen(nombre, "r");
	if (archivo == NULL) return false;
	fclose(archivo);
	return true;
}

void crear_nombre_liberados(char* nombre, char* fecha)
{
	sprintf(nombre, FORMATO_ARCHIVO_LIBERADOS, fecha);
}

int procesar_liberar(char** parametros)
{
	char* nombre_perdonables = parametros[0];
	fecha_t fecha_pedida = parsear_fecha(parametros[1]);
	int conducta_pedida = parsear_conducta(parametros[2]);
	char nombre_liberados[MAX_NOMBRE_LIBERADOS];
	crear_nombre_liberados(nombre_liberados, fecha_pedida.texto);

	if (existe_archivo(nombre_liberados)) return -1;

	FILE* perdonables = fopen(nombre_perdonables, "r");
	FILE* liberados = fopen(nombre_liberados, "w");
	if (liberados == NULL || perdonables == NULL)
	{
		if (liberados != NULL) fclose(liberados);
		if (perdonables != NULL) fclose(perdonables);
		return -1;
	}

	preso_t preso;
	size_t leidos;
	while ((leidos = leer_preso(&preso, perdonables)) > 0)
	{
		int conducta = parsear_conducta(&(preso.conducta));
		fecha_t fecha = parsear_fecha(preso.fecha);
		if (conducta <= conducta_pedida && comparar_fechas(fecha, fecha_pedida) <= 0)
		{
			fprintf(liberados, LIBERADOS_FORMATO, preso.nombre);
		}
	}

	fclose(perdonables);
	fclose(liberados);
	return 0;
}

int procesar_actualizar(char** parametros)
{
	return 0;
}

int procesar_mostrar_liberados(char** parametros)
{
	fecha_t fecha_pedida = parsear_fecha(parametros[0]);
	char nombre_liberados[MAX_NOMBRE_LIBERADOS];
	crear_nombre_liberados(nombre_liberados, fecha_pedida.texto);

	FILE* liberados = fopen(nombre_liberados, "r");
	if(liberados == NULL) return -1;

	int leidos;
	char nombre[MAX_NOMBRE];
	printf("Presos a liberar en '%s':\n", fecha_pedida.texto);
	while((leidos = fscanf(liberados, LIBERADOS_FORMATO, nombre)) > 0)
	{
		printf(" * %s\n", nombre);
	}
	fclose(liberados);
	return 0;
}

int procesar_ayuda(char** parametros)
{
	mostrar_ayuda();
	return 0;
}

void mostrar_ayuda()
{
	printf("Uso: ./azkaban comando [parametros]\n");
	printf("Comandos:\n");
	char* comandos[] = {
		"perdonables",
		"liberar",
		"actualizar",
		"mostrar_liberados",
		"ayuda"
	};
	char* descripciones[] = {
		"De los 2 archivos con los presos que realizaron la maldicion crea 1 con los que pueden ser perdonados. Ejemplo: ./azkaban perdonables <nombre_archivo>",
		"Crea un archivo de texto con los nombres de los presos que cumplan con las condiciones necesarias. Ejemplo: ./azkaban liberar <archivo_perdonables> aaaamm <B,R,M>",
		"Actualiza ambos ambos archivos binarios en los cuales figuran los presos de la maldicion. Ejemplo: ./azkaban actualizar aaaamm",
		"Muestra por pantalla los presos liberados en un determinado año mes. Ejemplo: ./azkaban mostrar_liberados aaaamm",
		"Muestra esta ayuda."
	};
	for(int i = 0; i < 5; ++i)
	{
		printf("\t%s\t\t%s\n", comandos[i], descripciones[i]);
	}
}