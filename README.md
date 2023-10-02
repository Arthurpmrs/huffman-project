# Huffman Project

Projeto final da disciplina de Estrutura de Dados ministrada pelo Professor Dr.
Márcio Ribeiro.

Equipe:

-   Arthur Pontes de Miranda Ramos Soares
-   Davi da Silva Romão
-   Luiza Bezerra Bastos

O projeto consiste nos seguintes tópicos:

-   Implementação do algoritmo de codificação de Huffman, usando a linguagem C;
-   Implementação de testes unitários para as estruturas de dados utilizadas no Huffman;
-   Seminário sobre o algoritmo de Dijkstra;
-   Contagem e plotagem para comparação do processo de inserção em uma fila de prioridade com Heap e sem Heap;

## Huffman

### Compressão

O algoritmo de compressão está presente no arquivo `zip.c`. Para compilar e executar o código utilize os comandos abaixo.

```bash
cd huffman
make zip
./zip
```

### Descompressão

O algoritmo de descompressão está presente no arquivo `unzip.c`. Para compilar e executar o código utilize os comandos abaixos.

```bash
cd huffman
make unzip
./unzip
```

> [!IMPORTANT]
> É requerido que os arquivos a serem descomprimidos possuam formato: `filename.ext.huff`, para que o arquivo de saída seja gerado corretamente.

### Testes

Para executar a suite de testes utilize os comandos abaixo.

```bash
cd huffman
make tests
tests/tests
```

## Djikstra

## Contagem e Plotagem

As estruturas comparadas na etapa de contagem e plotagem foram uma **fila de prioridade (com base em lista encadeada)** e uma **fila de prioridade com Heap**.

Nesse processo, buscou-se verificar que a fila de prioridade possuía insersão em $O(n)$ e que a Heap possuia inserção em $O(log \; n)$, sendo $n$ o tamanho da estrutura.

O processo foi realizado da seguinte maneira:

1. Foram gerados $m$ conjuntos de números aleatórios para insersão inicial nas duas estruturas, resultando em $m$ pares de estruturas avaliadas. Os tamanhos do conjuntos foram escolhidos de forma igualmente espassada no intervalo $[10, 12000]$. Os valores aleatórios para cada um dos conjuntos foram escolhidos dentro do invervalo $[0, 14400]$. Todos os dados foram salvos num arquivo `.txt`.
2. Depois de alimentar ambas as estruturas com o seu respectivo conjunto de dados aleatório, foram gerados $s$ números aleatórios, no invervalo $[6000, 24000]$, para inserção com contagem. O número de comparações necessárias para inserir cada um dos $s$ números aleatórios em cada uma dos $m$ pares de estruturas foi salva em um arquivo `.txt` para a plotagem.
3. Dentre as $s$ amostras de cada par de estrutura, apenas aquele com o maior número de comparações foi escolhido e um gráfico do tamanho da estrutura vs número comparações para inserção foi gerado.

### Execução do script da contagem e plotagem

O arquivo que controla o processo de Contagem e Plotagem é chamado `main.py`. Então, basta executá-lo com o comando abaixo. Esse script foi desenvolvido utilizado o Python 3.11.4.

```bash
cd plotting
python3 main.py
```

Ambas as etapas de geração das amostras iniciais aleatórias e plotagem foram feitas em Python. Entretanto, a etapa de geração de números aleatórios para contagem e implementação das estruturas foi feita em C.
