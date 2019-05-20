#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "laberinto.h"
#include "copa.h"

/* Auxiliares */

typedef struct direccion {
    int x;
    int y;
    char tecla;
} direccion_t;

/* Constantes del juego */

const char CODIGO_COPA = 'C';
const int VIDA_POR_TURNO = 3;
const int INDICE_DERECHA = 0;
const int INDICE_ABAJO = 1;
const int INDICE_IZQUIERDA = 2;
const int INDICE_ARRIBA = 3;
const int TOTAL_DIRECCIONES = 4;
const direccion_t MOVIMIENTOS_DISPONIBLES[] =
{
    (direccion_t) {0, 1, TECLA_DERECHA},
    (direccion_t) {1, 0, TECLA_ABAJO},
    (direccion_t) {0, -1, TECLA_IZQUIERDA},
    (direccion_t) {-1, 0, TECLA_ARRIBA}
};

/* Constantes del jugador */
const char CODIGO_JUGADOR = 'J';
const int DISTANCIA_MINIMA_COPA = 10;
const int MAXIMA_VIDA_JUGADOR = 500;

/* Constantes de los obstaculos */
const int DANIO_ESGREGUTO = 20;
const int DANIO_ACROMANTULA = 10;
const int DANIO_BOGGART = 15;

/* Constantes de las ayudas */
const int VIDA_POCION = 15;

/* Constantes para el rival */
const char CODIGO_RIVAL = 'G';
const int RIVAL_CANTIDAD_PASOS = 4;

/* Funciones de posicion y direccion auxiliares */
bool posicion_valida(int x, int y, char laberinto[TAMANIO][TAMANIO]);
bool comparar_coordenadas(coordenada_t x, coordenada_t y);
coordenada_t posicion_aleatoria_valida(char laberinto[TAMANIO][TAMANIO]);
direccion_t tecla_a_direccion(char tecla);
coordenada_t posicion_aleatoria_jugador(juego_t juego);

/* Funciones auxiliares para armar el laberinto que se va a mostrar */
void copiar_mapa(char original[TAMANIO][TAMANIO], char copia[TAMANIO][TAMANIO]);
void copiar_obstaculos(obstaculo_t obstaculos[TOTAL_OBSTACULOS], int tope_obstaculos, char copia[TAMANIO][TAMANIO]);
void copiar_ayudas(ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas, char copia[TAMANIO][TAMANIO]);
void copiar_rival(rival_t rival, char copia[TAMANIO][TAMANIO]);
void copiar_jugador(jugador_t jugador, char copia[TAMANIO][TAMANIO]);
void copiar_copa(copa_t copa, char copia[TAMANIO][TAMANIO]);

/* Funciones auxiliares para actualizar el juego */
bool copa_visible(jugador_t jugador);
bool tiene_ayuda(char codigo, ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas);
bool es_pocion(ayuda_t ayuda);
bool es_boggart(obstaculo_t obstaculo);
bool es_escreguto(obstaculo_t obstaculo);
void remover_ayuda(int indice, ayuda_t ayudas[TOTAL_AYUDAS], int* tope_ayudas);
void remover_obstaculo(int indice, obstaculo_t obstaculos[TOTAL_OBSTACULOS], int* tope_obstaculos);
void actualizar_turno(juego_t* juego);
void actualizar_ayudas(juego_t* juego);
void actualizar_obstaculos(juego_t* juego);

/* Funciones auxiliares de inicializacion */
void inicializar_copa(juego_t* juego);
void inicializar_obstaculos(juego_t* juego);
void inicializar_ayudas(juego_t* juego);
void inicializar_rival(juego_t* juego);
void inicializar_jugador(juego_t* juego);

/* Utilidades matematicas */
int min(int x, int y);
int absoluto(int x);
int distancia_manhattan(coordenada_t x, coordenada_t y);

/*
 * Inicializará todas las estructuras con los valores correspondientes,
 * creará el laberinto, posicionará todos los elementos, etc.
 */
void inicializar_laberinto(juego_t* juego)
{
    inicializar_paredes_laberinto(juego->laberinto_original);
    inicializar_copa(juego);
    inicializar_obstaculos(juego);
    inicializar_ayudas(juego);
    inicializar_rival(juego);
    inicializar_jugador(juego);
}

void inicializar_copa(juego_t* juego)
{
    juego->copa = (copa_t) {CODIGO_COPA, posicion_aleatoria_valida(juego->laberinto_original)};
}

