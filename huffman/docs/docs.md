### Fila de Huffman para Árvore de Huffman
O objetivo dessa função é transformar a fila de prioridade, construída a partir das frequências dos bytes, na árvore binária de Huffman.

```C
huff_node_t *huff_get_tree_from_queue(huff_queue_t *hq)
{
    if (hq->size == 1)
    {
        huff_node_t *node = huff_dequeue(hq);
        huff_node_t *root = malloc(sizeof(huff_node_t));

        root->byte = malloc(sizeof(uint8_t));
        *(uint8_t *)root->byte = '*';

        root->frequency = node->frequency;
        root->left = NULL;
        root->right = node;

        return root;
    }
    else
    {
        while (hq->size > 1)
        {
            huff_node_t *left = huff_dequeue(hq);
            huff_node_t *right = huff_dequeue(hq);

            void *internal_byte = malloc(sizeof(uint8_t));
            *(uint8_t *)internal_byte = '*';

            uint64_t new_frequency = left->frequency + right->frequency;

            huff_enqueue(hq, internal_byte, new_frequency, left, right);
        }

        return huff_dequeue(hq);
    }
}
```

A ideia é que sejam dados dois `dequeue` e um `enqueue` até que só reste um nó na fila. Esse nó será a raiz da árvore.

É importante destacar que, como as estruturas foram implementadas com `void *`, não é possível simplesmente passar uma cópia do byte \*, é necessário alocar memória por meio do `malloc`, como pode ser observado no código (variável `internal_byte`).

Também é importante informar que o bloco `if` é necessário para o caso em que o arquivo a ser comprimido só tenha um tipo de byte. Nesses casos, a estratégia tomada foi criar um nó interno, colocar o byte único como filho da direita e deixar o filho da esquerda como nulo.

### Escrever a árvore de huffman em pré-ordem no arquivo

Essa função tem como objetivo escrever a árvore de Huffman em pré-ordem em um arquivo, no caso, no cabeçalho do arquivo `.huff`, levando em consideração a diferença entre **\* presente no nó interno e \* folha**.

```C
void huff_write_tree_to_file(huff_node_t *ht, FILE *output)
{
    if (ht != NULL)
    {
        if (is_special_node(ht))
        {
            char scape_char = '\\';
            fwrite(&scape_char, sizeof(uint8_t), 1, output);
        }

        fwrite((uint8_t *)ht->byte, sizeof(uint8_t), 1, output);

        huff_write_tree_to_file(ht->left, output);
        huff_write_tree_to_file(ht->right, output);
    }
}
```

A função se assemelha com o percurso em pré-ordem normal. A diferença está no bloco `if` que checa se o nó atual é ou não especial. Mas o que é um nó especial? **É aquele que é um \* ou uma \\ e, ao mesmo tempo, é uma folha**.

```C
bool is_special_node(huff_node_t *node)
{
    bool is_leaf = node->left == NULL &&
                   node->right == NULL;

    bool is_special_char = *(uint8_t *)node->byte == '\\' ||
                           *(uint8_t *)node->byte == '*';

    return is_leaf && is_special_char;
}
```

Se for o caso, antes de escrevermos o byte em si, inserimos no arquivo um caractere de escape, a \\ (contra-barra).

### Tamanho da árvore

Essa função é semelhante à de cima, mas o objetivo dela é contar quantos nós a árvore de huffman terá **quando for escrita no arquivo**.

```c
uint16_t huff_get_tree_size(huff_node_t *ht)
{
    if (ht == NULL)
    {
        return 0;
    }
    else
    {
        if (is_special_node(ht))
        {
            return 2 + huff_get_tree_size(ht->left) + huff_get_tree_size(ht->right);
        }
        else
        {
            return 1 + huff_get_tree_size(ht->left) + huff_get_tree_size(ht->right);
        }
    }
}
```

O fato de retornar o tamanho da árvore quando ela for escrita é um detalhe importante, pois se ambos o byte \* e \\ estiverem presentes, o tamanho da árvore escrita no arquivo vai ter 2 elementos a mais, no caso, as duas \\ de escape.

Então, na contagem, se for um nó especial (como já definido acima), soma-se 2 na contagem ao invés de 1.

### Criação do array das sequências de bits referentes a cada byte

