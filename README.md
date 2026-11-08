# Autopeças: estoque com Hash e clientes com BST

**Integrante:** 

**Disciplina:** Algoritmos e Estruturas de Dados II - UFV Campus Florestal. Professor Lucas Bragança da Silva.

## Problema e proposta

Uma autopeças precisa localizar peças pelo código, controlar suas quantidades e verificar se o comprador está cadastrado. A aplicação de terminal permite cadastrar, consultar, remover e listar clientes e peças, além de realizar uma venda que reduz o estoque apenas após validar o cliente e a quantidade.

A estrutura **principal é a tabela hash do inventário**. A BST é uma estrutura auxiliar para o cadastro de clientes. A proposta está em `PROPOSTA.md`, para aprovação prévia pelo professor, inclusive quanto ao uso das duas estruturas. Este repositório não comprova essa aprovação.

## Escolha das estruturas

Na hash, a operação mais importante é a busca exata por código. Não precisamos de busca por prefixo nem de estoque ordenado. Usamos `codigo % capacidade` e encadeamento separado: cada posição aponta para uma lista de peças. Códigos diferentes podem produzir a mesma posição; a lista resolve a colisão. Inserção na cabeça é simples, mas a checagem de duplicata percorre a lista.

Com boa distribuição, busca, inserção com verificação e remoção custam O(1 + alfa), sendo alfa = n/m. No pior caso todos os códigos ficam na mesma lista e o custo é O(n). O número de posições é fixo (11 no menu), portanto não se promete custo constante quando o cadastro cresce. A listagem custa O(m+n). Uma AVL teria busca O(log n) no pior caso e percurso ordenado, mas exigiria rotações e controle de balanceamento. Para consulta exata de peças, a hash é adequada ao escopo.

Clientes usam uma BST por ID inteiro positivo. IDs menores ficam à esquerda; maiores, à direita; IDs repetidos são rejeitados. O percurso em ordem lista os clientes em ordem crescente de ID, não de nome. Busca, inserção e remoção custam O(h), com altura h: O(log n) quando a árvore está bem distribuída e O(n) no pior caso. Uma AVL evitaria a degeneração, com implementação mais complexa. Uma hash também verificaria clientes, mas não produziria essa ordem naturalmente. O ID é interno; não armazenamos CPF ou dados reais.

## TADs e operações

Os tipos dos nós são incompletos nos cabeçalhos: a aplicação não acessa seus campos. Os contêineres guardam a raiz ou os baldes e as contagens. Eles devem ser manipulados pelas funções públicas, sem cópias rasas.

- `clientes.h/.c`: inicialização, inserção, busca, remoção, altura, listagem e destruição.
- `estoque.h/.c`: inicialização, inserção, busca, remoção, retirada de quantidade, listagem, métricas e destruição.
- `sistema.h/.c`: integra as estruturas na venda.
- `main.c`: menu, leitura e mensagens; não implementa operações dos nós.

Na BST, remover uma folha libera o nó; com um filho, o pai passa a apontar para esse filho; com dois filhos, copia-se o registro inteiro do sucessor (menor da subárvore direita) e remove-se o sucessor. Na hash, a remoção reconecta o elo anterior ao próximo elemento antes de liberar o nó.

Inserções retornam 1 no sucesso, 0 para duplicata/dado inválido e -1 para falta de memória. Buscas retornam ponteiro constante para o registro interno ou NULL. Esse ponteiro não deve ser usado depois de remover/destruir a estrutura; remoções na BST também podem substituir registros. `sistema_vender` retorna 1 no sucesso, 0 se o cliente não existe e -1 para peça ausente ou quantidade inválida/insuficiente. Nenhuma venda rejeitada altera o estoque. Quantidade zero mantém a peça cadastrada.

## Organização

| Caminho | Responsabilidade |
|---|---|
| include/ | Interfaces dos TADs e da venda |
| src/ | Implementações e menu |
| tests/teste.c | Casos funcionais com assert |
| tests/teste_menu.py | Integração do executável |
| tests/experimento.c | Experimento determinístico |
| resultados/experimento.txt | Saída experimental coletada |
| resultados/validacao.txt | Registro da execução local |
| PROPOSTA.md | Texto para submeter ao professor |
| Makefile | Compilação e execução das verificações |
| .github/workflows/testes.yml | Verificações automáticas após publicação |

## Compilar e executar

No Linux, instale GCC e Make. Python 3 é necessário somente para o teste do menu. O programa C não usa bibliotecas externas.

```sh
make
./build/autopecas
make test
make experimento
make sanitize
```