void inicializar_obstaculos(juego_t* juego)
{
    juego->tope_obstaculos = 0;
    juego->obstaculos[(juego->tope_obstaculos)++] = (obstaculo_t) {CODIGO_ESCREGUTO, posicion_aleatoria_valida(juego->laberinto_original), DANIO_ESGREGUTO};
    juego->obstaculos[(juego->tope_obstaculos)++] = (obstaculo_t) {CODIGO_ACROMANTULA, posicion_aleatoria_valida(juego->laberinto_original), DANIO_ACROMANTULA};
    juego->obstaculos[(juego->tope_obstaculos)++] = (obstaculo_t) {CODIGO_BOGGART, posicion_aleatoria_valida(juego->laberinto_original), DANIO_BOGGART};
}

void inicializar_ayudas(juego_t* juego)
{
    juego->tope_ayudas = 0;
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_IMPEDIMENTA, posicion_aleatoria_valida(juego->laberinto_original), 0};
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_RIDDIKULUS, posicion_aleatoria_valida(juego->laberinto_original), 0};
    /* Pociones */
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_POCION, posicion_aleatoria_valida(juego->laberinto_original), VIDA_POCION};
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_POCION, posicion_aleatoria_valida(juego->laberinto_original), VIDA_POCION};
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_POCION, posicion_aleatoria_valida(juego->laberinto_original), VIDA_POCION};
    /* Esfinge */
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_ESFINGE, posicion_aleatoria_valida(juego->laberinto_original), 0};
}

void inicializar_rival(juego_t* juego)
{
    juego->rival = (rival_t) {CODIGO_RIVAL, posicion_aleatoria_jugador(*juego), MOVIMIENTOS_DISPONIBLES[INDICE_DERECHA].tecla, 0};
}

void inicializar_jugador(juego_t* juego)
{
    juego->jugador = (jugador_t) {CODIGO_JUGADOR, MAXIMA_VIDA_JUGADOR, posicion_aleatoria_jugador(*juego), 0, {}};
}

/*
 * Determinará si el caracter ingresado es válido, esto es, es el caracter ‘a’ o
 * ‘s’ o ‘d’ o ‘w’ y además el jugador puede moverse en esa dirección, o sea,
 * hay pasillo.
 */
bool es_movimiento_valido(juego_t* juego, char tecla)
{
    if (tecla != TECLA_ARRIBA && tecla != TECLA_IZQUIERDA && tecla != TECLA_ABAJO && tecla != TECLA_DERECHA) return false;
    direccion_t direccion = tecla_a_direccion(tecla);
    int x = juego->jugador.posicion.fil + direccion.x;
    int y = juego->jugador.posicion.col + direccion.y;
    return posicion_valida(x, y, juego->laberinto_original);
}

bool posicion_valida(int x, int y, char laberinto[TAMANIO][TAMANIO])
{
    return x >= 0 && y >= 0 && x < TAMANIO && y < TAMANIO && laberinto[x][y] == VACIO;
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
        /* Genero un indice para el array de movimientos a partir de la cantidad de pasos. Ej. 41 / 4 = 10 => 10 % 4 = 2 => [2] => (0, 1) */
        int indice_direccion = (juego->rival.cantidad_pasos / RIVAL_CANTIDAD_PASOS) % TOTAL_DIRECCIONES;
        direccion_t direccion = MOVIMIENTOS_DISPONIBLES[indice_direccion];
        x = juego->rival.posicion.fil + direccion.x;
        y = juego->rival.posicion.col + direccion.y;
        juego->rival.cantidad_pasos = (juego->rival.cantidad_pasos + 1) % (RIVAL_CANTIDAD_PASOS * TOTAL_DIRECCIONES);
        juego->rival.direccion = direccion.tecla;
    }
    while (!posicion_valida(x, y, juego->laberinto_original));
    juego->rival.posicion.fil = x;
    juego->rival.posicion.col = y;
}

/*
 * Actualizará el juego. Restará vida si el jugador está sobre un obstáculo
 * o lo eliminará si cuenta con el hechizo, aprenderá hechizos y todo lo
 * que pueda suceder luego de un turno.
 */
void actualizar_juego(juego_t* juego)
{
    actualizar_turno(juego);
    if (estado_juego(*juego) == 0)
    {
        actualizar_ayudas(juego);
        actualizar_obstaculos(juego);
    }
}

void actualizar_turno(juego_t* juego)
{
    juego->jugador.vida -= VIDA_POR_TURNO;
}

