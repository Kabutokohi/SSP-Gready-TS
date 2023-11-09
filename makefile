# Variables para especificar los nombres de los archivos
PROGRAMA = SSP
ARCHIVO_CPP = test.cpp
OBJETOS = $(ARCHIVO_CPP:.cpp=.o)

# Regla para compilar y ejecutar el programa
all: $(PROGRAMA)
	@./$(PROGRAMA)

$(PROGRAMA): $(OBJETOS)
	@g++ -o $(PROGRAMA) $(OBJETOS)

%.o: %.cpp
	@g++ -c $< -o $@

# Rutina make
make: all

# Rutina make clean
clean:
	@rm -f $(OBJETOS) $(PROGRAMA)

