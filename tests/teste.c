#include "sistema.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static Cliente cliente(int id) { Cliente c = {.id = id}; strcpy(c.nome, "Cliente teste"); return c; }
static Peca peca(int id) { Peca p = {.codigo = id, .quantidade = 10, .preco_centavos = 1250}; strcpy(p.nome, "Filtro"); return p; }
static void verificar(Clientes *a, const int *ids, size_t n) {
    assert(a->quantidade == n);
    for (size_t i = 0; i < n; i++) assert(clientes_buscar(a, ids[i], NULL));
}
static void teste_bst(void) {
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
    puts("BST: vazia, buscas, duplicatas, ordens, alturas, remocoes e liberacao OK");
}
static void teste_hash(void) {
    Estoque t; size_t comp;
    assert(!estoque_iniciar(&t, 0)); estoque_destruir(&t);
    assert(estoque_iniciar(&t, 5)); assert(!estoque_buscar(&t, 1, NULL));
    assert(!estoque_remover(&t, 1)); assert(estoque_fator_carga(&t) == 0);
    int ids[] = {1,6,11,16};
    for (int i = 0; i < 4; i++) assert(estoque_inserir(&t, peca(ids[i])) == 1);
    assert(t.quantidade == 4 && estoque_tamanho_balde(&t, 1) == 4);
    assert(estoque_fator_carga(&t) == 0.8);
    for (size_t i = 0; i < 5; i++) if (i != 1) assert(estoque_tamanho_balde(&t, i) == 0);
    assert(estoque_buscar(&t, 1, &comp) && comp == 4);
    assert(!estoque_buscar(&t, 21, &comp) && comp == 4);
    assert(!estoque_inserir(&t, peca(6))); assert(!estoque_inserir(&t, peca(-1)));
    Peca invalida = peca(2); invalida.quantidade = -1; assert(!estoque_inserir(&t, invalida));
    assert(estoque_remover(&t, 11)); /* meio */
    assert(estoque_remover(&t, 16)); /* inicio */
    assert(estoque_remover(&t, 1));  /* fim */
    assert(estoque_buscar(&t, 6, NULL)); assert(!estoque_buscar(&t, 11, NULL));
    assert(estoque_remover(&t, 6)); assert(!t.quantidade);
    assert(estoque_inserir(&t, peca(6)) == 1); /* reinsercao */
    estoque_destruir(&t); assert(!t.baldes && !t.quantidade && !t.capacidade); estoque_destruir(&t);
    puts("HASH: vazia, colisoes, distribuicao, carga, buscas, remocoes e liberacao OK");
}
static void teste_venda(void) {
    Clientes a; Estoque t; clientes_iniciar(&a); assert(estoque_iniciar(&t, 11));
    assert(clientes_inserir(&a, cliente(1)) == 1); assert(estoque_inserir(&t, peca(10)) == 1);
    assert(sistema_vender(&a, &t, 2, 10, 2) == 0);
    assert(sistema_vender(&a, &t, 1, 99, 2) == -1);
    assert(sistema_vender(&a, &t, 1, 10, 11) == -1);
    assert(sistema_vender(&a, &t, 1, 10, 0) == -1);
    assert(sistema_vender(&a, &t, 1, 10, -1) == -1);
    assert(estoque_buscar(&t, 10, NULL)->quantidade == 10);
    assert(sistema_vender(&a, &t, 1, 10, 3) == 1);
    assert(estoque_buscar(&t, 10, NULL)->quantidade == 7);
    assert(sistema_vender(&a, &t, 1, 10, 7) == 1);
    assert(estoque_buscar(&t, 10, NULL)->quantidade == 0);
    assert(sistema_vender(&a, &t, 1, 10, 1) == -1);
    clientes_destruir(&a); estoque_destruir(&t); puts("VENDA: validacao e atualizacao de estoque OK");
}
int main(void) { teste_bst(); teste_hash(); teste_venda(); puts("Todos os testes passaram."); return 0; }
