#include "clientes.h"
#include <stdio.h>
#include <string.h>

/* Retorna 1 se leu, 0 no fim da entrada e -1 se a linha e longa. */
static int ler_linha(const char *mensagem, char *texto, size_t tamanho) {
    printf("%s", mensagem);
    if (fgets(texto, (int)tamanho, stdin) == NULL) {
        return 0;
    }
    char *fim = strchr(texto, '\n');
    if (fim != NULL) {
        *fim = '\0';
        return 1;
    }
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
        /* Descarta o restante de uma linha longa. */
    }
    return -1;
}

/* Le o numero do texto. A letra extra ajuda a rejeitar entradas como 12abc. */
static int ler_numero(const char *mensagem, int *valor) {
    char texto[80];
    char extra;

    while (1) {
        int resultado = ler_linha(mensagem, texto, sizeof(texto));
        if (resultado == 0) {
            return 0;
        }
        /* Limita a leitura a 9 digitos para o numero caber em int. */
        if (resultado == 1 && sscanf(texto, "%9d %c", valor, &extra) == 1 &&
            *valor >= 0) {
            return 1;
        }
        puts("Informe um inteiro de 0 a 999999999.");
    }
}

static void mostrar_menu(void) {
    puts("\nAUTOPECAS - CADASTRO DE CLIENTES");
    puts("1 Cadastrar cliente");
    puts("2 Buscar cliente");
    puts("3 Remover cliente");
    puts("4 Listar clientes");
    puts("0 Sair");
}

/* Os cadastros retornam 0 apenas quando a entrada termina. */
static int cadastrar_cliente(Clientes *clientes) {
    Cliente cliente = {0};
    if (!ler_numero("ID cliente: ", &cliente.id)) {
        return 0;
    }
    int resultado = ler_linha("Nome: ", cliente.nome, sizeof(cliente.nome));
    if (resultado == 0) {
        return 0;
    }
    if (resultado == -1) {
        puts("Nome longo demais.");
        return 1;
    }
    resultado = clientes_inserir(clientes, cliente);
    if (resultado == 1) {
        puts("Cliente cadastrado.");
    } else if (resultado == -1) {
        puts("Sem memoria.");
    } else {
        puts("ID duplicado ou dados invalidos.");
    }
    return 1;
}

int main(void) {
    Clientes clientes;
    int opcao, id;
    int continuar = 1;
    clientes_iniciar(&clientes);

    while (continuar) {
        mostrar_menu();
        if (!ler_numero("Opcao: ", &opcao)) {
            break;
        }
        switch (opcao) {
            case 0:
                continuar = 0;
                break;
            case 1:
                continuar = cadastrar_cliente(&clientes);
                break;
            case 2: {
                if (!ler_numero("ID cliente: ", &id)) {
                    continuar = 0;
                    break;
                }
                const Cliente *cliente = clientes_buscar(&clientes, id, NULL);
                if (cliente != NULL) {
                    printf("%d | %s\n", cliente->id, cliente->nome);
                } else {
                    puts("Cliente nao encontrado.");
                }
                break;
            }
            case 3:
                if (!ler_numero("ID cliente: ", &id)) {
                    continuar = 0;
                    break;
                }
                if (clientes_remover(&clientes, id)) {
                    puts("Cliente removido.");
                } else {
                    puts("Cliente nao encontrado.");
                }
                break;
            case 4:
                clientes_listar(&clientes);
                break;
            default:
                puts("Opcao invalida.");
        }
    }
    clientes_destruir(&clientes);
    return 0;
}
