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