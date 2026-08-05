#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>

int main() {
    std::string nombreArchivo;
    char palabraBuscada[50];
    char linea[256];
    int contador = 0;

    std::cout << "=== Búsqueda con fgetc y fgets ===\n";
    std::cout << "Introduce el nombre del archivo a leer: ";
    std::cin >> nombreArchivo;
    std::cout << "Introduce la palabra que deseas buscar: ";
    std::cin >> palabraBuscada;

    // 1. Abrir el archivo en modo lectura ("r")
    FILE* archivo = std::fopen(nombreArchivo.c_str(), "r");
    if (archivo == nullptr) {
        std::perror("Error al abrir el archivo");
        return 1;
    }

    // 2. Demostración de fgets: Leemos línea por línea
    std::cout << "\nBuscando coincidencias...\n";
    while (std::fgets(linea, sizeof(linea), archivo) != nullptr) {
        // Buscamos la palabra dentro de la línea leída
        char* posicion = std::strstr(linea, palabraBuscada);
        while (posicion != nullptr) {
            contador++;
            // Avanzamos la posición para ver si la palabra se repite en la misma línea
            posicion = std::strstr(posicion + std::strlen(palabraBuscada), palabraBuscada);
        }
    }

    // 3. Demostración de fgetc: Volvemos al inicio para mostrar cómo leer carácter por carácter si fuera necesario
    // (En este caso, solo verificamos si realmente llegamos al final EOF)
    std::fseek(archivo, 0, SEEK_SET); // Rebobinar el archivo al inicio
    int c = std::fgetc(archivo);
    if (c == EOF) {
        std::cout << "(El archivo estaba vacío)\n";
    }

    // 4. Cerrar archivo y mostrar resultados
    std::fclose(archivo);
    std::cout << "La palabra '" << palabraBuscada << "' se encontró " << contador << " veces.\n";

    return 0;
}
