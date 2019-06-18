#ifndef __VALIDADOR_H__
#define __VALIDADOR_H__

#include <stdbool.h>

typedef struct comando {
	bool es_valido;
	int tipo;
	char** parametros;
	int tope_parametros;
} comando_t;

/*
 * PRE CONDICIONES: Que argc tenga la cantidad de argumentos y argv los argumentos.
 * POST CONDICIONES: Devuelve un comando_t con los parametros validados en caso de estar todo bien o en su defecto un comando_t no valido.
 */
comando_t validar_estructura(int argc, char** argv);

#endif