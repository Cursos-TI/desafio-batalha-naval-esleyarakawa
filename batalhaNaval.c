#include <stdio.h>

#define TAMANHO 10   // Tamanho fixo do tabuleiro (10x10)
#define NAVIO 3      // Valor que representa um navio
#define TAM_NAVIO 3  // Tamanho fixo dos navios

int main() {
    int tabuleiro[TAMANHO][TAMANHO];

    // 1. Inicializa o tabuleiro com água (0)
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            tabuleiro[i][j] = 0;
        }
    }

    // 2. Define as coordenadas iniciais dos navios (linha e coluna)
    // Navio horizontal começará na linha 2, coluna 1
    int linhaH = 2, colunaH = 1;

    // Navio vertical começará na linha 5, coluna 4
    int linhaV = 5, colunaV = 4;

    // 3. Valida se o navio horizontal cabe no tabuleiro
    if (colunaH + TAM_NAVIO <= TAMANHO) {
        // Preenche as posições do navio horizontal
        for (int j = 0; j < TAM_NAVIO; j++) {
            tabuleiro[linhaH][colunaH + j] = NAVIO;
        }
    } else {
        printf("Erro: navio horizontal fora dos limites do tabuleiro!\n");
        return 1;
    }

    // 4. Valida se o navio vertical cabe no tabuleiro
    if (linhaV + TAM_NAVIO <= TAMANHO) {
        // Verifica se há sobreposição antes de posicionar
        int sobreposicao = 0;
        for (int i = 0; i < TAM_NAVIO; i++) {
            if (tabuleiro[linhaV + i][colunaV] == NAVIO) {
                sobreposicao = 1;
                break;
            }
        }

        if (!sobreposicao) {
            // Posiciona o navio vertical
            for (int i = 0; i < TAM_NAVIO; i++) {
                tabuleiro[linhaV + i][colunaV] = NAVIO;
            }
        } else {
            printf("Erro: os navios se sobrepõem!\n");
            return 1;
        }
    } else {
        printf("Erro: navio vertical fora dos limites do tabuleiro!\n");
        return 1;
    }

    // 5. Exibe o tabuleiro
    printf("\n=== TABULEIRO BATALHA NAVAL ===\n\n");
    for (int i = 0; i < TAMANHO; i++) {
        for (int j = 0; j < TAMANHO; j++) {
            printf("%d ", tabuleiro[i][j]);
        }
        printf("\n");
    }

    return 0;
}