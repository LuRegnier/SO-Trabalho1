#include <stdio.h>

int main() {
    int n = 1000;

    printf("\n\n------- Sequência dos primeiros %d números ímpares -------\n\n", n);

    for (int i = 0; i < n; i++) {
        printf("ÍMPARES (iteração %d): %d\n", i, 2 * i + 1);

    }

    printf("\n\n------- FIM ÍMPARES -------\n\n");
    
    return 0;
}
