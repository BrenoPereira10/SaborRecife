#include "restaurante.h"
#include <time.h>

// variável global de pontuação
int pontuacao = 0;

// ======================================================
// INICIALIZA RESTAURANTE
// ======================================================

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



// ======================================================
// CRIA COZINHA
// ======================================================

NoLista* criarCozinha(){

    NoLista *novo = malloc(sizeof(NoLista));

    novo->tipo = COZINHA;

    novo->mesa = NULL;

    novo->anterior = NULL;

    novo->proximo = NULL;

    return novo;
}



// ======================================================
// CRIA NÓ DE MESA
// ======================================================

NoLista* criarNoMesa(int numero){

    NoLista *novo = malloc(sizeof(NoLista));

    novo->tipo = MESA;

    novo->mesa = criarMesa(numero);

    novo->anterior = NULL;

    novo->proximo = NULL;

    return novo;
}



// ======================================================
// ADICIONA POSIÇÃO NA LISTA
// ======================================================

void adicionarPosicao(Restaurante *r, NoLista *novo){

    // lista vazia
    if(r->inicio == NULL){

        r->inicio = novo;

        r->fim = novo;
    }

    // lista já possui elementos
    else{

        novo->anterior = r->fim;

        r->fim->proximo = novo;

        r->fim = novo;
    }
}



// ======================================================
// INICIALIZA GARÇOM
// ======================================================

void inicializarGarcom(Garcom *g, Restaurante *r){

    g->posicaoAtual = r->inicio;

    // começa sem prato
    g->pratoAtual = NULL;
}



// ======================================================
// IR PARA DIREITA
// ======================================================

void irParaDireita(NoLista **posicaoAtual){

    if((*posicaoAtual)->proximo != NULL){

        *posicaoAtual = (*posicaoAtual)->proximo;

        //printf("Garçom foi para a direita.\n");
    }

    else{

        printf("Fim do restaurante!\n");
    }
}



// ======================================================
// IR PARA ESQUERDA
// ======================================================

void irParaEsquerda(NoLista **posicaoAtual){

    if((*posicaoAtual)->anterior != NULL){

        *posicaoAtual = (*posicaoAtual)->anterior;

        //printf("Garçom foi para a esquerda.\n");
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

    // ==================================================
    // COZINHA
    // ==================================================

    if(posicaoAtual->tipo == COZINHA){

        // já está carregando prato
        if(garcom->pratoAtual != NULL){

            printf("Você já está carregando um prato!\n");

            return;
        }


        // pega prato da fila
        Prato pratoPegado = dequeue(cozinha);


        // verifica se veio prato válido
        if(strcmp(pratoPegado.nome, "Vazio") != 0){

            garcom->pratoAtual = malloc(sizeof(Prato));

            *(garcom->pratoAtual) = pratoPegado;

            printf("Garçom pegou: %s\n",
                   garcom->pratoAtual->nome);
        }

        return;
    }



    // ==================================================
    // MESA
    // ==================================================

    if(posicaoAtual->tipo == MESA){

        Mesa *mesa = posicaoAtual->mesa;


        // ==================================================
        // LIMPAR MESA
        // ==================================================

        if(mesa->status == SUJA){

            mesa->status = VAZIA;

            printf("Mesa %d foi limpa!\n",
                   mesa->numero);

            return;
        }


        // ==================================================
        // ENTREGAR PEDIDO
        // ==================================================

        if(mesa->status == OCUPADA &&
           mesa->cliente != NULL &&
           mesa->cliente->estado == ESPERANDO){

            // garçom sem prato
            if(garcom->pratoAtual == NULL){

                printf("Você não está carregando prato!\n");

                return;
            }


            // prato correto
            if(strcmp(garcom->pratoAtual->nome,
                      mesa->cliente->pratoDesejado.nome) == 0){

                printf("Pedido entregue corretamente!\n");


                mesa->cliente->estado = COMENDO;


                free(garcom->pratoAtual);

                garcom->pratoAtual = NULL;
            }

            // prato errado
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