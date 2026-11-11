#include "clientes.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
Cliente cliente(int id) { Cliente c = {.id = id}; strcpy(c.nome, "Cliente teste"); return c; }
void verificar(Clientes *a, const int *ids, size_t n) {
    assert(a->quantidade == n);
    for (size_t i = 0; i < n; i++) assert(clientes_buscar(a, ids[i], NULL));
}
void teste_bst(void) {
    Clientes a; clientes_iniciar(&a); size_t comp = 99;
    assert(clientes_altura(&a) == -1);
    assert(!clientes_buscar(&a, 1, &comp) && comp == 0);
    assert(!clientes_remover(&a, 1)); clientes_destruir(&a);
    int ids[] = {40,20,60,10,30,50,70,25};
    for (size_t i = 0; i < 8; i++) assert(clientes_inserir(&a, cliente(ids[i])) == 1);
    verificar(&a, ids, 8); assert(clientes_altura(&a) == 3);
    assert(!clientes_inserir(&a, cliente(40))); assert(!clientes_inserir(&a, cliente(0)));
    assert(!clientes_buscar(&a, 999, NULL));
    assert(clientes_remover(&a, 10)); /* folha */
    assert(clientes_remover(&a, 30)); /* um filho: 25 */
    assert(clientes_remover(&a, 40)); /* raiz com dois filhos */
    int restantes[] = {20,25,50,60,70}; verificar(&a, restantes, 5);
    assert(!clientes_buscar(&a, 40, NULL));
    assert(strcmp(clientes_buscar(&a, 50, NULL)->nome, "Cliente teste") == 0);
    clientes_destruir(&a); assert(!a.raiz && !a.quantidade); clientes_destruir(&a);
    for (int i = 1; i <= 7; i++) assert(clientes_inserir(&a, cliente(i)) == 1);
    assert(clientes_altura(&a) == 6); clientes_destruir(&a);
    for (int i = 7; i >= 1; i--) assert(clientes_inserir(&a, cliente(i)) == 1);
    assert(clientes_altura(&a) == 6); clientes_destruir(&a);
    int balanceada[] = {4,2,6,1,3,5,7};
    for (int i = 0; i < 7; i++) assert(clientes_inserir(&a, cliente(balanceada[i])) == 1);
    assert(clientes_altura(&a) == 2); clientes_destruir(&a);
    printf("BST: vazia, buscas, duplicatas, ordens, alturas, remocoes e liberacao OK\n");
}
int main(void) {
    teste_bst();
    printf("Todos os testes passaram.\n");
    return 0;
}
