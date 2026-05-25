#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ESPERANDO,
    COMENDO,
    FOI_EMBORA
} EstadoCliente;

typedef enum {
    VAZIA,
    OCUPADA,
    SUJA
} StatusMesa;

typedef enum {
    MESA,
    COZINHA,
    CAMINHO
} TipoPosicao;

typedef struct {
    char nome[50];
} Prato;

struct NoLista;

typedef struct Cliente {
    EstadoCliente estado;
    int paciencia;
    int tempoComendo;
    Prato pratoDesejado;
    int idSprite; 
    struct NoLista *posicaoAtual; 
} Cliente;

typedef struct {
    int numero;
    StatusMesa status;
    Cliente *cliente;
} Mesa;

typedef struct NoFila {
    Prato prato;
    struct NoFila *proximo;
} NoFila;

typedef struct {
    NoFila *inicio;
    NoFila *fim;
} FilaCozinha;

typedef struct NoLista {
    TipoPosicao tipo;
    Mesa *mesa;
    int posX; 
    int posY; 
    struct NoLista *anterior;
    struct NoLista *proximo;
} NoLista;

typedef struct {
    NoLista *inicio;
    NoLista *fim;
} Restaurante;

typedef struct {
    NoLista *posicaoAtual;
    Prato *pratoAtual;
    float timerTransicao;
    int direcao; 
} Garcom;

extern int pontuacao;

void inicializarRestaurante(Restaurante *r);
Mesa* criarMesa(int numero);
NoLista* criarCozinha(int x, int y);
NoLista* criarNoMesa(int numero, int x, int y);
NoLista* criarCaminho(int x, int y);
void adicionarPosicao(Restaurante *r, NoLista *novo);
void inicializarGarcom(Garcom *g, Restaurante *r);
void irParaDireita(Garcom *g);
void irParaEsquerda(Garcom *g);
void atualizarMovimentoGarcom(Garcom *g, float dt);
void interagir(NoLista *posicaoAtual, Garcom *garcom, FilaCozinha *cozinha);

int contarMesasOcupadas(NoLista *inicio);
void quickSortClientes(Mesa **mesas, int inicio, int fim);

#endif