# Compilador
CC = gcc

# =========================================================
# VERIFICAÇÃO AUTOMÁTICA DE SISTEMA OPERACIONAL
# =========================================================
ifeq ($(OS),Windows_NT)
    # Flags de Linkagem para Windows (MinGW)
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm
    # Extensão do executável no Windows
    TARGET = sabor_recife.exe
    # Comando para limpar arquivos no Windows (CMD)
    RM_CMD = del /Q /F *.o $(TARGET) 2>nul || exit 0
else
    # Flags de Linkagem para Linux (WSL / Ubuntu)
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    # Nome do executável no Linux
    TARGET = sabor_recife
    # Comando para limpar arquivos no Linux
    RM_CMD = rm -f $(OBJS) $(TARGET)
endif

INCLUDES = -I.

LIBRARIES = -L.

# Flags de compilação (Avisos e Otimização)
CFLAGS = -Wall -Wextra -O2 $(INCLUDES)

# Arquivos de código-fonte
SRCS = main.c entidades.c restaurante.c

# Objetos gerados a partir dos códigos-fonte (substitui .c por .o)
OBJS = $(SRCS:.c=.o)

# Regra principal (o que roda quando você digita apenas "make")
all: $(TARGET)

# Regra para compilar o executável final ligando os objetos
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBRARIES) $(LDFLAGS)

# Regra genérica para gerar os arquivos objeto (.o) a partir dos (.c)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar os arquivos de compilação
clean:
	$(RM_CMD)