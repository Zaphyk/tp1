#include "prision.h"
#include "conversion.h"
#include <stdio.h>

#define MAX_NOMBRE 200

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
	void (*ejecutor)(char**);
} procesador_comando_t;

void procesar_perdonables(char** parametros);
void procesar_liberar(char** parametros);
void procesar_actualizar(char** parametros);
void procesar_mostrar_liberados(char** parametros);
void procesar_ayuda(char** parametros);

const procesador_comando_t PROCESADORES[] =
{
	(procesador_comando_t) {COMANDO_PERDONABLES, &procesar_perdonables},
	(procesador_comando_t) {COMANDO_LIBERAR, &procesar_liberar},
	(procesador_comando_t) {COMANDO_ACTUALIZAR, &procesar_actualizar},
	(procesador_comando_t) {COMANDO_MOSTRAR_LIBERADOS, &procesar_mostrar_liberados},
	(procesador_comando_t) {COMANDO_AYUDA, &procesar_ayuda}
};

const char* PRESOS_CRUCIO = "crucio.dat";
const char* PRESOS_IMPERIUS = "imperius.dat";

void ejecutar_comando(int tipo, char** parametros)
{
	procesador_comando_t procesador;
	for(int i = 0; i < TOTAL_COMANDOS; ++i)
	{
		if(PROCESADORES[i].tipo == tipo)
			procesador = PROCESADORES[i];
	}
	(*(procesador.ejecutor))(parametros);
}

void procesar_perdonables(char** parametros)
{
	/*char* nombre_archivo = parametros[0];
	FILE* ptr = fopen(nombre_archivo, 'w');
	if(ptr != NULL){

	}
	fclose(ptr);*/
}

void procesar_liberar(char** parametros)
{

}

void procesar_actualizar(char** parametros)
{

}

void procesar_mostrar_liberados(char** parametros)
{
	//fecha_t fecha = parsear_fecha(parametros[0]);
}

void procesar_ayuda(char** parametros)
{
	mostrar_ayuda();
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