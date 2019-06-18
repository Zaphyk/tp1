#include "conversion.h"
#include <stdlib.h>

const int MAX_CONDUCTAS = 3;
const char CONDUCTAS[] =
{
	CONDUCTA_BUENA_LETRA,
	CONDUCTA_REGULAR_LETRA,
	CONDUCTA_MALA_LETRA
};

/*
 * PRE CONDICIONES: Recibir un string bien formado
 * POST CONDICIONES: Devuelve la cantidad de caracteres del string
 */
int calcular_tope(char* string)
{
	int tope = 0;
	while(string[tope] != '\0')
	{
		tope++;
	}
	return tope;
}

/*
 * PRE CONDICIONES: Recibir un char* con suficiente espacio para poner (termina - empieza) + 1 caracteres.
 * POST CONDICIONES: Una seccion del string dado.
 */
void slice(char* array, char* resultado, int empieza, int termina)
{
	for(int i = empieza; i < termina; ++i)
	{
		resultado[i - empieza] = array[i];
	}
	resultado[termina - empieza] = '\0';
}

/*
 * PRE CONDICIONES: Recibir un string bien formado. Y las fechas en formato aaaamm
 * POST CONDICIONES: Devuelve un objeto fecha
 */
fecha_t parsear_fecha(char* string)
{
	fecha_t fecha = {.es_valida = false, .texto = string};
	int tope = calcular_tope(string);

	if(tope != MAX_FECHA-1) return fecha;

	char anio_str[5];
	slice(string, anio_str, 0, 4);
	fecha.anio = atoi(anio_str);

	char mes_str[3];
	slice(string, mes_str, 4, MAX_FECHA-1);
	fecha.mes = atoi(mes_str);

	fecha.es_valida = fecha.mes > 0 && fecha.mes < 13;
	return fecha;
}

/*
 * PRE CONDICIONES: Recibir un string bien formado
 * POST CONDICIONES: Devuelve una conducta valida o -1 en caso de ser invalida
 */
int parsear_conducta(char* conducta)
{
	int tope = calcular_tope(conducta);
	if(tope != 1) return -1;
	for(int i = 0; i < MAX_CONDUCTAS; ++i)
	{
		if(conducta[0] == CONDUCTAS[i])
			return i;
	}
	return -1;
}

/*
 * PRE CONDICIONES: Un par de fecha inicializadas correctamente
 * POST CONDICIONES: Devuelve -1 si la fecha1 es menor, 0 si son iguales y 1 si es mayor
 */
int comparar_fechas(fecha_t fecha1, fecha_t fecha2)
{
	if (fecha1.anio < fecha2.anio)
		return -1;
	else if (fecha1.anio > fecha2.anio)
		return 1;
	if (fecha1.mes < fecha2.mes)
		return -1;
	else if (fecha1.mes > fecha2.mes)
		return 1;
	return 0;
}

/*
 * PRE CONDICIONES: Recibir un string bien formado
 * POST CONDICIONES: Devuelve un bool que representa si la fecha es valida
 */
bool fecha_valida(char* fecha)
{
	return (parsear_fecha(fecha)).es_valida;
}

/*
 * PRE CONDICIONES: Recibir un string bien formado
 * POST CONDICIONES: Devuelve un bool que representa si la conducta es valida
 */
bool conducta_valida(char* string)
{
	return parsear_conducta(string) != -1;
}