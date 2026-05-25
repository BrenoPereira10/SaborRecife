#include "entidades.h"

void inicializarFila(FilaCozinha *fila){
    fila->inicio = NULL;
    fila->fim = NULL;
}

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

Prato dequeue(FilaCozinha *fila){
    Prato pratoVazio;
    strcpy(pratoVazio.nome, "Vazio");
    if(fila->inicio == NULL) return pratoVazio;
    NoFila *remover = fila->inicio;
    Prato prato = remover->prato;
    fila->inicio = fila->inicio->proximo;
    if(fila->inicio == NULL) fila->fim = NULL;
    free(remover);
    return prato;
}

Cliente* criarCliente(NoLista *inicioLista){
    Cliente *novo = malloc(sizeof(Cliente));
    novo->estado = ESPERANDO;
    novo->paciencia = 15 + rand() % 10; 
    novo->tempoComendo = 5 + rand() % 6;
    novo->idSprite = rand() % 3; 
    novo->posicaoAtual = inicioLista; 
    char cardapio[5][50] = { "Tapioca", "Bolo de Rolo", "Cuscuz", "Cartola", "Caldinho" };
    strcpy(novo->pratoDesejado.nome, cardapio[rand() % 5]);
    return novo;
}

void atualizarFisicaClientes(NoLista *inicioLista, float dt){
    static float timerMove = 0.0f;
    timerMove += dt;
    if(timerMove >= 0.25f){ 
        timerMove = 0.0f;
        NoLista *aux = inicioLista;
        while(aux != NULL){
            if(aux->tipo == MESA && aux->mesa->cliente != NULL){
                Cliente *c = aux->mesa->cliente;
                if(c->posicaoAtual != aux && c->posicaoAtual->proximo != NULL){
                    c->posicaoAtual = c->posicaoAtual->proximo;
                }
            }
            aux = aux->proximo;
        }
    }
}

void atualizarClientes(NoLista *inicioLista){
    NoLista *aux = inicioLista;
    int chanceSpawn = rand() % 100;

    if(chanceSpawn < 30){ 
        int mesasVazias = 0;
        aux = inicioLista;
        while(aux != NULL){
            if(aux->tipo == MESA && aux->mesa->status == VAZIA) mesasVazias++;
            aux = aux->proximo;
        }

        if(mesasVazias > 0){
            int alvo = rand() % mesasVazias;
            int contador = 0;
            aux = inicioLista;
            while(aux != NULL){
                if(aux->tipo == MESA && aux->mesa->status == VAZIA){
                    if(contador == alvo){
                        aux->mesa->cliente = criarCliente(inicioLista);
                        aux->mesa->status = OCUPADA;
                        break;
                    }
                    contador++;
                }
                aux = aux->proximo;
            }
        }
    }

    aux = inicioLista;
    while(aux != NULL){
        if(aux->tipo == MESA && aux->mesa->cliente != NULL){
            Cliente *cliente = aux->mesa->cliente;
            if(cliente->posicaoAtual == aux) {
                if(cliente->estado == ESPERANDO){
                    cliente->paciencia--;
                    if(cliente->paciencia <= 0){
                        pontuacao -= 10;
                        free(aux->mesa->cliente);
                        aux->mesa->cliente = NULL;
                        aux->mesa->status = SUJA;
                    }
                }
                else if(cliente->estado == COMENDO){
                    cliente->tempoComendo--;
                    if(cliente->tempoComendo <= 0){
                        pontuacao += 10;
                        free(aux->mesa->cliente);
                        aux->mesa->cliente = NULL;
                        aux->mesa->status = SUJA;
                    }
                }
            }
        }
        aux = aux->proximo;
    }
}