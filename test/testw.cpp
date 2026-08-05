#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estructura adaptada para almacenar tambi�n la fecha de registro
typedef struct {
	char fecha[30];    // Guardar� el texto "DD/MM/AAAA - HH:MM:SS"
	char nombre[51];
	int puntaje;
} Jugador;

// Prototipos
void cargarJugador(Jugador *j);
void limpiarBuffer();

int main() {
	time_t tiempoActual;
	struct tm *infoTiempo;
	
	// Abrimos el archivo en modo binario
	FILE *archivo = fopen("ranking.bin", "ab");
	
	if (archivo == NULL) {
		printf("Error: No se pudo abrir o crear el archivo 'ranking.bin'.\n");
		return 1;
	}
	
	Jugador nuevoJugador;
	
	// Cargar datos del jugador (Nombre y Puntaje)
	cargarJugador(&nuevoJugador);
	
	// Fecha del sistema
	time(&tiempoActual);
	infoTiempo = localtime(&tiempoActual);
	
	strftime(nuevoJugador.fecha, sizeof(nuevoJugador.fecha), "%d/%m/%Y - %H:%M:%S", infoTiempo);
	
	// Escribir la estructura completa en el archivo usando fwrite
	// Par�metros: 
	// 		&nuevoJugador -> Direcci�n de memoria de la estructura.
	// 		sizeof(Jugador) -> Cu�ntos bytes mide la estructura completa.
	// 		1 -> Cu�ntas estructuras vamos a escribir en esta operaci�n.
	// 		archivo -> El puntero al archivo binario abierto.
	fwrite(&nuevoJugador, sizeof(Jugador), 1, archivo);
	
	// Cerrar el archivo correctamente
	fclose(archivo);
	
	printf("Registro guardado 'ranking.bin' usando fwrite.\n");
	return 0;
}

// Cargar datos
void cargarJugador(Jugador *j) {
	printf("Ingrese el nombre del jugador (max 50 caracteres): ");
	fgets(j->nombre, 51, stdin);
	
	// Le saco el enter del final
	int len = strlen(j->nombre);
	if (len > 0 && j->nombre[len - 1] == '\n') {
		j->nombre[len - 1] = '\0'; 
	} else {
		limpiarBuffer(); 
	}
	
	printf("Ingrese el puntaje: ");
	while (scanf("%d", &(j->puntaje)) != 1) {
		printf("Puntaje invalido. ");
		limpiarBuffer();
	}
	limpiarBuffer(); // Limpia el salto de l�nea del scanf
}

void limpiarBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}
