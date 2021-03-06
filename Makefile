#OBJS specifies which files to compile as part of the project

OBJS = auxiliares.cpp Mensaje.cpp Log.cpp Heartbeat.cpp  ProtocoloComando.cpp ProtocoloVistaUpdate.cpp Personaje.cpp ObjectManager.cpp ProtocoloNuevaVista.cpp ParserXML.cpp HandleKeyHoldServer.cpp HandleJumpServer.cpp HandleQuietoServer.cpp Bala.cpp HandleDisparoServer.cpp BalasManager.cpp Direccion.cpp EnemigosManager.cpp Enemigo.cpp NivelManager.cpp Nivel.cpp Bonus.cpp Boss.cpp Envolvente.cpp HMGun.cpp Plataforma.cpp Rectangulo.cpp RLauncher.cpp Rshobu.cpp SGun.cpp KillAll.cpp Recover.cpp BonusManager.cpp PersonajesManager.cpp GravedadManager.cpp DaiManji.cpp DiCokka.cpp

OBJS2 = auxiliares.cpp Mensaje.cpp Log.cpp Heartbeat.cpp Cliente.cpp Textura.cpp VistaPersonaje.cpp ProtocoloComando.cpp ProtocoloVistaUpdate.cpp ProtocoloNuevaVista.cpp Background.cpp Layer.cpp Texto.cpp VistaBala.cpp Arma.cpp VistaEnemigo.cpp VistaBonus.cpp VistaRshobu.cpp VistaBoss.cpp VistaPuntajes.cpp VistaPuntajesColaborativo.cpp VistaPuntajesGrupal.cpp VistaPuntajesIndividual.cpp GrayOutHandler.cpp VistaDaiManji.cpp VistaDiCokka.cpp FinalScreen.cpp WinScreen.cpp LoseScreen.cpp


#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -std=c++11 -O2

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -pthread -lxml2
LINKER_FLAGS2 = -pthread -lSDL2 -lSDL2_image -lSDL2_ttf

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
