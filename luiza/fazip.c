#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_BYTES 256
struct node
{
    unsigned char byte;
    int frequency;
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
void iniciando_array(char array[])
{
    for(int i=0;i<MAX_BYTES;i++)
    {array[i]=0;}
    return;
}
void pegando_frequencias(char array[], char nome[])
{
    unsigned char caracter;
    strcat(nome, ".txt");
    FILE *file=fopen(nome, "rb");
    while(fread(&caracter, sizeof(char), 1,file)>0)
    {array[caracter]=array[caracter]+1;}
    fclose(file);
    return;
}
struct node *adicionar_fila(struct node *fila, int item, int frequencia)
{
    struct node *novo = malloc(sizeof(struct node));
    novo->byte = item;
    novo->frequency = frequencia;
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
struct node *filtrar_array_filaprioridade(char array[], struct node *fila)
{
    for(int i=0;i<MAX_BYTES;i++)
    {   if(array[i]>0)
        {fila=adicionar_fila(fila, i, array[i]);}}
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
            fila=adicionando_arvore(fila,aux);}
    }
}
int main()
{
    char todosbytes[MAX_BYTES];
    iniciando_array(todosbytes);//checado
    char nome_arquivo[1200];
    printf("Inserir nome do arquivo: ");
    scanf("%s", nome_arquivo);
    pegando_frequencias(todosbytes, nome_arquivo);//checado
    struct node *filabytes=NULL;
    filabytes=filtrar_array_filaprioridade(todosbytes,filabytes);
    printar_fila(filabytes);//checado
    struct node *arvorebytes=fila_em_arvore(filabytes);
    printar_preordem(arvorebytes);
    printf("\n");
    return 0;
}