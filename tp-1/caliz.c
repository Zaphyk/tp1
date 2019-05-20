#include <stdio.h>
#include <stdbool.h>

#define CAPACIDAD_MAGICA_REGULAR 'R'
#define CAPACIDAD_MAGICA_BUENA 'B'
#define CAPACIDAD_MAGICA_MALA 'M'

const char CARACTER_ABORTAR_PROGRAMA = 'N';
const int FUERZA_MAXIMA = 10;
const int FUERZA_MINIMA = 0;
const int INTELIGENCIA_MAXIMA = 10;
const int INTELIGENCIA_MINIMA = 0;
const int EDAD_MINIMA = 17;

/*
 * PRE CONDICIONES: El usuario debe ingresar un valor numerico
 * POST CONDICIONES: Muestra un mensaje y escribe el valor ingresado ya validado en la variable dada.
 */
void recibir_numero_crudo(int* valor, char* mensaje) {
    printf("%s", mensaje);
    scanf("%i", valor);
}

/*
 * PRE CONDICIONES: El usuario debe ingresar un valor numerico
 * POST CONDICIONES: Muestra un mensaje y escribe el valor ingresado ya validado en la variable dada.
 */
void recibir_atributo(int* valor, int min, int max, char* mensaje) {
    printf("%s", mensaje);
    bool es_invalido = false;
    do {
        scanf("%i", valor);
        if ((es_invalido = ((*valor) < min || (*valor) > max)))
            printf("Dato invalido. El valor a ingresar debe ser mayor que %d y menor que %d\n", min, max);
    }
    while(es_invalido);
}

/*
 * PRE CONDICIONES: Se debe proveer un char que sea una de las 3 capacidades magicas.
 * POST CONDICIONES: Escribe en la variable otorgada un valor numerico que representa a la capacidad magica, para facilitar la comparacion.
 */
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

/*
 * PRE CONDICIONES: El usuario debe ingresar un valor char valido.
 * POST CONDICIONES: Escribe a una variable de tipo int un valor numerico que representa la capacidad magica, asi facilitando la comparacion.
 */
void recibir_capacidad_magica(int* capacidad_magica) {
    printf("Ingrese la capacidad magica del estudiante:\n");
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

/*
 * PRE CONDICIONES: Que el usuario ingrese una edad > 0
 * POST CONDICIONES: Recibe los atributos de un estudiante y los escribe en sus respectivas variables
 */
void recibir_estudiante(int* edad, int* inteligencia, int* fuerza, int* capacidad_magica) {
    recibir_numero_crudo(edad, "Ingresa la edad del estudiante:\n");
    if ((*edad) > EDAD_MINIMA) {
        recibir_atributo(inteligencia, INTELIGENCIA_MINIMA, INTELIGENCIA_MAXIMA, "Ingresa la inteligencia del estudiante:\n");
        recibir_atributo(fuerza, FUERZA_MINIMA, FUERZA_MAXIMA, "Ingresa la fuerza del estudiante:\n");
        recibir_capacidad_magica(capacidad_magica);
    }
}

/*
 * PRE CONDICIONES: Que las variables dadas tengan algun valor y sean validas
 * POST CONDICIONES: Guarda los valores del estudiante ingresando en las variables del campeon.
 */
void hacer_campeon(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int inteligencia, int fuerza, int capacidad_magica) {
    (*campeon_id) = id;
    (*campeon_inteligencia) = inteligencia;
    (*campeon_fuerza) = fuerza;
    (*campeon_capacidad_magica) = capacidad_magica;
}

/*
 * PRE CONDICIONES: Que las variables dadas tengan algun valor y sean validas
 * POST CONDICIONES: Devuelve true si el estudiante que se compara es mejor que el campeon y escribe en una variable de tipo bool si la comparacion con el campeon deberia continuar
 */
bool comparar_atributo(int campeon_atributo, int atributo, bool* seguir_comparando) {

    if (campeon_atributo < atributo) {
    	(*seguir_comparando) = false;
    	return true;
    }
    else if(campeon_atributo > atributo) {
    	(*seguir_comparando) = false;
    	return false;
    }
    (*seguir_comparando) = true;
    return false;
}

/*
 * PRE CONDICIONES: Que las variables dadas tengan algun valor y sean validas
 * POST CONDICIONES: Si el estudiante ingresando es mejor que el campeon, lo convierte en el campeon actual.
 */
void comparar_estudiantes(int* campeon_id, int* campeon_inteligencia, int* campeon_fuerza, int* campeon_capacidad_magica, int id, int edad, int inteligencia, int fuerza, int capacidad_magica) {

    if (edad < EDAD_MINIMA)
        return;

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

/*
 * PRE CONDICIONES: Un id de campeon != 0
 * POST CONDICIONES: Muestra un texto en consola diciendo quien fue el campeon o menciona su ausencia si no hay ninguno.
 */
void mostrar_campeon(int id) {
    if (id != 0) {
        printf("El campeón es el número %i :)\n", id);
    }
    else {
        printf("No hay campeón :(\n");
    }
}

/*
 * PRE CONDICIONES: Cantidad de alumnos >= 1
 * POST CONDICIONES: Escribe en una variable de tipo bool si el usuario desea seguir ingresando datos.
 */
void preguntar_si_ingresar(int cantidad_alumnos, bool* resultado) {
    char respuesta;
    printf("Desea ingresar %s alumno? (%c para ​NO​, cualquier otra tecla para SÍ)\n", (cantidad_alumnos == 1) ? "un" : "otro", CARACTER_ABORTAR_PROGRAMA);
    scanf(" %c", &respuesta);
    (*resultado) = respuesta != CARACTER_ABORTAR_PROGRAMA;
}


int main() {
    int campeon_id = 0, campeon_inteligencia, campeon_fuerza, campeon_capacidad_magica;
    int id_actual = 1, edad, inteligencia, fuerza, capacidad_magica;

    bool esta_ingresando = true;
    preguntar_si_ingresar(id_actual, &esta_ingresando);

    while(esta_ingresando) {
        recibir_estudiante(&edad, &inteligencia, &fuerza, &capacidad_magica);
        comparar_estudiantes(
            &campeon_id, &campeon_inteligencia, &campeon_fuerza, &campeon_capacidad_magica,
            id_actual++, edad, inteligencia, fuerza, capacidad_magica
        );
        preguntar_si_ingresar(id_actual, &esta_ingresando);
    }
    mostrar_campeon(campeon_id);
    return 0;
}