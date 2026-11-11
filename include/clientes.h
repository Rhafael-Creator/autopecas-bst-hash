#ifndef CLIENTES_H
#define CLIENTES_H
#include <stddef.h>

typedef struct {
    int id;
    char nome[80];
} Cliente;
typedef struct NoCliente NoCliente;
typedef struct {
    NoCliente *raiz;
    size_t quantidade;
} Clientes;
void clientes_iniciar(Clientes *a);
/* Insercao: 1 sucesso, 0 duplicado/invalido, -1 falta de memoria. */
int clientes_inserir(Clientes *a, Cliente c);
Cliente *clientes_buscar(Clientes *a, int id, size_t *comparacoes);
int clientes_remover(Clientes *a, int id);
int clientes_altura(Clientes *a); /* vazia = -1; folha = 0 */
void clientes_listar(Clientes *a);
void clientes_destruir(Clientes *a);
#endif
