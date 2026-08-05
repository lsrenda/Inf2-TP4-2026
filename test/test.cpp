#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// La estructura DEBE ser id�ntica a la que us� fwrite para guardar el archivo
typedef struct {
	char fecha[30];    
	char nombre[51];
	int puntaje;
} Jugador;

void mostrarArchivoBinario();

int main() {
	mostrarArchivoBinario();
	return 0;
}

// FUNCI�N MODIFICADA: Abre el archivo binario y lo muestra registro por registro
void mostrarArchivoBinario() {
	// Abrimos en modo lectura binaria ("rb")
	FILE *archivo = fopen("ranking.bin", "rb");
	
	// Si el archivo no existe o no se puede abrir
	if (archivo == NULL) {
		printf("El archivo 'ranking.bin' no existe o esta vacio.\n");
		return;
	}
	
	Jugador registroTemporal; // Variable temporal para almacenar cada bloque le�do
	
	printf("\n=== CONTENIDO DE 'ranking.bin' (BINARIO) ===\n");
	printf("%-23s | %-25s | %s\n", "Fecha de Registro", "Nombre", "Puntaje");
	printf("------------------------------------------------------------------\n");
	
	// fread intenta leer el tama�o exacto de un struct Jugador.
	// Devuelve la cantidad de elementos le�dos. Al llegar al final del archivo (EOF), devolver� 0.
	while (fread(&registroTemporal, sizeof(Jugador), 1, archivo) == 1) {
		// Imprimimos los campos limpios que se recuperaron de los bytes del archivo
		printf("%-23s | %-25s | %d\n", 
			   registroTemporal.fecha, 
			   registroTemporal.nombre, 
			   registroTemporal.puntaje);
	}
	printf("==================================================================\n");
	
	// Siempre cerrar el archivo al terminar de leer
	fclose(archivo);
}
