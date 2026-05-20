#include "restaurante.h"

int main(){

    Restaurante restaurante;

    inicializarRestaurante(&restaurante);


    FilaCozinha cozinha;

    inicializarFila(&cozinha);


    adicionarPosicao(&restaurante, criarCozinha());

    adicionarPosicao(&restaurante, criarNoMesa(1));



    Prato prato1;

    strcpy(prato1.nome, "Tapioca");


    enqueue(&cozinha, prato1);



    Cliente *cliente = malloc(sizeof(Cliente));

    cliente->estado = ESPERANDO;

    cliente->paciencia = 20;

    strcpy(cliente->pratoDesejado.nome,
           "Tapioca");



    restaurante.inicio->proximo->mesa->cliente = cliente;

    restaurante.inicio->proximo->mesa->status = OCUPADA;



    Garcom garcom;

    inicializarGarcom(&garcom,
                      &restaurante);



    interagir(garcom.posicaoAtual,
              &garcom,
              &cozinha);



    irParaDireita(&garcom.posicaoAtual);



    interagir(garcom.posicaoAtual,
              &garcom,
              &cozinha);


    return 0;
}
