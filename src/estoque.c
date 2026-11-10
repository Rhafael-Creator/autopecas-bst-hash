#include "estoque.h"
#include <stdio.h>
#include <stdlib.h>

struct NoPeca {
    Peca dado;
    struct NoPeca *prox;
};

/* Pecas com o mesmo resto ficam na mesma lista. */
static size_t hash(int codigo, size_t capacidade) {
    return (unsigned int)codigo % capacidade;
}

int estoque_iniciar(Estoque *estoque, size_t capacidade) {
    estoque->baldes = NULL;
    estoque->capacidade = 0;
    estoque->quantidade = 0;
    if (capacidade == 0) {
        return 0;
    }
    estoque->baldes = calloc(capacidade, sizeof(NoPeca *));
    if (estoque->baldes == NULL) {
        return 0;
    }
    estoque->capacidade = capacidade;
    return 1;
}

const Peca *estoque_buscar(const Estoque *estoque, int codigo, size_t *comparacoes) {
    if (comparacoes != NULL) {
        *comparacoes = 0;
    }
    if (estoque->capacidade == 0 || codigo <= 0) {
        return NULL;
    }
    size_t indice = hash(codigo, estoque->capacidade);
    NoPeca *atual = estoque->baldes[indice];
    while (atual != NULL) {
        if (comparacoes != NULL) {
            (*comparacoes)++;
        }
        if (atual->dado.codigo == codigo) {
            return &atual->dado;
        }
        atual = atual->prox;
    }
    return NULL;
}

int estoque_inserir(Estoque *estoque, Peca peca) {
    if (estoque->capacidade == 0 || peca.codigo <= 0 ||
        peca.nome[0] == '\0' || peca.quantidade < 0 || peca.preco_centavos < 0) {
        return 0;
    }
    if (estoque_buscar(estoque, peca.codigo, NULL) != NULL) {
        return 0;
    }
    NoPeca *novo = malloc(sizeof(NoPeca));
    if (novo == NULL) {
        return -1;
    }
    size_t indice = hash(peca.codigo, estoque->capacidade);
    /* Insere no inicio da lista dessa posicao. */
    novo->dado = peca;
    novo->prox = estoque->baldes[indice];
    estoque->baldes[indice] = novo;
    estoque->quantidade++;
    return 1;
}

int estoque_remover(Estoque *estoque, int codigo) {
    if (estoque->capacidade == 0 || codigo <= 0) {
        return 0;
    }
    size_t indice = hash(codigo, estoque->capacidade);
    NoPeca *atual = estoque->baldes[indice];
    NoPeca *anterior = NULL;
    while (atual != NULL && atual->dado.codigo != codigo) {
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) {
        return 0;
    }
    if (anterior == NULL) {
        estoque->baldes[indice] = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }
    free(atual);
    estoque->quantidade--;
    return 1;
}

int estoque_retirar(Estoque *estoque, int codigo, int quantidade) {
    if (estoque->capacidade == 0 || codigo <= 0 || quantidade <= 0) {
        return 0;
    }
    size_t indice = hash(codigo, estoque->capacidade);
    NoPeca *atual = estoque->baldes[indice];
    while (atual != NULL && atual->dado.codigo != codigo) {
        atual = atual->prox;
    }
    if (atual == NULL || atual->dado.quantidade < quantidade) {
        return 0;
    }
    atual->dado.quantidade -= quantidade;
    return 1;
}

size_t estoque_tamanho_balde(const Estoque *estoque, size_t indice) {
    size_t quantidade = 0;
    if (indice >= estoque->capacidade) {
        return 0;
    }
    NoPeca *atual = estoque->baldes[indice];
    while (atual != NULL) {
        quantidade++;
        atual = atual->prox;
    }
    return quantidade;
}

double estoque_fator_carga(const Estoque *estoque) {
    if (estoque->capacidade == 0) {
        return 0;
    }
    return (double)estoque->quantidade / estoque->capacidade;
}

void estoque_listar(const Estoque *estoque) {
    for (size_t i = 0; i < estoque->capacidade; i++) {
        NoPeca *atual = estoque->baldes[i];
        while (atual != NULL) {
            printf("[%zu] %d | %s | qtd %d | R$ %ld,%02ld\n", i,
                   atual->dado.codigo, atual->dado.nome, atual->dado.quantidade,
                   atual->dado.preco_centavos / 100, atual->dado.preco_centavos % 100);
            atual = atual->prox;
        }
    }
}

void estoque_destruir(Estoque *estoque) {
    for (size_t i = 0; i < estoque->capacidade; i++) {
        NoPeca *atual = estoque->baldes[i];
        while (atual != NULL) {
            NoPeca *proximo = atual->prox;
            free(atual);
            atual = proximo;
        }
    }
    free(estoque->baldes);
    estoque->baldes = NULL;
    estoque->capacidade = 0;
    estoque->quantidade = 0;
}
