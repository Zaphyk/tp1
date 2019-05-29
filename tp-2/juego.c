#include <stdbool.h>
#include <stdio.h>
#include "copa.h"

const int LIMPIEZA_PANTALLA_LINEAS = 512;

/*
 * PRE CONDICIONES: Ninguna
 * POST CONDICIONES: Crea el objeto juego de tipo juego_t, lo inicializa y lo devuelve
 */
juego_t inicializar_juego()
{
    juego_t juego;
    inicializar_laberinto(&juego);
    return juego;
}

/*
 * PRE CONDICIONES: Que el usuario ingrese un char valido
 * POST CONDICIONES: Lee una tecla ingresada por el usuario
 */
void leer_tecla(char* tecla)
{
    scanf("%c", tecla);
}

/*
 * PRE CONDICIONES: Que resultado sea 1 si el usuario gano
 * POST CONDICIONES: Muestra por pantalla el resultado del juego
 */
void mostrar_resultado(int resultado)
{
    printf("\n%s\n", resultado == 1 ? "HAS GANADO" : "HAS PERDIDO");
}

/*
 * PRE CONDICIONES: Ninguna
 * POST CONDICIONES: Limpia la pantalla de la consola para que cuando se dibuje no se vea lo anterior.
 *
 * Esto se que queda un poco feo pero es mas que nada es algo cosmetico para que no se encime la interfaz
 */
void limpiar_pantalla()
{
    /* En vez de usar system("clear"); mando varios newlines  */
    for(int i = 0; i < LIMPIEZA_PANTALLA_LINEAS; ++i)
    {
        printf("\n");
    }
}

/*
 * PRE CONDICIONES: Recibir un juego valido e inicializado
 * POST CONDICIONES: Dibuja la interfaz superior del juego (vida, ayudas)
 */
void dibujar_interfaz(juego_t juego)
{
    printf("--- TORNEO DE LOS MAGOS ---- \n");
    printf("VIDA: %i\n", juego.jugador.vida);
    printf("\n");
    printf("HECHIZOS APRENDIDOS:");
    if(juego.jugador.tope_ayudas == 0)
    {
        printf(" %s", "Ninguno");
    }
    else
    {
        for(int i = 0; i < juego.jugador.tope_ayudas; ++i)
        {
            printf(" %c", juego.jugador.ayudas[i].codigo);
        }
    }
    printf("\n\n");
}

/*
 * PRE CONDICIONES: Recibir un juego y laberinto validos
 * POST CONDICIONES: Dibuja toda la interfaz del juego
 */
void dibujar(juego_t juego, char laberinto_temporal[TAMANIO][TAMANIO])
{
    actualizar_laberinto(juego, laberinto_temporal);
    limpiar_pantalla();
    dibujar_interfaz(juego);
    mostrar_laberinto(laberinto_temporal);
}

int main()
{
    char tecla;
    char laberinto_temporal[TAMANIO][TAMANIO];
    int estado = 0;
    juego_t juego = inicializar_juego();
    dibujar(juego, laberinto_temporal);

    /* Loop principal del juego */
    while(estado == 0)
    {
        leer_tecla(&tecla);
        if(es_movimiento_valido(&juego, tecla))
        {
            mover_jugador(&(juego.jugador), tecla);
            actualizar_juego(&juego);
            if ((estado = estado_juego(juego)) == 0)
            {
                mover_rival(&juego);
            }
        }
        dibujar(juego, laberinto_temporal);
    }
    mostrar_resultado(estado);
    return 0;
}