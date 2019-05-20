#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "laberinto.h"
#include "copa.h"

const char TECLA_ARRIBA = 'w';
const char TECLA_ABAJO = 's';
const char TECLA_IZQUIERDA = 'a';
const char TECLA_DERECHA = 'd';
const char AYUDA_ESFINGE = 'F';

typedef struct direccion {
    int x;
    int y;
} direccion_t;

const int TOTAL_DIRECCIONES_RIVAL = 4;
const int RIVAL_CANTIDAD_PASOS = 4;
const direccion_t MOVIMIENTOS_RIVAL[TOTAL_DIRECCIONES_RIVAL] =
{
	(direccion_t) {1, 0},
	(direccion_t) {0, -1},
	(direccion_t) {-1, 0},
	(direccion_t) {0, 1}
};

bool posicion_valida(int x, int y, char laberinto[TAMANIO][TAMANIO]);
bool copa_visible(jugador_t jugador);
bool buscar_ayudas(char codigo, ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas, ayuda_t encontradas[TOTAL_AYUDAS], int* tope_encontradas);
void copiar_mapa(char original[TAMANIO][TAMANIO], char copia[TAMANIO][TAMANIO]);
void copiar_obstaculos(obstaculo_t obstaculos[TOTAL_OBSTACULOS], int tope_obstaculos, char copia[TAMANIO][TAMANIO]);
void copiar_ayudas(ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas, char copia[TAMANIO][TAMANIO]);
void copiar_rival(rival_t rival, char copia[TAMANIO][TAMANIO]);
void copiar_jugador(jugador_t jugador, char copia[TAMANIO][TAMANIO]);
void copiar_copa(copa_t copa, char copia[TAMANIO][TAMANIO]);
direccion_t tecla_a_direccion(char tecla);

/*
 * Inicializará todas las estructuras con los valores correspondientes,
 * creará el laberinto, posicionará todos los elementos, etc.
 */
void inicializar_laberinto(juego_t* juego)
{
	inicializar_paredes_laberinto(juego->laberinto_original);
	inicializar_copa();
	inicializar_obstaculos();
	inicializar_ayudas();
	inicializar_rival();
	inicializar_jugador();
	// ​ Copa - Escreguto - Acromántula - Boggart - Impedimenta - Riddikulus - Pociones - Esfinge - Rival - Jugador​ .
}

/*
 * Determinará si el caracter ingresado es válido, esto es, es el caracter ‘a’ o
 * ‘s’ o ‘d’ o ‘w’ y además el jugador puede moverse en esa dirección, o sea,
 * hay pasillo.
 */
bool es_movimiento_valido(juego_t* juego, char tecla)
{
	bool es_valido = (tecla == TECLA_ARRIBA || tecla == TECLA_IZQUIERDA || tecla == TECLA_ABAJO || tecla == TECLA_DERECHA);
	direccion_t direccion = tecla_a_direccion(tecla);
	int x = juego->jugador.posicion.fil + direccion.x;
	int y = juego->jugador.posicion.col + direccion.y;
	return es_valido && posicion_valida(x, y, juego->laberinto_original);
}

/*
 * Moverá el jugador hacia la dirección especificada.
 * Dicho movimiento es válido.
 */
void mover_jugador(jugador_t* jugador, char direccion)
{
	direccion_t dir = tecla_a_direccion(direccion);
	jugador->posicion.fil += dir.x;
	jugador->posicion.col += dir.y;
}

/*
 * Moverá el rival a la próxima posición.
 */
void mover_rival(juego_t* juego)
{
	int x;
	int y;
	do
	{
		/* Genero un indice para el array de movimientos. Ej. 41 / 4 = 10 => 10 % 4 = 2 => 2 + 4 = 6 => 6 % 4 = 2 */
		int indice_direccion = (((juego->rival.cantidad_de_pasos / RIVAL_CANTIDAD_PASOS) % TOTAL_DIRECCIONES_RIVAL) + TOTAL_DIRECCIONES_RIVAL) % TOTAL_DIRECCIONES_RIVAL;
		direccion_t direccion = MOVIMIENTOS_RIVAL[indice_direccion];
		x = juego->rival.posicion.x + direccion.x;
		y = juego->rival.posicion.y + direccion.y;
		juego->rival.cantidad_de_pasos++;
	} while(!posicion_valida(x, y, juego->laberinto_original));
}

/*
 * Actualizará el juego. Restará vida si el jugador está sobre un obstáculo
 * o lo eliminará si cuenta con el hechizo, aprenderá hechizos y todo lo
 * que pueda suceder luego de un turno.
 */
void actualizar_juego(juego_t* juego)
{

}

/*
 * Devolverá el estado del juego, 1 ganado, 0 en curso, -1 perdido.
 */
