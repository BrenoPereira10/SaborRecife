# Compilador
CC = gcc

# Flags de compilação (Avisos e Otimização)
CFLAGS = -Wall -Wextra -O2

# Flags de Linkagem para a biblioteca Raylib no Linux
# Se estiver usando Windows (MinGW), você precisará de flags diferentes:
# LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Nome do arquivo executável final
TARGET = sabor_recife

# Arquivos de código-fonte
SRCS = main.c entidades.c restaurante.c

# Objetos gerados a partir dos códigos-fonte (substitui .c por .o)
OBJS = $(SRCS:.c=.o)

# Regra principal (o que roda quando você digita apenas "make")
all: $(TARGET)

# Regra para compilar o executável final ligando os objetos
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Regra genérica para gerar os arquivos objeto (.o) a partir dos (.c)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos de compilação
clean:
	rm -f $(OBJS) $(TARGET)