# Autopeças em C: cadastro de clientes

Sistema de terminal para cadastrar, buscar, remover e listar clientes de uma autopeça.
Os clientes são organizados por ID em uma árvore binária de busca (BST).

## Como compilar

Na pasta do projeto, com GCC instalado:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/main.c src/clientes.c -o autopecas
./autopecas
```

Se tiver Make instalado, também pode executar `make` e `./build/autopecas`.

O sistema inicia vazio. Os dados ficam na memória e são perdidos ao sair.

## Menu

- 1: cadastrar cliente (ID e nome).
- 2: buscar cliente pelo ID.
- 3: remover cliente pelo ID.
- 4: listar clientes em ordem crescente de ID.
- 0: sair e liberar a memória.

IDs devem ser positivos e únicos. Nomes aceitam espaços e até 79 bytes.
A leitura usa `fgets` para nomes e `strtol` para validar números.

1. `include/clientes.h`: campos do cliente, árvore e funções disponíveis.
2. `src/clientes.c`: implementação da árvore.
3. `src/main.c`: leitura dos dados e menu com `switch`.

Cada nó guarda um cliente e dois ponteiros: esquerda e direita.
IDs menores ficam à esquerda e maiores à direita.

- Inserção e busca: percorrem a árvore comparando o ID.
- Listagem em ordem: visita esquerda, raiz e direita.
- Remoção de folha: retira o nó.
- Remoção com um filho: liga o pai diretamente ao filho.
- Remoção com dois filhos: copia o menor registro da subárvore direita e remove esse sucessor.

Busca, inserção e remoção custam O(h), sendo h a altura da árvore.
Uma árvore bem distribuída tem altura O(log n), mas esta BST não se balanceia e pode chegar a O(n).
A altura é contada em arestas: árvore vazia = -1 e folha = 0.
A listagem visita todos os clientes, com custo O(n).

## Organização

| Arquivo | Conteúdo |
|---|---|
| `include/clientes.h` | Tipos e declarações |
| `src/clientes.c` | BST de clientes |
| `src/main.c` | Menu e leitura |
| `tests/teste.c` | Testes do cadastro e da BST em C |
| `tests/experimento.c` | Comparação das ordens de inserção em C |
| `resultados/` | Registros da validação e do experimento |
| `PROPOSTA.md` | Proposta |
| `Makefile` | Atalhos opcionais de compilação |

## Retornos das funções

Inserção: 1 indica sucesso, 0 indica dado inválido ou duplicado e -1 indica falta de memória.
Busca: retorna um ponteiro para o cliente ou NULL quando não encontra.
Remoção: retorna 1 se removeu e 0 se não encontrou.

Inicialize a árvore antes de usar e destrua ao terminar.
Os ponteiros retornados pelas buscas são internos: não devem ser liberados pelo chamador nem usados após remoções ou destruição.
O contador de comparações é opcional; passe NULL quando não precisar dele.

## Testes e experimento

```sh
make test
make experimento
make sanitize
```

Os testes usam `assert` e verificam árvore vazia, buscas, duplicatas, IDs inválidos, alturas, remoções de nós com zero, um e dois filhos e liberação de memória.
Não compile os testes com `-DNDEBUG`.

O experimento insere os mesmos 127 IDs em ordem crescente, decrescente e de medianas.
As ordens sequenciais geram altura 126; a ordem de medianas gera altura 6.
Isso mostra como a ordem de inserção afeta o custo das buscas, sem implementar balanceamento automático.
Os resultados ficam em `resultados/experimento.txt`.

## Demonstração

1. Cadastre o cliente 20, Ana Silva.
2. Cadastre o cliente 10, Bruno Souza.
3. Cadastre o cliente 30, Carla Lima.
4. Liste os clientes: os IDs aparecem como 10, 20 e 30.
5. Busque o ID 20: o programa mostra Ana Silva.
6. Tente cadastrar o ID 20 novamente: o cadastro é rejeitado.
7. Remova o ID 20 e liste novamente: restam 10 e 30.

## Escopo do trabalho

O projeto é um cadastro de clientes com BST e não salva os dados em arquivo.
Listagem e cálculo de altura usam recursão, sujeita ao limite de pilha em árvores muito grandes.
