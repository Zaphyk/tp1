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

#define TECLA_ARRIBA 'w'
#define TECLA_ABAJO 's'
#define TECLA_IZQUIERDA 'a'
#define TECLA_DERECHA 'd'
#define CODIGO_COPA 'C'
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
const int MAXIMA_VIDA_JUGADOR = 50;

/* Constantes de los obstaculos */
const char CODIGO_ESCREGUTO = 'E';
const char CODIGO_ACROMANTULA = 'A';
const char CODIGO_BOGGART = 'B';
const int DANIO_ESGREGUTO = 20;
const int DANIO_ACROMANTULA = 10;
const int DANIO_BOGGART = 15;

/* Constantes de las ayudas */
const char CODIGO_ESFINGE = 'F';
const char CODIGO_IMPEDIMENTA = 'I';
const char CODIGO_RIDDIKULUS = 'R';
const char CODIGO_POCION = 'P';
const int VIDA_POCION = 15;

/* Constantes para el rival */
const char CODIGO_RIVAL = 'G';
const int RIVAL_CANTIDAD_PASOS = 4;

/* Funciones de posicion y direccion auxiliares */
bool posicion_valida(int x, int y, char laberinto[TAMANIO][TAMANIO]);
bool comparar_coordenadas(coordenada_t x, coordenada_t y);
coordenada_t posicion_aleatoria_valida(juego_t juego);
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
    juego->tope_ayudas = 0;
    juego->tope_obstaculos = 0;
    inicializar_paredes_laberinto(juego->laberinto_original);
    inicializar_copa(juego);
    inicializar_obstaculos(juego);
    inicializar_ayudas(juego);
    inicializar_rival(juego);
    inicializar_jugador(juego);
}

/*
 * PRE CONDICIONES: Un juego_t valido
 * POST CONDICIONES: Crea la copa y la posicion en una posicion valida
 */
void inicializar_copa(juego_t* juego)
{
    juego->copa = (copa_t) {CODIGO_COPA, posicion_aleatoria_valida(*juego)};
}

/*
 * PRE CONDICIONES: Un juego_t valido
 * POST CONDICIONES: Inicializa y añade todos los obstaculos en orden en posiciones aleatorias validas
 */
void inicializar_obstaculos(juego_t* juego)
{
    juego->obstaculos[(juego->tope_obstaculos)++] = (obstaculo_t) {CODIGO_ESCREGUTO, posicion_aleatoria_valida(*juego), DANIO_ESGREGUTO};
    juego->obstaculos[(juego->tope_obstaculos)++] = (obstaculo_t) {CODIGO_ACROMANTULA, posicion_aleatoria_valida(*juego), DANIO_ACROMANTULA};
    juego->obstaculos[(juego->tope_obstaculos)++] = (obstaculo_t) {CODIGO_BOGGART, posicion_aleatoria_valida(*juego), DANIO_BOGGART};
}

/*
 * PRE CONDICIONES: Un juego_t valido
 * POST CONDICIONES: Inicializa y añade todas las ayudas en posiciones aleatorias validas
 */
void inicializar_ayudas(juego_t* juego)
{
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_IMPEDIMENTA, posicion_aleatoria_valida(*juego), 0};
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_RIDDIKULUS, posicion_aleatoria_valida(*juego), 0};
    /* Pociones */
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_POCION, posicion_aleatoria_valida(*juego), VIDA_POCION};
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_POCION, posicion_aleatoria_valida(*juego), VIDA_POCION};
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_POCION, posicion_aleatoria_valida(*juego), VIDA_POCION};
    /* Esfinge */
    juego->ayudas[(juego->tope_ayudas)++] = (ayuda_t) {CODIGO_ESFINGE, posicion_aleatoria_valida(*juego), 0};
}

/*
 * PRE CONDICIONES: Un juego_t valido
 * POST CONDICIONES: Inicializa al rival en una posicion valida, con un minimo de 10 unidades lejos de la copa
 */
void inicializar_rival(juego_t* juego)
{
    juego->rival = (rival_t) {CODIGO_RIVAL, posicion_aleatoria_jugador(*juego), MOVIMIENTOS_DISPONIBLES[INDICE_DERECHA].tecla, 0};
}

/*
 * PRE CONDICIONES: Un juego_t valido
 * POST CONDICIONES: Inicializa al jugador en una posicion valida, con un minimo de 10 unidades lejos de la copa
 */
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

/*
 * PRE CONDICIONES: Que el laberinto[][] este inicializado y los ints sean validos
 * POST CONDICIONES: Devuelve TRUE si la posicion esta dentro del laberinto y no es una pared, de caso contrario FALSE
 */
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
        /* Mantengo siempre la cantidad de pasos en el intervalo [0, 15] para evitar un overflow */
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

