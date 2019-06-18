#include "validador.h"
#include "conversion.h"
#include <string.h>
#include <stdbool.h>

typedef struct estructura_comando {
	bool es_valido;
	char* nombre;
	int tipo;
	bool (*validador)(comando_t);
} estructura_comando_t;

bool validar_parametros_perdonables(comando_t comando);
bool validar_parametros_liberar(comando_t comando);
bool validar_parametros_actualizar(comando_t comando);
bool validar_parametros_mostrar_liberados(comando_t comando);
bool validar_parametros_ayuda(comando_t comando);

const int MAX_PARAMETROS = 4;
const estructura_comando_t COMANDOS_DISPONIBLES[] =
{
	(estructura_comando_t) {true, "perdonables", COMANDO_PERDONABLES, &validar_parametros_perdonables},
	(estructura_comando_t) {true, "liberar", COMANDO_LIBERAR, &validar_parametros_liberar},
	(estructura_comando_t) {true, "actualizar", COMANDO_ACTUALIZAR, &validar_parametros_actualizar},
	(estructura_comando_t) {true, "mostrar_liberados", COMANDO_MOSTRAR_LIBERADOS, &validar_parametros_mostrar_liberados},
	(estructura_comando_t) {true, "ayuda", COMANDO_AYUDA, &validar_parametros_ayuda}
};

/*
 * PRE CONDICIONES: Un nombre bien formado.
 * POST CONDICIONES: Devuelve un estructura_comando_t a partir del nombre del comando.
 */
estructura_comando_t parsear_nombre(char* nombre)
{
	for(int i = 0; i < TOTAL_COMANDOS; ++i) {
		if(strcmp(nombre, COMANDOS_DISPONIBLES[i].nombre) == 0)
			return COMANDOS_DISPONIBLES[i];
	}
	return (estructura_comando_t) {};
}

/*
 * PRE CONDICIONES: Que argc tenga la cantidad de argumentos y argv los argumentos.
 * POST CONDICIONES: Devuelve un comando_t con los parametros validados en caso de estar todo bien o en su defecto un comando_t no valido.
 */
comando_t validar_estructura(int argc, char** argv)
{
	comando_t comando = { .es_valido = false };

	if(argc == 1)
		return comando;

	estructura_comando_t estructura = parsear_nombre(argv[1]);
	if(estructura.es_valido)
	{
		comando.tipo = estructura.tipo;
		comando.tope_parametros = 0;
		char* parametros[MAX_PARAMETROS];
		for(int i = 2; i < argc; ++i)
		{
			parametros[(comando.tope_parametros)++] = argv[i];
		}
		comando.parametros = parametros;
		comando.es_valido = (*(estructura.validador))(comando);
	}
	return comando;
}

/*
 * PRE CONDICIONES: Que el comando dado haya sido inicializado.
 * POST CONDICIONES: Valida los parametros del comando perdonables
 */
bool validar_parametros_perdonables(comando_t comando)
{
	return comando.tope_parametros == 1;
}

/*
 * PRE CONDICIONES: Que el comando dado haya sido inicializado.
 * POST CONDICIONES: Valida los parametros del comando liberar
 */
bool validar_parametros_liberar(comando_t comando)
{
	return comando.tope_parametros == 3 && fecha_valida(comando.parametros[1]) && conducta_valida(comando.parametros[2]);
}

/*
 * PRE CONDICIONES: Que el comando dado haya sido inicializado.
 * POST CONDICIONES: Valida los parametros del comando actualizar
 */
bool validar_parametros_actualizar(comando_t comando)
{
	return comando.tope_parametros == 1 && fecha_valida(comando.parametros[0]);
}

/*
 * PRE CONDICIONES: Que el comando dado haya sido inicializado.
 * POST CONDICIONES: Valida los parametros del comando mostrar_liberados
 */
bool validar_parametros_mostrar_liberados(comando_t comando)
{
	return comando.tope_parametros == 1 && fecha_valida(comando.parametros[0]);
}

/*
 * PRE CONDICIONES: Que el comando dado haya sido inicializado.
 * POST CONDICIONES: Valida los parametros del comando ayuda
 */
bool validar_parametros_ayuda(comando_t comando)
{
	return comando.tope_parametros == 0;
}