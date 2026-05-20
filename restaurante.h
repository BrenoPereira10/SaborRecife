#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{

    ESPERANDO,
    COMENDO,
    FOI_EMBORA

}EstadoCliente;

typedef enum{

    VAZIA,
    OCUPADA,
    SUJA

}StatusMesa;

typedef enum{

    COZINHA,
    MESA

}TipoPosicao;

typedef struct{

    char nome[50];

}Prato;

typedef struct{

    EstadoCliente estado;

    int paciencia;

    int tempoComendo;

    Prato pratoDesejado;

}Cliente;

typedef struct{

    int numero;

    StatusMesa status;

    Cliente *cliente;

}Mesa;

typedef struct NoFila{

    Prato prato;

    struct NoFila *proximo;

}NoFila;

typedef struct{

    NoFila *inicio;

    NoFila *fim;

}FilaCozinha;

typedef struct NoLista{

    TipoPosicao tipo;

    Mesa *mesa;

    struct NoLista *anterior;

    struct NoLista *proximo;

}NoLista;

typedef struct{

    NoLista *inicio;

    NoLista *fim;

}Restaurante;

typedef struct{

    NoLista *posicaoAtual;

    Prato *pratoAtual;

}Garcom;

void inicializarRestaurante(Restaurante *r);

void inicializarFila(FilaCozinha *fila);

Mesa* criarMesa(int numero);

NoLista* criarCozinha();

NoLista* criarNoMesa(int numero);

void adicionarPosicao(Restaurante *r, NoLista *novo);

void inicializarGarcom(Garcom *g, Restaurante *r);

void irParaDireita(NoLista **posicaoAtual);

void irParaEsquerda(NoLista **posicaoAtual);

void enqueue(FilaCozinha *fila, Prato prato);

Prato dequeue(FilaCozinha *fila);

void interagir(NoLista *posicaoAtual,
               Garcom *garcom,
               FilaCozinha *cozinha);

Cliente* criarCliente();

void atualizarClientes(NoLista *inicioLista);

extern int pontuacao;

#endif