/*
 * PRE CONDICIONES: Que juego este inicializado
 * POST CONDICIONES: Actualiza el turno del jugador, restandole vida
 */
void actualizar_turno(juego_t* juego)
{
    juego->jugador.vida -= VIDA_POR_TURNO;
}

/*
 * PRE CONDICIONES: Que juego este inicializado
 * POST CONDICIONES: Actualiza y consume las ayudas del mapa
 */
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

/*
 * PRE CONDICIONES: Que juego este inicializado
 * POST CONDICIONES: Actualiza y consume los obstaculos del mapa
 */
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
    if (comparar_coordenadas(juego.jugador.posicion, juego.copa.posicion)) return 1;
    if (comparar_coordenadas(juego.rival.posicion, juego.copa.posicion)) return -1;
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

/*
 * PRE CONDICIONES: Que jugador este inicializado
 * POST CONDICIONES: Devuelve TRUE si la copa se debe dibujar (jugador con menos de 15 de vida o si tiene la esfinge)
 */
bool copa_visible(jugador_t jugador)
{
    return jugador.vida <= 15 || tiene_ayuda(CODIGO_ESFINGE, jugador.ayudas, jugador.tope_ayudas);
}

/*
 * PRE CONDICIONES: Que ambos laberintos sean validos
 * POST CONDICIONES: Copia los elementos (las paredes) de un laberinto a otro
 */
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

/*
 * PRE CONDICIONES: Que el vector de obstaculos y el laberinto sean validos
 * POST CONDICIONES: Llena el laberinto con los codigos de los obstaculos en las posiciones donde estan
 */
void copiar_obstaculos(obstaculo_t obstaculos[TOTAL_OBSTACULOS], int tope_obstaculos, char copia[TAMANIO][TAMANIO])
{
    for(int i = 0; i < tope_obstaculos; ++i)
    {
        copia[obstaculos[i].posicion.fil][obstaculos[i].posicion.col] = obstaculos[i].codigo;
    }
}

/*
 * PRE CONDICIONES: Que el vector de obstaculos y el laberinto sean validos
 * POST CONDICIONES: Llena el laberinto con los codigos de las ayudas en las posiciones donde estan
 */
void copiar_ayudas(ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas, char copia[TAMANIO][TAMANIO])
{
    for(int i = 0; i < tope_ayudas; ++i)
    {
        copia[ayudas[i].posicion.fil][ayudas[i].posicion.col] = ayudas[i].codigo;
    }
}

/*
 * PRE CONDICIONES: Que el rival y el laberinto sean validos
 * POST CONDICIONES: Marca en el laberinto la posicion del rival utilizando su codigo
 */
void copiar_rival(rival_t rival, char copia[TAMANIO][TAMANIO])
{
    copia[rival.posicion.fil][rival.posicion.col] = rival.codigo;
}

/*
 * PRE CONDICIONES: Que el jugador y el laberinto sean validos
 * POST CONDICIONES: Marca en el laberinto la posicion del jugador utilizando su codigo
 */
void copiar_jugador(jugador_t jugador, char copia[TAMANIO][TAMANIO])
{
    copia[jugador.posicion.fil][jugador.posicion.col] = jugador.codigo;
}

/*
 * PRE CONDICIONES: Que la copa y el laberinto sean validos
 * POST CONDICIONES: Marca en el laberinto la posicion de la copa utilizando su codigo
 */
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
 * PRE CONDICIONES: Un obstaculo_t inicializado
 * POST CONDICIONES: Devuelve TRUE si el obstaculo_t es un boggart o FALSE en caso contrario
 */
bool es_boggart(obstaculo_t obstaculo)
{
    return obstaculo.codigo == CODIGO_BOGGART;
}

/*
 * PRE CONDICIONES: Un obstaculo_t inicializado
 * POST CONDICIONES: Devuelve TRUE si el obstaculo_t es un escreguto o FALSE en caso contrario
 */
bool es_escreguto(obstaculo_t obstaculo)
{
    return obstaculo.codigo == CODIGO_ESCREGUTO;
}

/*
 * PRE CONDICIONES: Una ayuda_t inicializada
 * POST CONDICIONES: Devuelve TRUE si la ayuda_t es una pocion o FALSE en caso contrario
 */
bool es_pocion(ayuda_t ayuda)
{
    return ayuda.codigo == CODIGO_POCION;
}

/*
 * PRE CONDICIONES: Un indice valido dentro del vector
 * POST CONDICIONES: Remueve la ayuda que hay en el indice dado dentro del vector dado.
 */