Quando se deseja comprimir um byte de um arquivo utilizando a codificação de Huffman, basta andar na árvore de Huffman até encontrar o nó cujo item é igual ao byte de interesse e escrever o no arquivo o caminho para chegar até ele. Tal caminho é constituído por uma sequência de bits obtidos adicionado 0 sempre que o algoritmo seguir pela esquerda da árvore e 1 quando o algoritmo seguir pela direita da árvore.

Entretanto, esse processo não pode ser feito para cada byte de um arquivo, pois seria extremamente ineficiente. Então, as sequências ou caminhos de bits para cada byte são pré-processados e armazenados em um array, cujos índices equivalem a cada byte (entre 0 e 255 em decimal), possibilitando acesso em O(1).

A função responsável por popular esse array é descrita abaixo.

```c
void generate_huff_paths(huff_node_t *ht, list_t *paths_of_bytes[256], uint8_t path[256], int8_t index)
{
    if (ht->left == NULL && ht->right == NULL)
    {
        uint8_t leaf_byte = *(uint8_t *)ht->byte;
        paths_of_bytes[leaf_byte] = list_create();

        // copy auxiliary path array to current byte path list
        for (int i = index - 1; i >= 0; i--)
        {
            void *path_byte = malloc(sizeof(uint8_t));
            *(uint8_t *)path_byte = path[i];
            list_add_to_head(paths_of_bytes[leaf_byte], path_byte);
        }
    }

    if (ht->left != NULL)
    {
        path[index] = 0;
        generate_huff_paths(ht->left, paths_of_bytes, path, index + 1);
    }

    if (ht->right != NULL)
    {
        path[index] = 1;
        generate_huff_paths(ht->right, paths_of_bytes, path, index + 1);
    }
}
```

Tal função percorre a árvore apenas uma vez, em pré-ordem. Toda vez que o nó atual não for uma folha, o algoritmo adiciona um 0 no array auxiliar (`path`) e vai para a esquerda, aumentando 1 no índice. Quando ele volta, substituiu o 0 por 1 e vai para a direita, aumentando novamente o índice.

>[!important]
>O índice nesse caso é um valor passado por cópia. Isso facilita o processo, pois não é necessário reduzir o índice ao voltar da chamada recursiva. O índice será fixo para cada frame da função.

No evento de ser uma folha, o algoritmo apenas adiciona o conteúdo do array auxiliar na lista encadeada referente àquele determinado byte, que está presente no array `paths_of_bytes`.

### Compressão
Essa função tem como objetivo ler os bytes do arquivo original, montar os bytes comprimidos com base nos bytes lidos e escrevê-los no arquivo de saída (`.huff`).

```C
void zip_file(FILE *input, FILE *output, huff_node_t *ht, list_t *paths_of_bytes[256])
{
    uint16_t temp_header = 0;
    fwrite(&temp_header, sizeof(uint16_t), 1, output);

    huff_write_tree_to_file(ht, output);

    uint8_t byte;
    uint8_t byte_buffer = 0;
    int8_t byte_index = 7;

    while (fread(&byte, sizeof(uint8_t), 1, input) > 0)
    {
        list_t *path = paths_of_bytes[byte];
        list_node_t *current = path->head;
        while (current != NULL)
        {
            uint8_t bit = *(uint8_t *)current->item;

            if (bit == 1)
            {
                byte_buffer = set_bit(byte_buffer, byte_index);
            }

            byte_index--;

            if (byte_index == -1)
            {
                fwrite(&byte_buffer, sizeof(uint8_t), 1, output);
                byte_buffer = 0;
                byte_index = 7;
            }

            current = current->next;
        }
    }

    // Checks if there is any trash
    if (byte_index != 7)
    {
        fwrite(&byte_buffer, sizeof(uint8_t), 1, output);
    }

    // updates header with trash and tree size
    uint16_t tree_size = huff_get_tree_size(ht);
    uint16_t trash_size = (byte_index + 1) << 13;

    uint16_t header = trash_size | tree_size;
    uint8_t first_byte = header >> 8;
    uint8_t second_byte = header & 0b0000000011111111;

    fseek(output, 0, SEEK_SET);
    fwrite(&first_byte, sizeof(uint8_t), 1, output);
    fwrite(&second_byte, sizeof(uint8_t), 1, output);
}
```

A função é grande mas não é complexa, são apenas uma série de passos.

