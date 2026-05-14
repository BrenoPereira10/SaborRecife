#ifndef RESTAURANTE_H
#define RESTAURANTE_H
typedef struct No{

    char nome[30];

    struct No *proximo;

    struct No *anterior;
}No;

typedef struct{
    No *inicio;

    No *fim;

}Lista;

// inicializa a lista vazia
void inicializarLista(Lista *lista);


// adiciona uma nova posição no restaurante
void adicionarPosicao(Lista *lista, char nome[]);


// mostra todas as posições do restaurante
void mostrarRestaurante(Lista *lista);


#endif