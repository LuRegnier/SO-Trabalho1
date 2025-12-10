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
#define PROCESSOS 100;

typedef struct {
    char nome[BUFFER_SIZE];
    int prioridade;
    int i;
    int d;
} Processo;

Processo* adicionar_processo() {
    //adiciona sempre no fim da fila (memória compartilhada)
}

Processo* tirar_processo() { //só será chamada pelo escalonador quando for executar um processo
    //tira processo de qualquer posição da fila (memória compartilhada) e reorganiza o vetor
}

int main (void) {
    //INTERPRETADOR

    FILE *input = fopen("exec.txt", "r");

    if (!input) {
        perror("Erro ao abrir exec.txt");
        return 1;
    }

    //fazer memória compartilhada onde a fila de processos vai ser armazenada para passar informações para o escalonador

    int segmento_fila, status; //segmento_fila: mémoria compartilhada que contém o vetor, fila_processos e status
    
    int *fila_processos;

    //aloca a memória compartilhada
    segmento_fila = shmget (IPC_PRIVATE, (sizeof (int) * PROCESSOS), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
        
    //associa a memória compartilhada ao processo
    fila_processos = (int *) shmat (segmento_fila, 0, 0); // comparar o retorno com -1

    //ler arquivo linha por linha com uma pausa de 1 ut (segundo) entre cada leitura!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    //para cada elemento lido no arquivo txt é colocado na memória compartilhada
    for (int j = 0; j < PROCESSOS; j++) {
        //fila_processos[j].prioridade =;
        //.i=;
        //.d=;
    }

    int id = fork();
    if (id < 0) {
        perror("Erro ao criar processo");
        exit(1);
    }
    else if (id != 0){ //bloco do PAI
       waitpid(-1, &status, 0); 
    }
    else { //bloco do FILHO
    //processo do ESCALONADOR
    //chama função escalonador()?
    }

    //libera a memória compartilhada do processo
    shmdt(fila_processos);


    //libera a memória compartilhada
    shmctl(segmento_fila, IPC_RMID, 0);

    //fechando o arquivo 
    fclose(input);

    printf("Processo PAI: FIM\n");

    return 0;
}

