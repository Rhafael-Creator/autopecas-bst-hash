#ifndef SISTEMA_H
#define SISTEMA_H
#include "clientes.h"
#include "estoque.h"
/* 1 sucesso, 0 cliente ausente, -1 peca/quantidade invalida ou insuficiente. */
int sistema_vender(const Clientes *clientes, Estoque *estoque, int cliente, int peca, int quantidade);
#endif
