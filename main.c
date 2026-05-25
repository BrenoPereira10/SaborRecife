#include "restaurante.h"
#include "entidades.h"
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
          int totalEsperando = 0;
        Mesa *mesasOrdenadas[5];

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

            int totalEsperando = contarMesasOcupadas(restaurante.inicio);
            Mesa *mesasOrdenadas[5];
            int idx = 0;

            NoLista *percorre = restaurante.inicio;
            while (percorre != NULL) {
                if (percorre->tipo == MESA && percorre->mesa->cliente != NULL && percorre->mesa->cliente->estado == ESPERANDO) {
                    mesasOrdenadas[idx++] = percorre->mesa;
                }
                percorre = percorre->proximo;
            }

            if (totalEsperando > 1) {
                quickSortClientes(mesasOrdenadas, 0, totalEsperando - 1);
            }
            tempoAtualizacao = 0;
            
            if(pontuacao <= -30) {
                printf("FALÊNCIA! O restaurante fechou. Fim de jogo.\n");
                break; 
            }
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
        DrawRectangle(800, 120, 190, 360, Fade(DARKBROWN, 0.85f));
        DrawText("URGENCIA", 815, 130, 18, YELLOW);

        if (totalEsperando == 0) {
            DrawText("Sem pedidos", 815, 160, 16, LIGHTGRAY);
        } else {
            for (int k = 0; k < totalEsperando; k++) {
                Color cor = GREEN;
                if (mesasOrdenadas[k]->cliente->paciencia <= 3) cor = RED;
                else if (mesasOrdenadas[k]->cliente->paciencia <= 6) cor = ORANGE;

                DrawText(TextFormat("#%d Mesa %d | %s | Pac:%d",
                         k + 1,
                         mesasOrdenadas[k]->numero,
                         mesasOrdenadas[k]->cliente->pratoDesejado.nome,
                         mesasOrdenadas[k]->cliente->paciencia),
                         815, 160 + k * 45, 15, cor);
            }
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
// ==================================================
    // LIMPEZA DE MEMÓRIA (FIM DO EXPEDIENTE)
    // ==================================================
    
    // 1. Esvaziar a Fila da Cozinha
    while (cozinha.inicio != NULL) {
        dequeue(&cozinha); // A própria função dequeue já dá 'free' no nó
    }

    // 2. Limpar o mapa do Restaurante (Lista Duplamente Encadeada)
    NoLista *atual = restaurante.inicio;
    while (atual != NULL) {
        NoLista *remover = atual;
        atual = atual->proximo; // Salva o próximo antes de apagar o atual
        
        if (remover->tipo == MESA) {
            // Se tiver um cliente na mesa, limpa ele primeiro
            if (remover->mesa->cliente != NULL) {
                free(remover->mesa->cliente);
            }
            // Limpa a mesa em si
            free(remover->mesa);
        }
        // Limpa o "quadrado" (Nó) do chão
        free(remover);
    }

    // 3. Limpar o prato da mão do garçom, se ele estiver segurando algo
    if (garcom.pratoAtual != NULL) {
        free(garcom.pratoAtual);
    }
    return 0;
}