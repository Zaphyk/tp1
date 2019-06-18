#ifndef __PRISION_H__
#define __PRISION_H__

/*
 * PRE CONDICIONES: Ninguna.
 * POST CONDICIONES: Muestra por pantalla la lista de comandos y sus descripciones.
 */
void mostrar_ayuda();

/*
 * PRE CONDICIONES: Un tipo de comando valido y un char** con parametros ya validados
 * POST CONDICIONES: Ejecuta el comando dado.
 */
void ejecutar_comando(int tipo, char** parametros);

#endif