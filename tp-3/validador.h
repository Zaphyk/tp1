#ifndef __VALIDADOR_H__
#define __VALIDADOR_H__

#include <stdbool.h>

typedef struct comando {
	bool es_valido;
	int tipo;
	char** parametros;
	int tope_parametros;
} comando_t;

comando_t validar_estructura(int argc, char** argv);

#endif