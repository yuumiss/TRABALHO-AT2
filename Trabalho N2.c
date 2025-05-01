
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILMES 10
#define LINHAS 5
#define COLUNAS 5
#define MAX_BILHETES 100

// Estruturas para armazenar os dados do sistema

typedef struct {
    char nome[50];
    char tipo_ingresso[10]; // "inteiro" ou "meia"
    int numero_bilhete;
    int linha;
    int coluna;
} Bilhete;

typedef struct {
    char nome[50];
    char data_lancamento[20];
} Filme;

typedef struct {
    char matriz[LINHAS][COLUNAS]; // Matriz que representa os lugares
    Bilhete bilhetes[MAX_BILHETES];
    Filme filmes[MAX_FILMES];
    int contador_bilhetes;
} Sistema;

// Funções do sistema

void inicializarSistema(Sistema* sistema) {
    // Inicializa a matriz de assentos com 'D' (disponível)
    int i, j; // Declara as variáveis fora do loop
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            sistema->matriz[i][j] = 'D'; // 'D' para disponível
        }
    }
    sistema->contador_bilhetes = 0;
}

void listarFilmes(Filme filmes[], int n) {
    printf("Filmes disponíveis:\n");
    int i; // Declara a variável fora do loop
    for (i = 0; i < n; i++) {
        printf("%d. %s (Lançamento: %s)\n", i + 1, filmes[i].nome, filmes[i].data_lancamento);
    }
}

void exibirAssentos(Sistema* sistema) {
    printf("Assentos disponíveis (D) e ocupados (X):\n");
    int i, j; // Declara as variáveis fora do loop
    for (i = 0; i < LINHAS; i++) {
        for (j = 0; j < COLUNAS; j++) {
            printf("%c ", sistema->matriz[i][j]);
        }
        printf("\n");
    }
}

int reservarLugar(Sistema* sistema, int linha, int coluna, int numero_bilhete, char tipo_ingresso[], char nome_cliente[]) {
    if (sistema->matriz[linha][coluna] == 'D') {
        sistema->matriz[linha][coluna] = 'X'; // Marca como ocupado
        Bilhete novoBilhete;
        novoBilhete.numero_bilhete = numero_bilhete;
        novoBilhete.linha = linha;
        novoBilhete.coluna = coluna;
        strcpy(novoBilhete.tipo_ingresso, tipo_ingresso);
        strcpy(novoBilhete.nome, nome_cliente);
        sistema->bilhetes[sistema->contador_bilhetes++] = novoBilhete;
        printf("Lugar reservado com sucesso! Número do bilhete: %d\n", numero_bilhete);
        return 1;
    } else {
        printf("Assento já ocupado! Tente outro lugar.\n");
        return 0;
    }
}

int buscarBilhete(Bilhete bilhetes[], int n, int numero_bilhete) {
    int inicio = 0;
    int fim = n - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        if (bilhetes[meio].numero_bilhete == numero_bilhete) {
            return meio; // Bilhete encontrado
        } else if (bilhetes[meio].numero_bilhete < numero_bilhete) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1; // Bilhete não encontrado
}

void imprimirBilhete(Sistema* sistema, int numero_bilhete) {
    int indice = buscarBilhete(sistema->bilhetes, sistema->contador_bilhetes, numero_bilhete);
    if (indice != -1) {
        Bilhete bilhete = sistema->bilhetes[indice];
        printf("Bilhete encontrado:\n");
        printf("Número do Bilhete: %d\n", bilhete.numero_bilhete);
        printf("Nome do Cliente: %s\n", bilhete.nome);
        printf("Tipo de Ingresso: %s\n", bilhete.tipo_ingresso);
        printf("Assento: Linha %d, Coluna %d\n", bilhete.linha + 1, bilhete.coluna + 1);
    } else {
        printf("Bilhete não encontrado!\n");
    }
}

void exibirRelatorio(Sistema* sistema) {
    printf("Relatório de bilhetes emitidos:\n");
    int i; // Declara a variável fora do loop
    for (i = 0; i < sistema->contador_bilhetes; i++) {
        printf("Bilhete %d: %s, Assento: %d,%d, Tipo: %s\n",
               sistema->bilhetes[i].numero_bilhete,
               sistema->bilhetes[i].nome,
               sistema->bilhetes[i].linha + 1,
               sistema->bilhetes[i].coluna + 1,
               sistema->bilhetes[i].tipo_ingresso);
    }
}

void menuPrincipal(Sistema* sistema) {
    int opcao, filmeEscolhido, linha, coluna;
    char tipo_ingresso[10], nome_cliente[50];
    int numero_bilhete;

    while (1) {
        printf("\nMenu Principal:\n");
        printf("1. Reservar lugar\n");
        printf("2. Imprimir bilhete\n");
        printf("3. Exibir relatorio\n");
        printf("4. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                listarFilmes(sistema->filmes, MAX_FILMES);
                printf("Escolha o filme (1 a %d): ", MAX_FILMES);
                scanf("%d", &filmeEscolhido);
                filmeEscolhido--; // Ajuste para índice de vetor

                exibirAssentos(sistema);
                printf("Escolha a linha (1 a %d) e a coluna (1 a %d): ", LINHAS, COLUNAS);
                scanf("%d %d", &linha, &coluna);
                linha--; // Ajuste para índice de vetor
                coluna--; // Ajuste para índice de vetor

                printf("Digite o tipo de ingresso (inteiro ou meia): ");
                scanf("%s", tipo_ingresso);
                printf("Digite seu nome: ");
                scanf("%s", nome_cliente);
                numero_bilhete = sistema->contador_bilhetes + 1;

                reservarLugar(sistema, linha, coluna, numero_bilhete, tipo_ingresso, nome_cliente);
                break;
            case 2:
                printf("Digite o número do bilhete: ");
                scanf("%d", &numero_bilhete);
                imprimirBilhete(sistema, numero_bilhete);
                break;
            case 3:
                exibirRelatorio(sistema);
                break;
            case 4:
                printf("Saindo do sistema...\n");
                return;
            default:
                printf("Opção inválida!\n");
        }
    }
}

int main() {
    Sistema sistema;
    inicializarSistema(&sistema);

    // Adicionando alguns filmes fictícios
    strcpy(sistema.filmes[0].nome, "Vingadores");
    strcpy(sistema.filmes[0].data_lancamento, "2024-04-01");
    strcpy(sistema.filmes[1].nome, "O Rei Leão");
    strcpy(sistema.filmes[1].data_lancamento, "2023-08-15");

    menuPrincipal(&sistema);
    return 0;
}
