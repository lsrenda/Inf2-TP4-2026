#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


/*

1. Permita  almacenar  el  ranking  de  jugadores  guardando  los  datos  en  un  archivo  llamado ranking.txt que contenga el nombre de cada jugador y la cantidad de disparos realizados.
Este archivo debe actualizarse cuando se termine un juego. A cada línea del archivo, agregue una marca temporal (fecha y hora) para referencia.

2. Al iniciar el juego, recupere el ranking de jugadores y lo almacene en memoria. De esta manera el programa “recordará” quien ha sido el mejor jugador.

3. En la pantalla principal, y como “incentivo” para los otros jugadores, muestre quién ha sido el mejor jugador, y cuanto tiempo hace que no se supera el record.

4. Almacene  el  “historial  de  disparos”  de  cada  jugador  en  un  archivo  llamado disparos_<nombre_del_jugador>.txt guardando en dicho archivo el dato fila, columna y resultado (agua / hundido) separando los datos con una coma (,).

*/

typedef struct {
	char nombre[50];
	char fecha[30]; // "DD/MM/AAAA - HH:MM:SS"
	int disparos;
	time_t timestamp; // Para calcular diferencia de tiempo fácil
	int existeRecord; // 1 si el struct tiene datos validos, 0 si es un record vacio
} jugador;

typedef struct {
	int fila;
	int columna;
} disparo;

typedef struct {
	char *tablero; // Tablero dinámico
	char nombreActual[50];
	int n;
	int barcoF;
	int barcoC;
	int opcion;
	int tiros;
	int gano;
	disparo *historial;
	jugador *ranking; // ranking completo cargado en memoria
	int cantidadRanking; // cantidad de jugadores en el ranking
	jugador mejorRecord;
} EstadoJuego;

void inicializarTablero(EstadoJuego *juego);
void mostrarTablero(EstadoJuego *juego);
void mostrarRanking(EstadoJuego *juego);
void checkeoscan(int *destino, const char *mensaje_error);
void checkeonombre(char *destino, int tamano);
int realizarDisparo(EstadoJuego *juego, int f, int c);
int cargarGanador(EstadoJuego *juego);
void cargarRanking(EstadoJuego *juego);
void calcularMejorRecord(EstadoJuego *juego);
void guardarHistorialDisparos(EstadoJuego *juego);
void guardarPartida(EstadoJuego *juego);
int cargarPartida(EstadoJuego *juego);
void jugarBucle(EstadoJuego *juego);


int main() {
	EstadoJuego mega;
	mega.tablero = NULL;
	mega.historial = NULL;
	mega.ranking = NULL;
	mega.cantidadRanking = 0;

	srand(time(NULL));

	// requisito 2: el ranking completo se recupera y se guarda en memoria al iniciar
	cargarRanking(&mega);
	calcularMejorRecord(&mega);

	do {
		printf("\n=== TP3 INFORMATICA II ===\n");

		if (mega.mejorRecord.existeRecord) {
			time_t ahora = time(NULL);
			double segundos = difftime(ahora, mega.mejorRecord.timestamp);
			int dias = (int)(segundos / 86400);
			int horas = (int)((segundos - (dias * 86400)) / 3600);
			int minutos = (int)((segundos - (dias * 86400) - (horas * 3600)) / 60);

			printf("El cadete del mes: %s con %d tiros\n", mega.mejorRecord.nombre, mega.mejorRecord.disparos);
			printf("Record no superado desde hace: %d dias, %d horas y %d minutos.\n", dias, horas, minutos);
		} else {
			printf("Aun no hay record registrado en altamar. Esta es tu oportunidad recluta.\n");
		}

		printf("1. Iniciar Combate\n2. Guardar partida\n3. Cargar partida Guardada\n4. Mostrar ranking\n5. Salir\nSeleccione: ");

		if (scanf("%d", &mega.opcion) != 1) {
			checkeoscan(&mega.opcion, "Error, ingresa un numero valido recluta: ");
		}

		while (mega.opcion < 1 || mega.opcion > 5) {
			printf("Me estas tomando los pelos de la barba muchacho, sabes bien las opciones que tienes, del 1 al 5.\nCual es tu verdadera eleccion: ");
			if (scanf("%d", &mega.opcion) != 1) {
				checkeoscan(&mega.opcion, "Error, ingresa un numero valido recluda,y esta vez en serio: ");
			}
		}

		if (mega.opcion == 1) {
			printf("Tamano del tablero: ");

			if (scanf("%d", &mega.n) != 1) {
				checkeoscan(&mega.n, "Error, si no me dices tu nivel, no te puedo entrenar recluta: ");
			}

			// el tablero no puede tener tamano cero o negativo
			while (mega.n <= 0) {
				printf("el tamano tiene que ser mayor a cero: ");
				checkeoscan(&mega.n, "el tamano tiene que ser mayor a cero: ");
			}

			inicializarTablero(&mega);
			if (mega.tablero != NULL) {
				checkeonombre(mega.nombreActual, 50);
				jugarBucle(&mega);
			}
		}
		if (mega.opcion == 2) {
			guardarPartida(&mega); // esto tendria que ser un archivo
		}

		if (mega.opcion == 3) {
			if (cargarPartida(&mega)) {
				jugarBucle(&mega);
			}
		}

		if (mega.opcion == 4) {
			mostrarRanking(&mega);
		}
	} while (mega.opcion != 5);

	if (mega.tablero) free(mega.tablero);
	if (mega.historial) free(mega.historial);
	if (mega.ranking) free(mega.ranking); // libera el ranking cargado en memoria

	return 0;
}

