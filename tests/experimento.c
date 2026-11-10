#include "clientes.h"
#include <assert.h>
#include <stdio.h>
static void inserir_meios(Clientes *a, int inicio, int fim) {
    if (inicio > fim) return;
    int meio = inicio + (fim - inicio) / 2;
    Cliente c = {.id = meio, .nome = "Experimento"}; assert(clientes_inserir(a, c) == 1);
    inserir_meios(a, inicio, meio - 1); inserir_meios(a, meio + 1, fim);
}
int main(void) {
    puts("BST: 127 IDs; altura em arestas; comparacoes = nos visitados");
    puts("ordem,n,altura,media_busca_existente,comparacoes_busca_128");
    for (int modo = 0; modo < 3; modo++) {
        Clientes a; clientes_iniciar(&a);
        if (modo == 2) inserir_meios(&a, 1, 127);
        else for (int i = 1; i <= 127; i++) {
            Cliente c = {.id = modo == 0 ? i : 128-i, .nome = "Experimento"};
            assert(clientes_inserir(&a, c) == 1);
        }
        size_t total = 0, comp;
        for (int i = 1; i <= 127; i++) { assert(clientes_buscar(&a, i, &comp)); total += comp; }
        assert(!clientes_buscar(&a, 128, &comp));
        printf("%s,127,%d,%.4f,%zu\n", modo == 0 ? "crescente" : modo == 1 ? "decrescente" : "medianas", clientes_altura(&a), total/127.0, comp);
        clientes_destruir(&a);
    }
    return 0;
}
