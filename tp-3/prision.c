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
	void (*ejecutor)(char**, int);
} procesador_comando_t;

void procesar_perdonables(char** parametros, int tope);
void procesar_liberar(char** parametros, int tope);
void procesar_actualizar(char** parametros, int tope);
void procesar_mostrar_liberados(char** parametros, int tope);
void procesar_ayuda(char** parametros, int tope);

const procesador_comando_t PROCESADORES[] =
{
	(procesador_comando_t) {COMANDO_PERDONABLES, &procesar_perdonables},
	(procesador_comando_t) {COMANDO_LIBERAR, &procesar_liberar},
	(procesador_comando_t) {COMANDO_ACTUALIZAR, &procesar_actualizar},
	(procesador_comando_t) {COMANDO_MOSTRAR_LIBERADOS, &procesar_mostrar_liberados},
	(procesador_comando_t) {COMANDO_AYUDA, &procesar_ayuda}
};

void ejecutar_comando(int tipo, char** parametros, int tope)
{
	procesador_comando_t procesador;
	for(int i = 0; i < TOTAL_COMANDOS; ++i)
	{
		if(PROCESADORES[i].tipo == tipo)
			procesador = PROCESADORES[i];
	}
	(*(procesador.ejecutor))(parametros, tope);
}

void procesar_perdonables(char** parametros, int tope)
{

}

void procesar_liberar(char** parametros, int tope)
{

}

void procesar_actualizar(char** parametros, int tope)
{

}

void procesar_mostrar_liberados(char** parametros, int tope)
{

}

void procesar_ayuda(char** parametros, int tope)
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