#OBJS specifies which files to compile as part of the project
OBJS = auxiliares.cpp Mensaje.cpp Log.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -pthread


all : cliente servidor


cliente :
	$(CC) ClientePOO.cpp $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o cliente

servidor :
	$(CC) servidorPOO.cpp $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o servidor

clean : servidor cliente
	rm servidor
	rm cliente
