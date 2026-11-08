#include "clientes.h"
#include <stdio.h>
#include <stdlib.h>
struct NoCliente { Cliente dado; struct NoCliente *esq, *dir; };
void clientes_iniciar(Clientes *a) { a->raiz = NULL; a->quantidade = 0; }
int clientes_inserir(Clientes *a, Cliente c) {
    if (c.id <= 0 || c.nome[0] == '\0') return 0;
    NoCliente **p = &a->raiz;
    while (*p) {
        if (c.id == (*p)->dado.id) return 0;
        p = c.id < (*p)->dado.id ? &(*p)->esq : &(*p)->dir;
    }
    NoCliente *novo = malloc(sizeof *novo);
    if (!novo) return -1;
    novo->dado = c; novo->esq = novo->dir = NULL;
    *p = novo; a->quantidade++; return 1;
}
const Cliente *clientes_buscar(const Clientes *a, int id, size_t *comparacoes) {
    const NoCliente *p = a->raiz;
    if (comparacoes) *comparacoes = 0;
    while (p) {
        if (comparacoes) (*comparacoes)++;
        if (id == p->dado.id) return &p->dado;
        p = id < p->dado.id ? p->esq : p->dir;
    }
    return NULL;
}
int clientes_remover(Clientes *a, int id) {
    NoCliente **p = &a->raiz;
    while (*p && (*p)->dado.id != id)
        p = id < (*p)->dado.id ? &(*p)->esq : &(*p)->dir;
    if (!*p) return 0;
    NoCliente *alvo = *p;
    if (alvo->esq && alvo->dir) {
        /* Sucessor: menor registro da subarvore direita. */
        NoCliente **s = &alvo->dir;
        while ((*s)->esq) s = &(*s)->esq;
        NoCliente *sucessor = *s;
        alvo->dado = sucessor->dado;
        *s = sucessor->dir;
        free(sucessor);
    } else {
        *p = alvo->esq ? alvo->esq : alvo->dir;
        free(alvo);
    }
    a->quantidade--; return 1;
}
static int altura(const NoCliente *p) {
    if (!p) return -1;
    int e = altura(p->esq), d = altura(p->dir);
    return 1 + (e > d ? e : d);
}
int clientes_altura(const Clientes *a) { return altura(a->raiz); }
static void listar(const NoCliente *p) {
    if (!p) return;
    listar(p->esq); printf("%d | %s\n", p->dado.id, p->dado.nome); listar(p->dir);
}
void clientes_listar(const Clientes *a) { listar(a->raiz); }
void clientes_destruir(Clientes *a) {
    /* Remocao iterativa evita recursao na liberacao de arvore degenerada. */
    while (a->raiz) clientes_remover(a, a->raiz->dado.id);
}
