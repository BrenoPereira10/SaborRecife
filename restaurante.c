#include "restaurante.h"

// INICIALIZA RESTAURANTE
// ======================================================

void inicializarRestaurante(Restaurante *r){

    r->inicio = NULL;

    r->fim = NULL;
}

// INICIALIZA FILA
// ======================================================

void inicializarFila(FilaCozinha *fila){

    fila->inicio = NULL;

    fila->fim = NULL;
}

// CRIA UMA MESA
// ======================================================

Mesa* criarMesa(int numero){

    Mesa *novaMesa = malloc(sizeof(Mesa));

    novaMesa->numero = numero;

    novaMesa->status = VAZIA;

    novaMesa->cliente = NULL;

    return novaMesa;
}

// CRIA O NÓ DA COZINHA
// ======================================================

NoLista* criarCozinha(){

    NoLista *novo = malloc(sizeof(NoLista));

    novo->tipo = COZINHA;

    novo->mesa = NULL;

    novo->anterior = NULL;

    novo->proximo = NULL;

    return novo;
}

// CRIA UM NÓ DE MESA
// ======================================================

NoLista* criarNoMesa(int numero){

    NoLista *novo = malloc(sizeof(NoLista));

    novo->tipo = MESA;

    novo->mesa = criarMesa(numero);

    novo->anterior = NULL;

    novo->proximo = NULL;

    return novo;
}
// ADICIONA POSIÇÃO NA LISTA
// O restaurante é linear:
// Cozinha <-> Mesa1 <-> Mesa2
//
// ======================================================
void adicionarPosicao(Restaurante *r, NoLista *novo){

    // caso lista vazia
    if(r->inicio == NULL){

        r->inicio = novo;

        r->fim = novo;
    }

    // caso já existam elementos
    else{

        novo->anterior = r->fim;

        r->fim->proximo = novo;

        r->fim = novo;
    }
}
// INICIALIZA GARÇOM
// O garçom começa na cozinha.
// ======================================================
void inicializarGarcom(Garcom *g, Restaurante *r){

    g->posicaoAtual = r->inicio;

    g->carregandoPrato = 0;
}
// ======================================================
// MOVER PARA DIREITA
// ======================================================
// Move o garçom para o próximo nó.
// ======================================================

void irParaDireita(NoLista **posicaoAtual){

    // verifica se existe próximo
    if((*posicaoAtual)->proximo != NULL){

        *posicaoAtual = (*posicaoAtual)->proximo;

        printf("Garçom foi para a direita.\n");
    }
    else{

        printf("Fim do restaurante!\n");
    }
}



// ======================================================
// MOVER PARA ESQUERDA
// ======================================================
// Move o garçom para o nó anterior.
// ======================================================

void irParaEsquerda(NoLista **posicaoAtual){

    // verifica se existe anterior
    if((*posicaoAtual)->anterior != NULL){

        *posicaoAtual = (*posicaoAtual)->anterior;

        printf("Garçom foi para a esquerda.\n");
    }
    else{

        printf("Você já está na cozinha!\n");
    }
}



// ======================================================
// ENQUEUE
// ======================================================
// Adiciona prato no fim da fila.
// ======================================================

void enqueue(FilaCozinha *fila, Prato prato){

    NoFila *novo = malloc(sizeof(NoFila));

    novo->prato = prato;

    novo->proximo = NULL;


    // fila vazia
    if(fila->inicio == NULL){

        fila->inicio = novo;

        fila->fim = novo;
    }

    // fila já possui elementos
    else{

        fila->fim->proximo = novo;

        fila->fim = novo;
    }
}



// ======================================================
// DEQUEUE
// ======================================================
// Remove prato do início da fila.
// ======================================================

Prato dequeue(FilaCozinha *fila){

    Prato pratoVazio;

    strcpy(pratoVazio.nome, "Vazio");


    // fila vazia
    if(fila->inicio == NULL){

        printf("Nenhum prato pronto na cozinha!\n");

        return pratoVazio;
    }


    NoFila *remover = fila->inicio;

    Prato prato = remover->prato;


    fila->inicio = fila->inicio->proximo;


    // caso fila fique vazia
    if(fila->inicio == NULL){

        fila->fim = NULL;
    }


    free(remover);

    return prato;
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

        // verifica se garçom já está carregando algo
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


        // ==============================================
        // MESA SUJA
        // ==============================================

        if(mesa->status == SUJA){

            mesa->status = VAZIA;

            printf("Mesa %d foi limpa!\n",
                   mesa->numero);

            return;
        }


        // ==============================================
        // CLIENTE ESPERANDO
        // ==============================================

        if(mesa->status == OCUPADA &&
           mesa->cliente != NULL &&
           mesa->cliente->estado == ESPERANDO){

            // verifica se o garçom possui prato
            if(garcom->pratoAtual == NULL){

                printf("Você não está carregando prato!\n");

                return;
            }


            // verifica se prato está correto
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