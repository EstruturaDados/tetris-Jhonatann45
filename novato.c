#include <stdio.h>
#include <stdlib.h> // Para rand()
#include <time.h>   // Para srand()

// ===============================================
// Definições e Estruturas
// ===============================================

#define CAPACIDADE_MAXIMA 5 // Tamanho da fila

// Estrutura da Peça
typedef struct {
    char nome; 
    int id;   
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca pecas[CAPACIDADE_MAXIMA]; 
    int frente;                   
    int traseira;                 
    int tamanho;                  
    int proximo_id;               
} FilaCircular;

// ===============================================
// Protótipos das Funções
// ===============================================

void inicializarFila(FilaCircular *fila);
int filaCheia(FilaCircular *fila);
int filaVazia(FilaCircular *fila);
Peca gerarPeca(int *proximo_id);
void inserirPeca(FilaCircular *fila);
void jogarPeca(FilaCircular *fila);
void exibirFila(FilaCircular *fila);

// ===============================================
// Implementação das Funções
// ===============================================

void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->traseira = 0;
    fila->tamanho = 0;
    fila->proximo_id = 0;
}

int filaCheia(FilaCircular *fila) {
    return (fila->tamanho == CAPACIDADE_MAXIMA);
}

int filaVazia(FilaCircular *fila) {
    return (fila->tamanho == 0);
}

Peca gerarPeca(int *proximo_id) {
    Peca novaPeca;
    // Tipos de peças: I, O, T, L (simples, menos tipos para simplificar)
    char tipos[] = {'I', 'O', 'T', 'L'}; 
    int num_tipos = sizeof(tipos) / sizeof(tipos[0]);

    novaPeca.nome = tipos[rand() % num_tipos];
    novaPeca.id = (*proximo_id)++;

    return novaPeca;
}

void inserirPeca(FilaCircular *fila) {
    if (filaCheia(fila)) {
        printf("[ERRO] Fila cheia. Não é possível adicionar peça.\n");
        return;
    }

    Peca novaPeca = gerarPeca(&(fila->proximo_id));

    fila->pecas[fila->traseira] = novaPeca;
    fila->traseira = (fila->traseira + 1) % CAPACIDADE_MAXIMA;
    fila->tamanho++;

    printf("[ENQUEUE] Peça [%c %d] adicionada.\n", novaPeca.nome, novaPeca.id);
}

void jogarPeca(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("[ERRO] Fila vazia. Nenhuma peça para jogar.\n");
        return;
    }

    Peca pecaRemovida = fila->pecas[fila->frente];

    fila->frente = (fila->frente + 1) % CAPACIDADE_MAXIMA;
    fila->tamanho--;

    printf("[DEQUEUE] Peça [%c %d] jogada (removida).\n", pecaRemovida.nome, pecaRemovida.id);
}

void exibirFila(FilaCircular *fila) {
    printf("\n--- Fila de Peças (Tamanho: %d/%d) ---\n", fila->tamanho, CAPACIDADE_MAXIMA);

    if (filaVazia(fila)) {
        printf("Fila VAZIA.\n");
        return;
    }

    printf("Fila: ");

    for (int i = 0; i < fila->tamanho; i++) {
        int indice = (fila->frente + i) % CAPACIDADE_MAXIMA;
        printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
    }
    printf("\n");
}

// ===============================================
// Função Principal (main)
// ===============================================

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    FilaCircular fila;
    inicializarFila(&fila);
    int opcao;

    printf("--- Simulador Tetris Stack: Fila de Peças ---\n");
    
    // Inicialização da fila com 3 peças para demonstração
    printf("Inicializando com 3 peças...\n");
    for(int i = 0; i < 3; i++) {
        inserirPeca(&fila);
    }

    do {
        exibirFila(&fila);
        
        printf("\nOpções:\n");
        printf("1 - Jogar peça (Dequeue)\n");
        printf("2 - Inserir nova peça (Enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("[AVISO] Entrada inválida. Tente novamente.\n");
            while(getchar() != '\n'); // Limpa buffer
            opcao = -1;
            continue;
        }

        switch (opcao) {
            case 1:
                jogarPeca(&fila); 
                break;
            case 2:
                inserirPeca(&fila); 
                break;
            case 0:
                printf("\nSaindo do simulador.\n");
                break;
            default:
                printf("[AVISO] Opção inválida. Tente novamente.\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}