void jugarBucle(EstadoJuego *juego) {
	while (!juego->gano) {
		mostrarTablero(juego);
		printf("Disparo actual: %d\n", juego->tiros + 1);

		// menu de turno: permite disparar, guardar o cargar entre disparos
		int accion;
		printf("1. Disparar\n2. Guardar partida\n3. Cargar partida\nSeleccione: ");
		if (scanf("%d", &accion) != 1) {
			checkeoscan(&accion, "opcion invalida, elegi un numero del 1 al 3: ");
		}
		while (accion < 1 || accion > 3) {
			printf("opcion invalida, elegi un numero del 1 al 3: ");
			if (scanf("%d", &accion) != 1) {
				checkeoscan(&accion, "opcion invalida, elegi un numero del 1 al 3: ");
			}
		}

		if (accion == 2) {
			guardarPartida(juego);
			continue; // vuelve a mostrar el tablero sin gastar el turno
		}

		if (accion == 3) {
			if (cargarPartida(juego)) {
				printf("\npartida cargada, retomando el combate...\n");
			}
			continue; // el estado pudo haber cambiado por completo, se re-muestra el tablero
		}

		int f_actual, c_actual;

		printf("Coordenadas:\nFila: ");
		checkeoscan(&f_actual, "Sigue asi recluta y jamas veras el altamar.\nAhora dime un numero: ");

		printf("Columna: ");
		checkeoscan(&c_actual, "Sigue asi recluta y jamas veras el altamar.\nAhora dime un numero: ");

		int res = realizarDisparo(juego, f_actual, c_actual);
		if (res != -1) {
			juego->tiros++;
			disparo *temp = (disparo *)realloc(juego->historial, juego->tiros * sizeof(disparo));
			if (temp == NULL) {
				printf("Error de memoria en la reacignacion del historial.\n");
				return;
			}
			juego->historial = temp;
			juego->historial[juego->tiros - 1].fila = f_actual;
			juego->historial[juego->tiros - 1].columna = c_actual;
			if (res == 1) juego->gano = 1; // Le dio al barco
		}
	}
	mostrarTablero(juego);
	if (juego->gano) {
		printf("\nTe felicito %s, ya eres parte de esta tripulacion.\nLe diste en %d canhonazos.\n", juego->nombreActual, juego->tiros);
		if (juego->tiros < juego->mejorRecord.disparos) {
			printf("\n¡Y CON UN NUEVO RECORD!\n");
		}
		cargarGanador(juego);
		guardarHistorialDisparos(juego);
		// requisito 2: se refresca el ranking en memoria con el nuevo resultado
		cargarRanking(juego);
		calcularMejorRecord(juego);
	}
	printf("\n--- SECUENCIA DE DISPAROS ---\n");
	for (int i = 0; i < juego->tiros; i++) {
		printf("Tiro %d: Fila %d, Columna %d\n", i + 1, juego->historial[i].fila, juego->historial[i].columna);
	}
	free(juego->tablero);
	free(juego->historial);
	juego->tablero = NULL;
	juego->historial = NULL;
}

