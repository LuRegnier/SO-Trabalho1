#include "t1.h"

int main() {
    int shmid = shmget(SHM_KEY, sizeof(MemCompartilhada), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("Erro no shmget");
        return 1;
    }

    MemCompartilhada* mem = (MemCompartilhada*) shmat(shmid, NULL, 0);
    if (mem == (void*) -1) {
        perror("Erro no shmat");
        return 1;
    }

    mem->tamanho = 0;
    mem->segundo_atual = 0;

    FILE* arquivo = fopen("exec.txt", "r");
    if (!arquivo) {
        perror("Erro ao abrir exec.txt");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Erro no fork");
        return 1;
    }

    if (pid == 0) {
        // Processo filho: interpretador
        carregar_processos(arquivo, mem);
        exit(0);
    } else {
        // Processo pai: escalonador
        escalonador(mem, 120);
        wait(NULL); // espera interpretador terminar (opcional)
        shmdt(mem);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}

