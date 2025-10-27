#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ===============================================
// Definições de Constantes e Estruturas
// ===============================================

#define MAX_F 5   // Capacidade máxima da Fila
#define MAX_P 3   // Capacidade máxima da Pilha

// Estrutura da Peça
typedef struct {
    char nome; 
    int id;   
} Peca;

// Estrutura da Fila Circular (FIFO)
typedef struct {
    Peca p[MAX_F]; 
    int f, t, tam; // frente, traseira, tamanho
} FilaCircular;

// Estrutura da Pilha (LIFO)
typedef struct {
    Peca p[MAX_P]; 
    int topo; 
} PilhaReserva;

// Variável global para IDs sequenciais únicos
int G_ID = 0;

// ===============================================
// Funções de Suporte
// ===============================================

void inicializar(FilaCircular *f, PilhaReserva *p) {
    f->f = f->t = f->tam = 0;
    p->topo = -1;
}

Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = G_ID++;
    return nova;
}

void exibir(FilaCircular *f, PilhaReserva *p) {
    printf("\n--- ESTADO ---\n");
    
    // Exibe Fila
    printf("Fila (%d/%d): ", f->tam, MAX_F);
    if (f->tam == 0) {
        printf("[VAZIA]\n");
    } else {
        for (int i = 0; i < f->tam; i++) {
            int idx = (f->f + i) % MAX_F;
            printf("[%c %d] ", f->p[idx].nome, f->p[idx].id);
        }
        printf("\n");
    }
    
    // Exibe Pilha
    printf("Pilha (%d/%d) (Topo->Base): ", p->topo + 1, MAX_P);
    if (p->topo == -1) {
        printf("[VAZIA]\n");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->p[i].nome, p->p[i].id);
        }
        printf("\n");
    }
    printf("--------------\n");
}

void reporFila(FilaCircular *f) {
    if (f->tam == MAX_F) return; // Ação de reposição é só se houver espaço (teoricamente, sempre há)

    Peca nova = gerarPeca();
    f->p[f->t] = nova;
    f->t = (f->t + 1) % MAX_F;
    f->tam++;
    printf("[REPOSIÇÃO] Peça [%c %d] adicionada à Fila.\n", nova.nome, nova.id);
}

// ===============================================
// Funções de Ação (Ações do Jogador)
// ===============================================

// 1. Jogar peça (Dequeue da Fila + Reposição)
void acao_jogar(FilaCircular *f) {
    if (f->tam == 0) {
        printf("[ERRO] Fila vazia.\n");
        return;
    }
    
    Peca jogada = f->p[f->f];
    f->f = (f->f + 1) % MAX_F;
    f->tam--;
    printf("[AÇÃO 1] Peça [%c %d] JOGADA (removida da Fila).\n", jogada.nome, jogada.id);

    reporFila(f);
}

// 2. Reservar peça (Fila -> Pilha + Reposição)
void acao_reservar(FilaCircular *f, PilhaReserva *p) {
    if (p->topo == MAX_P - 1) {
        printf("[ERRO] Pilha cheia. Não é possível reservar.\n");
        return;
    }
    if (f->tam == 0) {
        printf("[ERRO] Fila vazia.\n");
        return;
    }
    
    // 1. Move da Fila (Dequeue)
    Peca reservada = f->p[f->f];
    f->f = (f->f + 1) % MAX_F;
    f->tam--;
    
    // 2. Insere na Pilha (Push)
    p->topo++;
    p->p[p->topo] = reservada;
    
    printf("[AÇÃO 2] Peça [%c %d] RESERVADA (Fila -> Pilha).\n", reservada.nome, reservada.id);

    reporFila(f);
}

// 3. Usar peça reservada (Pop da Pilha + Reposição da Fila)
void acao_usar(PilhaReserva *p, FilaCircular *f) {
    if (p->topo == -1) {
        printf("[ERRO] Pilha vazia. Nenhuma peça para usar.\n");
        return;
    }
    
    Peca usada = p->p[p->topo];
    p->topo--; // Pop
    printf("[AÇÃO 3] Peça [%c %d] USADA (removida da Pilha).\n", usada.nome, usada.id);

    reporFila(f);
}

// ===============================================
// Função Principal (main)
// ===============================================

int main() {
    srand(time(NULL));
    FilaCircular fila;
    PilhaReserva pilha;
    inicializar(&fila, &pilha);
    int opcao;

    printf("Simulador Tetris Stack (Fila Circular e Pilha)\n");

    // Preenche a fila inicialmente
    for(int i = 0; i < MAX_F; i++) {
        reporFila(&fila);
    }
    
    do {
        exibir(&fila, &pilha);
        
        printf("\nOpções: 1-Jogar | 2-Reservar | 3-Usar Reserva | 0-Sair\n");
        printf("Escolha: ");
        
        if (scanf("%d", &opcao) != 1) {
            printf("[ERRO] Entrada inválida.\n");
            while(getchar() != '\n'); 
            opcao = -1; 
            continue;
        }

        switch (opcao) {
            case 1: acao_jogar(&fila); break;
            case 2: acao_reservar(&fila, &pilha); break;
            case 3: acao_usar(&pilha, &fila); break;
            case 0: printf("Saindo do simulador.\n"); break;
            default: printf("[AVISO] Opção inválida.\n");