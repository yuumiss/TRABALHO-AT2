#include <stdio.h>
#include <string.h>
#include "expressao.h"

int main() {
    char expressao[256];
    int opcao;
    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. Infixa → Pos-fixada\n");
        printf("2. Pos-fixada → Infixa\n");
        printf("3. Avaliar expressao pos-fixada\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf(" %d", &opcao);
        getchar();
        switch (opcao) {
            case 1:
                printf("Digite a expressao infixada: ");
                fgets(expressao, 256, stdin);
                expressao[strcspn(expressao, "\n")] = 0;
                printf("Pos-fixada: %s\n", infixaParaPosfixa(expressao));
                break;
            case 2:
                printf("Digite a expressao pos-fixada: ");
                fgets(expressao, 256, stdin);
                expressao[strcspn(expressao, "\n")] = 0;
                printf("Infixada: %s\n", posfixaParaInfixa(expressao));
                break;
            case 3:
                printf("Digite a expressao pos-fixada: ");
                fgets(expressao, 256, stdin);
                expressao[strcspn(expressao, "\n")] = 0;
                printf("Resultado: %.4f\n", avaliarPosfixa(expressao));
                break;
            case 0:
                return 0;
            default:
                printf("Opcao invalida!\n");
        }
    }
    return 0;
}
