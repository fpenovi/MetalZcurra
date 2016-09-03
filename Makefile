#OBJS specifies which files to compile as part of the project
OBJS = auxiliares.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -pthread


all : servidor cliente ClientePOO servidorPOO

servidor :
	$(CC) servidor.cpp $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o servidor

cliente :
	$(CC) cliente.cpp $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o cliente

ClientePOO:
	$(CC) ClientePOO.cpp $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o ClientePOO

servidorPOO :
	$(CC) servidorPOO.cpp $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o servidorPOO

clean : servidor cliente ClientePOO servidorPOO
	rm servidor
	rm cliente
	rm servidorPOO
	rm ClientePOO
