#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// ======================================================
// ENUMS
// ======================================================

// Estado do cliente
typedef enum{

    ESPERANDO,
    COMENDO,
    FOI_EMBORA

}EstadoCliente;


// Status da mesa
typedef enum{

    VAZIA,
    OCUPADA,
    SUJA

}StatusMesa;


// Tipo da posição do mapa
typedef enum{

    COZINHA,
    MESA

}TipoPosicao;



// ======================================================
// STRUCT PRATO
// ======================================================

typedef struct{

    char nome[50];

}Prato;



// ======================================================
// STRUCT CLIENTE
// ======================================================

typedef struct{

    // estado atual do cliente
    EstadoCliente estado;

    // tempo máximo esperando pedido
    int paciencia;

    // tempo que o cliente leva comendo
    int tempoComendo;

    // prato desejado
    Prato pratoDesejado;

}Cliente;



// ======================================================
// STRUCT MESA
// ======================================================

typedef struct{

    int numero;

    StatusMesa status;

    Cliente *cliente;

}Mesa;



// ======================================================
// STRUCT FILA
// ======================================================

// nó da fila
typedef struct NoFila{

    Prato prato;

    struct NoFila *proximo;

}NoFila;


// fila principal
typedef struct{

    NoFila *inicio;

    NoFila *fim;

}FilaCozinha;



// ======================================================
// STRUCT DA LISTA
// ======================================================

typedef struct NoLista{

    TipoPosicao tipo;

    Mesa *mesa;

    struct NoLista *anterior;

    struct NoLista *proximo;

}NoLista;



// ======================================================
// STRUCT RESTAURANTE
// ======================================================

typedef struct{

    NoLista *inicio;

    NoLista *fim;

}Restaurante;



// ======================================================
// STRUCT GARÇOM
// ======================================================

typedef struct{

    // posição atual do garçom
    NoLista *posicaoAtual;

    // prato carregado
    // NULL = mãos vazias
    Prato *pratoAtual;

}Garcom;



// ======================================================
// FUNÇÕES DE INICIALIZAÇÃO
// ======================================================

void inicializarRestaurante(Restaurante *r);

void inicializarFila(FilaCozinha *fila);

Mesa* criarMesa(int numero);

NoLista* criarCozinha();

NoLista* criarNoMesa(int numero);

void adicionarPosicao(Restaurante *r, NoLista *novo);

void inicializarGarcom(Garcom *g, Restaurante *r);



// ======================================================
// MOVIMENTAÇÃO
// ======================================================

void irParaDireita(NoLista **posicaoAtual);

void irParaEsquerda(NoLista **posicaoAtual);



// ======================================================
// FILA
// ======================================================

void enqueue(FilaCozinha *fila, Prato prato);

Prato dequeue(FilaCozinha *fila);



// ======================================================
// INTERAÇÃO
// ======================================================

void interagir(NoLista *posicaoAtual,
               Garcom *garcom,
               FilaCozinha *cozinha);

Cliente* criarCliente();

void atualizarClientes(NoLista *inicioLista);


// pontuação do jogo
extern int pontuacao;
#endif