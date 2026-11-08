#include "sistema.h"
int sistema_vender(const Clientes *clientes, Estoque *estoque, int cliente, int peca, int quantidade) {
    if (!clientes_buscar(clientes, cliente, NULL)) return 0;
    return estoque_retirar(estoque, peca, quantidade) ? 1 : -1;
}
