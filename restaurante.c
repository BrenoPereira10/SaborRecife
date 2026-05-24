#include "restaurante.h"
#include <time.h>

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

        if(garcom->pratoAtual != NULL){

            printf("Você já está carregando um prato!\n");

            return;
        }

        Prato pratoPegado = dequeue(cozinha);

        if(strcmp(pratoPegado.nome, "Vazio") != 0){

            garcom->pratoAtual = malloc(sizeof(Prato));

            *(garcom->pratoAtual) = pratoPegado;

            printf("Garçom pegou: %s\n",
                   garcom->pratoAtual->nome);
        }

        return;
    }

    if(posicaoAtual->tipo == MESA){

        Mesa *mesa = posicaoAtual->mesa;

        if(mesa->status == SUJA){

            mesa->status = VAZIA;

            printf("Mesa %d foi limpa!\n",
                   mesa->numero);

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

                printf("Cliente pediu: %s\n",
                       mesa->cliente->pratoDesejado.nome);

                printf("Você trouxe: %s\n",
                       garcom->pratoAtual->nome);
            }
        }
    }
}
