// JUNTAR ISSO DAQUI COM O INTERPRETADOR DE COMANDOS
// A struct já tá lá

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <signal.h>

#define PROCESSOS 5
#define BUFFER_SIZE 256 //número de caracteres máximo de uma comando no arquivo txt

typedef struct { //estrutura da fila de processos
    char nome[BUFFER_SIZE];
    int prioridade; //prioridade de um processo - menor é mais prioritário
    int i; //início - real time
    int d; //duração - real time
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

//func de exibir elementos da fila

// Função principal do escalonador (filho) <<<<<<<<<<<
void escalonador(/*mem comp*/) {
    while (1) {
        sleep(1); 
        // 1 UT (unidade de tempo)

        // Aqui ele vai ver a lógica de escalonamento por politica
        for (int i = 0; i < total_processos; i++ /*chama funcao de busca no vetor*/)   {
            Processo *p = &fila_processos[i];

            // switch (p->tipo) {
            //     case 0: // caso prioridade
            //         Selecionar o de menor prioridade
            //         Dar SIGCONT no escolhido, SIGSTOP nos outros
            //         break;

            //     case 1: // caso round robin
            //         Executar um processo por vez
            //         1 UT
            //         break;

            //     case 2: // caso real time
            //         Verificar tempo atual e comparar com (i, i+d)
            //         Dar SIGCONT se dentro da janela, SIGSTOP se fora
            //         break;
            // }

            if (p.i != -1 && p.d != -1 && p.prioridade == -1) { //caso real-time

            }
            else if (p.i == -1 && p.d == -1 && p.prioridade != -1) { //caso prioridade

            }
            else if (p.i == -1 && p.d == -1 && p.prioridade == -1) { //caso round-robin
                //faz sla oq
            }
            else { //caso erro
                //mensagem de erro e exut(1) ou ignora e tira da fila
            } 
        }
    }
}

int main (int argc, char *argv[]) {
    //ESCALONADOR

    //fazer memória compartilhada onde a fila de processos vai ser armazenada para passar informações para o escalonador    

    Processo *fila_processos;

    //converte id da memória de string para int
    int segmento_fila = atoi(argv[1]);

    //associa a memória compartilhada ao processo
    fila_processos = (Processo *) shmat (segmento_fila, 0, 0); //comparar o retorno com -1
    if (fila_processos == (void *) -1) {
        perror("Erro ao associar memória compartilhada");
        return 1;
    }

    //chamar escalonador dando como parametro a memoria compartilhada!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // FIM DO CÓDIGO : ----------------- libera memória ------------------------

    //desanexa da memória compartilhada
    shmdt(fila_processos);

    printf("Processo ESCALONADOR: FIM\n");
}
