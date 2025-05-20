#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#define BUFFER_SIZE 256
#define TRUE 1
#define PROCESSOS 10;

typedef struct {
    char nome[BUFFER_SIZE];
    int prioridade;
    int i;
    int d;
    //ponteiro prox
} Processo;

//fazer memória compartilhada onde a fila de processos vai ser armazenada para passar informações para o escalonador

Processo* adicionar_processo() {
    //caso vazia cria a fila (lista)

    //adiciona sempre no fim da fila (lista)
}

Processo* tirar_processo() { //s'será chamada pelo escalonador quando for executar um processo
    //tira processo de qualquer posição fa fila (lista)
}

void read_command(FILE *input, char *command, char *parameters[]) {
    // lê a linha do arquivo
    if (!fgets(command, BUFFER_SIZE, input)) return;
    command[strcspn(command, "\n")] = '\0';

    int idx = 0;
    char *p = command;

    //pulando Run
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    while (*p && !isspace((unsigned char)*p)) {
        p++;
    }

    //guardando arquivo
    while (*p && isspace((unsigned char)*p)) p++;
    if (!*p) {
        parameters[0] = NULL;
        return;
    }

    parameters[idx++] = p;
    while (*p && !isspace((unsigned char)*p)) p++;
    if (*p) {
        *p = '\0';
        p++;
    }

    //guarda parâmetros P=, I= e D=
    while (*p) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;

        if ((p[0]=='P') && p[1]=='=' ) {
            parameters[idx++] = p;
        }
        else if ((p[0]=='I') && p[1]=='=' ) {
            parameters[idx++] = p;
        }
        else if ((p[0]=='D') && p[1]=='=' ) {
            parameters[idx++] = p;
        }

        while (*p && !isspace((unsigned char)*p)) p++;
        if (*p) {
            *p = '\0';
            p++;
        }
    }

    parameters[idx] = NULL;
}

int main(void) {
    char command[BUFFER_SIZE];
    char *parameters[BUFFER_SIZE / 2];
    int status;

    FILE *input = fopen("entrada.txt", "r");
    if (!input) {
        perror("Erro ao abrir entrada.txt");
        return 1;
    }

    while (TRUE) {
        sleep(1);  // espera 1 segundo (1 UT)

        read_command(input, command, parameters);
        if (feof(input)) break;  // fim do arquivo

        if (parameters[0] == NULL) continue;  // linha vazia ou mal formada

        pid_t pid = fork();
        if (pid < 0) {
            perror("Erro durante o fork!");
            exit(1);
        }
        else if (pid != 0) {
            waitpid(-1, &status, 0);
        } else {
            // Aqui entraria a lógica para adicionar à fila de prontos
            // (não implementada neste trecho)
            printf("Processo lido: %s\n", parameters[0]);
            for (int i = 1; parameters[i] != NULL; i++) {
                printf("  Param: %s\n", parameters[i]);
            }
            exit(0);  // termina o filho aqui
        }
    }

    fclose(input);
    return 0;
}