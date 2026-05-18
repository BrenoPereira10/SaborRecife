#include "restaurante.h"

int main(){

    Restaurante restaurante;

    inicializarRestaurante(&restaurante);


    FilaCozinha cozinha;

    inicializarFila(&cozinha);


    // mapa
    adicionarPosicao(&restaurante, criarCozinha());

    adicionarPosicao(&restaurante, criarNoMesa(1));



    // cria prato
    Prato prato1;

    strcpy(prato1.nome, "Tapioca");


    // adiciona prato na fila
    enqueue(&cozinha, prato1);



    // cria cliente
    Cliente *cliente = malloc(sizeof(Cliente));

    cliente->estado = ESPERANDO;

    cliente->paciencia = 20;

    strcpy(cliente->pratoDesejado.nome,
           "Tapioca");



    // coloca cliente na mesa
    restaurante.inicio->proximo->mesa->cliente = cliente;

    restaurante.inicio->proximo->mesa->status = OCUPADA;



    // cria garçom
    Garcom garcom;

    inicializarGarcom(&garcom,
                      &restaurante);



    // pega prato na cozinha
    interagir(garcom.posicaoAtual,
              &garcom,
              &cozinha);



    // vai para direita
    irParaDireita(&garcom.posicaoAtual);



    // entrega pedido
    interagir(garcom.posicaoAtual,
              &garcom,
              &cozinha);


    return 0;
}