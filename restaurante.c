#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "restaurante.h"


void inicializarLista(Lista *lista){

    lista->inicio = NULL;

    lista->fim = NULL;
}

void adicionarPosicao(Lista *lista, char nome[]){

    // malloc cria espaço na memória
    // para um novo nó
    No *novo = malloc(sizeof(No));


    // copia o nome recebido
    // para dentro do nó
    strcpy(novo->nome, nome);


    // inicialmente o novo nó
    // não aponta para ninguém
    novo->proximo = NULL;

    novo->anterior = NULL;


    // ======================================
    // CASO A LISTA ESTEJA VAZIA
    // ======================================

    if(lista->inicio == NULL){

        // o início será o novo nó
        lista->inicio = novo;

        // o fim também será o novo nó
        lista->fim = novo;
    }

    // ======================================
    // CASO JÁ EXISTAM ELEMENTOS
    // ======================================

    else{

        // o nó novo aponta para o antigo fim usando o ponteiro anterior
        novo->anterior = lista->fim;


        // o antigo fim aponta para o novo nó
        lista->fim->proximo = novo;


        // atualizamos o fim da lista
        lista->fim = novo;
    }
}

//Função para percorrer o restaurante

void mostrarRestaurante(Lista *lista){

    // auxiliar começa no início
    No *aux = lista->inicio;


    // percorre enquanto existir nó
    while(aux != NULL){

        // mostra o nome da posição
        printf("[%s] ", aux->nome);


        // vai para o próximo nó
        aux = aux->proximo;
    }


    printf("\n");
}