void remover_ayuda(int indice, ayuda_t ayudas[TOTAL_AYUDAS], int* tope_ayudas)
{
    for(int i = indice; i < (*tope_ayudas) - 1; ++i)
    {
        ayudas[i] = ayudas[i+1];
    }
    (*tope_ayudas)--;
}

/*
 * PRE CONDICIONES: Un indice valido dentro del vector
 * POST CONDICIONES: Remueve el obstaculo que hay en el indice dado dentro del vector dado.
 */
void remover_obstaculo(int indice, obstaculo_t obstaculos[TOTAL_OBSTACULOS], int* tope_obstaculos)
{
    for(int i = indice; i < (*tope_obstaculos) - 1; ++i)
    {
        obstaculos[i] = obstaculos[i+1];
    }
    (*tope_obstaculos)--;
}

/*
 * PRE CONDICIONES: Recibir coordenadas inicializadas
 * POST CONDICIONES: Devuelve TRUE si son iguales o FALSE en caso contrario
 */
bool comparar_coordenadas(coordenada_t x, coordenada_t y)
{
    return x.fil == y.fil && y.col == x.col;
}

/*
 * PRE CONDICIONES: Recibir juego y coordenada inicializados
 * POST CONDICIONES: Devuelve TRUE si la posicion no esta siendo utilizada por ningun elemento del juego o FALSE en caso contrario
 */
bool posicion_utilizada(juego_t juego, coordenada_t coordenada)
{
    for(int i = 0; i < juego.tope_ayudas; ++i)
    {
        if(comparar_coordenadas(juego.ayudas[i].posicion, coordenada))
            return true;
    }
    for(int i = 0; i < juego.tope_obstaculos; ++i)
    {
        if(comparar_coordenadas(juego.obstaculos[i].posicion, coordenada))
            return true;
    }
    return comparar_coordenadas(juego.rival.posicion, coordenada)
        || comparar_coordenadas(juego.copa.posicion, coordenada)
        || comparar_coordenadas(juego.jugador.posicion, coordenada);
}

/*
 * PRE CONDICIONES: Un juego inicializado correctamente y que exista una posicion libre en el mapa
 * POST CONDICIONES: Devuelve una posicion aleatoria que sea valida y no utilizada
 */
coordenada_t posicion_aleatoria_valida(juego_t juego)
{
    coordenada_t coordenada;
    do
    {
        coordenada = posicion_aleatoria();
    }
    while(!posicion_valida(coordenada.fil, coordenada.col, juego.laberinto_original) || posicion_utilizada(juego, coordenada));
    return coordenada;
}

/*
 * PRE CONDICIONES: Recibir un objeto juego_t valido
 * POST CONDICIONES: Devuelve una posicion aleatoria que este a > 10 de la copa, en distancia manhattan
 */
coordenada_t posicion_aleatoria_jugador(juego_t juego)
{
    coordenada_t coordenada;
    do
    {
        coordenada = posicion_aleatoria_valida(juego);
    }
    while(distancia_manhattan(coordenada, juego.copa.posicion) <= DISTANCIA_MINIMA_COPA);
    return coordenada;
}

/*
 * PRE CONDICIONES: Recibir un codigo de ayuda y vector de ayudas valido
 * POST CONDICIONES: Devuelve un bool que representa si el vector tiene esa ayuda.
 */
bool tiene_ayuda(char codigo, ayuda_t ayudas[TOTAL_AYUDAS], int tope_ayudas)
{
    for(int i = 0; i < tope_ayudas; ++i)
    {
        if (ayudas[i].codigo == codigo)
            return true;
    }
    return false;
}

/*
 * PRE CONDICIONES: La tecla dada debe ser una de las que se usan como controles
 * POST CONDICIONES: Devuelve la direccion_t correspondiente a esa tecla
 */
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

/*
 * PRE CONDICIONES: Un par de ints validos
 * POST CONDICIONES: Devuelve el menor de ambos ints dados
 */
int min(int x, int y)
{
    return x < y ? x : y;
}

/*
 * PRE CONDICIONES: Un int valido
 * POST CONDICIONES: Devuelve el valor absoluto del numero dado
 */
int absoluto(int x)
{
    return x > 0 ? x : -x;
}

/*
 * PRE CONDICIONES: Un par de coordenadas inicializados
 * POST CONDICIONES: Calcula la distancia manhattan entre ellas y lo devuelve como int
 */
int distancia_manhattan(coordenada_t x, coordenada_t y)
{
    return absoluto(x.fil - y.fil) + absoluto(x.col - y.col);
}
