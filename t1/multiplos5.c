#include <stdio.h>

int main() {
    int n = 1000;

    printf("\n\n------- Sequência dos primeiros %d múltiplos de 5 -------\n\n", n);

    for (int i = 1; i <= n; i++) {
        printf("MÚLTIPLOS DE 5 (iteração %d): %d\n", i, i * 5);
    }

    printf("\n\n------- FIM MÚLTIPLOS DE 5 -------\n\n");

    return 0;
}