void actualizar_ayudas(juego_t* juego)
{
    for(int i = juego->tope_ayudas-1; i > -1; --i)
    {
        if (comparar_coordenadas(juego->ayudas[i].posicion, juego->jugador.posicion))
        {
            if (es_pocion(juego->ayudas[i]))
                juego->jugador.vida = min(juego->jugador.vida + VIDA_POCION, MAXIMA_VIDA_JUGADOR);
            else
                juego->jugador.ayudas[juego->jugador.tope_ayudas++] = juego->ayudas[i];
            remover_ayuda(i, juego->ayudas, &(juego->tope_ayudas));
        }
    }
}

void actualizar_obstaculos(juego_t* juego)
{
    for(int i = juego->tope_obstaculos - 1; i > -1; --i)
    {
        if (comparar_coordenadas(juego->obstaculos[i].posicion, juego->jugador.posicion))
        {
            int danio = juego->obstaculos[i].danio;
            if (es_boggart(juego->obstaculos[i]) && tiene_ayuda(CODIGO_RIDDIKULUS, juego->jugador.ayudas, juego->jugador.tope_ayudas))
                danio = 0;
            if (es_escreguto(juego->obstaculos[i]) && tiene_ayuda(CODIGO_IMPEDIMENTA, juego->jugador.ayudas, juego->jugador.tope_ayudas))
                danio = 0;
            juego->jugador.vida -= danio;
            remover_obstaculo(i, juego->obstaculos, &(juego->tope_obstaculos));
        }
    }
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
        copiar_copa(juego.copa, laberinto);
    }
    copiar_rival(juego.rival, laberinto);
    copiar_jugador(juego.jugador, laberinto);
}

bool copa_visible(jugador_t jugador)
{
    return jugador.vida <= 15 || tiene_ayuda(CODIGO_ESFINGE, jugador.ayudas, jugador.tope_ayudas);
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

bool es_boggart(obstaculo_t obstaculo)
{
    return obstaculo.codigo == CODIGO_BOGGART;
}

bool es_escreguto(obstaculo_t obstaculo)
{
    return obstaculo.codigo == CODIGO_ESCREGUTO;
}

bool es_pocion(ayuda_t ayuda)
{
    return ayuda.codigo == CODIGO_POCION;
}

void remover_ayuda(int indice, ayuda_t ayudas[TOTAL_AYUDAS], int* tope_ayudas)
{
    for(int i = indice; i < (*tope_ayudas) - 1; ++i)
    {
        ayudas[i] = ayudas[i+1];
    }
    (*tope_ayudas)--;
}

void remover_obstaculo(int indice, obstaculo_t obstaculos[TOTAL_OBSTACULOS], int* tope_obstaculos)
{
    for(int i = indice; i < (*tope_obstaculos) - 1; ++i)
    {
        obstaculos[i] = obstaculos[i+1];
    }
    (*tope_obstaculos)--;
}

bool comparar_coordenadas(coordenada_t x, coordenada_t y)
{
    return x.fil == y.fil && y.col == x.col;
}

coordenada_t posicion_aleatoria_valida(char laberinto[TAMANIO][TAMANIO])
{
    coordenada_t coordenada;
    do
    {
        coordenada = posicion_aleatoria();
    }
    while(!posicion_valida(coordenada.fil, coordenada.col, laberinto));
    return coordenada;
}

coordenada_t posicion_aleatoria_jugador(juego_t juego)
{
    coordenada_t coordenada;
    do
    {
        coordenada = posicion_aleatoria_valida(juego.laberinto_original);
    }
    while(distancia_manhattan(coordenada, juego.copa.posicion) <= DISTANCIA_MINIMA_COPA);
    return coordenada;
}

bool tiene_ayuda(char codigo, ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas)
{
    for(int i = 0; i < tope_ayudas; ++i)
    {
        if (ayudas[i].codigo == codigo)
            return true;
    }
    return false;
}


direccion_t tecla_a_direccion(char tecla)
{
    for(int i = 0; i < TOTAL_DIRECCIONES; ++i)
    {
        if(MOVIMIENTOS_DISPONIBLES[i].tecla == tecla)
            return MOVIMIENTOS_DISPONIBLES[i];
    }
    return (direccion_t) {};
}

/* Utilidades matematicas */

int min(int x, int y)
{
    return x < y ? x : y;
}

int absoluto(int x)
{
    return x > 0 ? x : -x;
}

int distancia_manhattan(coordenada_t x, coordenada_t y)
{
    return absoluto(x.fil - y.fil) + absoluto(x.col - y.col);
}
