// JUNTAR ISSO DAQUI COM O INTERPRETADOR DE COMANDOS
// A struct já tá lá

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#define PROCESSOS 100

typedef struct {
    char nome[BUFFER_SIZE];
    int prioridade; //0-7
    int i; // Inicio real time
    int d; // Duração real time 
    int tipo; // 0 = PRIORIDADE, 1 = ROUND-ROBIN, 2 = REAL-TIME
} Processo;

// Vetor compartilhado
Processo *fila_processos; // Vetor de processos
int total_processos = 0;

// Adiciona um processo no fim da fila (no vetor compartilhado)
Processo* adicionar_processo(Processo novo) {

    // Copia o processo para o fim da fila
    fila_processos[total_processos] = novo;
    total_processos++;

    // Retorna ponteiro para o processo recem adicionado 
    // Mesmo usando vetor tem que ver ponteiro afeeee 
    return &fila_processos[total_processos - 1];
}

// Remove um processo de qualquer posição do vetor e reorganiza a fila
Processo* tirar_processo(int indice) {
    if (indice < 0 || indice >= total_processos) {
        printf("Índice inválido para remoção.\n");
        return NULL;
    }

    Processo *removido = malloc(sizeof(Processo));
    *removido = fila_processos[indice];

    // Move os elementos seguintes para a esquerda
    for (int i = indice; i < total_processos - 1; i++) {
        fila_processos[i] = fila_processos[i + 1];
    }

    total_processos--; // Atualiza tamanho da fila
    return removido;
}

// Fazer escalonador na base do switch q é mais fácil
// Função principal do escalonador (filho) <<<<<<<<<<<
void escalonador() {
    while (1) {
        sleep(1); 
        // 1 UT (unidade de tempo)

        // Aqui ele vai ver a lógica de escalonamento por politica
        for (int i = 0; i < total_processos; i++) {
            Processo *p = &fila_processos[i];

            switch (p->tipo) {
                case 0: // caso prioridade
                    // Selecionar o de menor prioridade
                    // Dar SIGCONT no escolhido, SIGSTOP nos outros
                    break;

                case 1: // caso round robin
                    // Executar um processo por vez
                    // 1 UT
                    break;

                case 2: // caso real time
                    // Verificar tempo atual e comparar com (i, i+d)
                    // Dar SIGCONT se dentro da janela, SIGSTOP se fora
                    break;
            }
        }
    }
}
