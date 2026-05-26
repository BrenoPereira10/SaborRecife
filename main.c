#include "restaurante.h"
#include "entidades.h"
#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

// Adicionado o ESTADO_AJUSTES na máquina de estados
typedef enum { ESTADO_MENU, ESTADO_JOGANDO, ESTADO_FALENCIA, ESTADO_AJUSTES } EstadoJogo;

const float ALTURA_MESA = 90.0f; 
const float ALTURA_PERSONAGEM = 85.0f; 

int main(){
    srand(time(NULL));
    
    InitWindow(1000, 600, "Sabor Recife - Versao Corrigida 2.0");
    InitAudioDevice(); // Habilita o sistema de som da Raylib
    SetTargetFPS(60);

    // ==============================================
    // CARREGAMENTO DE TEXTURAS
    // ==============================================
    Texture2D fundoMenu = LoadTexture("imagens/Telainicio.png"); 
    
    // Alinhamento exato com a imagem do menu original (retângulos invisíveis de clique)
    Rectangle btnJogar = { 390, 240, 220, 65 }; 
    Rectangle btnAjustes = { 390, 315, 220, 65 }; // Centralizado sobre o botão verde da imagem
    
    // Elementos internos da tela de Ajustes
    Rectangle btnVoltar = { 390, 460, 220, 55 };
    Rectangle btnToggleSom = { 550, 165, 130, 40 };
    Rectangle barraVolume = { 480, 235, 200, 12 };

    // Variáveis de controle de som
    bool somAtivado = true;
    float somVolume = 0.5f; 
    bool arrastandoVolume = false;

    // JOGO
    Texture2D mapa = LoadTexture("imagens/mapa.png"); 
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

    adicionarPosicao(&restaurante, criarNoMesa(1, 120, 470));
    adicionarPosicao(&restaurante, criarCaminho(210, 470));
    adicionarPosicao(&restaurante, criarNoMesa(2, 300, 470));
    adicionarPosicao(&restaurante, criarCaminho(390, 470));
    adicionarPosicao(&restaurante, criarNoMesa(3, 480, 470));
    adicionarPosicao(&restaurante, criarCaminho(680, 470)); 
    adicionarPosicao(&restaurante, criarCozinha(880, 470));

    FilaCozinha cozinha;
    inicializarFila(&cozinha);
    char cardapio[5][50] = { "Tapioca", "Bolo de Rolo", "Cuscuz", "Cartola", "Caldinho" };
    bool escolhendoPrato = false; 

    Garcom garcom;
    inicializarGarcom(&garcom, &restaurante);

    float tempoAtualizacao = 0;
    Mesa *mesasOrdenadas[3]; 
    int totalEsperando = 0;

    // Volume inicial configurado a 50%
    SetMasterVolume(somVolume);

    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        if(estadoAtual == ESTADO_MENU) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(CheckCollisionPointRec(mousePos, btnJogar)){
                    estadoAtual = ESTADO_JOGANDO;
                }
                // Clique do botão de ajustes detectado
                else if(CheckCollisionPointRec(mousePos, btnAjustes)){
                    estadoAtual = ESTADO_AJUSTES;
                }
            }
        }
        else if (estadoAtual == ESTADO_AJUSTES) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                // Voltar para o menu principal
                if (CheckCollisionPointRec(mousePos, btnVoltar)) {
                    estadoAtual = ESTADO_MENU;
                }
                // Ativar/Desativar som do Master
                if (CheckCollisionPointRec(mousePos, btnToggleSom)) {
                    somAtivado = !somAtivado;
                    SetMasterVolume(somAtivado ? somVolume : 0.0f);
                }
                
                // Verifica colisão com a barra para arrastar volume
                Rectangle handleVolume = { barraVolume.x + (somVolume * barraVolume.width) - 8, barraVolume.y - 4, 16, 20 };
                if (CheckCollisionPointRec(mousePos, handleVolume) || CheckCollisionPointRec(mousePos, barraVolume)) {
                    arrastandoVolume = true;
                }
            }
            
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                arrastandoVolume = false;
            }
            
            // Lógica de arrasto deslizante para o Slider
            if (arrastandoVolume) {
                somVolume = (mousePos.x - barraVolume.x) / barraVolume.width;
                if (somVolume < 0.0f) somVolume = 0.0f;
                if (somVolume > 1.0f) somVolume = 1.0f;
                
                if (somAtivado) SetMasterVolume(somVolume);
            }
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            atualizarMovimentoGarcom(&garcom, dt);
            atualizarFisicaClientes(restaurante.inicio, dt);

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

            tempoAtualizacao += dt;
            if(tempoAtualizacao >= 1.0f){
                atualizarClientes(restaurante.inicio);
                
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
        // DESENHO (RENDERIZAÇÃO)
        // ==============================================
        BeginDrawing();
        ClearBackground(BEIGE);

        if (estadoAtual == ESTADO_MENU) {
            if (fundoMenu.id != 0) { 
                DrawTexturePro(fundoMenu, 
                    (Rectangle){ 0, 0, (float)fundoMenu.width, (float)fundoMenu.height },
                    (Rectangle){ 0, 0, 1000, 600 },
                    (Vector2){ 0, 0 }, 0.0f, WHITE);
            } else {
                DrawText("CONVERTA Telainicio.jpg para .png na pasta imagens/", 200, 100, 20, RED);
                DrawRectangleRec(btnJogar, GREEN);
                DrawText("JOGAR (Fallback)", 415, 235, 20, BLACK);
            }

            // Feedback Visual de Hover nos botões da imagem usando o estilo retrô
            if(CheckCollisionPointRec(mousePos, btnJogar)) {
                DrawRectangleLinesEx(btnJogar, 4, GREEN);
            }
            if(CheckCollisionPointRec(mousePos, btnAjustes)) {
                DrawRectangleLinesEx(btnAjustes, 4, ORANGE);
            }
        }
        else if (estadoAtual == ESTADO_AJUSTES) {
            // Mantém a imagem de fundo do restaurante atrás da janela de configurações
            if (fundoMenu.id != 0) {
                DrawTexturePro(fundoMenu, 
                    (Rectangle){ 0, 0, (float)fundoMenu.width, (float)fundoMenu.height },
                    (Rectangle){ 0, 0, 1000, 600 },
                    (Vector2){ 0, 0 }, 0.0f, WHITE);
            }
            
            // Painel central semi-transparente simulando caixa escura medieval/retrô
            DrawRectangle(220, 50, 560, 490, Fade(BLACK, 0.88f));
            DrawRectangleLinesEx((Rectangle){ 220, 50, 560, 490 }, 5, MAROON); // Bordas imitando madeira escura

            DrawText("AJUSTES", 435, 75, 34, GOLD);

            // --- SEÇÃO DE AUDIO ---
            DrawText("ÁUDIO GERAL", 270, 130, 18, GOLD);
            DrawLine(270, 150, 730, 150, GRAY);

            DrawText("Efeitos & Sons:", 270, 172, 20, WHITE);
            
            // Botão Alternador LIGADO/DESLIGADO
            DrawRectangleRec(btnToggleSom, somAtivado ? DARKGREEN : RED);
            DrawRectangleLinesEx(btnToggleSom, 2, WHITE);
            DrawText(somAtivado ? "ATIVADO" : "MUTADO", btnToggleSom.x + (somAtivado ? 22 : 28), btnToggleSom.y + 10, 18, WHITE);

            // Slider de Volume
            DrawText("Volume:", 270, 230, 20, WHITE);
            DrawRectangleRec(barraVolume, DARKGRAY); // Fundo da trilha
            DrawRectangle(barraVolume.x, barraVolume.y, somVolume * barraVolume.width, barraVolume.height, ORANGE); // Preenchimento
            
            // Seletor (Cursor do Slider)
            DrawRectangle(barraVolume.x + (somVolume * barraVolume.width) - 8, barraVolume.y - 4, 16, 20, GOLD);
            DrawRectangleLinesEx((Rectangle){ barraVolume.x + (somVolume * barraVolume.width) - 8, barraVolume.y - 4, 16, 20 }, 2, WHITE);
            DrawText(TextFormat("%d%%", (int)(somVolume * 100)), barraVolume.x + barraVolume.width + 15, barraVolume.y - 4, 18, WHITE);

            // --- SEÇÃO DE CONTROLES ---
            DrawText("CONTROLES DO JOGO", 270, 300, 18, GOLD);
            DrawLine(270, 320, 730, 320, GRAY);

            int baseTxtY = 340;
            DrawText("- A / D ou SETAS:", 270, baseTxtY, 17, ORANGE);
            DrawText("Movimenta o Garçom", 450, baseTxtY, 17, LIGHTGRAY);
            
            DrawText("- ESPAÇO:", 270, baseTxtY + 25, 17, ORANGE);
            DrawText("Interagir (Pegar / Entregar / Limpar)", 450, baseTxtY + 25, 17, LIGHTGRAY);
            
            DrawText("- TECLA Q:", 270, baseTxtY + 50, 17, ORANGE);
            DrawText("Descarta o prato atual na lixeira", 450, baseTxtY + 50, 17, LIGHTGRAY);

            DrawText("- TECLAS 1 a 5:", 270, baseTxtY + 75, 17, ORANGE);
            DrawText("Seleciona o prato desejado na Cozinha", 450, baseTxtY + 75, 17, LIGHTGRAY);

            // --- BOTÃO VOLTAR ---
            bool hoverVoltar = CheckCollisionPointRec(mousePos, btnVoltar);
            DrawRectangleRec(btnVoltar, hoverVoltar ? DARKGREEN : MAROON);
            DrawRectangleLinesEx(btnVoltar, 3, GOLD);
            DrawText("VOLTAR", btnVoltar.x + 65, btnVoltar.y + 15, 22, WHITE);
        }
        else if (estadoAtual == ESTADO_FALENCIA) {
            DrawRectangle(0, 0, 1000, 600, RED);
            DrawText("FALÊNCIA!", 350, 220, 60, WHITE);
            DrawText("O restaurante fechou as portas.", 320, 300, 24, WHITE);
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            DrawTexturePro(mapa, 
                (Rectangle){ 0, 0, (float)mapa.width, (float)mapa.height },
                (Rectangle){ 0, 0, 1000, 600 },
                (Vector2){ 0, 0 }, 0.0f, WHITE);
                
            DrawText(TextFormat("Pontuacao: %d", pontuacao), 20, 20, 25, BLACK);

            NoLista *aux = restaurante.inicio;
            while(aux != NULL){
                if(aux->tipo == MESA){
                    Texture2D tex = (aux->mesa->status == SUJA) ? mesaSuja : mesaLimpa;
                    float ratio = (float)tex.width / (float)tex.height;
                    float destWidth = ALTURA_MESA * ratio;
                    Rectangle destRec = { (float)aux->posX, (float)aux->posY, destWidth, ALTURA_MESA };
                    Vector2 origin = { destWidth / 2.0f, ALTURA_MESA }; 
                    DrawTexturePro(tex, (Rectangle){ 0, 0, (float)tex.width, (float)tex.height }, destRec, origin, 0.0f, WHITE);
                }
                else if(aux->tipo == COZINHA){
                    DrawText("COZINHA", aux->posX - 35, aux->posY - 110, 18, BLACK);
                }
                aux = aux->proximo;
            }

            aux = restaurante.inicio;
            while(aux != NULL){
                if(aux->tipo == MESA && aux->mesa->cliente != NULL){
                    Cliente *c = aux->mesa->cliente;
                    NoLista *posC = c->posicaoAtual;
                    int id = c->idSprite;
                    Texture2D texCli;

                    if(posC == aux) texCli = clientesSit[id];
                    else texCli = (posC->tipo == CAMINHO) ? clientesWalk[id] : clientesIdle[id];

                    float ratio = (float)texCli.width / (float)texCli.height;
                    float destWidth = ALTURA_PERSONAGEM * ratio;
                    Rectangle destRec = { (float)posC->posX, (float)posC->posY, destWidth, ALTURA_PERSONAGEM };
                    Vector2 origin = { destWidth / 2.0f, ALTURA_PERSONAGEM }; 
                    
                    if(posC == aux) destRec.x -= 45.0f; 

                    DrawTexturePro(texCli, (Rectangle){ 0, 0, (float)texCli.width, (float)texCli.height }, destRec, origin, 0.0f, WHITE);

                    if(posC == aux) {
                        DrawText(c->pratoDesejado.nome, posC->posX - 60, posC->posY - 125, 16, BLACK);
                        DrawText(TextFormat("Pac: %d", c->paciencia), posC->posX - 60, posC->posY - 110, 16, RED);
                    }
                }
                aux = aux->proximo;
            }

            NoLista *posG = garcom.posicaoAtual;
            Texture2D texGarcom;
            if(garcom.pratoAtual != NULL) texGarcom = (garcom.timerTransicao > 0) ? prot_ap : prot_pp;
            else texGarcom = (garcom.timerTransicao > 0) ? prot_a : prot_p;

            float ratioG = (float)texGarcom.width / (float)texGarcom.height;
            float destWidthG = ALTURA_PERSONAGEM * ratioG;
            Rectangle destRecG = { (float)posG->posX, (float)posG->posY, destWidthG, ALTURA_PERSONAGEM };
            Vector2 originG = { destWidthG / 2.0f, ALTURA_PERSONAGEM }; 
            DrawTexturePro(texGarcom, (Rectangle){ 0, 0, (float)texGarcom.width, (float)texGarcom.height }, destRecG, originG, 0.0f, WHITE);

            if(garcom.pratoAtual != NULL) DrawText(TextFormat("Mão: %s", garcom.pratoAtual->nome), 20, 50, 20, DARKBLUE);
            if (escolhendoPrato) {
                DrawRectangle(250, 120, 500, 320, Fade(BLACK, 0.85f));
                DrawText("COZINHA: [1-5] para escolher", 300, 140, 22, YELLOW);
                for (int i = 0; i < 5; i++) DrawText(TextFormat("[%d] - %s", i + 1, cardapio[i]), 350, 180 + (i * 35), 20, WHITE);
            }

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
    
    CloseAudioDevice(); // Encerra o dispositivo de som corretamente antes de fechar
    CloseWindow();
    return 0;
}