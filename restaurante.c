#include "restaurante.h"
#include "entidades.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int pontuacao = 0;

void inicializarRestaurante(Restaurante *r){
    r->inicio = NULL;
    r->fim = NULL;
}

// ======================================================
// CRIA MESA
// ======================================================

Mesa* criarMesa(int numero){
    Mesa *novaMesa = malloc(sizeof(Mesa));
    novaMesa->numero = numero;
    novaMesa->status = VAZIA;
    novaMesa->cliente = NULL;
    return novaMesa;
}

NoLista* criarCozinha(){
    NoLista *novo = malloc(sizeof(NoLista));
    novo->tipo = COZINHA;
    novo->mesa = NULL;
    novo->anterior = NULL;
    novo->proximo = NULL;
    return novo;
}

NoLista* criarNoMesa(int numero){
    NoLista *novo = malloc(sizeof(NoLista));
    novo->tipo = MESA;
    novo->mesa = criarMesa(numero);
    novo->anterior = NULL;
    novo->proximo = NULL;
    return novo;
}

void adicionarPosicao(Restaurante *r, NoLista *novo){
    if(r->inicio == NULL){
        r->inicio = novo;
        r->fim = novo;
    }
    else{
        novo->anterior = r->fim;
        r->fim->proximo = novo;
        r->fim = novo;
    }
}

void inicializarGarcom(Garcom *g, Restaurante *r){
    g->posicaoAtual = r->inicio;
    g->pratoAtual = NULL;
}

void irParaDireita(NoLista **posicaoAtual){
    if((*posicaoAtual)->proximo != NULL){
        *posicaoAtual = (*posicaoAtual)->proximo;
    }
    else{
        printf("Fim do restaurante!\n");
    }
}

void irParaEsquerda(NoLista **posicaoAtual){
    if((*posicaoAtual)->anterior != NULL){
        *posicaoAtual = (*posicaoAtual)->anterior;
    }
    else{
        printf("Você já está na cozinha!\n");
    }
}

// ======================================================
// INTERAÇÃO PRINCIPAL
// ======================================================

void interagir(NoLista *posicaoAtual,
               Garcom *garcom,
               FilaCozinha *cozinha){

    if(posicaoAtual->tipo == COZINHA){
        // A lógica da cozinha agora fica a cargo da main.c (Menu de seleção numérico)
        return;
    }

    if(posicaoAtual->tipo == MESA){
        Mesa *mesa = posicaoAtual->mesa;

        if(mesa->status == SUJA){
            mesa->status = VAZIA;
            printf("Mesa %d foi limpa!\n", mesa->numero);
            return;
        }

        if(mesa->status == OCUPADA &&
           mesa->cliente != NULL &&
           mesa->cliente->estado == ESPERANDO){

            if(garcom->pratoAtual == NULL){
                printf("Você não está carregando prato!\n");
                return;
            }

            if(strcmp(garcom->pratoAtual->nome,
                      mesa->cliente->pratoDesejado.nome) == 0){

                printf("Pedido entregue corretamente!\n");
                mesa->cliente->estado = COMENDO;
                free(garcom->pratoAtual);
                garcom->pratoAtual = NULL;
            }
            else{
                printf("Pedido errado!\n");
                printf("Cliente pediu: %s\n", mesa->cliente->pratoDesejado.nome);
                printf("Você trouxe: %s\n", garcom->pratoAtual->nome);
            }
        }
    }
}

int contarMesasOcupadas(NoLista *inicio) {
    int count = 0;
    NoLista *aux = inicio;
    while (aux != NULL) {
        if (aux->tipo == MESA && aux->mesa->cliente != NULL && aux->mesa->cliente->estado == ESPERANDO) {
            count++;
        }
        aux = aux->proximo;
    }
    return count;
}

void quickSortClientes(Mesa **mesas, int inicio, int fim) {
    if (inicio >= fim) return;

    Mesa *pivo = mesas[fim];
    int i = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (mesas[j]->cliente->paciencia < pivo->cliente->paciencia) {
            i++;
            Mesa *temp = mesas[i];
            mesas[i] = mesas[j];
            mesas[j] = temp;
        }
    }

    Mesa *temp = mesas[i + 1];
    mesas[i + 1] = mesas[fim];
    mesas[fim] = temp;

    int posicaoPivo = i + 1;
    quickSortClientes(mesas, inicio, posicaoPivo - 1);
    quickSortClientes(mesas, posicaoPivo + 1, fim);
}

void exibirFilaDeEspera(Mesa **mesas, int total) {
    printf("\n=== FILA DE ESPERA (mais urgente primeiro) ===\n");
    if (total == 0) {
        printf("Nenhum cliente esperando.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("#%d | Mesa %d | Pedido: %s | Paciencia: %d\n",
               i + 1,
               mesas[i]->numero,
               mesas[i]->cliente->pratoDesejado.nome,
               mesas[i]->cliente->paciencia);
    }
    printf("=============================================\n");
}