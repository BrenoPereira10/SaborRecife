#include "restaurante.h"
#include "entidades.h"
#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum { ESTADO_MENU, ESTADO_JOGANDO, ESTADO_FALENCIA, ESTADO_AJUSTES, ESTADO_CREDITOS, ESTADO_FIM_DE_JOGO } EstadoJogo;
const float ALTURA_MESA = 150.0f; 
const float ALTURA_PERSONAGEM = 135.0f; 

int main(){
    srand(time(NULL));

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1000, 600, "Sabor Recife");
    
    InitAudioDevice();
    SetTargetFPS(60);
    RenderTexture2D alvo = LoadRenderTexture(1000, 600);
    Texture2D fundoMenu = LoadTexture("imagens/Telainicio.png");
    Rectangle btnJogar   = { 390, 240, 220, 65 }; 
    Rectangle btnAjustes = { 400, 335, 200, 63 };
    Rectangle btnCreditos= { 400, 422, 200, 63 };
    
    Rectangle btnVoltar      = { 390, 460, 220, 55 };
    Rectangle btnToggleSom   = { 550, 165, 130, 40 };
    Rectangle barraVolume    = { 480, 235, 200, 12 };

    Music musicaMenu = LoadMusicStream("sons/menu.ogg");
    Music musicaJogo = LoadMusicStream("sons/jogo.ogg");
    musicaMenu.looping = true;
    musicaJogo.looping = true;
    PlayMusicStream(musicaMenu);

    bool somAtivado = true;
    float somVolume = 0.5f; 
    bool arrastandoVolume = false;
    float tempoRestante = 60.0f;

    Texture2D mapa     = LoadTexture("imagens/mapa.png"); 
    Texture2D mesaLimpa= LoadTexture("imagens/mesa.png");
    Texture2D mesaSuja = LoadTexture("imagens/mesasuja.png");
    Texture2D prot_p  = LoadTexture("imagens/protagonistap.png");
    Texture2D prot_a  = LoadTexture("imagens/protagonistaa.png");
    Texture2D prot_pp = LoadTexture("imagens/protagonistapp.png");
    Texture2D prot_ap = LoadTexture("imagens/protagonistaap.png");
    Texture2D clientesWalk[3] = { LoadTexture("imagens/cliente1a.png"), LoadTexture("imagens/cliente2a.png"), LoadTexture("imagens/cliente3a.png") };
    Texture2D clientesIdle[3] = { LoadTexture("imagens/cliente1p.png"), LoadTexture("imagens/cliente2p.png"), LoadTexture("imagens/cliente3p.png") };
    Texture2D clientesSit[3]  = { LoadTexture("imagens/cliente1s.png"), LoadTexture("imagens/cliente2s.png"), LoadTexture("imagens/cliente3s.png") };

    Texture2D texPratos[5];
    texPratos[0] = LoadTexture("imagens/tapioca.png");
    texPratos[1] = LoadTexture("imagens/boloderolo.png");
    texPratos[2] = LoadTexture("imagens/cuscuz.png");
    texPratos[3] = LoadTexture("imagens/cartola.png");
    texPratos[4] = LoadTexture("imagens/caldinho.png");

    EstadoJogo estadoAtual = ESTADO_MENU;

    Restaurante restaurante;
    inicializarRestaurante(&restaurante);

    adicionarPosicao(&restaurante, criarNoMesa(1, 100, 480));
    adicionarPosicao(&restaurante, criarCaminho(205, 450));
    adicionarPosicao(&restaurante, criarNoMesa(2, 310, 480));
    adicionarPosicao(&restaurante, criarCaminho(415, 450));
    adicionarPosicao(&restaurante, criarNoMesa(3, 520, 480));
    adicionarPosicao(&restaurante, criarCaminho(700, 450)); 
    adicionarPosicao(&restaurante, criarCozinha(880, 450));
    
    FilaCozinha cozinha;
    inicializarFila(&cozinha);
    char cardapio[5][50] = { "Tapioca", "Bolo de Rolo", "Cuscuz", "Cartola", "Caldinho" };
    bool escolhendoPrato = false;
    
    Garcom garcom;
    inicializarGarcom(&garcom, &restaurante);
    float tempoAtualizacao = 0;
    Mesa *mesasOrdenadas[3]; 
    int totalEsperando = 0;

    SetMasterVolume(somVolume);
    while(!WindowShouldClose()){
        float dt = GetFrameTime();
        UpdateMusicStream(musicaMenu);
        UpdateMusicStream(musicaJogo);
        
        float escala = fminf((float)GetScreenWidth()/1000.0f, (float)GetScreenHeight()/600.0f);
        float offsetX = (GetScreenWidth()  - 1000.0f * escala) / 2.0f;
        float offsetY = (GetScreenHeight() - 600.0f  * escala) / 2.0f;

        Vector2 mouseRaw = GetMousePosition();
        Vector2 mousePos = {
            (mouseRaw.x - offsetX) / escala,
            (mouseRaw.y - offsetY) / escala
        };
        if(estadoAtual == ESTADO_MENU) {
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(CheckCollisionPointRec(mousePos, btnJogar)) {
                    estadoAtual = ESTADO_JOGANDO;
                    tempoRestante = 60.0f; 
                    pontuacao = 0;
                    escolhendoPrato = false;

                    if (garcom.pratoAtual != NULL) {
                        free(garcom.pratoAtual);
                        garcom.pratoAtual = NULL;
                    }

                    NoLista *atual = restaurante.inicio;
                    while (atual != NULL) {
                        if (atual->tipo == MESA) {
                            if (atual->mesa->cliente != NULL) {
                                free(atual->mesa->cliente);
                                atual->mesa->cliente = NULL;
                            }
                            atual->mesa->status = 0;
                        }
                        atual = atual->proximo;
                    }

                    while (cozinha.inicio != NULL) {
                        dequeue(&cozinha);
                    }
                    
                    inicializarFila(&cozinha); 
                    totalEsperando = 0;        
                    tempoAtualizacao = 0.0f;

                    inicializarGarcom(&garcom, &restaurante);
                    StopMusicStream(musicaMenu);
                    PlayMusicStream(musicaJogo);
                }
                else if(CheckCollisionPointRec(mousePos, btnAjustes))
                    estadoAtual = ESTADO_AJUSTES;
                else if(CheckCollisionPointRec(mousePos, btnCreditos))
                    estadoAtual = ESTADO_CREDITOS;
            }
        }
        else if (estadoAtual == ESTADO_AJUSTES) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mousePos, btnVoltar))
                    estadoAtual = ESTADO_MENU;
                if (CheckCollisionPointRec(mousePos, btnToggleSom)) {
                    somAtivado = !somAtivado;
                    SetMasterVolume(somAtivado ? somVolume : 0.0f);
                }
                Rectangle handleVolume = { barraVolume.x + (somVolume * barraVolume.width) - 8, barraVolume.y - 4, 16, 20 };
                if (CheckCollisionPointRec(mousePos, handleVolume) || CheckCollisionPointRec(mousePos, barraVolume))
                    arrastandoVolume = true;
            }
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
                arrastandoVolume = false;
            if (arrastandoVolume) {
                somVolume = (mousePos.x - barraVolume.x) / barraVolume.width;
                if (somVolume < 0.0f) somVolume = 0.0f;
                if (somVolume > 1.0f) somVolume = 1.0f;
                if (somAtivado) SetMasterVolume(somVolume);
            }
        }
        else if (estadoAtual == ESTADO_CREDITOS) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                if (CheckCollisionPointRec(mousePos, btnVoltar))
                    estadoAtual = ESTADO_MENU;
        }
        else if (estadoAtual == ESTADO_FALENCIA || estadoAtual == ESTADO_FIM_DE_JOGO) {
            if  (IsKeyPressed(KEY_ENTER)) {
                estadoAtual = ESTADO_MENU;
                StopMusicStream(musicaJogo);
                PlayMusicStream(musicaMenu);
            }
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            tempoRestante -= dt;
            if (tempoRestante <= 0.0f) {
                tempoRestante = 0.0f;
                estadoAtual = ESTADO_FIM_DE_JOGO; 
                StopMusicStream(musicaJogo);
            }

            atualizarMovimentoGarcom(&garcom, dt);
            atualizarFisicaClientes(restaurante.inicio, dt);
            if (IsKeyPressed(KEY_Q) && garcom.pratoAtual != NULL) {
                free(garcom.pratoAtual);
                garcom.pratoAtual = NULL;
            }

            if (escolhendoPrato) {
                int p = -1;
                if (IsKeyPressed(KEY_ONE))   p = 0;
                if (IsKeyPressed(KEY_TWO))   p = 1;
                if (IsKeyPressed(KEY_THREE)) p = 2;
                if (IsKeyPressed(KEY_FOUR))  p = 3;
                if (IsKeyPressed(KEY_FIVE))  p = 4;
                if (p != -1) {
                    garcom.pratoAtual = malloc(sizeof(Prato));
                    strcpy(garcom.pratoAtual->nome, cardapio[p]);
                    escolhendoPrato = false;
                }
                if (IsKeyPressed(KEY_ESCAPE)) escolhendoPrato = false;
            } else {
                if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) irParaDireita(&garcom);
                if(IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)) irParaEsquerda(&garcom);
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
                        percorre->mesa->cliente->posicaoAtual == percorre)
       
                        mesasOrdenadas[totalEsperando++] = percorre->mesa;
                    percorre = percorre->proximo;
                }
                if (totalEsperando > 1) quickSortClientes(mesasOrdenadas, 0, totalEsperando - 1);
                exibirFilaDeEspera(mesasOrdenadas, totalEsperando);
                tempoAtualizacao = 0;
                
                if(pontuacao <= -30) {
                    estadoAtual = ESTADO_FALENCIA;
                    StopMusicStream(musicaJogo);
                }
            }
        }

        BeginTextureMode(alvo);
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
            if(CheckCollisionPointRec(mousePos, btnJogar))
                DrawRectangleLinesEx(btnJogar, 4, GREEN);
            if(CheckCollisionPointRec(mousePos, btnAjustes))
                DrawRectangleLinesEx(btnAjustes, 4, ORANGE);
            if(CheckCollisionPointRec(mousePos, btnCreditos))
                DrawRectangleLinesEx(btnCreditos, 4, GOLD);
        }
        else if (estadoAtual == ESTADO_AJUSTES || estadoAtual == ESTADO_CREDITOS) {
            if (fundoMenu.id != 0) {
                DrawTexturePro(fundoMenu, 
                    (Rectangle){ 0, 0, (float)fundoMenu.width, (float)fundoMenu.height },
                    (Rectangle){ 0, 0, 1000, 600 },
                    (Vector2){ 0, 0 }, 0.0f, WHITE);
            }
            DrawRectangle(220, 50, 560, 490, Fade(BLACK, 0.88f));
            DrawRectangleLinesEx((Rectangle){ 220, 50, 560, 490 }, 5, MAROON);

            if (estadoAtual == ESTADO_AJUSTES) {
                DrawText("AJUSTES", 435, 75, 34, GOLD);
                DrawText("ÁUDIO GERAL", 270, 130, 18, GOLD);
                DrawLine(270, 150, 730, 150, GRAY);
                DrawText("Efeitos & Sons:", 270, 172, 20, WHITE);
                DrawRectangleRec(btnToggleSom, somAtivado ? DARKGREEN : RED);
                DrawRectangleLinesEx(btnToggleSom, 2, WHITE);
                DrawText(somAtivado ? "ATIVADO" : "MUTADO", btnToggleSom.x + (somAtivado ? 22 : 28), btnToggleSom.y + 10, 18, WHITE);
                DrawText("Volume:", 270, 230, 20, WHITE);
                DrawRectangleRec(barraVolume, DARKGRAY);
                DrawRectangle(barraVolume.x, barraVolume.y, somVolume * barraVolume.width, barraVolume.height, ORANGE);
                DrawRectangle(barraVolume.x + (somVolume * barraVolume.width) - 8, barraVolume.y - 4, 16, 20, GOLD);
                DrawRectangleLinesEx((Rectangle){ barraVolume.x + (somVolume * barraVolume.width) - 8, barraVolume.y - 4, 16, 20 }, 2, WHITE);
                DrawText(TextFormat("%d%%", (int)(somVolume * 100)), barraVolume.x + barraVolume.width + 15, barraVolume.y - 4, 18, WHITE);
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
            } 
            else if (estadoAtual == ESTADO_CREDITOS) {
                DrawText("CRÉDITOS", 415, 75, 34, GOLD);
                DrawText("EQUIPE DE DESENVOLVIMENTO", 270, 140, 18, GOLD);
                DrawLine(270, 160, 730, 160, GRAY);
                int y = 190;
                DrawText("- Julio Cesar Coutinho Holanda Cavalcanti", 270, y,        18, WHITE);
                DrawText("- João Luiz de Lima Bacelar",                270, y + 35,  18, WHITE);
                DrawText("- Rodrigo Vinhas Marques",                   270, y + 70,  18, WHITE);
                DrawText("- Breno Pereira de Oliveira Lima",           270, y + 105, 18, WHITE);
                DrawText("- João Carlos Vasconcelos de Gusmão",        270, y + 140, 18, WHITE);
                DrawText("CESAR SCHOOL - 2026", 400, 420, 18, ORANGE);
            }

            bool hoverVoltar = CheckCollisionPointRec(mousePos, btnVoltar);
            DrawRectangleRec(btnVoltar, hoverVoltar ? DARKGREEN : MAROON);
            DrawRectangleLinesEx(btnVoltar, 3, GOLD);
            DrawText("VOLTAR", btnVoltar.x + 65, btnVoltar.y + 15, 22, WHITE);
        }
        else if (estadoAtual == ESTADO_FALENCIA) {
            DrawRectangle(0, 0, 1000, 600, RED);
            DrawText("FALENCIA!", 350, 220, 60, WHITE);
            DrawText("O restaurante fechou as portas.", 320, 300, 24, WHITE);
            DrawText("Aperte ENTER para voltar ao menu", 290, 400, 24, LIGHTGRAY);
        }
        else if (estadoAtual == ESTADO_FIM_DE_JOGO) {
            DrawRectangle(0, 0, 1000, 600, DARKBLUE);
            DrawText("TEMPO ESGOTADO!", 260, 180, 50, GOLD);
            DrawText("O expediente acabou!", 370, 250, 24, LIGHTGRAY);
            DrawText(TextFormat("Pontuação Final: %d", pontuacao), 330, 320, 32, WHITE);
            DrawText("Pressione ENTER para voltar ao menu", 250, 460, 20, GRAY);
        }
        else if (estadoAtual == ESTADO_JOGANDO) {
            DrawTexturePro(mapa, 
                (Rectangle){ 0, 0, (float)mapa.width, (float)mapa.height },
                (Rectangle){ 0, 0, 1000, 600 },
                (Vector2){ 0, 0 }, 0.0f, WHITE);
            DrawText(TextFormat("Pontuacao: %d", pontuacao), 22, 22, 40, Fade(RED, 0.5f));
            DrawText(TextFormat("Pontuacao: %d", pontuacao), 20, 20, 40, RED);
            int minutos = (int)tempoRestante / 60;
            int segundos = (int)tempoRestante % 60;
            Color corTempo = (tempoRestante <= 10.0f) ? RED : BLUE;
            
            DrawText(TextFormat("%d:%02d", minutos, segundos), 432, 22, 40, Fade(corTempo, 0.5f));
            DrawText(TextFormat("%d:%02d", minutos, segundos), 430, 20, 40, corTempo);
            NoLista *aux = restaurante.inicio;
            while(aux != NULL){
                if(aux->tipo == MESA){
                    Texture2D tex = (aux->mesa->status == SUJA) ? mesaSuja : mesaLimpa;
                    float ratio = (float)tex.width / (float)tex.height;
                    float destWidth = ALTURA_MESA * ratio;
                    Rectangle destRec = { (float)aux->posX, (float)aux->posY, destWidth, ALTURA_MESA };
                    Vector2 origin = { destWidth / 2.0f, ALTURA_MESA };
                    DrawTexturePro(tex, (Rectangle){ 0, 0, (float)tex.width, (float)tex.height }, destRec, origin, 0.0f, WHITE);
                    if (aux->mesa->status == SUJA && aux->mesa->cliente == NULL) {
                        int textoX = (int)(destRec.x - origin.x);
                        int textoY = (int)(destRec.y - origin.y - 25.0f);
                        DrawText("LIMPE A MESA!", textoX, textoY, 18, RED);
                    }
                    
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
                    
                    float LARGURA_CLIENTE = 65.0f;
                    float ALTURA_CLIENTE  = 130.0f;
                    Rectangle destRec = { (float)posC->posX, (float)posC->posY, LARGURA_CLIENTE, ALTURA_CLIENTE };
                    Vector2 origin = { LARGURA_CLIENTE / 2.0f, ALTURA_CLIENTE };
                    if(posC == aux) {
                        destRec.x -= 48.0f;
                        destRec.y += -40.0f;
                    }
                    else if (posC->tipo == CAMINHO) {
                        destRec.y += 0.0f;
                    }
                    else{
                        destRec.y -= 30.0f;
                    }
                    
                    DrawTexturePro(texCli, (Rectangle){ 0, 0, (float)texCli.width, (float)texCli.height }, destRec, origin, 0.0f, WHITE);
                    if(posC == aux) {
                        DrawText(c->pratoDesejado.nome, posC->posX - 60, posC->posY - 200, 16, BLACK);
                        DrawText(TextFormat("Pac: %d", c->paciencia), posC->posX - 60, posC->posY - 185, 16, RED);
                    }
                }
                aux = aux->proximo;
            }

            NoLista *posG = garcom.posicaoAtual;
            Texture2D texGarcom;
            if(garcom.pratoAtual != NULL) texGarcom = (garcom.timerTransicao > 0) ? prot_ap : prot_pp;
            else texGarcom = (garcom.timerTransicao > 0) ? prot_a : prot_p;
            
            static int ultimaDirecaoOlharG = 1;
            if (garcom.direcao != 0) ultimaDirecaoOlharG = garcom.direcao;
            float ratioG = (float)texGarcom.width / (float)texGarcom.height;
            float destWidthG = ALTURA_PERSONAGEM * ratioG;
            Rectangle destRecG = { (float)posG->posX, 450.0f, destWidthG, ALTURA_PERSONAGEM };
            Vector2 originG = { destWidthG / 2.0f, ALTURA_PERSONAGEM };
            Rectangle sourceRecG = { 0, 0, (float)texGarcom.width * ultimaDirecaoOlharG, (float)texGarcom.height };
            DrawTexturePro(texGarcom, sourceRecG, destRecG, originG, 0.0f, WHITE);
            if(garcom.pratoAtual != NULL) DrawText(TextFormat("Mao: %s", garcom.pratoAtual->nome), 20, 70, 20, DARKBLUE);
            if (escolhendoPrato) {
                DrawRectangle(250, 120, 500, 320, Fade(BLACK, 0.85f));
                DrawText("COZINHA: [1-5] para escolher", 280, 140, 22, YELLOW);
                
                for (int i = 0; i < 5; i++) {
                    int posY = 180 + (i * 35);
                    Rectangle origem = { 0, 0, (float)texPratos[i].width, (float)texPratos[i].height };
                    Rectangle destino = { 315, posY - 10, 40, 40 };
                    Vector2 centro = { 0, 0 };
                    
                    DrawTexturePro(texPratos[i], origem, destino, centro, 0.0f, WHITE);
                    DrawText(TextFormat("[%d] - %s", i + 1, cardapio[i]), 370, posY, 20, WHITE);
                }
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
                    if (mesasOrdenadas[k]->cliente->paciencia <= 5)  cor = RED;
                    else if (mesasOrdenadas[k]->cliente->paciencia <= 10) cor = ORANGE;
                    DrawText(TextFormat("M%d | Pac:%d", mesasOrdenadas[k]->numero, mesasOrdenadas[k]->cliente->paciencia),
                             painelX + 15, 60 + k * 30, 17, cor);
                }
            }
            DrawText("A/D=Mover | ESPACO=Interagir | Q=Lixo", 20, 560, 15, RED);
        }

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(
            alvo.texture,
            (Rectangle){ 0, 0, 1000, -600 },   
            (Rectangle){ offsetX, offsetY, 1000.0f * escala, 600.0f * escala },
            (Vector2){ 0, 0 }, 0.0f, WHITE
        );
        EndDrawing();
    }

    for(int i = 0; i < 5; i++) {
        UnloadTexture(texPratos[i]);
    }

    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaJogo);
    UnloadRenderTexture(alvo);
    UnloadTexture(fundoMenu); UnloadTexture(mapa); UnloadTexture(mesaLimpa); UnloadTexture(mesaSuja);
    UnloadTexture(prot_p); UnloadTexture(prot_a); UnloadTexture(prot_pp);
    UnloadTexture(prot_ap);
    for(int i = 0; i < 3; i++){
        UnloadTexture(clientesWalk[i]);
        UnloadTexture(clientesIdle[i]);
        UnloadTexture(clientesSit[i]);
    }
    CloseAudioDevice();
    CloseWindow();
    return 0;
}