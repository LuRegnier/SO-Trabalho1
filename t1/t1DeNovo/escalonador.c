#include "t1.h"

Processo* busca_real_time(MemCompartilhada* mem, int segundo_atual) {
    for (int i = 0; i < mem->tamanho; i++) {
        Processo* p = &mem->processos[i];
        if (p->prioridade == -1 && p->i != -1 && p->d != -1) {
            if (p->i == segundo_atual || p->i + 60 == segundo_atual) {
                return p;
            }
        }
    }
    return NULL;
}

void remover_processos_e_deslocar(MemCompartilhada* mem, int indice) {
    for (int i = indice; i < mem->tamanho - 1; i++) {
        mem->processos[i] = mem->processos[i + 1];
    }
    mem->tamanho--;
}

void escalonador(MemCompartilhada* mem, int tempo_maximo) {
    for (mem->segundo_atual = 0; mem->segundo_atual <= tempo_maximo; mem->segundo_atual++) {
        printf("\n[Segundo %d]\n", mem->segundo_atual);

        Processo* em_exec = busca_real_time(mem, mem->segundo_atual);
        int indice_exec = 0;

        if (!em_exec) {
            if (mem->tamanho == 0) {
                printf("Nenhum processo disponível.\n");
                sleep(1);
                continue;
            }

            em_exec = &mem->processos[0];

            // Se for real-time mas fora do tempo → reposiciona
            if (em_exec->prioridade == -1 && em_exec->i != -1 && em_exec->d != -1) {
                if (em_exec->i != mem->segundo_atual && em_exec->i + 60 != mem->segundo_atual) {
                    Processo temp = *em_exec;
                    remover_processos_e_deslocar(mem, 0);
                    mem->processos[mem->tamanho++] = temp;

                    printf("Ignorando real-time %s fora do tempo, enviado ao fim da fila\n", temp.nome);
                    mem->segundo_atual--; // não consome segundo
                    continue;
                }
            }
        }

        // Encontra índice do processo a executar
        for (int i = 0; i < mem->tamanho; i++) {
            if (&mem->processos[i] == em_exec) {
                indice_exec = i;
                break;
            }
        }

        if (em_exec->novo) {
            pid_t pid = fork();
            if (pid == 0) {
                char path[BUFFER_SIZE];
                snprintf(path, sizeof(path), "./%s", em_exec->nome);
                char* args[] = {path, NULL};
                execvp(args[0], args);
                perror("execvp falhou");
                exit(1);
            } else if (pid > 0) {
                em_exec->id = pid;
                sleep(1);  // deixa tempo para executar antes de pausar
                kill(pid, SIGSTOP);
                //em_exec->novo = FALSE;
            } else {
                perror("Erro no fork");
                exit(1);
            }
        }

        // Execução real-time
        if (em_exec->prioridade == -1 && em_exec->i != -1 && em_exec->d != -1) {
            printf("Executando (real-time): %s\n", em_exec->nome);
            while (em_exec->d > 0) {
                kill(em_exec->id, SIGCONT);
                sleep(1);
                kill(em_exec->id, SIGSTOP);

                printf("... %s (restante: %d)\n", em_exec->nome, em_exec->d);
                em_exec->d--;
                mem->segundo_atual++;
            }
            em_exec->d = em_exec->d_original;
        }



        // Execução normal (prioridade ou round-robin)
        else {
            printf("Executando: %s\n", em_exec->nome);
            sleep(1);
        }

        // Reinsere no final
        Processo temp = *em_exec;
        remover_processos_e_deslocar(mem, indice_exec);
        mem->processos[mem->tamanho++] = temp;
    }
}