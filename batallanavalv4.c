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

typedef struct  {
  char nombre[50];
	char fecha[30]; // "DD/MM/AAAA - HH:MM:SS"
  int disparos;
}record;

typedef struct  {
  int fila;
  int columna;
}disparo;

typedef struct  {
  char *tablero; // Tablero dinámico
  char nombreActual[50];
  int n;
  int barcoF;
  int barcoC;
  int opcion;
  int tiros;
  int gano;
  disparo *historial;
  record ranking;
} EstadoJuego;

void inicializarTablero(EstadoJuego *juego);
void mostrarTablero(EstadoJuego *juego);
int realizarDisparo(EstadoJuego *juego, int f, int c);
void checkeoscan(int *destino, const char *mensaje_error);
int cargarGanador(EstadoJuego *juego);
void checkeonombre (char *destino, int tamano);

int main() {
  time_t tiempoActual;
  struct tm *infoTiempo;

  EstadoJuego mega;

  strcpy(mega.ranking.nombre, "Lucho");
  mega.ranking.disparos = 9999999;

  int resultado;

  srand(time(NULL));

  do {
    printf("\n=== TP3 INFORMATICA II ===\n");

    if (mega.ranking.disparos < 9999998) {
        printf("El cadete del mes: %s con %d tiros\n", mega.ranking.nombre, mega.ranking.disparos);
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

      inicializarTablero(&mega);
      printf("Cual es tu nombre muchacho: ");
      checkeonombre (mega.nombreActual, 50);
      while (!mega.gano) {
        mostrarTablero(&mega);
        printf("Disparo actual: %d\n", mega.tiros + 1);
        int f_actual, c_actual;

        printf("Coordenadas:\nFila: ");
        checkeoscan(&f_actual, "Sigue asi recluta y jamas veras el altamar.");

        printf("Columna: ");
        checkeoscan(&c_actual, "Sigue asi recluta y jamas veras el altamar.");

        int res = realizarDisparo(&mega, f_actual, c_actual);
        if (res != -1) {
          mega.tiros++;
          mega.historial = (disparo *)realloc(mega.historial, mega.tiros * sizeof(disparo));
          if (mega.historial == NULL) {
            printf("Error fatal: No hay memoria suficiente.\n");
            return 1;
          }
          mega.historial[mega.tiros - 1].fila = f_actual;
          mega.historial[mega.tiros - 1].columna = c_actual;
          if (res == 1) mega.gano = 1; // Le dio al barco
        }
      }
      mostrarTablero(&mega);
      if (mega.gano) {
        printf("\nTe felicito %s, ya eres parte de esta tripulacion.\nLe diste en %d canhonazos.\n", mega.nombreActual, mega.tiros);
        if (mega.tiros < mega.ranking.disparos) {
          printf("\n¡Y CON UN NUEVO RECORD!");
          scanf("%s", mega.ranking.nombre);
          mega.ranking.disparos = mega.tiros;
        }
      }
      printf("\n--- SECUENCIA DE DISPAROS ---\n");
      for (int i = 0; i < mega.tiros; i++) {
        printf("Tiro %d: Fila %d, Columna %d\n", i + 1, mega.historial[i].fila, mega.historial[i].columna);
      }
      free(mega.tablero);
      free(mega.historial);
    }
    if (mega.opcion == 2) {
      //aca va guardar partida
    }

    if (mega.opcion == 3) {
      //aca va cargar partida
    }

    if (mega.opcion == 4) {
      //aca va ver el ranking de la partida
    }
  }
  while (mega.opcion != 5);

  return 0;
}

void inicializarTablero( EstadoJuego *juego) {
  juego->tablero = (char *)malloc(juego->n * (*juego).n * sizeof(char));// jurgo->n == (*juego).n reservar memoria equivalente a n² elementos char
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

void mostrarTablero( EstadoJuego *juego) {
  printf("\n    ");
  for (int i = 0; i < juego->n; i++) printf("%2d ", i);// %2 reserva 2 espacios de caracter %.2 limita a 2 decimales(%.2f)
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

int realizarDisparo( EstadoJuego *juego, int f, int c) {
  // Validar límites
  if (f < 0 || f >= juego->n || c < 0 || c >= juego->n) {
    printf("\nTecnicamente... no fallaste. Para fallar, la bala tiene que haber tenido alguna posibilidad de dar. Lo tuyo fue otra cosa. Lo tuyo fue... arte.\n!\n");
    return -1; // -1 indica disparo inválido
  }
  
  // Validar si ya disparó ahí
  if (juego->tablero[(f * juego->n) + c] != '.') {
    printf("\nYa has disparado ahi, y ya has fallado ahi! Te perdonare la bala por esta vez recluta.\n");
    return -1; 
  }
  
  // Comprobar si acertó
  if (f == juego->barcoF && c == juego->barcoC) {
    juego->tablero[(f * juego->n) + c] = 'B';
    return 1; // 1 indica que ganó
  } else {
    juego->tablero[(f * juego->n) + c] = 'A';
    printf("\nAGUA!\n");
    return 0; // 0 indica que falló, pero el disparo es válido
  }
}

void checkeoscan(int *destino, const char *mensaje_error) {
  int leidos;
  while (1) {// 1 significa caracter invalido para el tipo requerido
    while (getchar() != '\n');// limpiar el buffer
    printf("%s", mensaje_error);
    leidos = scanf("%d", destino);
    if (leidos == 1) {//hay que ser boludo igual
      break; 
    }
  }
}

void checkeonombre (char *destino, int tamano) {
  while (1) {
    printf("Cual es tu nombre muchacho: ");
    if (fgets(destino, tamano, stdin) != NULL) {
      destino[strcspn(destino, "\n")] = '\0';// requisito para el \n que incluye fgets en lugar de \0 que indica fin de cadena de texto

      if (strlen(destino) > 0) {
        break; //el nombre no es nulo, salimos
      }
    }
    printf("No te me hagas el gracioso recluta, dime tu nombre.\n");
  }
}


int cargarGanador(EstadoJuego *juego) {
  FILE *archivo = fopen("ranking.bin", "ab");
  if (archivo == NULL) {
    printf("Error: No se pudo abrir o crear el archivo 'ranking.bin'.\n");
    return 1;
  }

  time_t tiempoActual;
  struct tm *infoTiempo;

  time(&tiempoActual);
  infoTiempo = localtime(&tiempoActual);
  strftime(juego->ranking.fecha, sizeof(juego->ranking.fecha), "%d/%m/%Y - %H:%M:%S", infoTiempo);

  fwrite(&juego->ranking, sizeof(record), 1, archivo);
  fclose(archivo);

  printf("Lo anote detras del mapa camarada, lo podras ver cuando estemos en altamar, bienvenido.\n");
  return 0;
}


//quedo hacer el grardado cargado y ranking basicamente todo pero andamos bulletproofeados

