#include "sistema.h"

int sistema_vender(const Clientes *clientes, Estoque *estoque, int cliente, int peca, int quantidade) {
    if (clientes_buscar(clientes, cliente, NULL) == NULL) {
        return 0;
    }
    if (!estoque_retirar(estoque, peca, quantidade)) {
        return -1;
    }
    return 1;
}
