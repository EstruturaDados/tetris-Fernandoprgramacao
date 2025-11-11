#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TAM_FILA 5
#define TAM_PILHA 3
#define TAM_HIST 10

// ==================== ESTRUTURAS ====================
typedef struct {
    int id;
    char nome;
} Peca;

typedef struct {
    Peca pecas[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaCircular;

typedef struct {
    Peca pecas[TAM_PILHA];
    int topo;
} Pilha;

typedef struct {
    char acao[30];
    Peca peca;
} Historico;

// ==================== PROTÓTIPOS ====================
void inicializarFila(FilaCircular *fila);
void inicializarPilha(Pilha *pilha);
int filaCheia(FilaCircular *fila);
int filaVazia(FilaCircular *fila);
int pilhaCheia(Pilha *pilha);
int pilhaVazia(Pilha *pilha);
void enfileirar(FilaCircular *fila, Peca novaPeca);
Peca desenfileirar(FilaCircular *fila);
void push(Pilha *pilha, Peca novaPeca);
Peca pop(Pilha *pilha);
Peca gerarPeca();
void exibirFila(FilaCircular *fila);
void exibirPilha(Pilha *pilha);
void trocarTopoComFrente(Pilha *pilha, FilaCircular *fila);
void inverterFilaEPilha(FilaCircular *fila, Pilha *pilha);
void desfazerUltimaJogada(FilaCircular *fila, Pilha *pilha, Historico *historico, int *indiceHist);

// ==================== FUNÇÃO PRINCIPAL ====================
int main() {
    FilaCircular fila;
    Pilha pilha;
    Historico historico[TAM_HIST];
    int indiceHist = 0;
    int opcao;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

do {
    printf("\n=========== TETRIS STACK - NIVEL MESTRE ===========\n");
    printf("1 - Jogar peca\n");
    printf("2 - Reservar peca\n");
    printf("3 - Usar peca reservada\n");
    printf("4 - Trocar topo da pilha com frente da fila\n");
    printf("5 - Desfazer ultima jogada\n");
    printf("6 - Inverter fila com pilha\n");
    printf("0 - Sair\n");
    printf("===========================================================\n");
    printf("Escolha: ");

    if (scanf("%d", &opcao) != 1) {
        printf("\nEntrada invalida! Digite apenas numeros.\n");
        while (getchar() != '\n'); // limpa o buffer
        opcao = -1;
        continue;
    }

        switch (opcao) {
            case 1: // Jogar peça
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n>> Peca '%c' (ID %d) jogada!\n", jogada.nome, jogada.id);

                    // Salva histórico
                    strcpy(historico[indiceHist % TAM_HIST].acao, "Jogar");
                    historico[indiceHist % TAM_HIST].peca = jogada;
                    indiceHist++;

                    // Substitui com nova peça
                    Peca nova = gerarPeca();
                    enfileirar(&fila, nova);
                    printf(">> Nova peca '%c' (ID %d) adicionada ao final!\n", nova.nome, nova.id);
                } else {
                    printf("\n Fila vazia!\n");
                }
                break;

            case 2: // Reservar peça
                if (pilhaCheia(&pilha)) {
                    printf("\n Pilha cheia! Nao é possivel reservar mais pecas.\n");
                } else if (!filaVazia(&fila)) {
                    Peca reservada = desenfileirar(&fila);
                    push(&pilha, reservada);
                    printf("\n>> Peça '%c' (ID %d) reservada!\n", reservada.nome, reservada.id);

                    // Salva histórico
                    strcpy(historico[indiceHist % TAM_HIST].acao, "Reservar");
                    historico[indiceHist % TAM_HIST].peca = reservada;
                    indiceHist++;

                    // Adiciona nova peça na fila
                    enfileirar(&fila, gerarPeca());
                }
                break;

            case 3: // Usar peça reservada
                if (pilhaVazia(&pilha)) {
                    printf("\n Nenhuma peca reservada!\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\n>> Peca reservada '%c' (ID %d) usada!\n", usada.nome, usada.id);

                    // Salva histórico
                    strcpy(historico[indiceHist % TAM_HIST].acao, "Usar Reservada");
                    historico[indiceHist % TAM_HIST].peca = usada;
                    indiceHist++;
                }
                break;

            case 4: // Trocar topo com frente
                trocarTopoComFrente(&pilha, &fila);
                break;

            case 5: // Desfazer
                desfazerUltimaJogada(&fila, &pilha, historico, &indiceHist);
                break;

            case 6: // Inverter fila com pilha
                inverterFilaEPilha(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

        printf("\n===== ESTADO ATUAL =====\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

    } while (opcao != 0);

    return 0;
}

// ==================== FUNÇÕES DE FILA ====================
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

int filaCheia(FilaCircular *fila) {
    return fila->tamanho == TAM_FILA;
}

int filaVazia(FilaCircular *fila) {
    return fila->tamanho == 0;
}

void enfileirar(FilaCircular *fila, Peca novaPeca) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->pecas[fila->tras] = novaPeca;
    fila->tamanho++;
}

Peca desenfileirar(FilaCircular *fila) {
    Peca removida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    return removida;
}

void exibirFila(FilaCircular *fila) {
    printf("\n Fila:\n");
    if (filaVazia(fila)) {
        printf("  [vazia]\n");
        return;
    }
    for (int i = 0; i < fila->tamanho; i++) {
        int index = (fila->frente + i) % TAM_FILA;
        printf("  [%d] '%c' (ID %d)\n", i + 1, fila->pecas[index].nome, fila->pecas[index].id);
    }
}

// ==================== FUNÇÕES DE PILHA ====================
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

void push(Pilha *pilha, Peca novaPeca) {
    if (pilhaCheia(pilha)) return;
    pilha->pecas[++pilha->topo] = novaPeca;
}

Peca pop(Pilha *pilha) {
    return pilha->pecas[pilha->topo--];
}

void exibirPilha(Pilha *pilha) {
    printf("\n Pilha de Reservas:\n");
    if (pilhaVazia(pilha)) {
        printf("  [vazia]\n");
        return;
    }
    for (int i = pilha->topo; i >= 0; i--) {
        printf("  [%d] '%c' (ID %d)\n", i + 1, pilha->pecas[i].nome, pilha->pecas[i].id);
    }
}

// ==================== OUTRAS FUNÇÕES ====================
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    nova.id = rand() % 1000 + 1;
    nova.nome = tipos[rand() % 7];
    return nova;
}

void trocarTopoComFrente(Pilha *pilha, FilaCircular *fila) {
    if (pilhaVazia(pilha) || filaVazia(fila)) {
        printf("\n Nao ha pecas suficientes para trocar!\n");
        return;
    }

    Peca temp = pilha->pecas[pilha->topo];
    pilha->pecas[pilha->topo] = fila->pecas[fila->frente];
    fila->pecas[fila->frente] = temp;

    printf("\n Peca do topo da pilha trocada com a da frente da fila!\n");
}

void inverterFilaEPilha(FilaCircular *fila, Pilha *pilha) {
    if (filaVazia(fila) && pilhaVazia(pilha)) {
        printf("\n Nada para inverter!\n");
        return;
    }

    // Salvar peças temporariamente
    Peca tempFila[TAM_FILA];
    int countFila = fila->tamanho;
    for (int i = 0; i < countFila; i++) {
        tempFila[i] = desenfileirar(fila);
    }

    Peca tempPilha[TAM_PILHA];
    int countPilha = pilha->topo + 1;
    for (int i = 0; i < countPilha; i++) {
        tempPilha[i] = pop(pilha);
    }

    // Inverte e recoloca
    for (int i = 0; i < countPilha; i++) {
        enfileirar(fila, tempPilha[i]);
    }

    for (int i = 0; i < countFila; i++) {
        push(pilha, tempFila[i]);
    }

    printf("\n Estruturas invertidas com sucesso!\n");
}

void desfazerUltimaJogada(FilaCircular *fila, Pilha *pilha, Historico *historico, int *indiceHist) {
    if (*indiceHist == 0) {
        printf("\n Nenhuma jogada para desfazer!\n");
        return;
    }

    (*indiceHist)--;
    Historico ultima = historico[*indiceHist % TAM_HIST];

    if (strcmp(ultima.acao, "Jogar") == 0) {
        enfileirar(fila, ultima.peca);
        printf("\n Desfeito: peca '%c' (ID %d) voltou à fila!\n", ultima.peca.nome, ultima.peca.id);
    } else if (strcmp(ultima.acao, "Reservar") == 0) {
        if (!pilhaVazia(pilha)) pop(pilha);
        enfileirar(fila, ultima.peca);
        printf("\n Desfeito: reserva cancelada!\n");
    } else if (strcmp(ultima.acao, "Usar Reservada") == 0) {
        push(pilha, ultima.peca);
        printf("\n↩ Desfeito: peca '%c' voltou à pilha!\n", ultima.peca.nome);
    } else {
        printf("\n Tipo de acao desconhecida.\n");
    }
}
