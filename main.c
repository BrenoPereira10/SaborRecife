#include "restaurante.h"
#include "entidades.h"
#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

typedef enum { ESTADO_MENU, ESTADO_JOGANDO, ESTADO_FALENCIA } EstadoJogo;

// Altura padrão desejada na tela para alinhar com o mapa
const float ALTURA_MESA = 90.0f; // Escala visual da mesa
const float ALTURA_PERSONAGEM = 85.0f; // Altura dos personagens

int main(){
    srand(time(NULL));
    // Mapa original é proporcional a 1000x600, mantemos a janela fixa
    InitWindow(1000, 600, "Sabor Recife - Versao Corrigida 2.0");
    SetTargetFPS(60);

    // ==============================================
    // CARREGAMENTO DE TEXTURAS (CAMINHOS IMAGENS/)
    // ==============================================
    
    // MENU RESTAURADO - Trocado de JPG para PNG para evitar falha silenciosa de carga!
    // VOCÊ PRECISA CONVERTER Telainicio.jpg para Telainicio.png do seu lado.
    Texture2D fundoMenu = LoadTexture("imagens/Telainicio.png"); 
    Rectangle btnJogar = { 390, 240, 220, 65 }; // Mantido da lógica original

    // JOGO
    Texture2D mapa = LoadTexture("imagens/mapa.png"); // Fundo axadrezado
    Texture2D mesaLimpa = LoadTexture("imagens/mesa.png");
    Texture2D mesaSuja = LoadTexture("imagens/mesasuja.png");
    
    Texture2D prot_p = LoadTexture("imagens/protagonistap.png");
    Texture2D prot_a = LoadTexture("imagens/protagonistaa.png");
    Texture2D prot_pp = LoadTexture("imagens/protagonistapp.png");
    Texture2D prot_ap = LoadTexture("imagens/protagonistaap.png");

    Texture2D clientesWalk[3] = { LoadTexture("imagens/cliente1a.png"), LoadTexture("imagens/cliente2a.png"), LoadTexture("imagens/cliente3a.png") };
    Texture2D clientesIdle[3] = { LoadTexture("imagens/cliente1p.png"), LoadTexture("imagens/cliente2p.png"), LoadTexture("imagens/cliente3p.png") };
    Texture2D clientesSit[3]  = { LoadTexture("imagens/cliente1s.png"), LoadTexture("imagens/cliente2s.png"), LoadTexture("imagens/cliente3s.png") };

    EstadoJogo estadoAtual = ESTADO_MENU;

    Restaurante restaurante;
    inicializarRestaurante(&restaurante);

    // ==============================================
    // NOVA ESTRUTURA MAPA (Apenas 3 Mesas, Y=470 no chão)
    // ==============================================
    // Espaçamento X agressivo para preencher a largura: Mesas em 120, 300, 480.
    adicionarPosicao(&restaurante, criarNoMesa(1, 120, 470));
    adicionarPosicao(&restaurante, criarCaminho(210, 470));
    adicionarPosicao(&restaurante, criarNoMesa(2, 300, 470));
    adicionarPosicao(&restaurante, criarCaminho(390, 470));
    adicionarPosicao(&restaurante, criarNoMesa(3, 480, 470));
    adicionarPosicao(&restaurante, criarCaminho(680, 470)); // Grande gap para a cozinha
    adicionarPosicao(&restaurante, criarCozinha(880, 470));

    FilaCozinha cozinha;
    inicializarFila(&cozinha);
    char cardapio[5][50] = { "Tapioca", "Bolo de Rolo", "Cuscuz", "Cartola", "Caldinho" };
    bool escolhendoPrato = false; 

    Garcom garcom;
    inicializarGarcom(&garcom, &restaurante);

    float tempoAtualizacao = 0;
    
    Mesa *mesasOrdenadas[3]; // Apenas 3 mesas agora
    int totalEsperando = 0;

    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        if(estadoAtual == ESTADO_MENU) {
            // Lógica do botão jogar restaurada
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(CheckCollisionPointRec(mousePos, btnJogar)){
                    estadoAtual = ESTADO_JOGANDO;
                }
            }
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            
            atualizarMovimentoGarcom(&garcom, dt);
            atualizarFisicaClientes(restaurante.inicio, dt);

            // INPUTS
            if (IsKeyPressed(KEY_Q) && garcom.pratoAtual != NULL) {
                free(garcom.pratoAtual);
                garcom.pratoAtual = NULL;
            }

            if (escolhendoPrato) {
                int p = -1;
                if (IsKeyPressed(KEY_ONE)) p = 0;
                if (IsKeyPressed(KEY_TWO)) p = 1;
                if (IsKeyPressed(KEY_THREE)) p = 2;
                if (IsKeyPressed(KEY_FOUR)) p = 3;
                if (IsKeyPressed(KEY_FIVE)) p = 4;

                if (p != -1) {
                    garcom.pratoAtual = malloc(sizeof(Prato));
                    strcpy(garcom.pratoAtual->nome, cardapio[p]);
                    escolhendoPrato = false;
                }
                if (IsKeyPressed(KEY_ESCAPE)) escolhendoPrato = false; 
            }
            else {
                if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) irParaDireita(&garcom);
                if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) irParaEsquerda(&garcom);
                if(IsKeyPressed(KEY_SPACE)){
                    if (garcom.posicaoAtual->tipo == COZINHA) {
                        if (garcom.pratoAtual == NULL) escolhendoPrato = true;
                    } else {
                        interagir(garcom.posicaoAtual, &garcom, &cozinha);
                    }
                }
            }

            // ATUALIZA LÓGICA TEMPORAL E URGÊNCIA (1 segundo)
            tempoAtualizacao += dt;
            if(tempoAtualizacao >= 1.0f){
                atualizarClientes(restaurante.inicio);
                
                // LÓGICA DE URGÊNCIA RESTAURADA
                totalEsperando = 0;
                NoLista *percorre = restaurante.inicio;
                while (percorre != NULL) {
                    if (percorre->tipo == MESA && percorre->mesa->cliente != NULL && 
                        percorre->mesa->cliente->estado == ESPERANDO &&
                        percorre->mesa->cliente->posicaoAtual == percorre) {
                        mesasOrdenadas[totalEsperando++] = percorre->mesa;
                    }
                    percorre = percorre->proximo;
                }
                if (totalEsperando > 1) quickSortClientes(mesasOrdenadas, 0, totalEsperando - 1);

                tempoAtualizacao = 0;
                if(pontuacao <= -30) estadoAtual = ESTADO_FALENCIA; 
            }
        }

        // ==============================================
        // DESENHO (RENDERIZAÇÃO CORRIGIDA)
        // ==============================================
        BeginDrawing();
        ClearBackground(BEIGE);

        if (estadoAtual == ESTADO_MENU) {
            // Desenha a imagem de fundo esticada para preencher a tela de 1000x600
            // CORREÇÃO: Garante que o menu cobre a tela bege se der erro de carga
            if (fundoMenu.id != 0) { // Se carregou
                DrawTexturePro(fundoMenu, 
                    (Rectangle){ 0, 0, (float)fundoMenu.width, (float)fundoMenu.height },
                    (Rectangle){ 0, 0, 1000, 600 },
                    (Vector2){ 0, 0 }, 0.0f, WHITE);
            } else {
                DrawText("CONVERTA Telainicio.jpg para .png na pasta imagens/", 200, 100, 20, RED);
                DrawRectangleRec(btnJogar, GREEN);
                DrawText("JOGAR (Fallback)", 415, 235, 20, BLACK);
            }

            // Feedback visual no botão JOGAR da lógica original
            if(CheckCollisionPointRec(mousePos, btnJogar)) {
                DrawRectangleLinesEx(btnJogar, 4, GREEN);
            }
        }
        else if (estadoAtual == ESTADO_FALENCIA) {
            DrawRectangle(0, 0, 1000, 600, RED);
            DrawText("FALÊNCIA!", 350, 220, 60, WHITE);
            DrawText("O restaurante fechou as portas.", 320, 300, 24, WHITE);
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            
            // MAPA ESTICADO PARA PREENCHER TELA INTEIRA (Elimina borda bege)
            DrawTexturePro(mapa, 
                (Rectangle){ 0, 0, (float)mapa.width, (float)mapa.height },
                (Rectangle){ 0, 0, 1000, 600 },
                (Vector2){ 0, 0 }, 0.0f, WHITE);
                
            DrawText(TextFormat("Pontuacao: %d", pontuacao), 20, 20, 25, BLACK);

            // 1. Renderiza Mesas com tamanho proporcional calculado
            NoLista *aux = restaurante.inicio;
            while(aux != NULL){
                if(aux->tipo == MESA){
                    Texture2D tex = (aux->mesa->status == SUJA) ? mesaSuja : mesaLimpa;
                    
                    // CORREÇÃO DE TAMANHO: Calcula largura proporcional à altura desejada
                    float ratio = (float)tex.width / (float)tex.height;
                    float destWidth = ALTURA_MESA * ratio;
                    
                    Rectangle destRec = { (float)aux->posX, (float)aux->posY, destWidth, ALTURA_MESA };
                    Vector2 origin = { destWidth / 2.0f, ALTURA_MESA }; // Origem base no chão
                    
                    DrawTexturePro(tex, (Rectangle){ 0, 0, (float)tex.width, (float)tex.height }, destRec, origin, 0.0f, WHITE);
                }
                else if(aux->tipo == COZINHA){
                    DrawText("COZINHA", aux->posX - 35, aux->posY - 110, 18, BLACK);
                }
                aux = aux->proximo;
            }

            // 2. Renderiza Clientes com proporção original (Fim do esticamento)
            aux = restaurante.inicio;
            while(aux != NULL){
                if(aux->tipo == MESA && aux->mesa->cliente != NULL){
                    Cliente *c = aux->mesa->cliente;
                    NoLista *posC = c->posicaoAtual;
                    int id = c->idSprite;
                    Texture2D texCli;

                    if(posC == aux) texCli = clientesSit[id];
                    else texCli = (posC->tipo == CAMINHO) ? clientesWalk[id] : clientesIdle[id];

                    // CORREÇÃO DE PROPORÇÃO: Fim do esticamento
                    float ratio = (float)texCli.width / (float)texCli.height;
                    float destWidth = ALTURA_PERSONAGEM * ratio;
                    
                    Rectangle destRec = { (float)posC->posX, (float)posC->posY, destWidth, ALTURA_PERSONAGEM };
                    // Origem no centro inferior (pés)
                    Vector2 origin = { destWidth / 2.0f, ALTURA_PERSONAGEM }; 
                    
                    // CORREÇÃO SEPARAÇÃO MESA: Empurrar mais para a cadeira (esquerda)
                    if(posC == aux) destRec.x -= 45.0f; // Aumentado separation

                    DrawTexturePro(texCli, (Rectangle){ 0, 0, (float)texCli.width, (float)texCli.height }, destRec, origin, 0.0f, WHITE);

                    // UI do Cliente Sentado
                    if(posC == aux) {
                        DrawText(c->pratoDesejado.nome, posC->posX - 60, posC->posY - 125, 16, BLACK);
                        DrawText(TextFormat("Pac: %d", c->paciencia), posC->posX - 60, posC->posY - 110, 16, RED);
                    }
                }
                aux = aux->proximo;
            }

            // 3. Renderiza Garçom PROPORCIONAL
            NoLista *posG = garcom.posicaoAtual;
            Texture2D texGarcom;
            if(garcom.pratoAtual != NULL) texGarcom = (garcom.timerTransicao > 0) ? prot_ap : prot_pp;
            else texGarcom = (garcom.timerTransicao > 0) ? prot_a : prot_p;

            float ratioG = (float)texGarcom.width / (float)texGarcom.height;
            float destWidthG = ALTURA_PERSONAGEM * ratioG;
            
            Rectangle destRecG = { (float)posG->posX, (float)posG->posY, destWidthG, ALTURA_PERSONAGEM };
            Vector2 originG = { destWidthG / 2.0f, ALTURA_PERSONAGEM }; // Pés no chão
            
            DrawTexturePro(texGarcom, (Rectangle){ 0, 0, (float)texGarcom.width, (float)texGarcom.height }, destRecG, originG, 0.0f, WHITE);

            // Garçom e Cozinha UI
            if(garcom.pratoAtual != NULL) DrawText(TextFormat("Mão: %s", garcom.pratoAtual->nome), 20, 50, 20, DARKBLUE);
            if (escolhendoPrato) {
                DrawRectangle(250, 120, 500, 320, Fade(BLACK, 0.85f));
                DrawText("COZINHA: [1-5] para escolher", 300, 140, 22, YELLOW);
                for (int i = 0; i < 5; i++) DrawText(TextFormat("[%d] - %s", i + 1, cardapio[i]), 350, 180 + (i * 35), 20, WHITE);
            }

            // 4. LISTA DE URGÊNCIA (QUICKSORT) RESTAURADA
            int painelX = 780;
            DrawRectangle(painelX, 20, 200, 150, Fade(BLACK, 0.7f));
            DrawRectangleLines(painelX, 20, 200, 150, WHITE);
            DrawText("URGENCIA", painelX + 50, 30, 18, RED);

            if (totalEsperando == 0) {
                DrawText("Sem pedidos", painelX + 40, 70, 16, LIGHTGRAY);
            } else {
                for (int k = 0; k < totalEsperando; k++) {
                    Color cor = GREEN;
                    if (mesasOrdenadas[k]->cliente->paciencia <= 5) cor = RED;
                    else if (mesasOrdenadas[k]->cliente->paciencia <= 10) cor = ORANGE;

                    DrawText(TextFormat("M%d | Pac:%d", mesasOrdenadas[k]->numero, mesasOrdenadas[k]->cliente->paciencia),
                             painelX + 15, 60 + k * 30, 17, cor);
                }
            }
            
            DrawText("A/D=Mover | ESPACO=Interagir | Q=Lixo", 20, 560, 15, BLACK);
        }

        EndDrawing();
    }

    // Limpeza
    UnloadTexture(fundoMenu); UnloadTexture(mapa); UnloadTexture(mesaLimpa); UnloadTexture(mesaSuja);
    UnloadTexture(prot_p); UnloadTexture(prot_a); UnloadTexture(prot_pp); UnloadTexture(prot_ap);
    for(int i=0; i<3; i++){ UnloadTexture(clientesWalk[i]); UnloadTexture(clientesIdle[i]); UnloadTexture(clientesSit[i]); }
    CloseWindow();
    return 0;
}