#include "estoque.h"
#include <stdio.h>
#include <stdlib.h>
struct NoPeca { Peca dado; struct NoPeca *prox; };
static size_t hash(int codigo, size_t capacidade) { return (unsigned int)codigo % capacidade; }
int estoque_iniciar(Estoque *t, size_t capacidade) {
    t->baldes = NULL; t->capacidade = t->quantidade = 0;
    if (!capacidade) return 0;
    t->baldes = calloc(capacidade, sizeof *t->baldes);
    if (!t->baldes) return 0;
    t->capacidade = capacidade; return 1;
}
const Peca *estoque_buscar(const Estoque *t, int codigo, size_t *comparacoes) {
    if (comparacoes) *comparacoes = 0;
    if (!t->capacidade || codigo <= 0) return NULL;
    for (NoPeca *p = t->baldes[hash(codigo, t->capacidade)]; p; p = p->prox) {
        if (comparacoes) (*comparacoes)++;
        if (p->dado.codigo == codigo) return &p->dado;
    }
    return NULL;
}
int estoque_inserir(Estoque *t, Peca p) {
    if (!t->capacidade || p.codigo <= 0 || !p.nome[0] || p.quantidade < 0 || p.preco_centavos < 0) return 0;
    if (estoque_buscar(t, p.codigo, NULL)) return 0;
    NoPeca *novo = malloc(sizeof *novo);
    if (!novo) return -1;
    size_t i = hash(p.codigo, t->capacidade);
    novo->dado = p; novo->prox = t->baldes[i]; t->baldes[i] = novo;
    t->quantidade++; return 1;
}
int estoque_remover(Estoque *t, int codigo) {
    if (!t->capacidade || codigo <= 0) return 0;
    NoPeca **p = &t->baldes[hash(codigo, t->capacidade)];
    while (*p && (*p)->dado.codigo != codigo) p = &(*p)->prox;
    if (!*p) return 0;
    NoPeca *alvo = *p; *p = alvo->prox; free(alvo); t->quantidade--; return 1;
}
int estoque_retirar(Estoque *t, int codigo, int quantidade) {
    if (!t->capacidade || codigo <= 0 || quantidade <= 0) return 0;
    NoPeca *p = t->baldes[hash(codigo, t->capacidade)];
    while (p && p->dado.codigo != codigo) p = p->prox;
    if (!p || p->dado.quantidade < quantidade) return 0;
    p->dado.quantidade -= quantidade; return 1;
}
size_t estoque_tamanho_balde(const Estoque *t, size_t indice) {
    size_t n = 0;
    if (indice >= t->capacidade) return 0;
    for (NoPeca *p = t->baldes[indice]; p; p = p->prox) n++;
    return n;
}
double estoque_fator_carga(const Estoque *t) { return t->capacidade ? (double)t->quantidade / t->capacidade : 0; }
void estoque_listar(const Estoque *t) {
    for (size_t i = 0; i < t->capacidade; i++)
        for (NoPeca *p = t->baldes[i]; p; p = p->prox)
            printf("[%zu] %d | %s | qtd %d | R$ %ld,%02ld\n", i, p->dado.codigo,
                   p->dado.nome, p->dado.quantidade, p->dado.preco_centavos / 100, p->dado.preco_centavos % 100);
}
void estoque_destruir(Estoque *t) {
    for (size_t i = 0; i < t->capacidade; i++) {
        NoPeca *p = t->baldes[i];
        while (p) { NoPeca *prox = p->prox; free(p); p = prox; }
    }
    free(t->baldes); t->baldes = NULL; t->capacidade = t->quantidade = 0;
}
