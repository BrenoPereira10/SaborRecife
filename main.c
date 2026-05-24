#include "restaurante.h"
#include "raylib.h"
#include <time.h>

int main(){

    srand(time(NULL));

    // ==================================================
    // INICIALIZA RAYLIB
    // ==================================================

    InitWindow(1000, 600, "Sabor Recife");

    SetTargetFPS(60);

    // ==================================================
    // INICIALIZA RESTAURANTE
    // ==================================================

    Restaurante restaurante;

    inicializarRestaurante(&restaurante);

    // mapa linear
    adicionarPosicao(&restaurante, criarCozinha());

    adicionarPosicao(&restaurante, criarNoMesa(1));

    adicionarPosicao(&restaurante, criarNoMesa(2));

    adicionarPosicao(&restaurante, criarNoMesa(3));

    adicionarPosicao(&restaurante, criarNoMesa(4));

    adicionarPosicao(&restaurante, criarNoMesa(5));

    // ==================================================
    // FILA DA COZINHA
    // ==================================================

    FilaCozinha cozinha;

    inicializarFila(&cozinha);

    // pratos iniciais
    Prato p1 = {"Tapioca"};
    Prato p2 = {"Cuscuz"};
    Prato p3 = {"Cartola"};

    enqueue(&cozinha, p1);
    enqueue(&cozinha, p2);
    enqueue(&cozinha, p3);

    // ==================================================
    // GARÇOM
    // ==================================================

    Garcom garcom;

    inicializarGarcom(&garcom, &restaurante);

    // ==================================================
    // TEMPORIZADOR
    // ==================================================

    float tempoAtualizacao = 0;

    // ==================================================
    // GAME LOOP
    // ==================================================

    while(!WindowShouldClose()){

        // ==============================================
        // INPUTS
        // ==============================================

        if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){

            irParaDireita(&garcom.posicaoAtual);
        }

        if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){

            irParaEsquerda(&garcom.posicaoAtual);
        }

        if(IsKeyPressed(KEY_SPACE)){

            interagir(garcom.posicaoAtual,
                       &garcom,
                       &cozinha);
        }

        // ==============================================
        // ATUALIZA CLIENTES A CADA 1 SEGUNDO
        // ==============================================

        tempoAtualizacao += GetFrameTime();

        if(tempoAtualizacao >= 1.0f){

            atualizarClientes(restaurante.inicio);

            tempoAtualizacao = 0;
        }

        // ==============================================
        // DESENHO
        // ==============================================

        BeginDrawing();

        ClearBackground(BEIGE);

        DrawText("SABOR RECIFE", 20, 20, 30, DARKBROWN);

        DrawText(TextFormat("Pontuacao: %d",
                 pontuacao),
                 20,
                 60,
                 25,
                 BLACK);

        // ==============================================
        // DESENHA MAPA
        // ==============================================

        NoLista *aux = restaurante.inicio;

        int x = 100;

        int y = 300;

        while(aux != NULL){

            // cozinha
            if(aux->tipo == COZINHA){

                DrawRectangle(x, y, 100, 100, GRAY);

                DrawText("COZINHA",
                         x + 5,
                         y + 35,
                         20,
                         BLACK);
            }

            // mesas
            else{

                Color corMesa = LIGHTGRAY;

                if(aux->mesa->status == OCUPADA){

                    corMesa = GREEN;
                }

                else if(aux->mesa->status == SUJA){

                    corMesa = BROWN;
                }

                DrawRectangle(x, y, 100, 100, corMesa);

                DrawText(TextFormat("Mesa %d",
                                    aux->mesa->numero),
                                    x + 10,
                                    y + 10,
                                    20,
                                    BLACK);

                // cliente
                if(aux->mesa->cliente != NULL){

                    DrawText(aux->mesa->cliente->pratoDesejado.nome,
                             x + 5,
                             y + 40,
                             15,
                             BLACK);

                    DrawText(TextFormat("Pac: %d",
                             aux->mesa->cliente->paciencia),
                             x + 5,
                             y + 65,
                             15,
                             RED);
                }
            }

            // desenha garçom
            if(aux == garcom.posicaoAtual){

                DrawCircle(x + 50,
                           y - 20,
                           15,
                           BLUE);
            }

            x += 150;

            aux = aux->proximo;
        }

        // prato carregado
        if(garcom.pratoAtual != NULL){

            DrawText(TextFormat("Carregando: %s",
                     garcom.pratoAtual->nome),
                     20,
                     100,
                     25,
                     DARKBLUE);
        }

        else{

            DrawText("Carregando: Nada",
                     20,
                     100,
                     25,
                     DARKBLUE);
        }

        DrawText("A/D ou Setas = mover",
                 20,
                 520,
                 20,
                 BLACK);

        DrawText("ESPACO = interagir",
                 20,
                 550,
                 20,
                 BLACK);

        EndDrawing();
    }

    // ==================================================
    // FECHA JANELA
    // ==================================================

    CloseWindow();

    return 0;
}