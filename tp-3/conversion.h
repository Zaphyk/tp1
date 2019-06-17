#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#include <stdbool.h>

#define MAX_FECHA 7

#define COMANDO_PERDONABLES 0
#define COMANDO_LIBERAR 1
#define COMANDO_ACTUALIZAR 2
#define COMANDO_MOSTRAR_LIBERADOS 3
#define COMANDO_AYUDA 4

#define CONDUCTA_MALA_LETRA 'M'
#define CONDUCTA_REGULAR_LETRA 'R'
#define CONDUCTA_BUENA_LETRA 'B'

extern const int MAX_CONDUCTAS;
extern const char CONDUCTAS[];

typedef struct fecha {
	bool es_valida;
	int anio;
	int mes;
} fecha_t;

fecha_t parsear_fecha(char* fecha);
int parsear_conducta(char* conducta);
bool fecha_valida(char* fecha);
bool conducta_valida(char* conducta);

#endif