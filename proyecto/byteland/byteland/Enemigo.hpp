#pragma once
#include <array>
#include <SFML/Graphics.hpp>
#include "Global.hpp"
using namespace std;
using namespace sf;

class Enemigo
{
	float direccion;
	float x;
	float y;

	Sprite enemigo_mapa_sprite;
	Texture enemigo_mapa_texture;
public:
	Enemigo(float i_x, float i_y);

	float get_centro_de_x() const;
	float get_centro_de_y() const;
	float Get_direccion() const;

	void dibujar_mapa(RenderWindow &i_window);
	void set_position(float i_x, float i_y);
	void update(const array<array<Cell, ALTURA_MAPA>,ANCHO_MAPA>&i_map, const RenderWindow &i_window);
};