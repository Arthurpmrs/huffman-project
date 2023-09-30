#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#define MAX_FILE_SIZE 1024
#define MAX_BYTES 256
struct node_list
{   int new_bit;
    struct node_list *next;};
struct info_bytes
{
    unsigned char original_byte;
    int frequency;
    int quantidade_nodes;
    struct node_list *ziped_byte;
};
struct node
{
    unsigned char byte;
    int frequency;
    int height;
    struct node *next;
    struct node *left;
    struct node *right;
};
bool vazio(struct node *fila)
{
    if(fila==NULL)
    {return true;}
    return false;
}
void printar_fila(struct node *fila)
{
    while(vazio(fila)==false)
    {   printf("Byte %c numero: %d, Frequencia: %d\n", fila->byte, fila->byte, fila->frequency);
        fila=fila->next;}
    return;
}
void printar_lista(struct node_list *lista)
{
    while(lista!=NULL)
    {   printf("%d", lista->new_bit);
        lista=lista->next;}
}
void printar_preordem(struct node *arvore)
{
    if(vazio(arvore)==false)
    {
        printf("byte %c frequencia %d, ", arvore->byte, arvore->frequency);
        printar_preordem(arvore->left);
        printar_preordem(arvore->right);
    }
    return;
}
int maximo (int n1, int n2)
{return (n1 > n2)? n1:n2;}
int altura_arvore(struct node *arvore)
{ 
    if(vazio(arvore)==true)
    {return -1;}
    else
    {return 1+ maximo(altura_arvore(arvore->left), altura_arvore(arvore->right));}
}
void iniciando_array(struct info_bytes array[])
{
    for(int i=0;i<MAX_BYTES;i++)
    {   array[i].frequency=0;
        array[i].ziped_byte=NULL;}
    return;
}
void pegando_frequencias(struct info_bytes array[], char nome[])
{
    FILE *file=fopen(nome, "rb");
    unsigned char caracter;
    while(fread(&caracter, sizeof(char), 1,file)>0)
    {array[caracter].frequency=array[caracter].frequency+1;}
    fclose(file);
    return;
}
struct node *adicionar_fila(struct node *fila, int item, int frequencia)
{
    struct node *novo = malloc(sizeof(struct node));
    novo->byte = item;
    novo->frequency = frequencia;
    novo->height=0;
    novo->left=NULL;
    novo->right=NULL;
    if ((vazio(fila))==true|| (frequencia < fila->frequency))
    {   novo->next = fila;
        fila= novo;}
    else 
    {
        struct node *atual = fila;
        while ((atual->next != NULL) && (atual->next->frequency < frequencia)) 
        {atual = atual->next;}
        novo->next = atual->next;
        atual->next = novo;
    }
    return fila;
}
struct node *filtrar_array_filaprioridade(struct info_bytes array[], struct node *fila)
{
    for(int i=0;i<MAX_BYTES;i++)
    {   if(array[i].frequency>0)
        {fila=adicionar_fila(fila, i, array[i].frequency);}}
    return fila;
}
struct node *adicionando_arvore(struct node *fila, struct node *node)
{
    if(vazio(fila)==true || node->frequency < fila->frequency)
    {   node->next=fila;
        fila=node;}
    else
    {
        struct node *atual=fila;
        while(atual->next !=NULL && atual->next->frequency <= node->frequency)
        {atual=atual->next;}
        node->next=atual->next;
        atual->next=node;
    }
    node->height=altura_arvore(node);
    return fila;
}
struct node *criar_no_especial(struct node *n1, struct node *n2)
{
    struct node *novo=malloc(sizeof(struct node));
    novo->byte='*';
    novo->frequency= n1->frequency+n2->frequency;
    if(n1->frequency >= n2->frequency)
    {   novo->left=n2;
        novo->right=n1;}
    else
    {   novo->left=n1;
        novo->right=n2;}
    novo->height=altura_arvore(novo);
    return novo;
}
struct node *remover(struct node *fila, struct node **n)
{
    if(vazio(fila)==true)
    {return NULL;}
    else
    {   struct node *auxiliar=fila;
        *n=auxiliar;
        fila=fila->next;
        auxiliar->next=NULL;
    }
    return fila;
}
struct node *fila_em_arvore(struct node *fila)
{
    while(vazio(fila)==false)
    {
        if(vazio(fila->next)==true)
        {return fila;}
        else
        {   struct node *n1=NULL;
            struct node *n2=NULL;
            fila=remover(fila, &n1);
            fila=remover(fila, &n2);
            struct node *aux=criar_no_especial(n1, n2);
            fila=adicionando_arvore(fila,aux);
        }
    }
}
struct node_list *adicionar_lista(struct node_list *lista, int item)
{
    struct node_list *novo=malloc(sizeof(struct node_list));
    novo->new_bit= item;
    novo->next=lista;
    return novo;
}
void backtrack(struct node *arvore, int byte[], int cont, struct info_bytes BYTES[])
{
    if(vazio(arvore)==false)
    {
        if(vazio(arvore->left)==true && vazio(arvore->right)==true)
        {
            struct node_list *new_list=NULL;
            int qnodes=0;
            for(int i=cont-1;i>=0;i--)
            {   
                new_list=adicionar_lista(new_list, byte[i]);
                qnodes++;
            }
            unsigned char caracter=arvore->byte;
            BYTES[caracter].ziped_byte=new_list;
            BYTES[caracter].original_byte=caracter;
            BYTES[caracter].quantidade_nodes=qnodes;
            cont--;
        }
        byte[cont]=0;
        backtrack(arvore->left, byte, cont+1, BYTES);
        byte[cont]=1;
        backtrack(arvore->right, byte, cont+1,BYTES);
    }
}
unsigned char set_bit(unsigned char byte, int quantidade)
{
    unsigned char mask= 1<< quantidade;
    return mask|byte;
}
int zip_arquivo_temporario(char nome[], struct info_bytes BYTES[], int altura)
{
    FILE *antigo=fopen(nome, "rb");
    strcat(nome,".huff");
    unsigned char byte_temporario=0;
    unsigned char caracter;
    FILE *file_temporario= fopen(nome, "wb");
    int contador_byte_temporario=7;
    while(fread(&caracter, sizeof(char), 1,antigo)>0)
    {
        struct node_list *novo_byte = BYTES[caracter].ziped_byte;
        while(novo_byte!=NULL)
        {
            if(contador_byte_temporario==-1)
            {   byte_temporario=0;
                contador_byte_temporario=7;}
            if(novo_byte->new_bit==1)
            {byte_temporario=set_bit(byte_temporario, contador_byte_temporario);}
            contador_byte_temporario=contador_byte_temporario-1;
            novo_byte=novo_byte->next;
        }
    }
    return 0;
}
int main()
{
    struct info_bytes todosbytes[MAX_BYTES];
    iniciando_array(todosbytes);
    char nome_arquivo[MAX_FILE_SIZE-5];
    printf("Inserir nome do arquivo: ");
    scanf("%s", nome_arquivo);
    pegando_frequencias(todosbytes, nome_arquivo);
    struct node *filabytes=NULL;
    filabytes=filtrar_array_filaprioridade(todosbytes,filabytes);
    struct node *arvorebytes=fila_em_arvore(filabytes);
    int altura=altura_arvore(arvorebytes);
    int novobyte[altura];
    backtrack(arvorebytes,novobyte,0, todosbytes);
    int byte_referencia[]={0,0,0,0,0,0,0,0};
    zip_arquivo_temporario(nome_arquivo, todosbytes, altura);
    return 0;
}