Compilação equivalente da aplicação:

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -Iinclude src/main.c src/clientes.c src/estoque.c src/sistema.c -o autopecas
```

O menu trabalha em memória: inicia vazio e perde os registros ao sair. Digite o preço em centavos: 1850 representa R$ 18,50. Exemplo de demonstração: cadastre cliente ID 20, peça código 1 com 10 unidades, venda 3 unidades ao cliente 20 e consulte a peça: restam 7. Tente vender para cliente inexistente e confira que nada é retirado.

## Testes conforme o enunciado

| Exigência | Evidência executável |
|---|---|
| Estruturas vazias e busca ausente | Testes iniciais de BST e hash |
| Inserção e busca existente | Conferência de registros e contagens |
| Remoção e liberação | Remoções, destruição e destruição repetida |
| Ordens diferentes de inserção na BST | Crescente, decrescente e medianas |
| Folha, um filho e dois filhos | Remoções de 10, 30 e 40 na árvore do teste |
| Altura resultante | Vazia -1, sequencial 6 e medianas 2 com sete nós |
| Colisões e busca com colisões | Códigos 1, 6, 11 e 16 em cinco baldes |
| Remoção na hash | Início, meio, fim, último elemento e reinserção |
| Distribuição e fator de carga | Tamanhos dos baldes e alfa conferidos |
| Regras da aplicação | Venda válida, cliente ausente, peça ausente, falta de estoque, zero e negativo |
| Interface | Nome com espaços, entrada inválida, ordem da listagem e fim de entrada |

`assert` interrompe os testes se uma condição falhar; não compilar os testes com `-DNDEBUG`. Compilação local sem avisos com GCC, usando `-Werror`. Os testes funcionais e o teste do menu passaram. AddressSanitizer/UndefinedBehaviorSanitizer passaram com `ASAN_OPTIONS=detect_leaks=0`. LeakSanitizer com detecção de vazamentos habilitada não executou neste ambiente, pois informou incompatibilidade com ptrace; portanto **não foi comprovada ausência de vazamentos por esse detector**. `make sanitize` mantém a detecção habilitada para execução em máquina compatível. Os testes exercitam a liberação, mas isso não substitui o detector. A execução do GitHub Actions só pode ser confirmada depois de publicar.

## Experimento reproduzível

Execute `make experimento`. Não há aleatoriedade nem medição de tempo: cada comparação equivale a um nó visitado. Todas as buscas existentes são executadas uma vez. Altura é medida em arestas, com árvore vazia = -1.

### BST: mesmos 127 IDs (1 a 127)

| Ordem | Altura | Média de comparações em buscas existentes | Comparações para ID 128 ausente |
|---|---:|---:|---:|
| Crescente | 126 | 64,0000 | 127 |
| Decrescente | 126 | 64,0000 | 1 |
| Medianas recursivas | 6 | 6,0551 | 7 |

A ordem altera a forma, embora as chaves sejam iguais. As duas ordens sequenciais degeneram a árvore; inserir medianas gera, para este conjunto, uma árvore perfeita. Isso não torna a implementação uma árvore balanceada automaticamente. A busca ausente depende da chave: 128 termina na raiz da árvore decrescente, mas percorre toda a crescente. Um único caso ausente não representa todas as buscas.

### Hash: 100 peças, 11 posições

Colisão é contada quando uma inserção bem-sucedida encontra um balde já ocupado; não representa o total de comparações.

| Códigos | Fator de carga | Colisões | Maior lista | Média de comparações em buscas existentes |
|---|---:|---:|---:|---:|
| 1 a 100 | 9,0909 | 89 | 10 | 5,0500 |
| 1 + 11i, i de 0 a 99 | 9,0909 | 99 | 100 | 50,5000 |

Distribuição por posição 0 a 10: sequencial `[9,10,9,9,9,9,9,9,9,9,9]`; mesmo resto `[0,100,0,0,0,0,0,0,0,0,0]`. O fator de carga é igual, mas a distribuição muda bastante o custo. Alfa pode ser maior que 1 com encadeamento separado. O cenário concentrado ilustra o pior caso da função módulo; não é uma amostra de uso real. O tamanho pequeno da tabela foi deliberado para evidenciar colisões.

## Limitações e melhorias

Sem persistência, histórico de vendas, reposição pelo menu, emissão fiscal ou autenticação. A venda apenas valida e retira unidades; não gera cobrança nem total financeiro. Nomes têm no máximo 79 bytes e as chaves são inteiros positivos. A API pressupõe nomes terminados em zero, estruturas inicializadas e ponteiros válidos. Não se deve reinicializar uma estrutura preenchida sem destruí-la antes. A BST não se balanceia; altura e listagem são recursivas e podem esgotar a pilha em árvores enormes. A destruição da BST é iterativa, mas pode ser custosa pela sucessão de remoções. A hash não redimensiona. Melhorias possíveis: persistência em arquivos, rehash, AVL para clientes, reposição e registro de vendas. Falhas de alocação são tratadas, mas não foram simuladas nos testes.

## Referências e apoio

Requisitos: enunciado “Trabalho Prático 1 - Aplicação com Estruturas de Dados”, professor Lucas Bragança da Silva, fornecido com a atividade. O ZIP de laboratório enviado foi consultado como contexto, mas não incorporado: continha uma implementação incompleta. Implementação e documentação produzidas com apoio de IA generativa (ChatGPT); o estudante deve revisar, compreender e justificar a versão entregue e observar a orientação do professor sobre esse uso.

## Apresentação e entrega

O enunciado também exige slides para apresentação de 8 a 10 minutos, enviados pelo Moodle/PVANet e contendo o link do repositório público. Este projeto não inclui slides. Roteiro sugerido: problema (1 min), estruturas e justificativa (2 min), arquitetura (1 min), demonstração (2 min), testes e métricas (2 min), limitações (1 min). Confirmar aprovação da proposta e identificação dos integrantes antes da entrega.
