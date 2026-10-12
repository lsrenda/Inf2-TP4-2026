#include <iostream>
#include <cstdio>
#include <string>

int main() {
    std::string nombreArchivo;
    std::string texto;

    // 1. Solicitar datos al usuario
    std::cout << "=== Escritura con fputc y fputs ===\n";
    std::cout << "Introduce el nombre del archivo (ej: nota.txt): ";
    std::cin >> nombreArchivo;
    std::cin.ignore(); // Limpiar el buffer del salto de línea anterior

    std::cout << "Introduce el texto que deseas guardar: ";
    std::getline(std::cin, texto);

    // 2. Abrir el archivo en modo escritura ("w")
    FILE* archivo = std::fopen(nombreArchivo.c_str(), "w");
    if (archivo == nullptr) {
        std::perror("Error al abrir el archivo");
        return 1;
    }

    // 3. Demostración de fputc: Escribimos el texto carácter por carácter
    std::cout << "\nGuardando texto carácter por carácter con fputc...\n";
    for (char c : texto) {
        std::fputc(c, archivo);
    }
    
    // Agregamos un salto de línea manual con fputc
    std::fputc('\n', archivo);

    // 4. Demostración de fputs: Escribimos una cadena constante completa
    std::cout << "Agregando línea de cierre con fputs...\n";
    std::fputs("--- Fin del reporte generado automáticamente ---\n", archivo);

    // 5. Cerrar el archivo
    std::fclose(archivo);
    std::cout << "Archivo guardado exitosamente.\n";

    return 0;
}
