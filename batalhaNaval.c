#include <stdio.h>

#define BOARD_SIZE 10

/* Valores no tabuleiro:
   0 -> água
   3 -> navio
   5 -> área afetada pela habilidade
*/

/* Gera dinamicamente a matriz em forma de cone (ponta no topo, expandindo para baixo).
   size deve ser ímpar (ex: 5, 7). O "topo" do cone corresponde à linha 0 da matriz e
   o cone abre conforme as linhas aumentam.
*/
void build_cone(int size, int cone[size][size]) {
    int center = size / 2;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            /* condição: na linha i, marcamos 1 se a distância horizontal ao centro
               for <= i (isso cria uma forma triangular que se abre para baixo) */
            if (abs(j - center) <= i) cone[i][j] = 1;
            else cone[i][j] = 0;
        }
    }
}

/* Gera dinamicamente a matriz em forma de cruz (centro marcado). */
void build_cross(int size, int cross[size][size]) {
    int center = size / 2;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            /* condição: linha central ou coluna central */
            if (i == center || j == center) cross[i][j] = 1;
            else cross[i][j] = 0;
        }
    }
}

/* Gera dinamicamente a matriz em forma de octaedro (vista frontal) -> losango/diamante.
   Condição: abs(i-center)+abs(j-center) <= center cria um losango.
*/
void build_octahedron(int size, int oct[size][size]) {
    int center = size / 2;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (abs(i - center) + abs(j - center) <= center) oct[i][j] = 1;
            else oct[i][j] = 0;
        }
    }
}

/* Sobrepõe a matriz de habilidade no tabuleiro.
   - board: tabuleiro 10x10
   - skill: matriz da habilidade (size x size)
   - size: tamanho da matriz da habilidade (assume ímpar)
   - origin_row, origin_col: ponto de origem no tabuleiro (LINHA, COLUNA)
   - align_mode: 0 = alinhar a matriz com o topo no origin (usado para cone),
                 1 = centralizar a matriz no origin (usado para cruz e octaedro)
   Quando skill[i][j] == 1, escreve 5 no tabuleiro (se dentro dos limites).
*/
void overlay_skill(int board[BOARD_SIZE][BOARD_SIZE], int size, int skill[size][size],
                   int origin_row, int origin_col, int align_mode) {
    int center = size / 2;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (skill[i][j] != 1) continue; /* só processa células afetadas */

            /* Calcula a posição no tabuleiro conforme o modo de alinhamento */
            int board_r, board_c;
            if (align_mode == 0) {
                /* topo do skill corresponde à origin_row, centro horizontal ao origin_col */
                board_r = origin_row + i;                 /* topo da matriz em origin_row */
                board_c = origin_col + (j - center);     /* ajustar horizontal pelo centro */
            } else {
                /* centraliza a matriz em relação à origem */
                board_r = origin_row + (i - center);
                board_c = origin_col + (j - center);
            }

            /* valida limites do tabuleiro */
            if (board_r >= 0 && board_r < BOARD_SIZE && board_c >= 0 && board_c < BOARD_SIZE) {
                /* escreve 5 mesmo que haja um navio (valor 3) — conforme especificação */
                board[board_r][board_c] = 5;
            }
        }
    }
}

/* Exibe o tabuleiro no console: imprime números separados por espaço:
   0 = água, 3 = navio, 5 = área afetada */
void print_board(int board[BOARD_SIZE][BOARD_SIZE]) {
    printf("Tabuleiro (0=agua, 3=navio, 5=area afetada):\n\n");
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    /* Inicializa tabuleiro com água (0) */
    int board[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; ++i)
        for (int j = 0; j < BOARD_SIZE; ++j)
            board[i][j] = 0;

    /* --- Exemplo: posiciona alguns navios manualmente (valor 3) ---
       (essas posições são fixas conforme simplificações do enunciado) */
    board[1][2] = 3;
    board[2][2] = 3;
    board[2][3] = 3;
    board[7][7] = 3;
    board[7][8] = 3;
    board[8][7] = 3;

    /* Tamanho das matrizes de habilidade (usa tamanho ímpar) */
    const int SKILL_SIZE = 7;
    int cone[SKILL_SIZE][SKILL_SIZE];
    int cross[SKILL_SIZE][SKILL_SIZE];
    int octa[SKILL_SIZE][SKILL_SIZE];

    /* Constrói as matrizes dinamicamente usando loops e condicionais */
    build_cone(SKILL_SIZE, cone);
    build_cross(SKILL_SIZE, cross);
    build_octahedron(SKILL_SIZE, octa);

    /* Define pontos de origem no tabuleiro (linha, coluna).
       Observação:
       - Para o CONE, o ponto de origem será tratado como o "topo" do cone.
         Ou seja, a primeira linha da matriz do cone será alinhada em origin.
       - Para CRUZ e OCTAEDRO, o ponto de origem será o centro da matriz.
       Esses pontos são definidos diretamente no código (simplificação pedida). */
    int origin_cone_row = 0;   int origin_cone_col = 4;  /* topo do cone em (0,4) */
    int origin_cross_row = 4;  int origin_cross_col = 2;  /* centro da cruz em (4,2) */
    int origin_octa_row = 6;   int origin_octa_col = 6;   /* centro do octaedro em (6,6) */

    /* Sobrepõe as habilidades no tabuleiro.
       align_mode: 0 -> top-aligned (cone), 1 -> centered (cross, octa) */
    overlay_skill(board, SKILL_SIZE, cone, origin_cone_row, origin_cone_col, 0);
    overlay_skill(board, SKILL_SIZE, cross, origin_cross_row, origin_cross_col, 1);
    overlay_skill(board, SKILL_SIZE, octa, origin_octa_row, origin_octa_col, 1);

    /* Exibe o tabuleiro final */
    print_board(board);

    /* (Opcional) Exibe as matrizes das habilidades geradas (para debug/visualização) */
    printf("Matriz CONE (1=afetado, 0=nao):\n");
    for (int i = 0; i < SKILL_SIZE; ++i) {
        for (int j = 0; j < SKILL_SIZE; ++j) printf("%d ", cone[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("Matriz CRUZ (1=afetado, 0=nao):\n");
    for (int i = 0; i < SKILL_SIZE; ++i) {
        for (int j = 0; j < SKILL_SIZE; ++j) printf("%d ", cross[i][j]);
        printf("\n");
    }
    printf("\n");

    printf("Matriz OCTAEDRO (1=afetado, 0=nao):\n");
    for (int i = 0; i < SKILL_SIZE; ++i) {
        for (int j = 0; j < SKILL_SIZE; ++j) printf("%d ", octa[i][j]);
        printf("\n");
    }
    printf("\n");

    return 0;
}