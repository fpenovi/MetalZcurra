cmake_minimum_required(VERSION 3.3)
project(MetalZcurra)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES Servidor.cpp Cliente.cpp Cliente.h Mensaje.cpp Mensaje.h Log.cpp Log.h auxiliares.cpp auxiliares.h Heartbeat.h Heartbeat.cpp Heartbeat.h Textura.cpp Textura.h VistaMarco.cpp VistaMarco.h)
add_executable(MetalZcurra ${SOURCE_FILES})