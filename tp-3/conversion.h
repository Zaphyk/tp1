#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#include <stdbool.h>

#define MAX_FECHA 7

#define COMANDO_PERDONABLES 0
#define COMANDO_LIBERAR 1
#define COMANDO_ACTUALIZAR 2
#define COMANDO_MOSTRAR_LIBERADOS 3
#define COMANDO_AYUDA 4
#define TOTAL_COMANDOS 5

#define CONDUCTA_MALA_LETRA 'M'
#define CONDUCTA_REGULAR_LETRA 'R'
#define CONDUCTA_BUENA_LETRA 'B'

extern const int MAX_CONDUCTAS;
extern const char CONDUCTAS[];

typedef struct fecha {
	bool es_valida;
	int anio;
	int mes;
	char* texto;
} fecha_t;

/*
 * PRE CONDICIONES: Recibir un string bien formado
 * POST CONDICIONES: Devuelve una conducta valida o -1 en caso de ser invalida
 */
fecha_t parsear_fecha(char* fecha);

/*
 * PRE CONDICIONES: Recibir un string bien formado
 * POST CONDICIONES: Devuelve una conducta valida o -1 en caso de ser invalida
 */
int parsear_conducta(char* conducta);

/*
 * PRE CONDICIONES: Recibir un string bien formado
 * POST CONDICIONES: Devuelve un bool que representa si la fecha es valida
 */
bool fecha_valida(char* fecha);

/*
 * PRE CONDICIONES: Recibir un string bien formado
 * POST CONDICIONES: Devuelve un bool que representa si la conducta es valida
 */
bool conducta_valida(char* conducta);

/*
 * PRE CONDICIONES: Un par de fecha inicializadas correctamente
 * POST CONDICIONES: Devuelve -1 si la fecha1 es menor, 0 si son iguales y 1 si es mayor
 */
int comparar_fechas(fecha_t fecha1, fecha_t fecha2);

#endif