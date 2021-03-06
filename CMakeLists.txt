cmake_minimum_required(VERSION 3.3)
project(MetalZcurra)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Servidor.cpp Cliente.cpp Cliente.h Mensaje.cpp Mensaje.h Log.cpp Log.h auxiliares.cpp auxiliares.h
        Heartbeat.h Heartbeat.cpp Heartbeat.h Textura.cpp Textura.h VistaPersonaje.cpp VistaPersonaje.h
        ProtocoloComando.cpp ProtocoloComando.h ProtocoloVistaUpdate.cpp
        ProtocoloVistaUpdate.h Personaje.cpp Personaje.h ObjectManager.cpp Juego.cpp Juego.h ParserXML.cpp ParserXML.h
        ObjectManager.h ProtocoloNuevaVista.cpp ProtocoloNuevaVista.h Background.cpp Background.h Layer.cpp Layer.h
        HandleKeyHoldServer.cpp HandleKeyHoldServer.h HandleJumpServer.cpp HandleJumpServer.h HandleQuietoServer.cpp
        HandleQuietoServer.h Texto.cpp Texto.h Bala.cpp Bala.h VistaBala.cpp VistaBala.h BalasManager.cpp BalasManager.h
        HandleDisparoServer.cpp HandleDisparoServer.h Direccion.cpp Direccion.h Arma.cpp Arma.h Enemigo.cpp Enemigo.h
        EnemigosManager.cpp EnemigosManager.h VistaEnemigo.cpp VistaEnemigo.h NivelManager.cpp NivelManager.h
        Nivel.cpp Nivel.h Bonus.cpp Bonus.h Boss.cpp Boss.h Envolvente.cpp Envolvente.h HMGun.cpp HMGun.h
        Plataforma.cpp Plataforma.h Rectangulo.cpp Direccion.h RLauncher.cpp RLauncher.h Rshobu.cpp Rshobu.h
        SGun.cpp SGun.h BalaShotgun.cpp BalaShotgun.h Recover.cpp Recover.h KillAll.cpp KillAll.h
        VistaPuntajes.cpp VistaPuntajes.h VistaPuntajesIndividual.cpp VistaPuntajesIndividual.h
        VistaPuntajesColaborativo.cpp VistaPuntajesColaborativo.h VistaPuntajesGrupal.cpp VistaPuntajesGrupal.h
        BonusManager.cpp BonusManager.h VistaBonus.cpp VistaBonus.h VistaRshobu.cpp VistaRshobu.h VistaBoss.cpp VistaBoss.h
        PersonajesManager.cpp PersonajesManager.h GrayOutHandler.cpp GrayOutHandler.h GravedadManager.cpp GravedadManager.h
        VistaDaiManji.cpp VistaDaiManji.h DaiManji.cpp DaiManji.h VistaDiCokka.cpp VistaDiCokka.h DiCokka.cpp DiCokka.h
        FinalScreen.cpp FinalScreen.h WinScreen.cpp WinScreen.h LoseScreen.cpp LoseScreen.h)

add_executable(MetalZcurra ${SOURCE_FILES})