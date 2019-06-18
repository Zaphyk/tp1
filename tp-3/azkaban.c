#include "validador.h"
#include "prision.h"

int main(int argc, char** argv)
{
	comando_t comando = validar_estructura(argc, argv);
	if (!comando.es_valido)
	{
		mostrar_ayuda();
	}
	else
	{
		ejecutar_comando(comando.tipo, comando.parametros);
	}
	return 0;
}