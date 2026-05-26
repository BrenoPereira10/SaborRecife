#include "restaurante.h"

int pontuacao = 0;

void inicializarRestaurante(Restaurante *r){
    r->inicio = NULL;
    r->fim = NULL;
}

Mesa* criarMesa(int numero){
    Mesa *novaMesa = malloc(sizeof(Mesa));
    novaMesa->numero = numero;
    novaMesa->status = VAZIA;
    novaMesa->cliente = NULL;
    return novaMesa;
}

NoLista* criarCozinha(int x, int y){
    NoLista *novo = malloc(sizeof(NoLista));
    novo->tipo = COZINHA;
    novo->mesa = NULL;
    novo->posX = x;
    novo->posY = y;
    novo->anterior = NULL;
    novo->proximo = NULL;
    return novo;
}

NoLista* criarNoMesa(int numero, int x, int y){
    NoLista *novo = malloc(sizeof(NoLista));
    novo->tipo = MESA;
    novo->mesa = criarMesa(numero);
    novo->posX = x;
    novo->posY = y;
    novo->anterior = NULL;
    novo->proximo = NULL;
    return novo;
}

NoLista* criarCaminho(int x, int y){
    NoLista *novo = malloc(sizeof(NoLista));
    novo->tipo = CAMINHO;
    novo->mesa = NULL;
    novo->posX = x;
    novo->posY = y;
    novo->anterior = NULL;
    novo->proximo = NULL;
    return novo;
}

void adicionarPosicao(Restaurante *r, NoLista *novo){
    if(r->inicio == NULL){
        r->inicio = novo;
        r->fim = novo;
    } else {
        novo->anterior = r->fim;
        r->fim->proximo = novo;
        r->fim = novo;
    }
}

void inicializarGarcom(Garcom *g, Restaurante *r){
    g->posicaoAtual = r->inicio; 
    g->pratoAtual = NULL;
    g->timerTransicao = 0.0f;
    g->direcao = 0;
}

void irParaDireita(Garcom *g){
    if (g->timerTransicao > 0) return; 
    if(g->posicaoAtual->proximo != NULL){
        g->direcao = 1;
        g->posicaoAtual = g->posicaoAtual->proximo;
        if(g->posicaoAtual->tipo == CAMINHO) g->timerTransicao = 0.15f; 
    }
}

void irParaEsquerda(Garcom *g){
    if (g->timerTransicao > 0) return;
    if(g->posicaoAtual->anterior != NULL){
        g->direcao = -1;
        g->posicaoAtual = g->posicaoAtual->anterior;
        if(g->posicaoAtual->tipo == CAMINHO) g->timerTransicao = 0.15f;
    }
}

void atualizarMovimentoGarcom(Garcom *g, float dt) {
    if (g->timerTransicao > 0) {
        g->timerTransicao -= dt;
        if (g->timerTransicao <= 0) {
            g->timerTransicao = 0;
            if (g->direcao == 1 && g->posicaoAtual->proximo != NULL) {
                g->posicaoAtual = g->posicaoAtual->proximo;
            } else if (g->direcao == -1 && g->posicaoAtual->anterior != NULL) {
                g->posicaoAtual = g->posicaoAtual->anterior;
            }
            g->direcao = 0;
        }
    }
}

void interagir(NoLista *posicaoAtual, Garcom *garcom, FilaCozinha *cozinha){
    if (garcom->timerTransicao > 0) return; 
    if(posicaoAtual->tipo == COZINHA) return;
    
    if(posicaoAtual->tipo == MESA){
        Mesa *mesa = posicaoAtual->mesa;

        if(mesa->status == SUJA){
            mesa->status = VAZIA;
            return;
        }

        if(mesa->status == OCUPADA && mesa->cliente != NULL && mesa->cliente->estado == ESPERANDO){
            if(garcom->pratoAtual == NULL) return;

            if(strcmp(garcom->pratoAtual->nome, mesa->cliente->pratoDesejado.nome) == 0){
                mesa->cliente->estado = COMENDO;
                free(garcom->pratoAtual);
                garcom->pratoAtual = NULL;
            }
        }
    }
}

int contarMesasOcupadas(NoLista *inicio) {
    int count = 0;
    NoLista *aux = inicio;
    while (aux != NULL) {
        if (aux->tipo == MESA && aux->mesa->cliente != NULL && aux->mesa->cliente->estado == ESPERANDO) {
            if (aux->mesa->cliente->posicaoAtual == aux) count++;
        }
        aux = aux->proximo;
    }
    return count;
}

void quickSortClientes(Mesa **mesas, int inicio, int fim) {
    if (inicio >= fim) return;
    Mesa *pivo = mesas[fim];
    int i = inicio - 1;
    for (int j = inicio; j < fim; j++) {
        if (mesas[j]->cliente->paciencia < pivo->cliente->paciencia) {
            i++;
            Mesa *temp = mesas[i];
            mesas[i] = mesas[j];
            mesas[j] = temp;
        }
    }
    Mesa *temp = mesas[i + 1];
    mesas[i + 1] = mesas[fim];
    mesas[fim] = temp;
    int posicaoPivo = i + 1;
    quickSortClientes(mesas, inicio, posicaoPivo - 1);
    quickSortClientes(mesas, posicaoPivo + 1, fim);
}
void exibirFilaDeEspera(Mesa **mesas, int total) {
    printf("\n=== FILA DE ESPERA (mais urgente primeiro) ===\n");
    if (total == 0) {
        printf("Nenhum cliente esperando.\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf("#%d | Mesa %d | Pedido: %s | Paciencia: %d\n",
               i + 1,
               mesas[i]->numero,
               mesas[i]->cliente->pratoDesejado.nome,
               mesas[i]->cliente->paciencia);
    }
    printf("=============================================\n");
}