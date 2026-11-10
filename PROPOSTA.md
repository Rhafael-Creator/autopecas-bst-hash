# Proposta

Integrante: Rhafael Sousa Fernandes-6591; Luan Barros Reis-6576.

Título: Autopeças - controle de estoque e cadastro de clientes.

Uma pequena autopeças precisa consultar peças por código, manter as quantidades disponíveis e verificar se o comprador está cadastrado antes de retirar produtos. A aplicação será executada no terminal e manterá dados em memória durante a sessão.

Estrutura principal: tabela hash com encadeamento separado para as peças, indexadas por código. Estrutura auxiliar proposta: BST para clientes, indexados por ID interno.

Operações: inicialização, inserção, busca, remoção, listagem e liberação de memória; retirada de estoque após validação do cliente. Métricas: fator de carga, colisões, distribuição e comparações na hash; altura e comparações na BST sob diferentes ordens de inserção.

Justificativa: a hash atende consultas exatas frequentes de estoque; a BST permite verificar clientes e listar seus IDs em ordem. O escopo não inclui banco de dados nem interface gráfica. Solicitar aprovação do tema e do uso de uma estrutura auxiliar antes da entrega final.
