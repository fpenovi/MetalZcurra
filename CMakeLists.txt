cmake_minimum_required(VERSION 3.3)
project(MetalZcurra)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Servidor.cpp Cliente.cpp Cliente.h Mensaje.cpp Mensaje.h Log.cpp Log.h auxiliares.cpp auxiliares.h
        Heartbeat.h Heartbeat.cpp Heartbeat.h Textura.cpp Textura.h VistaPersonaje.cpp VistaPersonaje.h
        ProtocoloComando.cpp ProtocoloComando.h ProtocoloVistaUpdate.cpp
        ProtocoloVistaUpdate.h Personaje.cpp Personaje.h ObjectManager.cpp
        ObjectManager.h ProtocoloNuevaVista.cpp ProtocoloNuevaVista.h Background.cpp Background.h Layer.cpp Layer.h HandleKeyHoldServer.cpp HandleKeyHoldServer.h HandleJumpServer.cpp HandleJumpServer.h HandleQuietoServer.cpp HandleQuietoServer.h Texto.cpp Texto.h Bala.cpp Bala.h VistaBala.cpp VistaBala.h BalasManager.cpp BalasManager.h HandleDisparoServer.cpp HandleDisparoServer.h Direccion.cpp Direccion.h)
add_executable(MetalZcurra ${SOURCE_FILES})