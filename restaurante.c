#include "restaurante.h"
#include <time.h>

// variável global de pontuação
int pontuacao = 0;

// ======================================================
// INICIALIZA RESTAURANTE
// ======================================================

void inicializarRestaurante(Restaurante *r){

    r->inicio = NULL;

    r->fim = NULL;
}



// ======================================================
// INICIALIZA FILA
// ======================================================

void inicializarFila(FilaCozinha *fila){

    fila->inicio = NULL;

    fila->fim = NULL;
}



// ======================================================
// CRIA MESA
// ======================================================

Mesa* criarMesa(int numero){

    Mesa *novaMesa = malloc(sizeof(Mesa));

    novaMesa->numero = numero;

    novaMesa->status = VAZIA;

    novaMesa->cliente = NULL;

    return novaMesa;
}



// ======================================================
// CRIA COZINHA
// ======================================================

NoLista* criarCozinha(){

    NoLista *novo = malloc(sizeof(NoLista));

    novo->tipo = COZINHA;

    novo->mesa = NULL;

    novo->anterior = NULL;

    novo->proximo = NULL;

    return novo;
}



// ======================================================
// CRIA NÓ DE MESA
// ======================================================

NoLista* criarNoMesa(int numero){

    NoLista *novo = malloc(sizeof(NoLista));

    novo->tipo = MESA;

    novo->mesa = criarMesa(numero);

    novo->anterior = NULL;

    novo->proximo = NULL;

    return novo;
}



// ======================================================
// ADICIONA POSIÇÃO NA LISTA
// ======================================================

void adicionarPosicao(Restaurante *r, NoLista *novo){

    // lista vazia
    if(r->inicio == NULL){

        r->inicio = novo;

        r->fim = novo;
    }

    // lista já possui elementos
    else{

        novo->anterior = r->fim;

        r->fim->proximo = novo;

        r->fim = novo;
    }
}



// ======================================================
// INICIALIZA GARÇOM
// ======================================================

void inicializarGarcom(Garcom *g, Restaurante *r){

    g->posicaoAtual = r->inicio;

    // começa sem prato
    g->pratoAtual = NULL;
}



// ======================================================
// IR PARA DIREITA
// ======================================================

void irParaDireita(NoLista **posicaoAtual){

    if((*posicaoAtual)->proximo != NULL){

        *posicaoAtual = (*posicaoAtual)->proximo;

        printf("Garçom foi para a direita.\n");
    }

    else{

        printf("Fim do restaurante!\n");
    }
}



// ======================================================
// IR PARA ESQUERDA
// ======================================================

void irParaEsquerda(NoLista **posicaoAtual){

    if((*posicaoAtual)->anterior != NULL){

        *posicaoAtual = (*posicaoAtual)->anterior;

        printf("Garçom foi para a esquerda.\n");
    }

    else{

        printf("Você já está na cozinha!\n");
    }
}



// ======================================================
// ENQUEUE
// ======================================================

void enqueue(FilaCozinha *fila, Prato prato){

    NoFila *novo = malloc(sizeof(NoFila));

    novo->prato = prato;

    novo->proximo = NULL;


    // fila vazia
    if(fila->inicio == NULL){

        fila->inicio = novo;

        fila->fim = novo;
    }

    // fila com elementos
    else{

        fila->fim->proximo = novo;

        fila->fim = novo;
    }
}



// ======================================================
// DEQUEUE
// ======================================================

Prato dequeue(FilaCozinha *fila){

    Prato pratoVazio;

    strcpy(pratoVazio.nome, "Vazio");


    // fila vazia
    if(fila->inicio == NULL){

        printf("Nenhum prato pronto na cozinha!\n");

        return pratoVazio;
    }


    NoFila *remover = fila->inicio;

    Prato prato = remover->prato;


    fila->inicio = fila->inicio->proximo;


    // fila ficou vazia
    if(fila->inicio == NULL){

        fila->fim = NULL;
    }


    free(remover);

    return prato;
}



// ======================================================
// INTERAÇÃO PRINCIPAL
// ======================================================

void interagir(NoLista *posicaoAtual,
               Garcom *garcom,
               FilaCozinha *cozinha){

    // ==================================================
    // COZINHA
    // ==================================================

    if(posicaoAtual->tipo == COZINHA){

        // já está carregando prato
        if(garcom->pratoAtual != NULL){

            printf("Você já está carregando um prato!\n");

            return;
        }


        // pega prato da fila
        Prato pratoPegado = dequeue(cozinha);


        // verifica se veio prato válido
        if(strcmp(pratoPegado.nome, "Vazio") != 0){

            garcom->pratoAtual = malloc(sizeof(Prato));

            *(garcom->pratoAtual) = pratoPegado;

            printf("Garçom pegou: %s\n",
                   garcom->pratoAtual->nome);
        }

        return;
    }



    // ==================================================
    // MESA
    // ==================================================

    if(posicaoAtual->tipo == MESA){

        Mesa *mesa = posicaoAtual->mesa;


        // ==================================================
        // LIMPAR MESA
        // ==================================================

        if(mesa->status == SUJA){

            mesa->status = VAZIA;

            printf("Mesa %d foi limpa!\n",
                   mesa->numero);

            return;
        }


        // ==================================================
        // ENTREGAR PEDIDO
        // ==================================================

        if(mesa->status == OCUPADA &&
           mesa->cliente != NULL &&
           mesa->cliente->estado == ESPERANDO){

            // garçom sem prato
            if(garcom->pratoAtual == NULL){

                printf("Você não está carregando prato!\n");

                return;
            }


            // prato correto
            if(strcmp(garcom->pratoAtual->nome,
                      mesa->cliente->pratoDesejado.nome) == 0){

                printf("Pedido entregue corretamente!\n");


                mesa->cliente->estado = COMENDO;


                free(garcom->pratoAtual);

                garcom->pratoAtual = NULL;
            }

            // prato errado
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
// ======================================================
// CRIA CLIENTE
// ======================================================

Cliente* criarCliente(){

    Cliente *novo = malloc(sizeof(Cliente));


    // cliente começa esperando
    novo->estado = ESPERANDO;


    // tempo aleatório de paciência
    novo->paciencia = 5 + rand() % 11;


    // tempo para comer
    novo->tempoComendo = 5 + rand() % 6;


    // cardápio recifense
    char cardapio[5][50] = {

        "Tapioca",
        "Bolo de Rolo",
        "Cuscuz",
        "Cartola",
        "Caldinho"
    };


    // escolhe prato aleatório
    int indice = rand() % 5;

    strcpy(novo->pratoDesejado.nome,
           cardapio[indice]);


    return novo;
}



// ======================================================
// ATUALIZA CLIENTES
// ======================================================
// Essa função representa um "tick" do jogo.
//
// Ela:
// - gera clientes
// - reduz paciência
// - processa clientes comendo
// - gera pontuação
//
// ======================================================

void atualizarClientes(NoLista *inicioLista){

    NoLista *aux = inicioLista;


    // ==================================================
    // SPAWN DE CLIENTE
    // ==================================================
    // 30% de chance de gerar cliente
    // ==================================================

    int chanceSpawn = rand() % 100;


    if(chanceSpawn < 30){

        // conta mesas vazias
        int mesasVazias = 0;


        aux = inicioLista;

        while(aux != NULL){

            if(aux->tipo == MESA &&
               aux->mesa->status == VAZIA){

                mesasVazias++;
            }

            aux = aux->proximo;
        }


        // existe mesa disponível
        if(mesasVazias > 0){

            int alvo = rand() % mesasVazias;

            int contador = 0;

            aux = inicioLista;


            while(aux != NULL){

                if(aux->tipo == MESA &&
                   aux->mesa->status == VAZIA){

                    if(contador == alvo){

                        // cria cliente
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



    // ==================================================
    // PERCORRE TODAS AS MESAS
    // ==================================================

    aux = inicioLista;


    while(aux != NULL){

        if(aux->tipo == MESA &&
           aux->mesa->cliente != NULL){

            Cliente *cliente = aux->mesa->cliente;


            // ==========================================
            // CLIENTE ESPERANDO
            // ==========================================

            if(cliente->estado == ESPERANDO){

                cliente->paciencia--;


                printf("Mesa %d esperando... (%d)\n",
                       aux->mesa->numero,
                       cliente->paciencia);


                // cliente perdeu paciência
                if(cliente->paciencia <= 0){

                    printf("Cliente da Mesa %d foi embora!\n",
                           aux->mesa->numero);


                    free(aux->mesa->cliente);

                    aux->mesa->cliente = NULL;

                    aux->mesa->status = SUJA;
                }
            }


            // ==========================================
            // CLIENTE COMENDO
            // ==========================================

            else if(cliente->estado == COMENDO){

                cliente->tempoComendo--;


                printf("Mesa %d comendo... (%d)\n",
                       aux->mesa->numero,
                       cliente->tempoComendo);


                // terminou de comer
                if(cliente->tempoComendo <= 0){

                    printf("Cliente da Mesa %d terminou de comer!\n",
                           aux->mesa->numero);


                    // aumenta pontuação
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