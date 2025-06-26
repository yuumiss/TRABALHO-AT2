#include "expressao.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define TAM_TOKEN 64

// ----------- ESTRUTURA DE PILHA -----------

typedef struct {
    char** dados;
    int topo;
    int capacidade;
} Pilha;

// Cria e inicializa uma nova pilha
Pilha* criarPilha(int capacidade) {
    Pilha* p = (Pilha*)malloc(sizeof(Pilha));
    p->dados = (char**)malloc(sizeof(char*) * capacidade);
    p->topo = -1;
    p->capacidade = capacidade;
    return p;
}

// Insere elemento no topo da pilha
void empilhar(Pilha* p, char* elemento) {
    p->dados[++p->topo] = strdup(elemento);
}

// Remove o topo da pilha
char* desempilhar(Pilha* p) {
    if (p->topo == -1) return NULL;
    return p->dados[p->topo--];
}

// Retorna o elemento no topo sem remover
char* topo(Pilha* p) {
    if (p->topo == -1) return NULL;
    return p->dados[p->topo];
}

// Verifica se a pilha está vazia
int estaVazia(Pilha* p) {
    return p->topo == -1;
}

// Libera a memória da pilha
void liberarPilha(Pilha* p) {
    while (!estaVazia(p)) free(desempilhar(p));
    free(p->dados);
    free(p);
}

// ----------- FUNÇÕES AUXILIARES -----------

// Define precedência dos operadores
int precedencia(char* op) {
    if (strcmp(op, "sin") == 0 || strcmp(op, "cos") == 0 || strcmp(op, "v") == 0) return 4;
    if (strcmp(op, "^") == 0) return 3;
    if (strcmp(op, "*") == 0 || strcmp(op, "/") == 0) return 2;
    if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0) return 1;
    return 0;
}

// Verifica se o token é um operador
int ehOperador(char* token) {
    return strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
           strcmp(token, "*") == 0 || strcmp(token, "/") == 0 ||
           strcmp(token, "^") == 0 || strcmp(token, "v") == 0 ||
           strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0;
}

// ----------- CONVERSÃO: INFIXA ? PÓS-FIXADA -----------

char* infixaParaPosfixa(const char* expressao) {
    Pilha* operadores = criarPilha(100);
    char* saida = (char*)malloc(1024);
    saida[0] = '\0';

    char token[TAM_TOKEN];
    int i = 0, j;

    while (expressao[i] != '\0') {
        if (isspace(expressao[i])) { i++; continue; }

        if (isdigit(expressao[i]) || expressao[i] == '.') {
            j = 0;
            while (isdigit(expressao[i]) || expressao[i] == '.') {
                token[j++] = expressao[i++];
            }
            token[j] = '\0';
            strcat(saida, token);
            strcat(saida, " ");
            continue;
        }

        if (isalpha(expressao[i])) {
            j = 0;
            while (isalpha(expressao[i])) {
                token[j++] = expressao[i++];
            }
            token[j] = '\0';
            empilhar(operadores, token);
            continue;
        }

        char op[2] = {expressao[i], '\0'};

        if (op[0] == '(') {
            empilhar(operadores, op);
        } else if (op[0] == ')') {
            while (!estaVazia(operadores) && strcmp(topo(operadores), "(") != 0) {
                strcat(saida, desempilhar(operadores));
                strcat(saida, " ");
            }
            desempilhar(operadores); // Remove o '('
        } else {
            while (!estaVazia(operadores) &&
                   precedencia(topo(operadores)) >= precedencia(op)) {
                strcat(saida, desempilhar(operadores));
                strcat(saida, " ");
            }
            empilhar(operadores, op);
        }

        i++;
    }

    while (!estaVazia(operadores)) {
        strcat(saida, desempilhar(operadores));
        strcat(saida, " ");
    }

    liberarPilha(operadores);
    return saida;
}

// ----------- CONVERSÃO: PÓS-FIXADA ? INFIXA -----------

char* posfixaParaInfixa(const char* expressao) {
    Pilha* pilha = criarPilha(100);
    char copia[1024];
    strcpy(copia, expressao);
    char* token = strtok(copia, " ");

    while (token != NULL) {
        if (ehOperador(token)) {
            if (strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "v") == 0) {
                char* a = desempilhar(pilha);
                char* res = (char*)malloc(128);
                sprintf(res, "%s(%s)", token, a);
                empilhar(pilha, res);
                free(a);
            } else {
                char* b = desempilhar(pilha);
                char* a = desempilhar(pilha);
                char* res = (char*)malloc(128);
                sprintf(res, "(%s %s %s)", a, token, b);
                empilhar(pilha, res);
                free(a); free(b);
            }
        } else {
            empilhar(pilha, token);
        }

        token = strtok(NULL, " ");
    }

    char* resultado = strdup(desempilhar(pilha));
    liberarPilha(pilha);
    return resultado;
}

// ----------- AVALIAÇÃO DE EXPRESSÃO PÓS-FIXADA -----------

float avaliarPosfixa(const char* expressao) {
    Pilha* pilha = criarPilha(100);
    char copia[1024];
    strcpy(copia, expressao);
    char* token = strtok(copia, " ");

    while (token != NULL) {
        if (!ehOperador(token)) {
            empilhar(pilha, token);
        } else {
            float resultado = 0;
            float b = atof(desempilhar(pilha));
            float a = 0;

            if (!(strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0 || strcmp(token, "v") == 0)) {
                a = atof(desempilhar(pilha));
            }

            if (strcmp(token, "+") == 0) resultado = a + b;
            else if (strcmp(token, "-") == 0) resultado = a - b;
            else if (strcmp(token, "*") == 0) resultado = a * b;
            else if (strcmp(token, "/") == 0) resultado = a / b;
            else if (strcmp(token, "^") == 0) resultado = powf(a, b);
            else if (strcmp(token, "v") == 0) resultado = sqrtf(b);
            else if (strcmp(token, "sin") == 0) resultado = sinf(b);
            else if (strcmp(token, "cos") == 0) resultado = cosf(b);

            char* resStr = (char*)malloc(32);
            sprintf(resStr, "%f", resultado);
            empilhar(pilha, resStr);
        }

        token = strtok(NULL, " ");
    }

    float resultadoFinal = atof(desempilhar(pilha));
    liberarPilha(pilha);
    return resultadoFinal;
}

