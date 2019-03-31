#include <stdio.h>
#include <stdbool.h>

#define CAPACIDAD_MAGICA_REGULAR 'R'
#define CAPACIDAD_MAGICA_BUENA 'B'
#define CAPACIDAD_MAGICA_MALA 'M'

const int FUERZA_MAXIMA = 10;
const int FUERZA_MINIMA = 0;
const int INTELIGENCIA_MAXIMA = 10;
const int INTELIGENCIA_MINIMA = 0;
const int EDAD_MAXIMA = 2147483647;
const int EDAD_MINIMA = 17;

void recibir_numero(int* valor, int min, int max, char* mensaje, char* mensaje_de_error) {
    printf(mensaje);
    bool es_invalido = false;
    do {
        scanf("%i", valor);
        if (es_invalido = ((*valor) < min || (*valor) > max)) 
            printf(mensaje_de_error, min, max);
    }
    while(es_invalido);
}

void recibir_atributo(int* valor, int min, int max, char* mensaje) {
    recibir_numero(valor, min, max, mensaje, "Dato invalido. El valor a ingresar debe ser mayor que %d y menor que %d\n");
}

void parsear_capacidad_magica(char raw, int* capacidad_magica) {
    switch(raw) {
        case CAPACIDAD_MAGICA_MALA:
            (*capacidad_magica) = 0;
            break;
        case CAPACIDAD_MAGICA_REGULAR:
            (*capacidad_magica) = 1;
            break;
        case CAPACIDAD_MAGICA_BUENA:
            (*capacidad_magica) = 2;
            break;
    }
}

void recibir_capacidad_magica(int* capacidad_magica) {
    printf("Ingrese la capacidad magica del estudiante\n");
    char raw_capacidad_magica;
    bool es_invalido = false;
    do {
        scanf(" %c", &raw_capacidad_magica);
        es_invalido = !(raw_capacidad_magica == CAPACIDAD_MAGICA_MALA || raw_capacidad_magica == CAPACIDAD_MAGICA_REGULAR || raw_capacidad_magica == CAPACIDAD_MAGICA_BUENA); 
        if (es_invalido)
            printf("Dato invalido. El valor a ingresar debe ser alguna de estas opciones: '%c', '%c' o '%c'\n", CAPACIDAD_MAGICA_MALA, CAPACIDAD_MAGICA_REGULAR, CAPACIDAD_MAGICA_BUENA);
    }
    while(es_invalido);
    parsear_capacidad_magica(raw_capacidad_magica, capacidad_magica);
}

void recibir_estudiante(int* edad, int* inteligencia, int* fuerza, int* capacidad_magica) {
    recibir_numero(edad, EDAD_MINIMA, EDAD_MAXIMA, "Ingresa la edad del estudiante:\n", "Dato invalido. El valor a ingresar debe ser mayor que %d\n");
    recibir_atributo(inteligencia, INTELIGENCIA_MINIMA, INTELIGENCIA_MAXIMA, "Ingresa la inteligencia del estudiante:\n");
    recibir_atributo(fuerza, FUERZA_MINIMA, FUERZA_MAXIMA, "Ingresa la fuerza del estudiante:\n");
    recibir_capacidad_magica(capacidad_magica);
}

void hacer_campeon(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int inteligencia, int fuerza, int capacidad_magica) {
    (*campeon_id) = id;
    (*campeon_inteligencia) = inteligencia;
    (*campeon_fuerza) = fuerza;
    (*campeon_capacidad_magica) = capacidad_magica;
}

/*
 * Devuelve true si el estudiante que se compara es mejor que el campeon
 */
bool comparar_atributo(int campeon_atributo, int atributo, bool* seguir_comparando) {

    if (campeon_atributo < atributo) {
    	seguir_comparando = false;
    	return true;
    }
    else if(campeon_atributo > atributo) {
    	seguir_comparando = false;
    	return false;
    }
    seguir_comparando = true;
    return false;
}

void comparar_estudiantes(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int edad, int inteligencia, int fuerza, int capacidad_magica) {

    if (edad < 18) return;
    
    bool seguir_comparando = true;
    
    if (seguir_comparando && comparar_atributo((*campeon_inteligencia), inteligencia, &seguir_comparando)) {
    	hacer_campeon(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica);
    }
    
    if (seguir_comparando && comparar_atributo((*campeon_fuerza), fuerza, &seguir_comparando)) {
    	hacer_campeon(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica);
    }
    
    if (seguir_comparando && comparar_atributo((*campeon_capacidad_magica), capacidad_magica, &seguir_comparando)) {
    	hacer_campeon(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica);
    }
}

void mostrar_campeon(int id) {
    if (id != 0)
        printf("El campeón es el número %i :)\n", id);
    else
        printf("No hay campeón :(\n");
}

void preguntar_si_seguir_ingresando(bool* resultado) {
    char respuesta;
    printf("Desea ingresar otro alumno? (N para ​NO​, cualquier otra tecla para sí)\n");
    scanf(" %c", &respuesta);
    (*resultado) = respuesta != 'N';
}

int main() {
    int campeon_id = 0, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica;
    int id_actual = 1, edad, inteligencia, fuerza, capacidad_magica;

    bool esta_ingresando = true;
    while(esta_ingresando) {
        recibir_estudiante(&edad, &inteligencia, &fuerza, &capacidad_magica);
        comparar_estudiantes(
            &campeon_id, &campeon_inteligencia, &campeon_fuerza, &campeon_capacidad_magica,
            id_actual++, edad, inteligencia, fuerza, capacidad_magica
        );
        preguntar_si_seguir_ingresando(&esta_ingresando);
    }
    mostrar_campeon(campeon_id);
    return 0;
}