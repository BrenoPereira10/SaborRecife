#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "restaurante.h"

void inicializarFila(FilaCozinha *fila);
void enqueue(FilaCozinha *fila, Prato prato);
Prato dequeue(FilaCozinha *fila);

Cliente* criarCliente(NoLista *inicioLista);
void atualizarClientes(NoLista *inicioLista);
void atualizarFisicaClientes(NoLista *inicioLista, float dt);

#endif