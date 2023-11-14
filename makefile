# Variables para especificar los nombres de los archivos
PROGRAMA = SSP
ARCHIVO_CPP = main.cpp
OBJETOS = $(ARCHIVO_CPP:.cpp=.o) File_RW.o 

# Opciones de compilación
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Regla por defecto: compilar y ejecutar el programa
all: $(PROGRAMA)
	@./$(PROGRAMA)

# Regla para compilar el programa
$(PROGRAMA): $(OBJETOS)
	@$(CXX) -o $(PROGRAMA) $(OBJETOS)

# Regla para compilar archivos fuente a archivos objeto
%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Rutina make
make: all

# Rutina make clean
clean:
	@rm -f $(OBJETOS) $(PROGRAMA)

