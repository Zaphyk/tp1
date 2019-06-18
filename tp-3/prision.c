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
const char* ARCHIVO_CRUCIO_TEMP = "crucio.tmp";
const char* ARCHIVO_IMPERIUS_TEMP = "imperius.tmp";
const char* FORMATO_ARCHIVO_LIBERADOS = "liberados_%s.txt";
const char* LIBERADOS_FORMATO_ESCRITURA = "%s\n";
const char* LIBERADOS_FORMATO_LECTURA = "%[^\n]\n";

/*
 * PRE CONDICIONES: Un tipo de comando valido y un char** con parametros ya validados
 * POST CONDICIONES: Ejecuta el comando dado. Escribe un mensaje de error si falla.
 */
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

/*
 * PRE CONDICIONES: Un FILE* valido, no NULL.
 * POST CONDICIONES: Lee 1 preso_t del archivo dado y devuelve el resultado.
 */
size_t leer_preso(preso_t* preso, FILE* archivo)
{
	return fread(preso, sizeof(preso_t), 1, archivo);
}

/*
 * PRE CONDICIONES: Un FILE* valido, no NULL.
 * POST CONDICIONES: Escribe el preso_t dado en el archivo dado.
 */
void escribir_preso(preso_t* preso, FILE* archivo)
{
	fwrite(preso, sizeof(preso_t), 1, archivo);
}

/*
 * PRE CONDICIONES: Un char** con parametros ya validados
 * POST CONDICIONES: Ejecuta el comando perdonables. Devuelve 0 en caso de ser exitoso o -1 en caso de fallar.
 */
int procesar_perdonables(char** parametros)
{
	char* nombre_archivo = parametros[0];
	FILE* crucio = fopen(ARCHIVO_CRUCIO, "rb");
	FILE* imperius = fopen(ARCHIVO_IMPERIUS, "rb");
	FILE* perdonables = fopen(nombre_archivo, "wb");
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
		if (leidos_crucio > 0 && (leidos_imperius <= 0 || strcmp(preso_crucio.nombre, preso_imperius.nombre) < 0))
		{
			escribir_preso(&preso_crucio, perdonables);
			leidos_crucio = leer_preso(&preso_crucio, crucio);
		}
		if(leidos_imperius > 0 && (leidos_crucio <= 0 || strcmp(preso_imperius.nombre, preso_crucio.nombre) < 0))
		{
			escribir_preso(&preso_imperius, perdonables);
			leidos_imperius = leer_preso(&preso_imperius, imperius);
		}
		if(leidos_imperius > 0 && leidos_crucio > 0 && strcmp(preso_crucio.nombre, preso_imperius.nombre) == 0)
		{
			leidos_crucio = leer_preso(&preso_crucio, crucio);
			leidos_imperius = leer_preso(&preso_imperius, imperius);
		}
	} while(leidos_imperius > 0 || leidos_crucio > 0);

	fclose(crucio);
	fclose(imperius);
	fclose(perdonables);
	return 0;
}

/*
 * PRE CONDICIONES: Un nombre de archivo valido.
 * POST CONDICIONES: Devuelve un bool que representa si el archivo existe o no.
 */
bool existe_archivo(char* nombre)
{
	FILE* archivo = fopen(nombre, "r");
	if (archivo == NULL) return false;
	fclose(archivo);
	return true;
}

/*
 * PRE CONDICIONES: Un char* con suficiente lugar para el nombre y un string de fecha valido
 * POST CONDICIONES: Genera el nombre del archivo liberados_aaaamm.txt con esa fecha usando el formato ya definido.
 */
void crear_nombre_liberados(char* nombre, char* fecha)
{
	sprintf(nombre, FORMATO_ARCHIVO_LIBERADOS, fecha);
}

/*
 * PRE CONDICIONES: Un char** con parametros ya validados
 * POST CONDICIONES: Ejecuta el comando liberar. Devuelve 0 en caso de ser exitoso o -1 en caso de fallar.
 */
int procesar_liberar(char** parametros)
{
	char* nombre_perdonables = parametros[0];
	fecha_t fecha_pedida = parsear_fecha(parametros[1]);
	int conducta_pedida = parsear_conducta(parametros[2]);
	char nombre_liberados[MAX_NOMBRE_LIBERADOS];
	crear_nombre_liberados(nombre_liberados, fecha_pedida.texto);

	if (existe_archivo(nombre_liberados)) return 0;

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
			fprintf(liberados, LIBERADOS_FORMATO_ESCRITURA, preso.nombre);
		}
	}

	fclose(perdonables);
	fclose(liberados);
	return 0;
}

