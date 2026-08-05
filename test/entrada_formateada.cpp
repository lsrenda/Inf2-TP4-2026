#include <iostream>
#include <cstdio>
#include <string>

int main() {
    std::string nombreArchivo = "datos.txt";
    int num1, num2;
    int leido1, leido2;

    std::cout << "=== Entrada/Salida Formateada con fprintf y fscanf ===\n";
    std::cout << "Introduce el primer número entero: ";
    std::cin >> num1;
    std::cout << "Introduce el segundo número entero: ";
    std::cin >> num2;

    // --- PARTE 1: ESCRITURA FORMATEADA ---
    FILE* archivoEscribir = std::fopen(nombreArchivo.c_str(), "w");
    if (archivoEscribir == nullptr) {
        std::perror("Error al crear el archivo de datos");
        return 1;
    }

    // Guardamos los dos enteros separados por un espacio usando fprintf
    std::fprintf(archivoEscribir, "%d %d\n", num1, num2);
    std::fclose(archivoEscribir);
    std::cout << "Datos guardados en '" << nombreArchivo << "' y archivo cerrado.\n\n";


    // --- PARTE 2: LECTURA FORMATEADA Y SUMA ---
    FILE* archivoLeer = std::fopen(nombreArchivo.c_str(), "r");
    if (archivoLeer == nullptr) {
        std::perror("Error al abrir el archivo de datos");
        return 1;
    }

    // Leemos los dos enteros usando fscanf mapeando el formato guardado
    if (std::fscanf(archivoLeer, "%d %d", &leido1, &leido2) == 2) {
        std::cout << "Datos recuperados del archivo con éxito.\n";
        std::cout << "Número 1: " << leido1 << "\n";
        std::cout << "Número 2: " << leido2 << "\n";
        
        // Realizamos la suma requerida
        int suma = leido1 + leido2;
        std::cout << "---------------------------------\n";
        std::cout << "Resultado de la suma: " << suma << "\n";
    } else {
        std::cerr << "Error: No se pudo leer el formato esperado desde el archivo.\n";
    }

    std::fclose(archivoLeer);
    return 0;
}
