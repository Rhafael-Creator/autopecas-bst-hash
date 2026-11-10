#ifndef ESTOQUE_H
#define ESTOQUE_H
#include <stddef.h>

typedef struct {
    int codigo;
    char nome[80];
    int quantidade;
    long preco_centavos;
} Peca;
typedef struct NoPeca NoPeca;
typedef struct {
    NoPeca **baldes;
    size_t capacidade;
    size_t quantidade;
} Estoque;
int estoque_iniciar(Estoque *t, size_t capacidade);
int estoque_inserir(Estoque *t, Peca p);
const Peca *estoque_buscar(const Estoque *t, int codigo, size_t *comparacoes);
int estoque_remover(Estoque *t, int codigo);
/* Retira somente se houver quantidade suficiente. */
int estoque_retirar(Estoque *t, int codigo, int quantidade);
size_t estoque_tamanho_balde(const Estoque *t, size_t indice);
double estoque_fator_carga(const Estoque *t);
void estoque_listar(const Estoque *t);
void estoque_destruir(Estoque *t);
#endif
