#include <stdio.h>
#include <stdbool.h>

#define CAPACIDAD_MAGICA_REGULAR 'R'
#define CAPACIDAD_MAGICA_BUENA 'B'
#define CAPACIDAD_MAGICA_MALA 'M'
#define MAX_INT 2147483647

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
    recibir_numero(edad, 17, MAX_INT, "Ingresa la edad del estudiante:\n", "Dato invalido. El valor a ingresar debe ser mayor que %d\n");
    recibir_atributo(inteligencia, 0, 10, "Ingresa la inteligencia del estudiante:\n");
    recibir_atributo(fuerza, 0, 10, "Ingresa la fuerza del estudiante:\n");
    recibir_capacidad_magica(capacidad_magica);
}

bool hacer_campeon(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int inteligencia, int fuerza, int capacidad_magica) {
    (*campeon_id) = id;
    (*campeon_inteligencia) = inteligencia;
    (*campeon_fuerza) = fuerza;
    (*campeon_capacidad_magica) = capacidad_magica;
    return true;
}

bool comparar_atributo(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int inteligencia, int fuerza, int capacidad_magica, int campeon_atributo, int atributo) {

    if (campeon_atributo < atributo) return hacer_campeon(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica);
    else if(campeon_atributo > atributo) return true;
    return false;
}

void comparar_estudiantes(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int edad, int inteligencia, int fuerza, int capacidad_magica) {

    if (edad < 18) return;
    if (comparar_atributo(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica, (*campeon_inteligencia), inteligencia)) return;
    if (comparar_atributo(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica, (*campeon_fuerza), fuerza)) return;
    if (comparar_atributo(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica, (*campeon_capacidad_magica), capacidad_magica)) return;
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