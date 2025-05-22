#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sys/shm.h>

#define BUFFER_SIZE 256 //número de caracteres máximo de uma comando no arquivo txt
#define PROCESSOS 5 //número de processos máximo para nosso escalonador

typedef struct { //estrutura da fila de processos
    char nome[BUFFER_SIZE];
    int prioridade; //prioridade de um processo - menor é mais prioritário
    int i; //início - real time
    int d; //duração - real time
} Processo;

void reconhece_comando(FILE *input, Processo *vetor_processos, int *indice_atual) { //função de interpretação de comandos do arquivo exec.txt
    char linha[BUFFER_SIZE];

    //lê uma única linha do arquivo
    if (fgets(linha, sizeof(linha), input) != NULL) {
        Processo p;
        p.prioridade = -1;
        p.i = -1;
        p.d = -1;

        //verifica se linha começa com "Run"
        if (strncmp(linha, "Run ", 4) == 0) {
            char *token = strtok(linha + 4, " \n"); //pula "Run "

            //primeiro token após "Run" é o nome do arquivo -> deve ser guardado
            if (token != NULL) {
                strncpy(p.nome, token, BUFFER_SIZE);
            }

            //processa os parâmetros restantes -> guarda prioridade ou início e duração
            while ((token = strtok(NULL, " \n")) != NULL) {
                if (strncmp(token, "P=", 2) == 0) {
                    p.prioridade = atoi(token + 2);
                } else if (strncmp(token, "I=", 2) == 0) {
                    p.i = atoi(token + 2);
                } else if (strncmp(token, "D=", 2) == 0) {
                    p.d = atoi(token + 2);
                }
            }

            //armazena o processo no vetor
            vetor_processos[*indice_atual] = p;
            (*indice_atual)++;
        }
    }
}

int main (int argc, char *argv[]) {
    //INTERPRETADOR

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

    FILE *input = fopen("exec.txt", "r");
    if (!input) {
        perror("Erro ao abrir entrada.txt");
        return 1;
    } 

    int indice = 0;

    //ler arquivo linha por linha com uma pausa de 1 ut (segundo) entre cada leitura!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    while (!feof(input)) {
        reconhece_comando(input, fila_processos, &indice);
        sleep(1);
    }

    // FIM DO CÓDIGO : ----------------- libera memória, libera o arquivo texto ------------------------

    //fechando o arquivo 
    fclose(input);

    //desanexa da memória compartilhada
    shmdt(fila_processos);

    printf("Processo INTERPRETADOR: FIM\n");
}