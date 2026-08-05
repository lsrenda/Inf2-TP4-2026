#include <stdio.h>
#include <stdlib.h>

int main() {
    char linea[100]; // Un "recipiente" temporal para guardar lo que el usuario escriba
    int numero;
    int leidos;

    while (1) {
        printf("Ingresa un numero entero: ");

        // PASO 1: Leer TODA la línea (hasta 99 letras o hasta que presione Enter)
        // stdin significa que leemos del teclado (Standard Input)
        if (fgets(linea, sizeof(linea), stdin) != NULL) {
            
            // PASO 2: Buscar un número DENTRO del texto que acabamos de capturar
            leidos = sscanf(linea, "%d", &numero);

            if (leidos == 1) {
                printf("¡Exito! El numero es: %d\n", numero);
                break; // Salimos del bucle
            } else {
                printf("Error: No ingresaste un numero valido. Intenta de nuevo.\n");
            }
        } else {
            // Manejo de error si fgets falla (ej. si el usuario manda EOF)
            printf("\nSe corto la entrada de datos. Saliendo...\n");
            break; 
        }
    }

    return 0;
}
