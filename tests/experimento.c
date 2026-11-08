#include "clientes.h"
#include "estoque.h"
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
    puts("HASH: 100 pecas; colisao = insercao em balde ja ocupado");
    puts("padrao,m,n,carga,colisoes,maior_lista,media_busca_existente,distribuicao");
    for (int modo = 0; modo < 2; modo++) {
        Estoque t; assert(estoque_iniciar(&t, 11)); size_t colisoes = 0, maior = 0, total = 0;
        for (int i = 0; i < 100; i++) {
            int codigo = modo ? 1 + 11*i : 1+i;
            if (estoque_tamanho_balde(&t, (size_t)codigo % 11)) colisoes++;
            Peca p = {.codigo = codigo, .nome = "Filtro", .quantidade = 1, .preco_centavos = 100};
            assert(estoque_inserir(&t, p) == 1);
        }
        for (int i = 0; i < 100; i++) { size_t c; assert(estoque_buscar(&t, modo ? 1+11*i : 1+i, &c)); total += c; }
        for (size_t i = 0; i < 11; i++) { size_t n = estoque_tamanho_balde(&t, i); if (n > maior) maior = n; }
        printf("%s,11,100,%.4f,%zu,%zu,%.4f,", modo ? "mesmo_resto" : "sequencial", estoque_fator_carga(&t), colisoes, maior, total/100.0);
        for (size_t i = 0; i < 11; i++) printf("%s%zu", i ? ";" : "", estoque_tamanho_balde(&t, i));
        puts(""); estoque_destruir(&t);
    }
    return 0;
}
