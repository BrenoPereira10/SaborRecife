#include "restaurante.h"
#include <time.h>

int pontuacao = 0;

void inicializarRestaurante(Restaurante *r){

    r->inicio = NULL;

    r->fim = NULL;
}

void inicializarFila(FilaCozinha *fila){

    fila->inicio = NULL;

    fila->fim = NULL;
}

Mesa* criarMesa(int numero){

    Mesa *novaMesa = malloc(sizeof(Mesa));

    novaMesa->numero = numero;

    novaMesa->status = VAZIA;

    novaMesa->cliente = NULL;

    return novaMesa;
}

NoLista* criarCozinha(){

    NoLista *novo = malloc(sizeof(NoLista));

    novo->tipo = COZINHA;

    novo->mesa = NULL;

    novo->anterior = NULL;

    novo->proximo = NULL;

    return novo;
}

NoLista* criarNoMesa(int numero){

    NoLista *novo = malloc(sizeof(NoLista));

    novo->tipo = MESA;

    novo->mesa = criarMesa(numero);

    novo->anterior = NULL;

    novo->proximo = NULL;

    return novo;
}

void adicionarPosicao(Restaurante *r, NoLista *novo){

    if(r->inicio == NULL){

        r->inicio = novo;

        r->fim = novo;
    }

    else{

        novo->anterior = r->fim;

        r->fim->proximo = novo;

        r->fim = novo;
    }
}

void inicializarGarcom(Garcom *g, Restaurante *r){

    g->posicaoAtual = r->inicio;

    g->pratoAtual = NULL;
}

void irParaDireita(NoLista **posicaoAtual){

    if((*posicaoAtual)->proximo != NULL){

        *posicaoAtual = (*posicaoAtual)->proximo;
    }

    else{

        printf("Fim do restaurante!\n");
    }
}

void irParaEsquerda(NoLista **posicaoAtual){

    if((*posicaoAtual)->anterior != NULL){

        *posicaoAtual = (*posicaoAtual)->anterior;
    }

    else{

        printf("Você já está na cozinha!\n");
    }
}

void enqueue(FilaCozinha *fila, Prato prato){

    NoFila *novo = malloc(sizeof(NoFila));

    novo->prato = prato;

    novo->proximo = NULL;

    if(fila->inicio == NULL){

        fila->inicio = novo;

        fila->fim = novo;
    }

    else{

        fila->fim->proximo = novo;

        fila->fim = novo;
    }
}

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

void interagir(NoLista *posicaoAtual,
               Garcom *garcom,
               FilaCozinha *cozinha){

    if(posicaoAtual->tipo == COZINHA){

        if(garcom->pratoAtual != NULL){

            printf("Você já está carregando um prato!\n");

            return;
        }

        Prato pratoPegado = dequeue(cozinha);

        if(strcmp(pratoPegado.nome, "Vazio") != 0){

            garcom->pratoAtual = malloc(sizeof(Prato));

            *(garcom->pratoAtual) = pratoPegado;

            printf("Garçom pegou: %s\n",
                   garcom->pratoAtual->nome);
        }

        return;
    }

    if(posicaoAtual->tipo == MESA){

        Mesa *mesa = posicaoAtual->mesa;

        if(mesa->status == SUJA){

            mesa->status = VAZIA;

            printf("Mesa %d foi limpa!\n",
                   mesa->numero);

            return;
        }

        if(mesa->status == OCUPADA &&
           mesa->cliente != NULL &&
           mesa->cliente->estado == ESPERANDO){

            if(garcom->pratoAtual == NULL){

                printf("Você não está carregando prato!\n");

                return;
            }

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

Cliente* criarCliente(){

    Cliente *novo = malloc(sizeof(Cliente));

    novo->estado = ESPERANDO;

    novo->paciencia = 5 + rand() % 11;

    novo->tempoComendo = 5 + rand() % 6;

    char cardapio[5][50] = {

        "Tapioca",
        "Bolo de Rolo",
        "Cuscuz",
        "Cartola",
        "Caldinho"
    };

    int indice = rand() % 5;

    strcpy(novo->pratoDesejado.nome,
           cardapio[indice]);

    return novo;
}

void atualizarClientes(NoLista *inicioLista){

    NoLista *aux = inicioLista;

    int chanceSpawn = rand() % 100;

    if(chanceSpawn < 30){

        int mesasVazias = 0;

        aux = inicioLista;

        while(aux != NULL){

            if(aux->tipo == MESA &&
               aux->mesa->status == VAZIA){

                mesasVazias++;
            }

            aux = aux->proximo;
        }

        if(mesasVazias > 0){

            int alvo = rand() % mesasVazias;

            int contador = 0;

            aux = inicioLista;

            while(aux != NULL){

                if(aux->tipo == MESA &&
                   aux->mesa->status == VAZIA){

                    if(contador == alvo){

                        aux->mesa->cliente = criarCliente();

                        aux->mesa->status = OCUPADA;

                        printf("\nNovo cliente chegou na Mesa %d\n",
                               aux->mesa->numero);

                        printf("Pedido: %s\n",
                               aux->mesa->cliente->pratoDesejado.nome);

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

        if(aux->tipo == MESA &&
           aux->mesa->cliente != NULL){

            Cliente *cliente = aux->mesa->cliente;

            if(cliente->estado == ESPERANDO){

                cliente->paciencia--;

                printf("Mesa %d esperando... (%d)\n",
                       aux->mesa->numero,
                       cliente->paciencia);

                if(cliente->paciencia <= 0){

                    printf("Cliente da Mesa %d foi embora!\n",
                           aux->mesa->numero);

                    free(aux->mesa->cliente);

                    aux->mesa->cliente = NULL;

                    aux->mesa->status = SUJA;
                }
            }

            else if(cliente->estado == COMENDO){

                cliente->tempoComendo--;

                printf("Mesa %d comendo... (%d)\n",
                       aux->mesa->numero,
                       cliente->tempoComendo);

                if(cliente->tempoComendo <= 0){

                    printf("Cliente da Mesa %d terminou de comer!\n",
                           aux->mesa->numero);

                    pontuacao += 10;

                    printf("Pontuação: %d\n",
                           pontuacao);

                    free(aux->mesa->cliente);

                    aux->mesa->cliente = NULL;

                    aux->mesa->status = SUJA;
                }
            }
        }
        aux = aux->proximo;
    }
}
