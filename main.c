#include "restaurante.h"


int main(){

    Restaurante restaurante;

    inicializarRestaurante(&restaurante);

 
     FilaCozinha fila;

    inicializarFila(&fila);

    // ==================================================
    // CRIAÇÃO DO MAPA
    // Cozinha <-> Mesa1 <-> Mesa2 <-> Mesa3
    adicionarPosicao(&restaurante, criarCozinha());

    adicionarPosicao(&restaurante, criarNoMesa(1));

    adicionarPosicao(&restaurante, criarNoMesa(2));

    adicionarPosicao(&restaurante, criarNoMesa(3));

    Garcom garcom;

    inicializarGarcom(&garcom, &restaurante);

    printf("Estruturas inicializadas com sucesso!\n");


    return 0;
}