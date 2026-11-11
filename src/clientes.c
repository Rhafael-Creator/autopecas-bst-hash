#include "clientes.h"
#include <stdio.h>
#include <stdlib.h>

struct NoCliente {
    Cliente dado;
    struct NoCliente *esq;
    struct NoCliente *dir;
};

void clientes_iniciar(Clientes *arvore) {
    arvore->raiz = NULL;
    arvore->quantidade = 0;
}

int clientes_inserir(Clientes *arvore, Cliente cliente) {
    NoCliente *atual = arvore->raiz;
    NoCliente *pai = NULL;

    if (cliente.id <= 0 || cliente.nome[0] == '\0') {
        return 0;
    }

    /* Procura a posicao onde o novo cliente sera ligado. */
    while (atual != NULL) {
        pai = atual;
        if (cliente.id == atual->dado.id) {
            return 0;
        }
        if (cliente.id < atual->dado.id) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }

    NoCliente *novo = malloc(sizeof(NoCliente));
    if (novo == NULL) {
        return -1;
    }
    novo->dado = cliente;
    novo->esq = NULL;
    novo->dir = NULL;

    if (pai == NULL) {
        arvore->raiz = novo;
    } else if (cliente.id < pai->dado.id) {
        pai->esq = novo;
    } else {
        pai->dir = novo;
    }
    arvore->quantidade++;
    return 1;
}

Cliente *clientes_buscar(Clientes *arvore, int id, size_t *comparacoes) {
    NoCliente *atual = arvore->raiz;
    if (comparacoes != NULL) {
        *comparacoes = 0;
    }
    while (atual != NULL) {
        if (comparacoes != NULL) {
            (*comparacoes)++;
        }
        if (id == atual->dado.id) {
            return &atual->dado;
        }
        if (id < atual->dado.id) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }
    return NULL;
}

int clientes_remover(Clientes *arvore, int id) {
    NoCliente *atual = arvore->raiz;
    NoCliente *pai = NULL;

    while (atual != NULL && atual->dado.id != id) {
        pai = atual;
        if (id < atual->dado.id) {
            atual = atual->esq;
        } else {
            atual = atual->dir;
        }
    }
    if (atual == NULL) {
        return 0;
    }

    /* Com dois filhos, copia o menor cliente da subarvore direita.
       Depois remove esse sucessor, que tem no maximo um filho. */
    if (atual->esq != NULL && atual->dir != NULL) {
        NoCliente *sucessor = atual->dir;
        NoCliente *pai_sucessor = atual;
        while (sucessor->esq != NULL) {
            pai_sucessor = sucessor;
            sucessor = sucessor->esq;
        }
        atual->dado = sucessor->dado;
        pai = pai_sucessor;
        atual = sucessor;
    }

    NoCliente *filho = atual->dir;
    if (atual->esq != NULL) {
        filho = atual->esq;
    }
    if (pai == NULL) {
        arvore->raiz = filho;
    } else if (pai->esq == atual) {
        pai->esq = filho;
    } else {
        pai->dir = filho;
    }
    free(atual);
    arvore->quantidade--;
    return 1;
}

int altura(NoCliente *no) {
    if (no == NULL) {
        return -1;
    }
    int esquerda = altura(no->esq);
    int direita = altura(no->dir);
    if (esquerda > direita) {
        return esquerda + 1;
    }
    return direita + 1;
}

int clientes_altura(Clientes *arvore) {
    return altura(arvore->raiz);
}

void listar(NoCliente *no) {
    if (no == NULL) {
        return;
    }
    /* Esquerda, raiz, direita: imprime os IDs em ordem crescente. */
    listar(no->esq);
    printf("%d | %s\n", no->dado.id, no->dado.nome);
    listar(no->dir);
}

void clientes_listar(Clientes *arvore) {
    listar(arvore->raiz);
}

void clientes_destruir(Clientes *arvore) {
    while (arvore->raiz != NULL) {
        clientes_remover(arvore, arvore->raiz->dado.id);
    }
}
