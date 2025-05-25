
#ifndef T1_H
#define T1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFFER_SIZE 256
#define MAX_PROCESSOS 50
#define SHM_KEY 0x1234
#define TRUE 1
#define FALSE 0

typedef struct {
    char nome[BUFFER_SIZE];
    int prioridade;
    int i;
    int d;
    int d_original;
    int novo;
    pid_t id;
} Processo;

typedef struct {
    Processo processos[MAX_PROCESSOS];
    int tamanho;
    int segundo_atual;
} MemCompartilhada;

Processo* ler_proximo_processo(FILE* arquivo);
void carregar_processos(FILE* arquivo, MemCompartilhada* mem);
Processo* busca_real_time(MemCompartilhada* mem, int segundo_atual);
void escalonador(MemCompartilhada* mem, int tempo_maximo);

#endif