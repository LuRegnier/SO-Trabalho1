#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define BUFFER_SIZE 256 //número de caracteres máximo de uma comando no arquivo txt
#define PROCESSOS 5 //número de processos máximo para nosso escalonador

typedef struct { //estrutura da fila de processos
    char nome[BUFFER_SIZE];
    int prioridade; //prioridade de um processo - menor é mais prioritário
    int i; //início - real time
    int d; //duração - real time
} Processo;

int main(void) {
    //fazer memória compartilhada onde a fila de processos vai ser armazenada para passar informações para o escalonador

    int segmento_fila, status; //segmento_fila: mémoria compartilhada que contém o vetor, fila_processos e status
    
    Processo *fila_processos;

    //aloca a memória compartilhada
    segmento_fila = shmget (IPC_PRIVATE, (sizeof (Processo) * PROCESSOS), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        
    //associa a memória compartilhada ao processo
    fila_processos = (Processo *) shmat (segmento_fila, 0, 0); // comparar o retorno com -1

    //converte o id da memória para string para passar como argumento no execvp
    char id_memoria[16];
    sprintf(id_memoria, "%d", segmento_fila);

    int interpretador = fork();
    int escalonador = fork();

    if (interpretador < 0 || escalonador < 0) {
        if (interpretador < 0) {
            perror("Erro ao criar processo - INTERPRETADOR");
            exit(1);
        }
        else {
            perror("Erro ao criar processo - ESCALONADOR");
        }
    }
    else if (interpretador == 0){ //bloco do FILHO - INTERPRETADOR
        //aqui faremos execvp() passando nome da memoria compartilhada como argumento!
        char *args[] = {"./interpretador", id_memoria, NULL};
        execvp(args[0], args);
    }
    else if (escalonador == 0){ //bloco do FILHO - ESCALONADOR
       //aqui faremos execvp() passando nome da memoria compartilhada como argumento!
       char *args[] = {"./escalonador", id_memoria, NULL};
        execvp(args[0], args);
    }
    else { //bloco do PAI
        waitpid(-1, &status, 0); 
    }

    // FIM DO CÓDIGO : ----------------- libera memória ------------------------
    //libera a memória compartilhada do processo
    shmdt(fila_processos);

    //libera a memória compartilhada
    shmctl(segmento_fila, IPC_RMID, 0);

    printf("Processo PAI: FIM\n");

    return 0;
}