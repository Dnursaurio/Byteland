#include <array>
#include <chrono>
#include <cmath>

#include "SFML/Graphics.hpp"
#include "Grados_a_Radianes.hpp"
#include "Tomar_Grados.hpp"
#include "Global.hpp"
#include "colision_del_mapa.hpp"
#include "Enemigo.hpp"
using namespace std;
using namespace sf;

Enemigo::Enemigo(float i_x, float i_y) :
	direccion(0),
	x(i_x),
	y(i_y),
	enemigo_mapa_sprite(enemigo_mapa_texture)
{
	enemigo_mapa_texture.loadFromFile("sprites/enemigo2d" + to_string(TAMANO_CASILLA_EN_MAPA) + ".png");
}

float Enemigo::get_centro_de_x() const
{
	return x + 0.5f * TAMANO_CASILLA;
}

float Enemigo::get_centro_de_y() const
{
	return y + 0.5f * TAMANO_CASILLA;
}

float Enemigo::Get_direccion() const
{
	return direccion;
}

void Enemigo::dibujar_mapa(RenderWindow& i_ventana)
{
	float angulo_fotograma = 360.f * TAMANO_CASILLA_EN_MAPA / enemigo_mapa_texture.getSize().x;
	float direccion_cambiada = tomar_grados(direccion + 0.5f * angulo_fotograma);

	enemigo_mapa_sprite.setPosition(round(TAMANO_CASILLA_EN_MAPA * x / TAMANO_CASILLA), round(TAMANO_CASILLA_EN_MAPA * y / TAMANO_CASILLA));
	enemigo_mapa_sprite.setTextureRect(IntRect(static_cast<unsigned short>(TAMANO_CASILLA_EN_MAPA * floor(direccion_cambiada / angulo_fotograma)), 0, TAMANO_CASILLA_EN_MAPA, TAMANO_CASILLA_EN_MAPA));

	i_ventana.draw(enemigo_mapa_sprite);
}

void Enemigo::set_position(float i_x, float i_y)
{
	x = i_x;
	y = i_y;
}

void Enemigo::update(const array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA>& i_map, const RenderWindow& i_ventana)
{
	float paso_x = 0;
	float paso_y = 0;

	if (1 == Keyboard::isKeyPressed(Keyboard::Left))
	{
		direccion += VELOCIDAD_DE_ROTACION_DEL_ENEMIGO;
	}
	else if (1 == Keyboard::isKeyPressed(Keyboard::Right))
	{
		direccion -= VELOCIDAD_DE_ROTACION_DEL_ENEMIGO;
	}

	direccion = tomar_grados(direccion);

	if (1 == Keyboard::isKeyPressed(Keyboard::Down))
	{
		paso_x -= VEL_MOV * cos(grados_a_radianes(direccion));
		paso_y += VEL_MOV + sin(grados_a_radianes(direccion));
	}
	else if (1 == Keyboard::isKeyPressed(Keyboard::Up))
	{
		paso_x += VEL_MOV * cos(grados_a_radianes(direccion));
		paso_y -= VEL_MOV * sin(grados_a_radianes(direccion));
	}

	if (0 == colision_mapa(paso_x + x, paso_y + y, i_map))
	{
		x += paso_x;
		y += paso_y;
	}
	else if (0 == colision_mapa(paso_x + x, y, i_map))
	{
		x += paso_x;
		y = TAMANO_CASILLA * round(y / TAMANO_CASILLA);
	}
	else if ()
	{
		x = TAMANO_CASILLA * round(x / TAMANO_CASILLA);
		y += paso_y;
	}
	else
	{
		x = TAMANO_CASILLA * round(x / TAMANO_CASILLA);
		y = TAMANO_CASILLA * round(y / TAMANO_CASILLA);
	}
}