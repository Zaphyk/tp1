#include <stdbool.h>
#include <stdio.h>
#include "copa.h"

const int LIMPIEZA_PANTALLA_LINEAS = 512;

juego_t inicializar_juego()
{
    juego_t juego;
    inicializar_laberinto(&juego);
    return juego;
}

void leer_tecla(char* tecla)
{
    scanf("%c", tecla);
}

void mostrar_resultado(int resultado)
{
    printf("\n%s\n", resultado == 1 ? "HAS GANADO" : "HAS PERDIDO");
}

/* Esto mas que nada es algo cosmetico para que no se encime la interfaz */
void limpiar_pantalla()
{
    /* En vez de usar system("clear"); mando varios \n */
    for(int i = 0; i < LIMPIEZA_PANTALLA_LINEAS; ++i)
    {
        printf("\n");
    }
}

void dibujar_interfaz(juego_t juego)
{
    printf("--- TORNEO DE LOS MAGOS ---- \n");
    printf("VIDA: %i\n", juego.jugador.vida);
    printf("\n");
    printf("HECHIZOS APRENDIDOS:");
    for(int i = 0; i < juego.jugador.tope_ayudas; ++i)
    {
        printf(" %c", juego.jugador.ayudas[i].codigo);
    }
    printf("\n\n");
}

void dibujar_ayuda()
{
    printf("\n\n");
    printf("CONTROLES: Arriba - %c, Abajo - %c, Izquierda - %c, Derecha -%c\n", TECLA_ARRIBA, TECLA_ABAJO, TECLA_IZQUIERDA, TECLA_DERECHA);
    printf("AYUDAS: Esfinge - %c, Riddikulus - %c, Pocion - %c, Impedimenta - %c\n", CODIGO_ESFINGE, CODIGO_RIDDIKULUS, CODIGO_POCION, CODIGO_IMPEDIMENTA);
    printf("OBSTACULOS: Boggart - %c, Escreguto de cola explosiva - %c, Acromantula - %c\n", CODIGO_BOGGART, CODIGO_ESCREGUTO, CODIGO_ACROMANTULA);
    printf("\n\n");
}

void dibujar(juego_t juego, char laberinto_temporal[TAMANIO][TAMANIO])
{
    actualizar_laberinto(juego, laberinto_temporal);
    limpiar_pantalla();
    dibujar_interfaz(juego);
    mostrar_laberinto(laberinto_temporal);
    dibujar_ayuda();
}

int main()
{
    int estado = 0;
    char laberinto_temporal[TAMANIO][TAMANIO];
    juego_t juego = inicializar_juego();
    dibujar(juego, laberinto_temporal);

    /* Loop principal del juego */
    while(estado == 0)
    {
        char tecla;
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