void inicializarTablero(EstadoJuego *juego) {
	if (juego->tablero) free(juego->tablero);
	if (juego->historial) free(juego->historial);
	juego->tablero = (char *)malloc(juego->n * juego->n * sizeof(char)); // reservar memoria equivalente a n² elementos char
	if (juego->tablero == NULL) {
		printf("error de memoria al crear el tablero.\n");
		return;
	}
	juego->tiros = 0;
	juego->gano = 0;
	juego->historial = NULL; // para que el primer realloc funcione como malloc
	// llenamos de '.' usando aritmetica de punteros 1d
	for (int i = 0; i < (juego->n * juego->n); i++) {
		juego->tablero[i] = '.';
	}
	juego->barcoF = rand() % juego->n;
	juego->barcoC = rand() % juego->n;
}

void mostrarTablero(EstadoJuego *juego) {
	printf("\n    ");
	for (int i = 0; i < juego->n; i++) printf("%2d ", i); // %2 reserva 2 espacios de caracter %.2 limita a 2 decimales(%.2f)
	printf("\n");
	for (int i = 0; i < juego->n; i++) {
		printf("%2d |", i);
		for (int j = 0; j < juego->n; j++) {
			// aritmetica para arreglo 1D: (fila * n) + columna
			printf(" %c ", juego->tablero[(i * juego->n) + j]);
		}
		printf("\n");
	}
}

int realizarDisparo(EstadoJuego *juego, int f, int c) {
	// Validar límites
	if (f < 0 || f >= juego->n || c < 0 || c >= juego->n) {
		printf("\nTecnicamente... no fallaste. Para fallar, la bala tiene que haber tenido alguna posibilidad de dar. Lo tuyo fue otra cosa. Lo tuyo fue... arte.\n!\n");
		return -1; // -1 indica disparo invalido
	}
	
	// validar si ya disparo ahí
	if (juego->tablero[(f * juego->n) + c] != '.') {
		printf("\nYa has disparado ahi, y ya has fallado ahi! Te perdonare la bala por esta vez recluta.\n");
		return -1; 
	}
	
	// comprobar si acerto
	if (f == juego->barcoF && c == juego->barcoC) {
		juego->tablero[(f * juego->n) + c] = 'B';
		return 1; // 1 indica que gano
	} else {
		juego->tablero[(f * juego->n) + c] = 'A';
		printf("\nAGUA!\n");
		return 0; // 0 indica que fallo, pero el disparo es valido
	}
}

void checkeoscan(int *destino, const char *mensaje_error) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF); // Limpia buffer sobrante
	while (scanf("%d", destino) != 1) {
		while ((c = getchar()) != '\n' && c != EOF);
		printf("%s", mensaje_error);
	}
}

void checkeonombre(char *destino, int tamano) {
	int c;
	while ((c = getchar()) != '\n' && c != EOF); // Limpia el \n de scanfs previos

	while (1) {
		printf("Cual es tu nombre muchacho: ");
		if (fgets(destino, tamano, stdin) != NULL) {
			destino[strcspn(destino, "\n")] = '\0'; // requisito para el \n que incluye fgets en lugar de \0 que indica fin de cadena de texto
			if (strlen(destino) > 0) {
				break; // el nombre no es nulo, salimos
			}
		}
		printf("No te me hagas el gracioso recluta, dime tu nombre.\n");
	}
}


int cargarGanador(EstadoJuego *juego) {
	FILE *archivo = fopen("ranking.txt", "a");
	if (archivo == NULL) {
		printf("Error: No se pudo abrir o crear el archivo 'ranking.txt'.\n");
		return 1;
	}

	time_t ahora = time(NULL);
	struct tm *infoTiempo = localtime(&ahora);
	char fechaFormat[30];
	strftime(fechaFormat, sizeof(fechaFormat), "%d/%m/%Y - %H:%M:%S", infoTiempo);

	fprintf(archivo, "%s;%d;%s;%ld\n", juego->nombreActual, juego->tiros, fechaFormat, (long)ahora);
	fclose(archivo);

	printf("\nLo anote detras del mapa camarada, lo podras ver cuando estemos en altamar, bienvenido.\n");
	return 0;
}

