
CXX = g++

CXXFLAGS = -std=c++11 -Iinclude -Wall -g

# Diretórios
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Arquivos fonte
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
# Converte a lista de fontes (e.g., src/main.cpp) para lista de objetos (e.g., obj/main.o)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

# Nome do executável final [cite: 173]
TARGET = $(BINDIR)/tp1.out

# Regra principal: 'make all' ou apenas 'make'
all: $(TARGET)

# Regra para linkar e criar o executável final [cite: 172]
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR) # Cria o diretório bin se ele não existir
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Regra para compilar os arquivos objeto [cite: 172]
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR) # Cria o diretório obj se ele não existir
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para limpar os arquivos compilados
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean