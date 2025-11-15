#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

// Estrutura de uma sala da mansão (árvore binária)
typedef struct Sala {
    char nome[30];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura da árvore BST de pistas
typedef struct PistaNode {
    char pista[50];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Estrutura da tabela hash
typedef struct HashNode {
    char pista[50];
    char suspeito[30];
    struct HashNode *prox;
} HashNode;

HashNode *tabelaHash[TAM_HASH]; // tabela global

// Função auxiliar para gerar pista associada à sala
const char* gerarPistaPorSala(const char* nomeSala) {
    if (strcmp(nomeSala, "Biblioteca") == 0) return "Luvas ensanguentadas";
    if (strcmp(nomeSala, "Cozinha") == 0) return "Faca desaparecida";
    if (strcmp(nomeSala, "Sala de Jantar") == 0) return "Taça quebrada";
    if (strcmp(nomeSala, "Escritório") == 0) return "Bilhete ameaçador";
    if (strcmp(nomeSala, "Porão") == 0) return "Pegadas de lama";
    return NULL;
}

// Função criarSala: cria dinamicamente um cômodo
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = nova->direita = NULL;
    return nova;
}

// Função inserirPista: insere pista na BST
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        PistaNode* nova = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(nova->pista, pista);
        nova->esquerda = nova->direita = NULL;
        return nova;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

// Função inserirNaHash: insere pista e suspeito na tabela hash
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = abs((int)pista[0]) % TAM_HASH;
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

// Função encontrarSuspeito: retorna suspeito associado à pista
const char* encontrarSuspeito(const char* pista) {
    int indice = abs((int)pista[0]) % TAM_HASH;
    HashNode* atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0)
            return atual->suspeito;
        atual = atual->prox;
    }
    return NULL;
}

// Função explorarSalas: navega pela árvore e coleta pistas
void explorarSalas(Sala* atual, PistaNode** arvorePistas) {
    char escolha;
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);
        const char* pista = gerarPistaPorSala(atual->nome);
        if (pista != NULL) {
            printf("Pista encontrada: %s\n", pista);
            *arvorePistas = inserirPista(*arvorePistas, pista);
        } else {
            printf("Nenhuma pista nesta sala.\n");
        }

        printf("Escolha o caminho: (e) esquerda, (d) direita, (s) sair: ");
        scanf(" %c", &escolha);

        if (escolha == 'e')
            atual = atual->esquerda;
        else if (escolha == 'd')
            atual = atual->direita;
        else if (escolha == 's')
            break;
        else
            printf("Opção inválida.\n");
    }
}

// Função para exibir pistas coletadas (em ordem)
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// Função verificarSuspeitoFinal: avalia acusação do jogador
void verificarSuspeitoFinal(PistaNode* raiz, const char* suspeito, int* contador) {
    if (raiz != NULL) {
        verificarSuspeitoFinal(raiz->esquerda, suspeito, contador);
        const char* s = encontrarSuspeito(raiz->pista);
        if (s != NULL && strcmp(s, suspeito) == 0)
            (*contador)++;
        verificarSuspeitoFinal(raiz->direita, suspeito, contador);
    }
}