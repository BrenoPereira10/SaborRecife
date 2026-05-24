#include "entidades.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

// ======================================================
// INICIALIZA FILA
// ======================================================
void inicializarFila(FilaCozinha *fila){
    fila->inicio = NULL;
    fila->fim = NULL;
}

// ======================================================
// ENQUEUE (Adiciona prato na fila da cozinha)
// ======================================================
void enqueue(FilaCozinha *fila, Prato prato){
    NoFila *novo = malloc(sizeof(NoFila));
    novo->prato = prato;
    novo->proximo = NULL;

    if(fila->inicio == NULL){
        fila->inicio = novo;
        fila->fim = novo;
    } else {
        fila->fim->proximo = novo;
        fila->fim = novo;
    }
}

// ======================================================
// DEQUEUE (Retira prato da fila da cozinha)
// ======================================================
Prato dequeue(FilaCozinha *fila){
    Prato pratoVazio;
    strcpy(pratoVazio.nome, "Vazio");

    if(fila->inicio == NULL){
        printf("Nenhum prato pronto na cozinha!\n");
        return pratoVazio;
    }

    NoFila *remover = fila->inicio;
    Prato prato = remover->prato;
    fila->inicio = fila->inicio->proximo;

    if(fila->inicio == NULL){
        fila->fim = NULL;
    }

    free(remover);
    return prato;
}

// ======================================================
// CRIA CLIENTE
// ======================================================
Cliente* criarCliente(){
    Cliente *novo = malloc(sizeof(Cliente));
    novo->estado = ESPERANDO;
    novo->paciencia = 5 + rand() % 11;
    novo->tempoComendo = 5 + rand() % 6;

    char cardapio[5][50] = {
        "Tapioca", "Bolo de Rolo", "Cuscuz", "Cartola", "Caldinho"
    };

    int indice = rand() % 5;
    strcpy(novo->pratoDesejado.nome, cardapio[indice]);

    return novo;
}

// ======================================================
// ATUALIZA CLIENTES (IA dos clientes)
// ======================================================
void atualizarClientes(NoLista *inicioLista){
    NoLista *aux = inicioLista;
    int chanceSpawn = rand() % 100;

    // SPAWN DE CLIENTE (30% de chance)
    if(chanceSpawn < 30){
        int mesasVazias = 0;
        aux = inicioLista;
        while(aux != NULL){
            if(aux->tipo == MESA && aux->mesa->status == VAZIA){
                mesasVazias++;
            }
            aux = aux->proximo;
        }

        if(mesasVazias > 0){
            int alvo = rand() % mesasVazias;
            int contador = 0;
            aux = inicioLista;

            while(aux != NULL){
                if(aux->tipo == MESA && aux->mesa->status == VAZIA){
                    if(contador == alvo){
                        aux->mesa->cliente = criarCliente();
                        aux->mesa->status = OCUPADA;
                        printf("\nNovo cliente chegou na Mesa %d\n", aux->mesa->numero);
                        printf("Pedido: %s\n", aux->mesa->cliente->pratoDesejado.nome);
                        break;
                    }
                    contador++;
                }
                aux = aux->proximo;
            }
        }
    }

    // PERCORRE TODAS AS MESAS PARA REDUZIR TEMPO
    aux = inicioLista;
    while(aux != NULL){
        if(aux->tipo == MESA && aux->mesa->cliente != NULL){
            Cliente *cliente = aux->mesa->cliente;

            if(cliente->estado == ESPERANDO){
                cliente->paciencia--;
                printf("Mesa %d esperando... (%d)\n", aux->mesa->numero, cliente->paciencia);

                if(cliente->paciencia <= 0){
                    printf("Cliente da Mesa %d foi embora!\n", aux->mesa->numero);
                    free(aux->mesa->cliente);
                    aux->mesa->cliente = NULL;
                    aux->mesa->status = SUJA;
                }
            }
            else if(cliente->estado == COMENDO){
                cliente->tempoComendo--;
                printf("Mesa %d comendo... (%d)\n", aux->mesa->numero, cliente->tempoComendo);

                if(cliente->tempoComendo <= 0){
                    printf("Cliente da Mesa %d terminou de comer!\n", aux->mesa->numero);
                    pontuacao += 10;
                    printf("Pontuação: %d\n", pontuacao);
                    free(aux->mesa->cliente);
                    aux->mesa->cliente = NULL;
                    aux->mesa->status = SUJA;
                }
            }
        }
        aux = aux->proximo;
    }
}