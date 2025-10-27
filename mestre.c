#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ===============================================
// Definições de Constantes e Estruturas
// ===============================================

#define MAX_FILA 5   // Capacidade máxima da Fila Circular de peças futuras
#define MAX_PILHA 3  // Capacidade máxima da Pilha de Reserva

// Estrutura para representar uma peça
typedef struct {
    char nome; 
    int id;   
} Peca;

// Estrutura para a Fila Circular (FIFO)
typedef struct {
    Peca p[MAX_FILA]; 
    int f, t, tam; // frente, traseira, tamanho
} FilaCircular;

// Estrutura para a Pilha (LIFO)
typedef struct {
    Peca p[MAX_PILHA]; 
    int topo; // Índice do elemento no topo (-1 = vazia)
} PilhaReserva;

// Variável global para gerar IDs sequenciais únicos para todas as peças
int G_ID = 0;

// ===============================================
// Protótipos das Funções
// ===============================================

// Funções de Suporte
void inicializar(FilaCircular *f, PilhaReserva *p);
Peca gerarPeca();
void exibir(FilaCircular *f, PilhaReserva *p);
void reporFila(FilaCircular *f);
void preencherFilaInicial(FilaCircular *f);

// Funções de Ação
void acao_jogar(FilaCircular *f);
void acao_reservar(FilaCircular *f, PilhaReserva *p);
void acao_usar(PilhaReserva *p, FilaCircular *f);
void acao_trocar_simples(FilaCircular *f, PilhaReserva *p);
void acao_trocar_multipla(FilaCircular *f, PilhaReserva *p);
void exibirMenu();

// ===============================================
// Implementação das Funções de Suporte
// ===============================================

/**
 * @brief Inicializa a Fila e a Pilha.
 */
void inicializar(FilaCircular *f, PilhaReserva *p) {
    f->f = f->t = f->tam = 0;
    p->topo = -1;
}

/**
 * @brief Gera uma nova peça com tipo aleatório e ID sequencial.
 */
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = G_ID++;
    return nova;
}

/**
 * @brief Adiciona uma nova peça ao final da fila (Enqueue) se houver espaço.
 * Usada para manter a Fila sempre cheia após uma remoção/transferência.
 */
void reporFila(FilaCircular *f) {
    if (f->tam == MAX_FILA) return;

    Peca nova = gerarPeca();
    f->p[f->t] = nova;
    f->t = (f->t + 1) % MAX_FILA;
    f->tam++;
    printf("[REPOSIÇÃO] Peça [%c %d] adicionada à Fila.\n", nova.nome, nova.id);
}

/**
 * @brief Preenche a fila com peças geradas até a capacidade máxima na inicialização.
 */
void preencherFilaInicial(FilaCircular *f) {
    printf("Inicializando a fila com %d peças...\n", MAX_FILA);
    for(int i = 0; i < MAX_FILA; i++) {
        reporFila(f);
    }
}

/**
 * @brief Exibe o estado atual da Fila e da Pilha.
 */
void exibir(FilaCircular *f, PilhaReserva *p) {
    printf("\n======================================================\n");
    printf("ESTADO ATUAL DO JOGO\n");
    printf("======================================================\n");
    
    // Exibe Fila (Frente -> Traseira)
    printf("Fila (%d/%d): ", f->tam, MAX_FILA);
    if (f->tam == 0) {
        printf("[VAZIA]");
    } else {
        for (int i = 0; i < f->tam; i++) {
            int idx = (f->f + i) % MAX_FILA;
            printf("[%c %d] ", f->p[idx].nome, f->p[idx].id);
        }
    }
    printf("\n");
    
    // Exibe Pilha (Topo -> Base)
    printf("Pilha de reserva (%d/%d) (Topo->Base): ",