1. Dois bytes vazios são escritos no arquivo de saída. Eles servem para "marcar o lugar" dos bytes referentes ao tamanho do lixo e o tamanho da árvore. Nesse ponto, **não sabemos o tamanho do lixo ainda.**
2. É feita a leitura de cada byte do arquivo original e a consulta dos caminhos (ou sequências) de bits no array path_of_bytes. 
	1. Os caminhos vão sendo escritos em um byte buffer até que ele esteja completo. Quando estiver completo é escrito no arquivo .huff, e em sequência seu valor é zerado e o índice retorna ao início.
3. Ao final desse processo é necessário checar se existe lixo. Se existir (se o índice for diferente de 7), temos que escrever o byte incompleto no arquivo .huff.
4. Calcula-se o tamanho do lixo a partir do índice (e também o da árvore)
5. Realiza a operação de bits para juntar o tamanho da árvore e do lixo em em 2 bytes
6. O ponteiro do arquivo retorna ao início (onde se encontram os dois bytes zerados, colocados no início do processo)
7. Os dois primeiros bytes são atualizados com o tamanho do lixo e da árvore.

Uma etapa que vale destaque é a operação de bits para obter os dois primeiros bytes. Suponha que o tamanho da árvore é 533 e o tamanho do lixo 5.

```c
// Em binário:
uint16_t trash_size -> 00000000 00000101
uint16_t tree_size  -> 00000010 00010101

// Para que a operação funcione, o 101 do trash deve estar na parte esquerda do byte. Ou seja, ele deve andar 13 casas para a esquerda

trash_size = trash_size << 13

// Resultando em:
uint16_t trash_size -> 10100000 00000000


// Podemos juntar os dois agora sem problemas, utilizando o operador "bitwise OR" |
uint16_t header = trash_size | tree_size

// resultando em:
uint16_t header -> 10100010 00010101


// Agora, queremos separar em duas variáveis diferentes. Para o primeiro byte, só queremos os 8 bits da esquerda, então basta empurrá-los para a direita e armazená-los numa variável de 8 bits. 
uint8_t first_byte = header >> 8

// Resultando em:
uint8_t first_byte -> 10100010

// Para o segundo byte, queremos os 8 bits da direita. Para isso é utilizada uma máscara (mask) que tem o formato 00000000 11111111. Com ela, e utilizando o operador "bitwise and" &, conseguimos pegar somente a parte da esquerda e zerar os demais, pois qualquer número & 0, vai ser 0.

uint8_t second_byte = header & mask

// Resultando em
uint8_t second_byte -> 00010101
```

### Reconstrução da árvore

Essa função faz parte do algoritmo de descompressão (`unzip.c`) e tem como objetivo reconstruir a árvore que foi escrita em pré-ordem e salva no cabeçalho do arquivo `.huff`.

Normalmente, não é possível reconstruir uma árvore a partir de uma árvore escrita em pré-ordem, a não ser que seja escrita na forma de parêntesis. Entretanto, a árvore de Huffman não é uma árvore binária qualquer. 

Podemos nos apoiar no fato de que nós internos sempre serão \* e que uma eventual folha cujo item for igual a \* sempre será precedida por uma \\.

```C
binary_tree_t *_reconstruct_tree(uint16_t *i, uint8_t preorder_tree[])
{
    uint8_t *item = malloc(sizeof(uint8_t));

    if (preorder_tree[*i] == '*')
    {
        *item = '*';
        *i += 1;
        binary_tree_t *left = _reconstruct_tree(i, preorder_tree);
        binary_tree_t *right = _reconstruct_tree(i, preorder_tree);
        return create_binary_tree((void *)item, left, right);
    }
    else
    {
        if (preorder_tree[*i] == '\\') // é uma barra solo
        {
            *item = preorder_tree[*i + 1];
            *i += 2;
        }
        else
        {
            *item = preorder_tree[*i];
            *i += 1;
        }

        return create_binary_tree((void *)item, NULL, NULL);
    }
}
```

Nessa implementação, a árvore escrita em pré-ordem, proveniente do arquivo `.huff`, está armazenada no array `preorder_tree` e `i` é o índice desse array, iniciando em 0.

A ideia é checar se o valor do i-ésimo item da árvore em pré-ordem é um \* ou não. Se for, _encontramos um nó interno_. Nesse caso, primeiro movemos o índice (`i`) uma unidade para frente e depois continuamos nos "aprofundando" na árvore de forma recursiva para reconstruir o filho da esquerda e depois da direita.

