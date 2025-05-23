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
#define TRUE 1
#define FALSE 0

typedef struct { //estrutura da fila de processos
    int id;
    char nome[BUFFER_SIZE];
    int prioridade; //prioridade de um processo - menor é mais prioritário
    int i; //início - real time
    int d; //duração - real time
    int novo;
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
Processo* retirar_processo(int indice) {
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

int acha_maior_prioridade(Processo *fila_processos) {
    int indice_maior = -1;
    int maior_prioridade = 8;  //fora do intervalo válido (0–7)

    for (int i = 0; i < PROCESSOS; i++) {
        if (fila_processos[i].prioridade >= 0 && fila_processos[i].prioridade <= 7) {
            if (fila_processos[i].prioridade < maior_prioridade) {
                maior_prioridade = fila_processos[i].prioridade;
                indice_maior = i;
            }
        }
    }

    return indice_maior;  //retorna o índice do processo com maior prioridade
}

//func de exibir elementos da fila

// Função principal do escalonador (filho) <<<<<<<<<<<
void escalonador(/*mem comp*/, int segundo_atual) {

    Processo executando;
    executando.prioridade = -1;
    executando.i = -1;
    executando.d = -1;

    while (TRUE) {
        sleep(1); //1 UT (unidade de tempo)

        for (int i = 0; i < PROCESSOS; i++ /*chama funcao de busca no vetor*/)   {
            if (p.novo == TRUE){
                execvp("./nome", NULL );
                kill(p.id, SIGSTOP);
                p.novo = FALSE;
            }

            Processo p = fila_processos[i];

            int fim = p.i + p.d; //pode ser desnecessário! caso de comparação abaixo também.

            if (p.i != -1 && p.d != -1 && p.prioridade == -1 && p.i == segundo_atual && segundo_atual <= fim) { //caso real-time
                strcpy(executando.nome, p.nome);
                executando.prioridade = p.prioridade;
                executando.i = p.i;
                executando.d = p.d;
                
                retirar_processo(p);
                kill (p.id,SIGCONT);
                sleep(1);
                if (executando.d > 0) {
                    sleep(1);
                    executando.d = executando.d -1;
                }
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

    int status;

    for (int j = 0; j < PROCESSOS; j++) {
        int pid = fork();
        fila_processos.id = pid;
        if (pid < 0) {
            perror();
            exit(1);
        }
        else if (pid == 0) {
            //execvp("./nome", NULL);
            kill(p.id, SIGSTOP);
        }   
    }

    //chamar escalonador dando como parametro a memoria compartilhada!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    int segundo_atual = 0;
    //escalonador();

    // FIM DO CÓDIGO : ----------------- libera memória ------------------------

    //desanexa da memória compartilhada
    shmdt(fila_processos);

    printf("Processo ESCALONADOR: FIM\n");
}
