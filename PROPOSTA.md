# Proposta

Integrantes: Rhafael Sousa Fernandes-6591; Luan Barros Reis-6576.

Título: Autopeças - cadastro de clientes com árvore binária de busca.

Uma pequena autopeças precisa cadastrar seus clientes, consultar os dados pelo ID, remover cadastros e listar os clientes em ordem de identificação. A aplicação será executada no terminal, em C, e manterá os dados em memória durante a sessão.

Estrutura principal: árvore binária de busca (BST), com clientes indexados por ID interno. Cada cliente possui ID positivo e nome; IDs repetidos não são aceitos.

Operações: inicialização, inserção, busca, remoção, listagem em ordem e liberação de memória.

Métricas: altura da árvore e quantidade de comparações nas buscas, usando os mesmos IDs inseridos em ordem crescente, decrescente e de medianas.

Justificativa: a BST organiza os clientes por ID e permite listá-los em ordem. O experimento mostra como a ordem de inserção influencia a altura e o custo das buscas, pois a árvore não possui balanceamento automático.

A proposta revisada deve ser validada pelo professor.
