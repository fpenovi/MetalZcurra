#OBJS specifies which files to compile as part of the project
OBJS = auxiliares.cpp Mensaje.cpp Log.cpp Heartbeat.cpp  ProtocoloComando.cpp ProtocoloVistaUpdate.cpp Personaje.cpp
OBJS2 = auxiliares.cpp Mensaje.cpp Log.cpp Heartbeat.cpp Cliente.cpp Textura.cpp VistaMarco.cpp ProtocoloComando.cpp ProtocoloVistaUpdate.cpp


#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -std=c++11

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -pthread
LINKER_FLAGS2 = -pthread -lSDL2 -lSDL2_image

all : juego servidor


juego :
	$(CC) Juego.cpp $(OBJS2) $(COMPILER_FLAGS) $(LINKER_FLAGS2) -o juego

servidor :
	$(CC) Servidor.cpp $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o servidor

clean :
	rm servidor
	rm juego