/*
 * PRE CONDICIONES: Un src y dest validos y existentes
 * POST CONDICIONES: Borra el archivo de destino y el mueve el de origen a su lugar.
 */
int reemplazar_archivo(const char* src, const char* dst)
{
	if(remove(dst) != 0)
		return -1;
	if(rename(src, dst) != 0)
		return -1;
	return 0;
}

/*
 * PRE CONDICIONES: Un char** con parametros ya validados
 * POST CONDICIONES: Ejecuta el comando actualizar. Devuelve 0 en caso de ser exitoso o -1 en caso de fallar.
 */
int procesar_actualizar(char** parametros)
{
	fecha_t fecha_pedida = parsear_fecha(parametros[0]);
	char nombre_liberados[MAX_NOMBRE_LIBERADOS];
	crear_nombre_liberados(nombre_liberados, fecha_pedida.texto);

	FILE* imperius_r = fopen(ARCHIVO_IMPERIUS, "r");
	FILE* crucio_r = fopen(ARCHIVO_CRUCIO, "r");
	FILE* imperius_w = fopen(ARCHIVO_IMPERIUS_TEMP, "w");
	FILE* crucio_w = fopen(ARCHIVO_CRUCIO_TEMP, "w");
	FILE* liberados = fopen(nombre_liberados, "r");
	if (imperius_r == NULL || crucio_r == NULL || imperius_w == NULL || crucio_w == NULL || liberados == NULL)
	{
		if (imperius_r != NULL) fclose(imperius_r);
		if (crucio_r != NULL) fclose(crucio_r);
		if (imperius_w != NULL) fclose(imperius_w);
		if (crucio_w != NULL) fclose(crucio_w);
		if (liberados != NULL) fclose(liberados);
		return -1;
	}

	preso_t preso_crucio;
	preso_t preso_imperius;
	size_t leidos_crucio = leer_preso(&preso_crucio, crucio_r);
	size_t leidos_imperius = leer_preso(&preso_imperius, imperius_r);

	int leidos;
	char nombre[MAX_NOMBRE];
	while((leidos = fscanf(liberados, LIBERADOS_FORMATO_LECTURA, nombre)) > 0)
	{
		while(leidos_crucio > 0 && strcmp(nombre, preso_crucio.nombre) >= 0)
		{
			if(strcmp(nombre, preso_crucio.nombre) != 0)
				escribir_preso(&preso_crucio, crucio_w);
			leidos_crucio = leer_preso(&preso_crucio, crucio_r);
		}

		while(leidos_imperius > 0 && strcmp(nombre, preso_imperius.nombre) >= 0)
		{
			if(strcmp(nombre, preso_imperius.nombre) != 0)
				escribir_preso(&preso_imperius, imperius_w);
			leidos_imperius = leer_preso(&preso_imperius, imperius_r);
		}
	}

	while(leidos_crucio > 0)
	{
		if(strcmp(nombre, preso_crucio.nombre) != 0)
			escribir_preso(&preso_crucio, crucio_w);
		leidos_crucio = leer_preso(&preso_crucio, crucio_r);
	}

	while(leidos_imperius > 0)
	{
		if(strcmp(nombre, preso_imperius.nombre) != 0)
			escribir_preso(&preso_imperius, imperius_w);
		leidos_imperius = leer_preso(&preso_imperius, imperius_r);
	}

	fclose(imperius_r);
	fclose(crucio_r);
	fclose(imperius_w);
	fclose(crucio_w);
	fclose(liberados);
	return reemplazar_archivo(ARCHIVO_IMPERIUS_TEMP, ARCHIVO_IMPERIUS) | reemplazar_archivo(ARCHIVO_CRUCIO_TEMP, ARCHIVO_CRUCIO);
}

/*
 * PRE CONDICIONES: Un char** con parametros ya validados
 * POST CONDICIONES: Ejecuta el comando mostrar_liberados. Devuelve 0 en caso de ser exitoso o -1 en caso de fallar.
 */
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
	while((leidos = fscanf(liberados, LIBERADOS_FORMATO_LECTURA, nombre)) > 0)
	{
		printf(" * %s\n", nombre);
	}
	fclose(liberados);
	return 0;
}

/*
 * PRE CONDICIONES: Un char** con parametros ya validados
 * POST CONDICIONES: Ejecuta el comando ayuda. Devuelve 0 en caso de ser exitoso o -1 en caso de fallar.
 */
int procesar_ayuda(char** parametros)
{
	mostrar_ayuda();
	return 0;
}

/*
 * PRE CONDICIONES: Ninguna.
 * POST CONDICIONES: Muestra por pantalla la lista de comandos y sus descripciones.
 */
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