// requisito 2: lee ranking.txt entero y arma el arreglo en memoria (reemplaza el ranking previo si existia)
void cargarRanking(EstadoJuego *juego) {
	if (juego->ranking) {
		free(juego->ranking);
		juego->ranking = NULL;
	}
	juego->cantidadRanking = 0;

	FILE *archivo = fopen("ranking.txt", "r");
	if (archivo == NULL) return;

	jugador temp;
	long ts;
	while (fscanf(archivo, "%49[^;];%d;%29[^;];%ld\n", temp.nombre, &temp.disparos, temp.fecha, &ts) == 4) {
		temp.timestamp = (time_t)ts;
		temp.existeRecord = 1;

		jugador *tmp = (jugador *)realloc(juego->ranking, (juego->cantidadRanking + 1) * sizeof(jugador));
		if (tmp == NULL) {
			printf("error de memoria cargando el ranking.\n");
			break;
		}
		juego->ranking = tmp;
		juego->ranking[juego->cantidadRanking] = temp;
		juego->cantidadRanking++;
	}
	fclose(archivo);
}

// recorre el ranking ya cargado en memoria y determina el mejor puntaje
void calcularMejorRecord(EstadoJuego *juego) {
	juego->mejorRecord.existeRecord = 0;
	strcpy(juego->mejorRecord.nombre, "Sin Récord");
	juego->mejorRecord.disparos = 0;

	for (int i = 0; i < juego->cantidadRanking; i++) {
		if (!juego->mejorRecord.existeRecord || juego->ranking[i].disparos < juego->mejorRecord.disparos) {
			juego->mejorRecord = juego->ranking[i];
			juego->mejorRecord.existeRecord = 1;
		}
	}
}

// muestra el ranking desde memoria, no vuelve a leer el archivo
void mostrarRanking(EstadoJuego *juego) {
	if (juego->cantidadRanking == 0) {
		printf("\nAun no hay registros de ranking guardados en altamar.\n");
		return;
	}

	printf("\n=======================================================\n");
	printf("                RANKING DE CADETES                     \n");
	printf("=======================================================\n");
	printf("%-5s | %-20s | %-8s | %-20s\n", "Nº", "Jugador", "Disparos", "Fecha y Hora");
	printf("-------------------------------------------------------\n");

	for (int i = 0; i < juego->cantidadRanking; i++) {
		printf("%-5d | %-20s | %-8d | %-20s\n", i + 1, juego->ranking[i].nombre, juego->ranking[i].disparos, juego->ranking[i].fecha);
	}

	printf("=======================================================\n");
}


// requisito 4: guarda todos los disparos del jugador, se acumulan entre partidas (no se sobreescriben)
void guardarHistorialDisparos(EstadoJuego *juego) {
	char nombreArchivo[100];
	snprintf(nombreArchivo, sizeof(nombreArchivo), "disparos_%s.txt", juego->nombreActual);

	FILE *archivo = fopen(nombreArchivo, "a");
	if (archivo == NULL) {
		printf("Error guardando el historial de disparos.\n");
		return;
	}

	time_t ahora = time(NULL);
	struct tm *infoTiempo = localtime(&ahora);
	char fechaFormat[30];
	strftime(fechaFormat, sizeof(fechaFormat), "%d/%m/%Y - %H:%M:%S", infoTiempo);

	// marca el inicio de la partida, no forma parte del formato fila,columna,resultado
	fprintf(archivo, "# partida del %s\n", fechaFormat);

	for (int i = 0; i < juego->tiros; i++) {
		int esHundido = (juego->historial[i].fila == juego->barcoF && juego->historial[i].columna == juego->barcoC);
		fprintf(archivo, "%d,%d,%s\n", juego->historial[i].fila, juego->historial[i].columna, esHundido ? "hundido" : "agua");
	}

	fclose(archivo);
	printf("Historial actualizado en: %s\n", nombreArchivo);
}

