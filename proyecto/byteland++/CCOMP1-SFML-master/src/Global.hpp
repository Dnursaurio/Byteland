#pragma once
#include <chrono>
using namespace std;

constexpr float CDV_HORIZONRTAL = 90;
constexpr float CDV_VERTICAL = 58.75f;
constexpr float VEL_MOV = 2;
//valor aprox de PI en c++
constexpr float PI = 3.141592653589793116f;
constexpr float DISTANCIA_DE_RENDER = 1024;
constexpr float VELOCIDAD_DE_ROTACION_DEL_ENEMIGO = 2;

//tamaño de cada casilla en el juego
constexpr unsigned char TAMANO_CASILLA = 64;
//tamaño de dada casilla en el minimapa
constexpr unsigned char TAMANO_CASILLA_EN_MAPA = 8;
constexpr unsigned char TAMANO_CASILLA_DE_GRILLA_EN_MAPA = 16;
constexpr unsigned char ALTURA_MAPA = 24;
constexpr unsigned char ANCHO_MAPA = 40;
constexpr unsigned char REESCALADO_DE_PANTALLA = 1;
//tamaño de nuesstra pantalla
constexpr unsigned short ALTURA_PANTALLA = 720;
constexpr unsigned short ANCHO_PANTALLA = 1280;

//en caso de dividir 1 segundo en 60 fotogramas (60 FPS), la duracion de cada fotograma debe de ser de 16.667 ms.
constexpr chrono::microseconds DURACION_FOTOGRAMA(16667);

//en esta parte enmumeramos las casillas dependiendo del tipo de textura que se usara
enum Cell
{
	Empty,
	Wall
};