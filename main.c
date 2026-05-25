#include "restaurante.h"
#include "entidades.h"
#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

// Definição dos estados do jogo
typedef enum {
    ESTADO_MENU,
    ESTADO_JOGANDO,
    ESTADO_FALENCIA
} EstadoJogo;

int main(){

    srand(time(NULL));

    // ==================================================
    // INICIALIZA RAYLIB
    // ==================================================
    InitWindow(1000, 600, "Sabor Recife");
    SetTargetFPS(60);

    // Carrega a imagem da tela inicial
    Texture2D fundoMenu = LoadTexture("imagens/Telainicio.png");

    // Definição dos botões invisíveis em cima da sua imagem (ajuste se necessário)
    Rectangle btnJogar = { 400, 220, 220, 65 };
    
    EstadoJogo estadoAtual = ESTADO_MENU;

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
    // SISTEMA DA COZINHA (CARDÁPIO)
    // ==================================================
    // A fila antiga foi mantida na inicialização para não quebrar a assinatura da função interagir
    FilaCozinha cozinha;
    inicializarFila(&cozinha);

    // Lista de pratos disponíveis na Cozinha
    char cardapio[5][50] = { "Tapioca", "Bolo de Rolo", "Cuscuz", "Cartola", "Caldinho" };
    bool escolhendoPrato = false; 

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
        Vector2 mousePos = GetMousePosition();

        // ==============================================
        // LÓGICA POR ESTADO
        // ==============================================
        if(estadoAtual == ESTADO_MENU) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(CheckCollisionPointRec(mousePos, btnJogar)){
                    estadoAtual = ESTADO_JOGANDO;
                }
            }
        }
        else if (estadoAtual == ESTADO_JOGANDO) {

            // ==============================================
            // INPUTS
            // ==============================================
            // Descartar prato na lixeira
            if (IsKeyPressed(KEY_Q) && garcom.pratoAtual != NULL) {
                free(garcom.pratoAtual);
                garcom.pratoAtual = NULL;
                printf("Prato descartado na lixeira!\n");
            }

            if (escolhendoPrato) {
                int pratoEscolhido = -1;
                if (IsKeyPressed(KEY_ONE)) pratoEscolhido = 0;
                if (IsKeyPressed(KEY_TWO)) pratoEscolhido = 1;
                if (IsKeyPressed(KEY_THREE)) pratoEscolhido = 2;
                if (IsKeyPressed(KEY_FOUR)) pratoEscolhido = 3;
                if (IsKeyPressed(KEY_FIVE)) pratoEscolhido = 4;

                if (pratoEscolhido != -1) {
                    garcom.pratoAtual = malloc(sizeof(Prato));
                    strcpy(garcom.pratoAtual->nome, cardapio[pratoEscolhido]);
                    printf("Garçom preparou e pegou: %s\n", garcom.pratoAtual->nome);
                    escolhendoPrato = false;
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    escolhendoPrato = false; 
                }
            }
            else {
                if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
                    irParaDireita(&garcom.posicaoAtual);
                }

                if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
                    irParaEsquerda(&garcom.posicaoAtual);
                }

                if(IsKeyPressed(KEY_SPACE)){
                    if (garcom.posicaoAtual->tipo == COZINHA) {
                        if (garcom.pratoAtual != NULL) {
                            printf("Você já está carregando um prato! Use 'Q' para descartar.\n");
                        } else {
                            escolhendoPrato = true;
                        }
                    } else {
                        interagir(garcom.posicaoAtual, &garcom, &cozinha);
                    }
                }
            }

            // ==============================================
            // ATUALIZA CLIENTES A CADA 1 SEGUNDO
            // ==============================================
            tempoAtualizacao += GetFrameTime();

            if(tempoAtualizacao >= 1.0f){
                atualizarClientes(restaurante.inicio);

                totalEsperando = contarMesasOcupadas(restaurante.inicio);
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
                exibirFilaDeEspera(mesasOrdenadas, totalEsperando);
                tempoAtualizacao = 0;
                
                if(pontuacao <= -30) {
                    printf("FALÊNCIA! O restaurante fechou. Fim de jogo.\n");
                    estadoAtual = ESTADO_FALENCIA; 
                }
            }
        }

        // ==============================================
        // DESENHO
        // ==============================================
        BeginDrawing();
        ClearBackground(BEIGE);

        if (estadoAtual == ESTADO_MENU) {
            DrawTexture(fundoMenu, 0, 0, WHITE);
            // Feedback visual no botão JOGAR
            if(CheckCollisionPointRec(mousePos, btnJogar)) {
                DrawRectangleLinesEx(btnJogar, 3, GREEN);
            }
        }
        else if (estadoAtual == ESTADO_FALENCIA) {
            DrawRectangle(0, 0, 1000, 600, RED);
            DrawText("FALÊNCIA!", 350, 220, 60, WHITE);
            DrawText("O restaurante fechou as portas.", 320, 300, 24, WHITE);
            DrawText("Pressione ESC para sair.", 370, 400, 20, LIGHTGRAY);
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            DrawText("SABOR RECIFE", 20, 20, 30, DARKBROWN);

            DrawText(TextFormat("Pontuacao: %d", pontuacao), 20, 60, 25, BLACK);

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
                    DrawText("COZINHA", x + 5, y + 35, 20, BLACK);
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

                    DrawText(TextFormat("Mesa %d", aux->mesa->numero), x + 10, y + 10, 20, BLACK);

                    // cliente
                    if(aux->mesa->cliente != NULL){
                        DrawText(aux->mesa->cliente->pratoDesejado.nome, x + 5, y + 40, 15, BLACK);
                        DrawText(TextFormat("Pac: %d", aux->mesa->cliente->paciencia), x + 5, y + 65, 15, RED);
                    }
                }

                // desenha garçom
                if(aux == garcom.posicaoAtual){
                    DrawCircle(x + 50, y - 20, 15, BLUE);
                }

                x += 150;
                aux = aux->proximo;
            }

            // prato carregado
            if(garcom.pratoAtual != NULL){
                DrawText(TextFormat("Carregando: %s", garcom.pratoAtual->nome), 20, 100, 25, DARKBLUE);
            } else{
                DrawText("Carregando: Nada", 20, 100, 25, DARKBLUE);
            }

            // Quadro de Urgência
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

            // Overlay Menu Cozinha
            if (escolhendoPrato) {
                DrawRectangle(250, 120, 500, 350, Fade(BLACK, 0.9f));
                DrawRectangleLines(250, 120, 500, 350, WHITE);
                DrawText("--- COZINHA: PREPARAR PRATO ---", 300, 140, 22, YELLOW);
                
                for (int i = 0; i < 5; i++) {
                    DrawText(TextFormat("[%d] - %s", i + 1, cardapio[i]), 350, 200 + (i * 40), 20, WHITE);
                }
                DrawText("Pressione ESC para cancelar", 360, 420, 16, RED);
            }

            DrawText("A/D ou Setas = mover", 20, 500, 20, BLACK);
            DrawText("ESPACO = interagir (Cozinha/Mesas)", 20, 530, 20, BLACK);
            DrawText("Q = Descartar Prato", 20, 560, 20, DARKGRAY);
        }

        EndDrawing();
    }

    // ==================================================
    // FECHA JANELA E LIMPEZA DE MEMÓRIA (FIM DO EXPEDIENTE)
    // ==================================================
    UnloadTexture(fundoMenu);
    CloseWindow();
    
    // 1. Esvaziar a Fila da Cozinha
    while (cozinha.inicio != NULL) {
        dequeue(&cozinha); 
    }

    // 2. Limpar o mapa do Restaurante
    NoLista *atual = restaurante.inicio;
    while (atual != NULL) {
        NoLista *remover = atual;
        atual = atual->proximo; 
        
        if (remover->tipo == MESA) {
            if (remover->mesa->cliente != NULL) {
                free(remover->mesa->cliente);
            }
            free(remover->mesa);
        }
        free(remover);
    }

    // 3. Limpar o prato da mão do garçom
    if (garcom.pratoAtual != NULL) {
        free(garcom.pratoAtual);
    }
    
    return 0;
}