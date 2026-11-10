# Autopecas em C: clientes e estoque

Sistema de terminal para cadastrar, buscar, remover e listar clientes e pecas.
Tambem realiza vendas, diminuindo a quantidade da peca quando o cliente existe e ha estoque suficiente.

## Como compilar

O projeto precisa de GCC. Na pasta do projeto, execute:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -Iinclude src/main.c src/clientes.c src/estoque.c src/sistema.c -o autopecas
./autopecas
```

Se tiver Make instalado, tambem pode usar:

```sh
make
./build/autopecas
```

O sistema inicia vazio. Os dados ficam na memoria e sao perdidos ao sair.
O preco e informado em centavos: 1850 significa R$ 18,50.

## Por onde comecar a estudar

1. `include/clientes.h` e `include/estoque.h`: veja os campos de cada registro e as funcoes disponiveis.
2. `src/clientes.c`: acompanhe a insercao e a busca na arvore.
3. `src/estoque.c`: acompanhe o calculo da posicao e o percurso da lista.
4. `src/sistema.c`: veja como uma venda liga as duas estruturas.
5. `src/main.c`: veja a leitura dos dados e o menu com `switch`.

As funcoes usam nomes descritivos, blocos com chaves e comentarios nos passos das estruturas.
O menu separa o cadastro de clientes e pecas em funcoes menores.
A leitura usa `fgets` para aceitar nomes com espacos e `strtol` para validar numeros.

## Estruturas de dados

### Clientes: arvore binaria de busca (BST)

Cada no guarda um cliente e dois ponteiros: esquerda e direita.
IDs menores ficam a esquerda; maiores ficam a direita. IDs repetidos sao rejeitados.

- Busca e insercao: percorrem a arvore comparando o ID.
- Listagem: visita esquerda, raiz e direita, exibindo IDs em ordem crescente.
- Remocao de folha: retira o no.
- Remocao com um filho: liga o pai diretamente ao filho.
- Remocao com dois filhos: copia o menor registro da direita e remove esse sucessor.

Busca, insercao e remocao custam O(h), sendo h a altura. Uma arvore bem distribuida tem altura O(log n), mas esta BST nao se balanceia e pode chegar a O(n).
A altura e contada em arestas: vazia = -1 e folha = 0.

### Pecas: tabela hash com listas encadeadas

A posicao e calculada por `codigo % capacidade`. No menu, a capacidade e 11.
Cada posicao (balde) aponta para uma lista. Se dois codigos caem na mesma posicao, ambos ficam nessa lista: isso resolve uma colisao.

- Insercao: confere duplicata e coloca a peca no inicio da lista.
- Busca: calcula a posicao e percorre apenas aquela lista.
- Remocao: usa os ponteiros anterior e atual para religar a lista.
- Venda: diminui a quantidade somente quando a retirada e valida.

Com boa distribuicao, o custo esperado e O(1 + alfa), onde alfa = quantidade / capacidade.
No pior caso, todas as pecas ficam na mesma lista e o custo e O(n).
A tabela tem tamanho fixo; nao faz redimensionamento.

## Organizacao

| Arquivo ou pasta | Conteudo |
|---|---|
| `include/` | Tipos e declaracoes das funcoes |
| `src/clientes.c` | BST de clientes |
| `src/estoque.c` | Hash de pecas |
| `src/sistema.c` | Validacao da venda |
| `src/main.c` | Menu e leitura |
| `tests/teste.c` | Testes das estruturas e vendas em C |
| `tests/experimento.c` | Comparacao de alturas e colisoes em C |
| `resultados/` | Registros de validacao e experimento |
| `PROPOSTA.md` | Proposta e integrantes |
| `Makefile` | Atalhos opcionais de compilacao |

## Retornos das funcoes

Insercao: 1 indica sucesso, 0 indica dado invalido ou duplicado e -1 indica falta de memoria.
Busca: retorna um ponteiro para o registro ou NULL quando nao encontra.
Remocao: retorna 1 se removeu e 0 se nao encontrou.
Venda: retorna 1 no sucesso, 0 se o cliente nao existe e -1 se a peca ou a quantidade nao permite a venda.
Uma venda rejeitada nao altera o estoque. Quantidade zero nao remove o cadastro da peca.

As estruturas devem ser inicializadas antes do uso e destruidas ao terminar.
Ponteiros retornados pelas buscas apontam para registros internos: nao devem ser liberados pelo chamador nem usados apos remocoes ou destruicao.

## Testes e experimento

```sh
make test
make experimento
make sanitize
```

Os testes usam `assert` e verificam duplicatas, buscas, remocoes da BST, colisoes da hash e vendas validas e invalidas. Nao compile os testes com `-DNDEBUG`.
O experimento usa os mesmos 127 IDs em ordens crescente, decrescente e de medianas, e 100 pecas com codigos distribuidos ou concentrados em uma posicao.
Os resultados completos ficam em `resultados/experimento.txt`.

Na BST, as ordens sequenciais geram altura 126; a ordem de medianas gera altura 6.
Na hash, os codigos de 1 a 100 geram maior lista de tamanho 10; codigos com o mesmo resto geram uma lista de tamanho 100.
Isso mostra como a ordem de insercao e a distribuicao afetam as buscas.
O contador de comparacoes das buscas e opcional: passe NULL quando nao precisar dele.

A verificacao do menu inclui nomes com espacos, entrada invalida, listagem em ordem, venda e fim da entrada.
O registro em `resultados/validacao.txt` informa quais verificacoes foram executadas e as limitacoes do detector de vazamentos neste ambiente.

## Demonstracao

1. Cadastre o cliente 20, com nome Ana Silva.
2. Cadastre a peca 1, Filtro de oleo, com 10 unidades e preco 1850.
3. Venda 3 unidades da peca 1 para o cliente 20.
4. Consulte a peca: devem restar 7 unidades.
5. Tente vender para um cliente inexistente: o estoque deve permanecer igual.

## Escopo do trabalho

A hash e a estrutura principal; a BST e auxiliar. A proposta deve ser validada pelo professor.
O sistema nao salva arquivos de dados, nao emite nota fiscal e nao registra historico financeiro.
Nomes podem ter ate 79 bytes. IDs e codigos devem ser positivos.
Listagem e altura da BST usam recursao, que tem limite de pilha para arvores muito grandes.
A entrega e os slides devem seguir as orientacoes da disciplina.
