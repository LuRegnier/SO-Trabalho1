#include <stdio.h>

int main() {
    int n = 5;
    
    printf("\n\n------- Sequência dos primeiros %d números pares -------\n\n", n);

    for (int i = 1; i <= n; i++) {
        printf("PARES (iteração %d): %d\n", i, i * 2);
    }
    
    printf("\n\n------- FIM PARES -------\n\n");

    return 0;
}