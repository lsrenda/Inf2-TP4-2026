#include <stdio.h>

void duplicar(int *ptr) {
    *ptr = (*ptr) * 2; // Desreferenciación: modifica directamente la variable original
}

int main() {
    int x = 10;
    int *p = &x; // 'p' guarda la dirección de memoria de 'x'
    int **h = &p;

    printf("Valor de x: %d\n", x);
    printf("Dirección de x: %p\n", (void*)&x);
    printf("Valor al que apunta p: %d\n", *p);

    duplicar(&x); // Pasamos la dirección de memoria de 'p'
    printf("Valor de x duplicado: %d\n", x);

    return 0;
}
