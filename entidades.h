#ifndef ENTIDADES_H
#define ENTIDADES_H

// Importamos o restaurante.h porque precisamos saber 
// o que é um Cliente, um Prato e uma Fila
#include "restaurante.h" 

// ======================================================
// FUNÇÕES DA COZINHA
// ======================================================
void inicializarFila(FilaCozinha *fila);
void enqueue(FilaCozinha *fila, Prato prato);
Prato dequeue(FilaCozinha *fila);

// ======================================================
// FUNÇÕES DOS CLIENTES E TEMPO
// ======================================================
Cliente* criarCliente();
void atualizarClientes(NoLista *inicioLista);

#endif