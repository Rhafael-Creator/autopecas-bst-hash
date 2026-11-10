#include "sistema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

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

static int ler_numero(const char *mensagem, int *valor) {
    char texto[80];
    char *fim;
    while (1) {
        int resultado = ler_linha(mensagem, texto, sizeof(texto));
        if (resultado == 0) {
            return 0;
        }
        if (resultado == -1) {
            puts("Entrada longa demais.");
            continue;
        }
        /* strtol permite detectar letras e numeros fora do limite de int. */
        errno = 0;
        long numero = strtol(texto, &fim, 10);
        if (errno == 0 && fim != texto && *fim == '\0' &&
            numero >= 0 && numero <= INT_MAX) {
            *valor = (int)numero;
            return 1;
        }
        puts("Informe um inteiro nao negativo.");
    }
}

static void mostrar_menu(void) {
    puts("\nAUTOPECAS");
    puts("1 Cadastrar cliente");
    puts("2 Buscar cliente");
    puts("3 Remover cliente");
    puts("4 Listar clientes");
    puts("5 Cadastrar peca");
    puts("6 Buscar peca");
    puts("7 Remover peca");
    puts("8 Listar estoque");
    puts("9 Vender");
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

static int cadastrar_peca(Estoque *estoque) {
    Peca peca = {0};
    int preco;
    if (!ler_numero("Codigo peca: ", &peca.codigo)) {
        return 0;
    }
    int resultado = ler_linha("Nome: ", peca.nome, sizeof(peca.nome));
    if (resultado == 0) {
        return 0;
    }
    if (resultado == -1) {
        puts("Nome longo demais.");
        return 1;
    }
    if (!ler_numero("Quantidade: ", &peca.quantidade) ||
        !ler_numero("Preco em centavos (1850 = R$18,50): ", &preco)) {
        return 0;
    }
    peca.preco_centavos = preco;
    resultado = estoque_inserir(estoque, peca);
    if (resultado == 1) {
        puts("Peca cadastrada.");
    } else if (resultado == -1) {
        puts("Sem memoria.");
    } else {
        puts("Codigo duplicado ou dados invalidos.");
    }
    return 1;
}

int main(void) {
    Clientes clientes;
    Estoque estoque;
    int opcao, id, codigo, quantidade;
    int continuar = 1;
    clientes_iniciar(&clientes);
    if (!estoque_iniciar(&estoque, 11)) {
        fputs("Sem memoria.\n", stderr);
        return 1;
    }

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
            case 5:
                continuar = cadastrar_peca(&estoque);
                break;
            case 6: {
                if (!ler_numero("Codigo peca: ", &codigo)) {
                    continuar = 0;
                    break;
                }
                const Peca *peca = estoque_buscar(&estoque, codigo, NULL);
                if (peca != NULL) {
                    printf("%s | qtd %d | R$ %ld,%02ld\n", peca->nome,
                           peca->quantidade, peca->preco_centavos / 100,
                           peca->preco_centavos % 100);
                } else {
                    puts("Peca nao encontrada.");
                }
                break;
            }
            case 7:
                if (!ler_numero("Codigo peca: ", &codigo)) {
                    continuar = 0;
                    break;
                }
                if (estoque_remover(&estoque, codigo)) {
                    puts("Peca removida.");
                } else {
                    puts("Peca nao encontrada.");
                }
                break;
            case 8:
                estoque_listar(&estoque);
                break;
            case 9: {
                if (!ler_numero("ID cliente: ", &id) ||
                    !ler_numero("Codigo peca: ", &codigo) ||
                    !ler_numero("Quantidade: ", &quantidade)) {
                    continuar = 0;
                    break;
                }
                int resultado = sistema_vender(&clientes, &estoque, id, codigo, quantidade);
                if (resultado == 1) {
                    puts("Venda realizada; estoque atualizado.");
                } else if (resultado == 0) {
                    puts("Cliente nao cadastrado.");
                } else {
                    puts("Peca ausente ou quantidade invalida/insuficiente.");
                }
                break;
            }
            default:
                puts("Opcao invalida.");
        }
    }
    clientes_destruir(&clientes);
    estoque_destruir(&estoque);
    return 0;
}