int estado_juego(juego_t juego)
{
	if (juego.jugador.posicion.fil == juego.copa.posicion.fil && juego.jugador.posicion.col == juego.copa.posicion.col) return 1;
	if (juego.rival.posicion.fil == juego.copa.posicion.fil && juego.rival.posicion.col == juego.copa.posicion.col) return -1;
	if (juego.jugador.vida <= 0) return -1;
	return 0;
}

/*
 * Devolverá una coordenada aleatoria dentro del rango TAMANIOxTAMANIO.
 * No valida que dicha coordenada coincida con un pasillo ni que exista
 * otro objeto en esa posición.
 */
coordenada_t posicion_aleatoria()
{
	coordenada_t coordenada;
	coordenada.fil = rand() % TAMANIO;
	coordenada.col = rand() % TAMANIO;
	return coordenada;
}

/*
 * Actualizará la matriz mostrada al usuario, con los elementos presentes
 * en el juego.
 */
void actualizar_laberinto(juego_t juego, char laberinto[TAMANIO][TAMANIO])
{
	copiar_mapa(juego.laberinto_original, laberinto);
	copiar_obstaculos(juego.obstaculos, juego.tope_obstaculos, laberinto);
	copiar_ayudas(juego.ayudas, juego.tope_ayudas, laberinto);
	if (copa_visible(juego.jugador))
	{
		copiar_copa(laberinto);
	}
	copiar_rival(laberinto);
	copiar_jugador(laberinto);
}

/*
 * Mostrará el laberinto por pantalla.
 */
void mostrar_laberinto(char laberinto[TAMANIO][TAMANIO])
{
	for(int i = 0; i < TAMANIO; ++i)
	{
		for(int j = 0; j < TAMANIO; ++j)
		{
			printf("%c", laberinto[i][j]);
		}
		printf("\n");
	}
}

/*
 * Metodos Auxiliares
 */

bool posicion_valida(int x, int y, char laberinto[TAMANIO][TAMANIO])
{
	return x >= 0 && y >= 0 && x < TAMANIO && y < TAMANIO && laberinto[x][y] == VACIO;
}

bool buscar_ayudas(char codigo, ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas, ayuda_t encontradas[TOTAL_AYUDAS], int* tope_encontradas)
{
	(*tope_encontradas) = 0;
	for(int i = 0; i < tope_ayudas; ++i)
	{
		if (ayudas[i].codigo == codigo)
			encontradas[(*tope_encontradas)++] = ayudas[i];
	}
	return (*tope_encontradas) != 0;
}

bool copa_visible(jugador_t jugador)
{
	int tope_encontradas;
	ayuda_t encontradas[TOTAL_AYUDAS];
	return jugador.vida <= 15 || buscar_ayudas(AYUDA_ESFINGE, jugador.ayudas, jugador.tope_ayudas, encontradas, &tope_encontradas);
}

void copiar_mapa(char original[TAMANIO][TAMANIO], char copia[TAMANIO][TAMANIO])
{
	for(int i = 0; i < TAMANIO; ++i)
	{
		for(int j = 0; j < TAMANIO; ++j)
		{
			copia[i][j] = original[i][j];
		}
	}
}

void copiar_obstaculos(obstaculo_t obstaculos[TOTAL_OBSTACULOS], int tope_obstaculos, char copia[TAMANIO][TAMANIO])
{
	for(int i = 0; i < tope_obstaculos; ++i)
	{
		copia[obstaculos[i].posicion.fil][obstaculos[i].posicion.col] = obstaculos[i].codigo;
	}
}

void copiar_ayudas(ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas, char copia[TAMANIO][TAMANIO])
{
	for(int i = 0; i < tope_ayudas; ++i)
	{
		copia[ayudas[i].posicion.fil][ayudas[i].posicion.col] = ayudas[i].codigo;
	}
}

void copiar_rival(rival_t rival, char copia[TAMANIO][TAMANIO])
{
	copia[rival.posicion.fil][rival.posicion.col] = rival.codigo;
}


void copiar_jugador(jugador_t jugador, char copia[TAMANIO][TAMANIO])
{
	copia[jugador.posicion.fil][jugador.posicion.col] = jugador.codigo;
}

void copiar_copa(copa_t copa, char copia[TAMANIO][TAMANIO])
{
	copia[copa.posicion.fil][copa.posicion.col] = copa.codigo;
}

direccion_t tecla_a_direccion(char tecla)
{
	direccion_t direccion;
	direccion.x = tecla == TECLA_DERECHA ? 1 : direccion.x;
	direccion.x = tecla == TECLA_IZQUIERDA ? -1 : direccion.x;
	direccion.y = tecla == TECLA_ARRIBA ? 1 : direccion.y;
	direccion.y = tecla == TECLA_ABAJO ? -1 : direccion.y;
	return direccion;
}