>[!important]
> Essa parte do algoritmo se baseia no fato de que uma árvore binária é:
> - `NULL` ou 
> - Nó cujo filho da esquerda é também uma árvore binária e cujo filho da direita também é uma árvore binária.
> 
> Portanto, a cada novo nó, andamos recursivamente até o caso de parada (folha) e na volta reconstruímos o filho da esquerda. Depois, andamos novamente de forma recursiva até o caso de parada (folha) e na volta reconstruímos o filho da direita.

Caso o i-ésimo byte não seja um \*, **temos certeza que ele é uma folha**. Mas e se for a folha cujo item é \*?? Se esse fosse o caso, ele deve ser precedido por uma \\ (contra-barra). 

Portanto, devemos checar se o valor do byte da i-ésima posição é uma \\. Se for, **sabemos que o próximo é uma folha**, independe de ser o byte  \* ou o byte \\. Então, criamos um nó com o valor do byte seguinte (i + 1) e movemos o índice duas posições para frente (*uma referente à \\ de escape e uma referente ao byte que foi escapado*).

Caso não seja uma \\, então o próprio byte é a folha. Então, criamos o nó com com esse byte e movemos o índice uma unidade para frente.

### Calcular o tamanho do arquivo comprimido

Essa é uma função utilitária que tem como objetivo retornar o tamanho do arquivo .huff. Isso é necessário, pois durante a descompressão, é necessário saber o tamanho do arquivo para que o processo pare um byte antes e no último seja considerado o tamanho do lixo.

```C
#include <sys/stat.h>

uint64_t get_file_size_in_bytes(char filename[MAX_FILENAME_SIZE])
{
    struct stat sb;
    stat(filename, &sb);
    return (uint64_t)sb.st_size;
}
```

Para isso, usamos a função `stat` pertencente à biblioteca padrão `sys/stat.h`. Essa função recebe o nome do arquivo e um ponteiro para uma instância da struct `struct stat`, implementada também na biblioteca. 

Essa struct serve para armazenar os metadados do arquivo. No nosso caso, só queremos a propriedade `st_size`, que é referente ao tamanho do arquivo.

### Processamento do tamanho da árvore e do lixo
Essa função faz o processo inverso do que é feito na compressão. Separa-se os dois primeiros bytes do cabeçalho nas variáveis `trash_size` e `tree_size`.

```c
bool get_sizes_from_header(FILE *input, uint8_t *trash_size, uint16_t *tree_size)
{
    if (input == NULL)
    {
        return false;
    }

    uint8_t header_bytes[2];
    fread(header_bytes, sizeof(uint8_t), 2, input);

    // get trash size
    *trash_size = header_bytes[0] >> 5;

    // get tree size
    uint8_t mask = 0b00011111;
    *tree_size = (header_bytes[0] & mask) << 8 | header_bytes[1];

    return true;
}
```

Suponha que o tamanho do lixo seja 5 e o tamanho da árvore 533.
```C
// No final, temos que obter o seguinte:
uint16_t trash_size -> 00000000 00000101
uint16_t tree_size  -> 00000010 00010101

// Temos agora (lido do arquivo):
uint8_t header_bytes[2] -> {10100010, 00010101}

// Primeiro, pegamos o trash size. Nesse caso, só queremos so três primeiros bits da esquerda. Então, movemos o header_bytes[0] para a direita 5 posições.
uint8_t trash_size = header_bytes[0] >> 5

// Resultado
uint8_t trash_size -> 00000101

// Depois, queremos o os 5 primeiros bits do primeiro byte e os 8 do segundo byte. Novamente, usaremos uma máscara de formato 00011111 e o operador &.

uint8_t temporario = header_bytes[0] & mask

// Assim, eliminamos todos que estão nas posições onde tem 0 na máscara. O resultado é:
uint8_t temporario -> 00000010

// Agora, armazenamos o temporário e o header_bytes[1] numa variável de 16bits
uint16_t temp_larger     -> 00000000 00000010
uint16_t header_bytes[1] -> 00000000 00010101

// Para que a operação dê certo, os 8 bits da direita do temp_larger devem andar 8 posições para a esquerda.

uint16_t temp_larger = temp_larget << 8

// Resultando em:
uint16_t temp_larger -> 00000010 00000000

// Agora basta usar o operador |
uint16_t temp_larger     -> 00000010 00000000
uint16_t header_bytes[1] -> 00000000 00010101

uint16_t tree_size = temp_larger | header_bytes[1]

// Resultado em:
uint16_t tree_size -> 00000010 00010101
```