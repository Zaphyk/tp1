#include <stdbool.h>
#include "copa.h"

juego_t inicializar_juego()
{
	juego_t juego;
	inicializar_laberinto(&juego);
	return juego;
}

char leer_tecla(char* tecla)
{
	scanf("%s", tecla);
}

void mostrar_resultado(int resultado)
{
	printf("\n%s\n", resultado == 1 ? "HAS GANADO" : "HAS PERDIDO");
}


int main()
{
	bool estado = 0;
	char laberinto_temporal[TAMANIO][TAMANIO];
	juego_t juego = inicializar_juego();

	while(estado == 0)
	{
		char tecla;
		leer_tecla(&tecla);
		if(es_movimiento_valido(tecla))
		{
			mover_jugador(&(juego.jugador), tecla);
			// mover_rival();
			actualizar_juego(&juego);
			actualizar_laberinto(juego, laberinto_temporal);
			mostrar_laberinto(laberinto_temporal);
		}
		estado = estado_juego();
	}
	mostrar_resultado(estado);
	return 0;
}