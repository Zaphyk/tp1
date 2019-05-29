#ifndef __LABERINTO_H__
#define __LABERINTO_H__

#define TAMANIO 15
#define PARED '#'
#define VACIO ' '
#define TOTAL_AYUDAS 10
#define TOTAL_OBSTACULOS 10

typedef struct coordenada {
    int fil;
    int col;
} coordenada_t;

typedef struct obstaculo {
    char codigo;
    coordenada_t posicion;
    int danio;
} obstaculo_t;

typedef struct ayuda {
    char codigo;
    coordenada_t posicion;
    int vida_a_recuperar;
} ayuda_t;

typedef struct jugador {
    char codigo;
    int vida;
    coordenada_t posicion;
    int tope_ayudas;
    ayuda_t ayudas[TOTAL_AYUDAS];
} jugador_t;


typedef struct rival {
    char codigo;
    coordenada_t posicion;
    char direccion;
    int cantidad_pasos;
} rival_t;

typedef struct copa {
    char codigo;
    coordenada_t posicion;
} copa_t;

typedef struct juego{
    char laberinto_original[TAMANIO][TAMANIO];
    jugador_t jugador;
    rival_t rival;
    copa_t copa;
    obstaculo_t obstaculos[TOTAL_OBSTACULOS];
    int tope_obstaculos;
    ayuda_t ayudas[TOTAL_AYUDAS];
    int tope_ayudas;
} juego_t;

/*
 * Inicializará todas las estructuras con los valores correspondientes,
 * creará el laberinto, posicionará todos los elementos, etc.
 *
 * PRE CONDICIONES: Que el usuario ingrese un char valido
 * POST CONDICIONES: Lee una tecla ingresada por el usuario
 */
void inicializar_laberinto(juego_t* juego);

/*
 * Determinará si el caracter ingresado es válido, esto es, es el caracter ‘a’ o
 * ‘s’ o ‘d’ o ‘w’ y además el jugador puede moverse en esa dirección, o sea,
 * hay pasillo.
 *
 * PRE CONDICIONES: Que el usuario ingrese un char valido
 * POST CONDICIONES: Lee una tecla ingresada por el usuario
 */
bool es_movimiento_valido(juego_t* juego, char tecla);

/*
 * Moverá el jugador hacia la dirección especificada.
 * Dicho movimiento es válido.
 *
 * PRE CONDICIONES: Que el movimiento sea valido
 * POST CONDICIONES: Mueve al jugador
 */
void mover_jugador(jugador_t* jugador, char direccion);

/*
 * Moverá el rival a la próxima posición.
 *
 * PRE CONDICIONES: Que juego este inicializado y el rival en una posicion valida
 * POST CONDICIONES: Mueve al rival
 */
void mover_rival(juego_t* juego);

/*
 * Actualizará el juego. Restará vida si el jugador está sobre un obstáculo
 * o lo eliminará si cuenta con el hechizo, aprenderá hechizos y todo lo
 * que pueda suceder luego de un turno.
 *
 * PRE CONDICIONES: Que juego este inicializado y sea valido
 * POST CONDICIONES: Actualiza el juego, turno, ayudas y obstaculos
 */
void actualizar_juego(juego_t* juego);

/*
 * Devolverá el estado del juego, 1 ganado, 0 en curso, -1 perdido.
 *
 * PRE CONDICIONES: Que juego este inicializado y sea valido
 * POST CONDICIONES: Devuelve un int que representa el estado del juego
 */
int estado_juego(juego_t juego);

/*
 * Devolverá una coordenada aleatoria dentro del rango TAMANIOxTAMANIO.
 * No valida que dicha coordenada coincida con un pasillo ni que exista
 * otro objeto en esa posición.
 *
 * PRE CONDICIONES: Ninguna
 * POST CONDICIONES: Devuelve una posicion aleatoria NO validada dentro del laberinto
 */
coordenada_t posicion_aleatoria();

/*
 * Actualizará la matriz mostrada al usuario, con los elementos presentes
 * en el juego.
 *
 * PRE CONDICIONES: Que juego sea valido y este incializado
 * POST CONDICIONES: Llena la matriz dada con los elementos presentes en el juego y las paredes del laberinto.
 */
void actualizar_laberinto(juego_t juego, char laberinto[TAMANIO][TAMANIO]);

/*
 * Mostrará el laberinto por pantalla.
 *
 * PRE CONDICIONES: Que laberinto[][] tenga valores validos
 * POST CONDICIONES: Muestra la matriz dada por pantalla
 */
void mostrar_laberinto(char laberinto[TAMANIO][TAMANIO]);

#endif /* __LABERINTO_H__ */