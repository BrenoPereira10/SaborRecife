#include <stdio.h>
#include "restaurante.h"


int main(){
    Lista restaurante;
    inicializarLista(&restaurante);

    // ADICIONANDO POSIÇÕES NO RESTAURANTE
    // A lista ficará:
    // Entrada <-> Mesa1 <-> Mesa2
    // <-> Cozinha <-> Mesa3
    adicionarPosicao(&restaurante, "Entrada");

    adicionarPosicao(&restaurante, "Mesa 1");

    adicionarPosicao(&restaurante, "Mesa 2");

    adicionarPosicao(&restaurante, "Cozinha");

    adicionarPosicao(&restaurante, "Mesa 3");


    // ======================================
    // MOSTRAR O MAPA
    // ======================================

    printf("MAPA DO RESTAURANTE:\n\n");


    // chama a função que percorre
    // e imprime a lista
    mostrarRestaurante(&restaurante);


    return 0;
}