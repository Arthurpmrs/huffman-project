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
    // struct node *right;
    // struct node *left;
};
struct priority_queue
{
    struct node *head;
};
bool vazio(struct priority_queue *fila)
{
    if(fila->head==NULL)
    {return true;}
    return false;
}
void printar_fila(struct priority_queue *fila)
{
    while(fila->head!=NULL)
    {   printf("Byte numero: %d, Frequencia: %d\n", fila->head->byte, fila->head->frequency);
        fila->head=fila->head->next;}
    return;
}
void adicionar_fila(struct priority_queue *fila, int item, int frequencia)
{
    struct node *novo_no=malloc(sizeof(struct node));
    novo_no->byte=item;
    novo_no->frequency=frequencia;
    if(vazio(fila)==true|| frequencia < fila->head->frequency)
    {    novo_no->next=fila->head;
        fila->head=novo_no;}
    else
    {
        struct node *atual=fila->head;
        while(atual->next!=NULL && frequencia > atual->next->frequency)
        {atual= atual->next;}
        novo_no->next=atual->next;
        atual->next=novo_no;
    }
    return;
}
struct priority_queue *filtrar_array_filaprioridade(char array[], struct priority_queue *fila)
{
    for(int i=0;i<MAX_BYTES;i++)
    {   if(array[i]>0)
        {adicionar_fila(fila, i, array[i]);}}
    return fila;
}
struct priority_queue *criar_filaprioridade()
{
    struct priority_queue *nova=malloc(sizeof(struct priority_queue));
    nova->head=NULL;
    return nova;
}
void pegando_frequencias(char array[])
{
    unsigned char caracter;
    FILE *file=fopen("teste.txt", "rb");
    while(fread(&caracter, sizeof(char), 1,file)>0)
    {array[caracter]=array[caracter]+1;
    printf("caracter:%c %d\n",caracter, caracter);}
    fclose(file);
    return;
}
void iniciando_array(char array[])
{
    for(int i=0;i<MAX_BYTES;i++)
    {array[i]=0;}
    return;
}
int main()
{
    char todosbytes[MAX_BYTES];
    iniciando_array(todosbytes);//checado
    pegando_frequencias(todosbytes);//checado
    struct priority_queue *filabytes=criar_filaprioridade();
    filabytes=filtrar_array_filaprioridade(todosbytes,filabytes);
    printar_fila(filabytes);
    return 0;
}