# 🍽️ Sabor Recife

---

## Descrição

Sabor Recife é um jogo de gerenciamento de restaurante desenvolvido em **C** com a biblioteca gráfica **Raylib**.  

A proposta do jogo é oferecer uma experiência dinâmica e divertida, onde o jogador controla um garçom responsável por atender clientes em um restaurante temático de comidas típicas recifenses.

O jogador precisa:
- Descobrir o pedido do cliente;
- Buscar o prato correto na cozinha;
- Entregar a comida antes que a paciência acabe;
- Limpar as mesas para novos clientes.

Com partidas rápidas e mecânicas simples, o jogo mistura lógica, velocidade e estratégia utilizando estruturas de dados clássicas da disciplina de Algoritmos e Estrutura de Dados.

---

## 🎥 Vídeo Demonstrativo

https://youtu.be/Ff6DpmycdMw

---

## 🛠️ Tecnologias Utilizadas

- Linguagem C
- Raylib

---

# 📂 Estrutura do Projeto

```bash
SaborRecife/
├── imagens/             # Sprites e imagens do jogo
├── build/               # Executáveis (caso utilizado)
├── include/             # Headers do projeto (caso utilizado)
├── src/                 # Arquivos fonte (caso utilizado)
├── main.c
├── restaurante.c
├── restaurante.h
├── entidades.c
├── entidades.h
├── Makefile
└── README.md
```

---

# 🖼️ Interface Gráfica

O jogo possui interface gráfica utilizando a biblioteca:

- Raylib

Elementos implementados:
- Menu inicial;
- Sistema de movimentação;
- Cozinha;
- Mesas;
- Painel de urgência;
- Sistema de interação.

(Adicionar screenshots aqui)

---

# 🚀 Instalação e Execução

## ✅ Pré-requisitos

- Linux/WSL Ubuntu recomendado
- GCC
- Make
- Raylib instalada

---

# 🔧 Passo a Passo

## Atualize os pacotes do sistema

```bash
sudo apt update && sudo apt upgrade -y
```

---

## Instale dependências básicas

```bash
sudo apt install -y build-essential make cmake git
```

---

## Instale bibliotecas gráficas

```bash
sudo apt install -y libgl1-mesa-dev libglu1-mesa-dev libx11-dev \
libxcursor-dev libxrandr-dev libxi-dev libxinerama-dev \
xorg-dev xserver-xorg-dev libopenal-dev libasound2-dev
```

---

# 📦 Instalação da Raylib

```bash
cd ~
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build && cd build
cmake -DPLATFORM=Desktop ..
make
sudo make install
```

✅ Caso já tenha a Raylib instalada, você pode pular este passo.

---

# 📥 Clone o Repositório

```bash
git clone https://github.com/BrenoPereira10/SaborRecife.git
cd SaborRecife
```

---

# ▶️ Execução do Jogo

## 1º opção - Compilar e rodar com Makefile

```bash
make run
```

---

## Apenas compilar

```bash
make
```

---

## Limpar arquivos objeto

```bash
make clean
```

---

# ▶️ 2º opção - Compilação Manual

## Compilar

```bash
gcc main.c restaurante.c entidades.c -o sabor_recife \
-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

---

## Executar

```bash
./sabor_recife
```

---

# ⚠️ Caso ocorra erro gráfico no WSL

Execute utilizando renderização por software:

```bash
LIBGL_ALWAYS_SOFTWARE=1 ./sabor_recife
```

---

# 🎯 Objetivo do Jogo

O objetivo do jogo é entregar o máximo possível de pedidos corretamente antes que os clientes percam a paciência e abandonem o restaurante.

---
# 🎮 Controles do Jogo

| Tecla | Função |
|------|------|
| `A` / `←` | Mover para esquerda |
| `D` / `→` | Mover para direita |
| `ESPAÇO` | Interagir |
| `ESPAÇO` na cozinha | Pegar comida |
| `ESPAÇO` na mesa | Entregar prato |
| `ESPAÇO` em mesa suja | Limpar mesa |
| `Q` | Descartar item na lixeira |
| `1 a 5` | Selecionar prato na cozinha |

---

# 📚 Referências

- Maid Cafe Minigame — Yandere Simulator

---

# 👨‍💻 Desenvolvedores
Breno Pereira de Oliveira Lima

João Carlos Vasconcelos de Gusmão

João Luiz de Lima Bacelar

Julio Cesar COutinho Holanda Cavalcanti

Rodrigo Vinhas Marques

---

# Obrigado por jogar!

Esperamos que você se divirta jogando Sabor Recife tanto quanto nós nos divertimos desenvolvendo o projeto!
