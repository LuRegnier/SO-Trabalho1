#include <stdio.h>

int main() {
    int n = 5;

    printf("\n\n------- Sequência dos primeiros %d números naturais -------\n\n", n);

    for (int i = 0; i < n; i++) {
        printf("NÚMEROS NATURAIS (iteração %d): %d\n", i, i);
    }

    printf("\n\n------- FIM NATURAIS -------\n\n");
    return 0;
}    