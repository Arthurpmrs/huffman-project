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
void printar_preordem(struct node *fila)
{
    while(vazio(fila)==false)
    {
        printf("byte %c ", fila->byte);
        printar_preordem(fila->left);
        printar_preordem(fila->right);
    }
    printf("\n");
}
void iniciando_array(char array[])
{
    for(int i=0;i<MAX_BYTES;i++)
    {array[i]=0;}
    return;
}
// struct priority_queue *criar_filaprioridade()
// {
//     struct priority_queue *nova=malloc(sizeof(struct priority_queue));
//     nova->head=NULL;
//     return nova;
// }
void pegando_frequencias(char array[], char nome[])
{
    unsigned char caracter;
    strcat(nome, ".txt");
    FILE *file=fopen(nome, "rb");
    while(fread(&caracter, sizeof(char), 1,file)>0)
    {array[caracter]=array[caracter]+1;
    // printf("caracter:%c %d\n",caracter, caracter);
    }
    fclose(file);
    return;
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
    
    return 0;
}