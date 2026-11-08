#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
/* Toda entrada passa por fgets: evita scanf misturado com leitura de nomes. */
static int linha(const char *mensagem, char *s, size_t tamanho) {
    printf("%s", mensagem);
    if (!fgets(s, (int)tamanho, stdin)) return 0;
    char *fim = strchr(s, '\n');
    if (fim) *fim = '\0';
    else { int c; while ((c = getchar()) != '\n' && c != EOF) {} return -1; }
    return 1;
}
static int numero(const char *mensagem, int *valor) {
    char s[80], *fim;
    while (1) {
        int r = linha(mensagem, s, sizeof s);
        if (!r) return 0;
        if (r < 0) { puts("Entrada longa demais."); continue; }
        errno = 0; long n = strtol(s, &fim, 10);
        if (!errno && fim != s && *fim == '\0' && n >= 0 && n <= INT_MAX) { *valor = (int)n; return 1; }
        puts("Informe um inteiro nao negativo.");
    }
}
int main(void) {
    Clientes clientes; Estoque estoque;
    clientes_iniciar(&clientes);
    if (!estoque_iniciar(&estoque, 11)) { fputs("Sem memoria.\n", stderr); return 1; }
    int op, id, codigo, qtd, preco;
    while (1) {
        puts("\nAUTOPECAS\n1 Cadastrar cliente\n2 Buscar cliente\n3 Remover cliente\n4 Listar clientes\n5 Cadastrar peca\n6 Buscar peca\n7 Remover peca\n8 Listar estoque\n9 Vender\n0 Sair");
        if (!numero("Opcao: ", &op) || op == 0) break;
        if (op >= 1 && op <= 3) {
            if (!numero("ID cliente: ", &id)) break;
            if (op == 1) {
                Cliente c = {.id = id};
                int r = linha("Nome: ", c.nome, sizeof c.nome);
                if (!r) break;
                if (r < 0) { puts("Nome longo demais."); continue; }
                r = clientes_inserir(&clientes, c);
                puts(r == 1 ? "Cliente cadastrado." : r == -1 ? "Sem memoria." : "ID duplicado ou dados invalidos.");
            } else if (op == 2) {
                const Cliente *c = clientes_buscar(&clientes, id, NULL);
                if (c) printf("%d | %s\n", c->id, c->nome); else puts("Cliente nao encontrado.");
            } else puts(clientes_remover(&clientes, id) ? "Cliente removido." : "Cliente nao encontrado.");
        } else if (op == 4) clientes_listar(&clientes);
        else if (op >= 5 && op <= 7) {
            if (!numero("Codigo peca: ", &codigo)) break;
            if (op == 5) {
                Peca p = {.codigo = codigo};
                int r = linha("Nome: ", p.nome, sizeof p.nome);
                if (!r) break;
                if (r < 0) { puts("Nome longo demais."); continue; }
                if (!numero("Quantidade: ", &qtd) || !numero("Preco em centavos (1850 = R$18,50): ", &preco)) break;
                p.quantidade = qtd; p.preco_centavos = preco;
                r = estoque_inserir(&estoque, p);
                puts(r == 1 ? "Peca cadastrada." : r == -1 ? "Sem memoria." : "Codigo duplicado ou dados invalidos.");
            } else if (op == 6) {
                const Peca *p = estoque_buscar(&estoque, codigo, NULL);
                if (p) printf("%s | qtd %d | R$ %ld,%02ld\n", p->nome, p->quantidade, p->preco_centavos / 100, p->preco_centavos % 100);
                else puts("Peca nao encontrada.");
            } else puts(estoque_remover(&estoque, codigo) ? "Peca removida." : "Peca nao encontrada.");
        } else if (op == 8) estoque_listar(&estoque);
        else if (op == 9) {
            if (!numero("ID cliente: ", &id) || !numero("Codigo peca: ", &codigo) || !numero("Quantidade: ", &qtd)) break;
            int r = sistema_vender(&clientes, &estoque, id, codigo, qtd);
            puts(r == 1 ? "Venda realizada; estoque atualizado." : r == 0 ? "Cliente nao cadastrado." : "Peca ausente ou quantidade invalida/insuficiente.");
        } else puts("Opcao invalida.");
    }
    clientes_destruir(&clientes); estoque_destruir(&estoque); return 0;
}
