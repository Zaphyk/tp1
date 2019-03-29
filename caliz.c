#include <stdio.h>

#define CAPACIDAD_MAGICA_REGULAR "R"
#define CAPACIDAD_MAGICA_BUENA "B"
#define CAPACIDAD_MAGICA_MALA "M"
#define MAX_INT 2147483647

void recibir_numero(int* valor, int min, int max) {
    do {
        scanf("%i", valor);
        if (valor >= min && valor <= max) break;
        printf("Dato invalido. El valor a ingresar debe ser mayor que %d y menor que %d\n", min, max);
    }
    while(true);
}

void recibir_inteligencia(int* inteligencia) {
    printf("Ingresa la inteligencia del estudiante:\n");
    recibir_numero(inteligencia, 0, 10);
}

void recibir_fuerza(int* fuerza) {
    printf("Ingresa la fuerza del estudiante:\n");
    recibir_numero(fuerza, 0, 10);
}

void recibir_edad(int* edad) {
    printf("Ingresa la edad del estudiante:\n");
    recibir_numero(edad, 17, MAX_INT);
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
    char raw_capacidad_magica;
    do {
        scanf("%c", &raw_capacidad_magica);
        if (raw_capacidad_magica == CAPACIDAD_MAGICA_MALA || raw_capacidad_magica == CAPACIDAD_MAGICA_REGULAR || raw_capacidad_magica == CAPACIDAD_MAGICA_BUENA) break;
        printf("Dato invalido. El valor a ingresar debe ser alguna de estas opciones: '%c', '%c' o '%c'\n", CAPACIDAD_MAGICA_MALA, CAPACIDAD_MAGICA_REGULAR, CAPACIDAD_MAGICA_BUENA);
    }
    while(true);
    parsear_capacidad_magica(raw_capacidad_magica, capacidad_magica);
}

bool recibir_estudiante(int* edad, int* inteligencia, int* fuerza, int* capacidad_magica) {
    recibir_edad(edad);
    recibir_inteligencia(inteligencia);
    recibir_fuerza(fuerza);
    recibir_capacidad_magica(capacidad_magica);
    return true;
}

void hacer_campeon(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int inteligencia, int fuerza, int capacidad_magica) {
    (*campeon_id) = id;
    (*campeon_inteligencia) = inteligencia;
    (*campeon_fuerza) = fuerza;
    (*campeon_capacidad_magica) = capacidad_magica;
}

void comparar_estudiantes(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int edad, int inteligencia, int fuerza, int capacidad_magica) {

    if (edad < 18) return;

    if((*campeon_inteligencia) < inteligencia) hacer_campeon(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica);
    else if ((*campeon_inteligencia) > inteligencia) return;

    if((*campeon_fuerza) < fuerza)hacer_campeon(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica);
    else if ((*campeon_fuerza) > fuerza) return;

    if((*campeon_capacidad_magica) < capacidad_magica) hacer_campeon(campeon_id, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica, id, inteligencia, fuerza, capacidad_magica);
    else if ((*campeon_capacidad_magica) > capacidad_magica) return;
}

void mostrar_campeon(int id) {
    if (id != 0)
        printf("El campeón es el número %i :)\n", id);
    else
        printf("No hay campeón :(\n");
}

bool no_desea_ingresar_mas() {
    char respuesta;
    printf("Desea ingresar otro alumno? (N para ​NO​, cualquier otra tecla para sí): S\n");
    scanf("%c", &respuesta);
    return respuesta == "N";
}

int main() {
    int campeon_id = 0, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica;
    int id_actual = 1, edad, inteligencia, fuerza, capacidad_magica;

    while(recibir_estudiante(&edad, &inteligencia, &fuerza, &capacidad_magica)) {
        comparar_estudiantes(
            &campeon_id, &campeon_inteligencia, &campeon_fuerza, &campeon_capacidad_magica,
            id_actual++, edad, inteligencia, fuerza, capacidad_magica
        );
        if (no_desea_ingresar_mas()) break;
    }
    mostrar_campeon(campeon_id);
    return 0;
}