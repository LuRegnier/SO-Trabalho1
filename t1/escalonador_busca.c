#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#define PROCESSOS 5
#define BUFFER_SIZE 256 //número de caracteres máximo de uma comando no arquivo txt
#define TRUE 1
#define FALSE 0

typedef struct { //estrutura da fila de processos
    int id;
    char nome[BUFFER_SIZE];
    int prioridade; //prioridade de um processo - menor é mais prioritário
    int i; //início - real time
    int d; //duração - real time
    int novo; //indica se o processo acabou de chegar na fila de processos
} Processo;

//vetor compartilhado
Processo *fila_processos; //vetor de processos
int total_processos = 0;

//adiciona um processo no fim da fila (no vetor compartilhado)
// Processo* adicionar_processo(Processo novo) {

//     // Copia o processo para o fim da fila
//     fila_processos[total_processos] = novo;
//     total_processos++;

//     // Retorna ponteiro para o processo recem adicionado 
//     // Mesmo usando vetor tem que ver ponteiro afeeee 
//     return &fila_processos[total_processos - 1];
// }

// Remove um processo de qualquer posição do vetor e reorganiza a fila
// Processo* retirar_processo(int indice) {
//     if (indice < 0 || indice >= total_processos) {
//         printf("Índice inválido para remoção.\n");
//         return NULL;
//     }

//     Processo *removido = malloc(sizeof(Processo));
//     *removido = fila_processos[indice];

//     // Move os elementos seguintes para a esquerda
//     for (int i = indice; i < total_processos - 1; i++) {
//         fila_processos[i] = fila_processos[i + 1];
//     }

//     total_processos--; // Atualiza tamanho da fila
//     return removido;
// }

int busca_processos(Processo* fila_processos, int segundo_atual){
    
    int indice = -1;
    int indice_maior = -1;
    int maior_prioridade = 8;  //fora do intervalo válido (0–7)

    for (int i = 0; i < PROCESSOS; i++){
        Processo p = fila_processos[i];
        if (p.prioridade ==-1 && p.i !=-1 && p.d!=-1 ) { //caso real-time
            if(p.i == segundo_atual){
                indice = i;
                return indice;
            }
        }
        else if (p.i == -1 && p.d == -1 && p.prioridade != -1) { //caso prioridade
            
            if (p.prioridade >= 0 && p.prioridade <= 7) {
                if (p.prioridade < maior_prioridade) {
                    maior_prioridade = p.prioridade;
                    indice_maior = i;
                    indice = indice_maior;
                }
            }
            
        }
        else if (p.i != -1 && p.d != -1 && p.prioridade != -1 && indice == -1) { //caso round robin
            indice = i;
        }        
    }
    return indice;
}
//func de exibir elementos da fila

// Função principal do escalonador (filho) <<<<<<<<<<<
// void escalonador(/*mem comp*/, int segundo_atual) {

//     Processo executando;
//     executando.prioridade = -1;
//     executando.i = -1;
//     executando.d = -1;

//     while (TRUE) {
//         sleep(1); //1 UT (unidade de tempo)

//         for (int i = 0; i < PROCESSOS; i++ )   { //esse for inteiro é desnecessário
//             if (p.novo == TRUE){
//                 execvp("./nome", NULL );
//                 kill(p.id, SIGSTOP);
//                 p.novo = FALSE;
//             }

//             Processo p = fila_processos[i];

//             if (p.i != -1 && p.d != -1 && p.prioridade == -1 && p.i == segundo_atual) { //caso real-time
//                 strcpy(executando.nome, p.nome);
//                 executando.prioridade = p.prioridade;
//                 executando.i = p.i;
//                 executando.d = p.d;              
//                 retirar_processo(p);
//                 kill (p.id,SIGCONT);
//                 while (executando.d > 0) {
//                     sleep(1);
//                     segundo_atual = segundo_atual + 1;
//                     executando.d = executando.d -1;
//                 }              
//             }
//             else if (acha_real_time (fila_processos) != -1) {
//                 //executa o processo do índice retornado pela func acima.
//             }
//             else if (p.i == -1 && p.d == -1 && p.prioridade != -1) { //caso prioridade

//             }
//             else if (p.i == -1 && p.d == -1 && p.prioridade == -1) { //caso round-robin
//                 //faz sla oq
//             }
//             else { //caso erro
//                 //mensagem de erro e exut(1) ou ignora e tira da fila
//             } 
//         }
//     }
// }


int main() {
    int segundo_atual = 3;

    Processo fila1[PROCESSOS] = {
        {0, "rt_3", -1, 3, 2, 1},
        {1, "prio_2", 2, -1, -1, 1},
        {2, "rr_0", 1, 0, 2, 1},
        {3, "prio_5", 5, -1, -1, 1},
        {4, "", -1, -1, -1, 0}
    };

    int idx = busca_processos(fila1, segundo_atual);
        if (idx != -1) {
            Processo escolhido = fila1[idx];
            printf(">> Processo escolhido: %s\n", fila1[idx].nome);
        } else {
            printf(">> Nenhum processo escolhido.\n");
        }

    Processo fila2[PROCESSOS] = {
        {0, "prio_1", 1, -1, -1, 1},
        {1, "prio_2", 0, -1, -1, 1},
        {2, "rr_0", -1, -1, -1, 1},
        {3, "rt_2", -1, 2, 2, 1},
        {4, "rt_1", -1, 2, 3, 0}
    };//prio_2

    int idx = busca_processos(fila2, segundo_atual);
        if (idx != -1) {
            Processo escolhido = fila2[idx];
            printf(">> Processo escolhido: %s\n", fila2[idx].nome);
        } else {
            printf(">> Nenhum processo escolhido.\n");
        }

    Processo fila3[PROCESSOS] = {
        {0, "rr_2", -1, -1, -1, 1},
        {1, "rr_3", -1, -1, -1, 1},
        {2, "prio_4", 4, -1, -1, 1},
        {3, "rt_4", -1, 4, 1, 1},
        {4, "prio_1", 1, -1, -1, 1}
    }; //prio_1

    Processo fila4[PROCESSOS] = {
        {0, "rt_1", -1, 1, 3, 1},
        {1, "rt_3", -1, 3, 1, 1},
        {2, "prio_3", 3, -1, -1, 1},
        {3, "rr_0", 6, 0, 2, 1},
        {4, "", -1, -1, -1, 0}
    }; //rt_3

    Processo fila5[PROCESSOS] = {
        {0, "prio_7", 7, -1, -1, 1},
        {1, "prio_6", 6, -1, -1, 1},
        {2, "prio_2", 2, -1, -1, 1},
        {3, "prio_1", 1, -1, -1, 1},
        {4, "prio_0", 0, -1, -1, 1}
    }; //prio_0

    Processo fila6[PROCESSOS] = {
        {0, "rr_1", -1, -1, -1, 1},
        {1, "rr_2",-1, -1, -1, 1},
        {2, "rr_3",-1, -1, -1, 1},
        {3, "rr_4", -1, -1, -1, 1},
        {4, "rr_5", -1, -1, -1, 0}
    };

    return 0;
}