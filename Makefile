#OBJS specifies which files to compile as part of the project
#OBJS = 

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lpthread


all : servidor cliente

servidor :
	$(CC) servidor.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o servidor

cliente :
	$(CC) cliente.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o cliente

clean : servidor cliente
	rm servidor
	rm cliente

