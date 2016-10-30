#OBJS specifies which files to compile as part of the project
OBJS = auxiliares.cpp Mensaje.cpp Log.cpp Heartbeat.cpp  ProtocoloComando.cpp ProtocoloVistaUpdate.cpp Personaje.cpp GameObject.cpp ObjectManager.cpp ProtocoloNuevaVista.cpp ParserXML.cpp HandleKeyHoldServer.cpp HandleJumpServer.cpp
OBJS2 = auxiliares.cpp Mensaje.cpp Log.cpp Heartbeat.cpp Cliente.cpp Textura.cpp VistaPersonaje.cpp ProtocoloComando.cpp ProtocoloVistaUpdate.cpp HandleKeyHold.cpp HandleJump.cpp ProtocoloNuevaVista.cpp Background.cpp Layer.cpp


#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -std=c++11 -O2

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -pthread -lxml2
LINKER_FLAGS2 = -pthread -lSDL2 -lSDL2_image

#LIBXML2
LIBXML = -I/usr/include/libxml2

all : juego servidor


juego :
	$(CC) Juego.cpp $(OBJS2) $(COMPILER_FLAGS) $(LINKER_FLAGS2) -o juego

servidor :
	$(CC) Servidor.cpp $(OBJS) $(LIBXML) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o servidor

clean :
	rm servidor
	rm juego
