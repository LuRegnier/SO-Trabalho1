#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>

#define BUFFER_SIZE 256
#define MAX_PROCESSOS 10

typedef struct {
    char nome[BUFFER_SIZE];
    int prioridade;
    int i;
    int d;
} Processo;

int main() {
    // Criação da memória compartilhada
    int shm_id = shmget(IPC_PRIVATE, sizeof(Processo) * MAX_PROCESSOS, IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("Erro ao criar memória compartilhada");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Erro no fork");
        return 1;
    }

    if (pid == 0) {
        // Processo filho: executa interpretador com execvp
        char shm_id_str[16];
        snprintf(shm_id_str, sizeof(shm_id_str), "%d", shm_id);
        char *args[] = {"./interpretador", shm_id_str, NULL};

        execvp(args[0], args);
        perror("Erro ao executar interpretador");
        exit(1);
    } else {
        // Processo pai: espera o interpretador terminar
        wait(NULL);

        // Acessa a memória compartilhada para ler os processos
        Processo *processos = (Processo *) shmat(shm_id, NULL, 0);
        if (processos == (void *) -1) {
            perror("Erro ao acessar memória compartilhada no pai");
            return 1;
        }

        printf("Processos armazenados na memória compartilhada:\n");
        for (int i = 0; i < MAX_PROCESSOS; i++) {
            if (strlen(processos[i].nome) == 0) break; // fim dos processos válidos
            printf("Processo %d:\n", i);
            printf("  Nome: %s\n", processos[i].nome);
            printf("  Prioridade: %d\n", processos[i].prioridade);
            printf("  Início: %d\n", processos[i].i);
            printf("  Duração: %d\n", processos[i].d);
        }

        // Libera memória compartilhada
        shmdt(processos);
        shmctl(shm_id, IPC_RMID, NULL); // remove memória
    }

    return 0;
}
