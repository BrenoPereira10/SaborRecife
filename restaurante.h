#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estado atual do cliente
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

typedef enum{

    COZINHA,
    MESA

}TipoPosicao;



// ======================================================
// STRUCT PRATO
// Representa uma comida do jogo.
// Ex:
// - Tapioca
// - Bolo de Rolo
// - Cuscuz

typedef struct{

    char nome[50];

}Prato;

// STRUCT CLIENTE
// Representa um cliente sentado em uma mesa.

typedef struct{

    EstadoCliente estado;

    int paciencia;

    Prato pratoDesejado;

}Cliente;



// ======================================================
// STRUCT MESA
// Cada mesa possui:
// - número identificador
// - status atual
// - ponteiro para cliente
typedef struct{

    int numero;

    StatusMesa status;

    Cliente *cliente;

}Mesa;



// ======================================================
// STRUCT FILA DA COZINHA
// Fila simples para armazenar pratos prontos.
typedef struct NoFila{

    Prato prato;

    struct NoFila *proximo;

}NoFila;



// estrutura principal da fila
typedef struct{

    NoFila *inicio;

    NoFila *fim;

}FilaCozinha;

// ======================================================
// STRUCT DO NÓ DA LISTA
// Cada nó representa uma posição física do restaurante.
// Exemplo:
// [Cozinha] <-> [Mesa 1] <-> [Mesa 2]
// ======================================================
typedef struct NoLista{

    TipoPosicao tipo;
    Mesa *mesa;


    // ponteiros da lista duplamente encadeada
    struct NoLista *anterior;

    struct NoLista *proximo;
}NoLista;

// STRUCT DO RESTAURANTE
// Guarda:
// - início da lista
// - fim da lista
typedef struct{

    NoLista *inicio;

    NoLista *fim;

}Restaurante;



// ======================================================
// STRUCT DO GARÇOM
// ======================================================
// O garçom se move utilizando um ponteiro
// para a posição atual.
// ======================================================

typedef struct{

    NoLista *posicaoAtual;

    Prato pratoAtual;

    int carregandoPrato;

}Garcom;

// FUNÇÕES DE INICIALIZAÇÃO
// ======================================================
// restaurante
void inicializarRestaurante(Restaurante *r);


// fila
void inicializarFila(FilaCozinha *fila);


// cria mesa
Mesa* criarMesa(int numero);


// cria nó cozinha
NoLista* criarCozinha();


// cria nó mesa
NoLista* criarNoMesa(int numero);


// adiciona posição no restaurante
void adicionarPosicao(Restaurante *r, NoLista *novo);


// inicializa garçom
void inicializarGarcom(Garcom *g, Restaurante *r);

#endif

void irParaDireita(NoLista **posicaoAtual);

void irParaEsquerda(NoLista **posicaoAtual);


void enqueue(FilaCozinha *fila, Prato prato);

Prato dequeue(FilaCozinha *fila);


void interagir(NoLista *posicaoAtual,
               Garcom *garcom,
               FilaCozinha *cozinha);