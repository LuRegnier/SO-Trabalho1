#include <stdio.h>
#include <unistd.h>

int main() {
    int n = 93;
    unsigned long long primeiro = 1, segundo = 1, proximo;

    printf("\n\n------- Sequência de Fibonacci -------\n\n");

    printf("FIBONACCI (iteração %d): %llu\n", 1, primeiro);
    printf("------- dormindo até o próximo FIBONACCI -------\n\n");
    sleep(1);
    printf("------- Acordou! -------\n");
    printf("FIBONACCI (iteração %d): %llu\n", 2, segundo);
    printf("------- dormindo até o próximo FIBONACCI -------\n\n");
    sleep(1);

    for (int i = 3; i <= n; i++) {
        printf("------- Acordou! -------\n");
        proximo = primeiro + segundo;
        printf("FIBONACCI (iteração %d): %llu\n", i, proximo);
        primeiro = segundo;
        segundo = proximo;
        printf("------- dormindo até o próximo FIBONACCI -------\n\n");
        sleep(1);
    }

    printf("\n\n------- FIM FIBONACCI -------\n\n");
    
    return 0;
}
