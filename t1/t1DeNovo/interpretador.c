#include "t1.h"

Processo* ler_proximo_processo(FILE* arquivo) {
    Processo* novo = malloc(sizeof(Processo));
    if (!novo) return NULL;

    char linha[BUFFER_SIZE];
    if (fgets(linha, BUFFER_SIZE, arquivo) == NULL) {
        free(novo);
        return NULL;
    }

    novo->i = -1;
    novo->d = -1;
    novo->prioridade = -1;
    novo->d_original = -1;
    novo->novo = TRUE;
    novo->id = -1;

    char nome[BUFFER_SIZE];
    int valor1, valor2;

    if (strstr(linha, "P=")) {
        if (sscanf(linha, "Run %s P=%d", nome, &valor1) == 2) {
            nome[strcspn(nome, "\n")] = '\0';
            strcpy(novo->nome, nome);
            novo->prioridade = valor1;
        }
    } else if (strstr(linha, "I=") && strstr(linha, "D=")) {
        if (sscanf(linha, "Run %s I=%d D=%d", nome, &valor1, &valor2) == 3) {
            nome[strcspn(nome, "\n")] = '\0';
            strcpy(novo->nome, nome);
            novo->i = valor1;
            novo->d = valor2;
            novo->d_original = valor2;
        }
    } else {
        if (sscanf(linha, "Run %s", nome) == 1) {
            nome[strcspn(nome, "\n")] = '\0';
            strcpy(novo->nome, nome);
        }
    }

    return novo;
}

void carregar_processos(FILE* arquivo, MemCompartilhada* mem) {
    mem->tamanho = 0;
    Processo* p;
    
    while ((p = ler_proximo_processo(arquivo)) != NULL && mem->tamanho < MAX_PROCESSOS) {
        mem->processos[mem->tamanho++] = *p;
        free(p);
        printf("Interpretador: adicionou processo %s\n", mem->processos[mem->tamanho - 1].nome);
        sleep(1);
    }
}