// Opciones 2 y 3: Guardar y Cargar Partida
// cada jugador tiene su propio archivo de guardado, identificado por su nombre
void guardarPartida(EstadoJuego *juego) {
	if (juego->tablero == NULL) {
		printf("No hay ninguna partida activa en curso para guardar.\n");
		return;
	}

	char nombreArchivo[100];
	snprintf(nombreArchivo, sizeof(nombreArchivo), "save_%s.bin", juego->nombreActual);

	FILE *archivo = fopen(nombreArchivo, "wb");
	if (archivo == NULL) {
		printf("Error al guardar la partida.\n");
		return;
	}

	// 1. Guardar variables primitivas
	fwrite(&juego->n, sizeof(int), 1, archivo);
	fwrite(&juego->barcoF, sizeof(int), 1, archivo);
	fwrite(&juego->barcoC, sizeof(int), 1, archivo);
	fwrite(&juego->tiros, sizeof(int), 1, archivo);
	fwrite(&juego->gano, sizeof(int), 1, archivo);
	fwrite(juego->nombreActual, sizeof(char), 50, archivo);

	// 2. Guardar arreglos dinámicos
	fwrite(juego->tablero, sizeof(char), juego->n * juego->n, archivo);
	if (juego->tiros > 0) {
		fwrite(juego->historial, sizeof(disparo), juego->tiros, archivo);
	}

	fclose(archivo);
	printf("\nPartida guardada exitosamente en '%s'.\n", nombreArchivo);
}

int cargarPartida(EstadoJuego *juego) {
	// primero se pide el nombre para saber que archivo de guardado buscar
	checkeonombre(juego->nombreActual, 50);

	char nombreArchivo[100];
	snprintf(nombreArchivo, sizeof(nombreArchivo), "save_%s.bin", juego->nombreActual);

	FILE *archivo = fopen(nombreArchivo, "rb");
	if (archivo == NULL) {
		printf("\nNo se encontro ninguna partida guardada para %s.\n", juego->nombreActual);
		return 0;
	}

	// Liberar memoria si había algo cargado previamente
	if (juego->tablero) free(juego->tablero);
	if (juego->historial) free(juego->historial);
	juego->tablero = NULL;
	juego->historial = NULL;

	// 1. leer primitivas, validando que cada lectura haya sido completa
	if (fread(&juego->n, sizeof(int), 1, archivo) != 1 ||
		fread(&juego->barcoF, sizeof(int), 1, archivo) != 1 ||
		fread(&juego->barcoC, sizeof(int), 1, archivo) != 1 ||
		fread(&juego->tiros, sizeof(int), 1, archivo) != 1 ||
		fread(&juego->gano, sizeof(int), 1, archivo) != 1 ||
		fread(juego->nombreActual, sizeof(char), 50, archivo) != 50) {
		printf("\nerror: el archivo de partida guardada esta corrupto o incompleto.\n");
		fclose(archivo);
		return 0;
	}

	// 2. asignar memoria y leer el tablero, validando la reserva y la lectura
	juego->tablero = (char *)malloc(juego->n * juego->n * sizeof(char));
	if (juego->tablero == NULL) {
		printf("\nerror de memoria al cargar el tablero.\n");
		fclose(archivo);
		return 0;
	}
	if (fread(juego->tablero, sizeof(char), juego->n * juego->n, archivo) != (size_t)(juego->n * juego->n)) {
		printf("\nerror: no se pudo leer el tablero guardado.\n");
		free(juego->tablero);
		juego->tablero = NULL;
		fclose(archivo);
		return 0;
	}

	// 3. asignar memoria y leer el historial de disparos, misma validacion
	if (juego->tiros > 0) {
		juego->historial = (disparo *)malloc(juego->tiros * sizeof(disparo));
		if (juego->historial == NULL) {
			printf("\nerror de memoria al cargar el historial.\n");
			free(juego->tablero);
			juego->tablero = NULL;
			fclose(archivo);
			return 0;
		}
		if (fread(juego->historial, sizeof(disparo), juego->tiros, archivo) != (size_t)juego->tiros) {
			printf("\nerror: no se pudo leer el historial guardado.\n");
			free(juego->tablero);
			free(juego->historial);
			juego->tablero = NULL;
			juego->historial = NULL;
			fclose(archivo);
			return 0;
		}
	} else {
		juego->historial = NULL;
	}

	fclose(archivo);
	printf("\nPartida cargada exitosamente. Reanudando combate de %s...\n", juego->nombreActual);
	return 1;
}
