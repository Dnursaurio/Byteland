#pragma once
#include <SFML/Graphics.hpp>
#include <array> 
#include "Global.hpp"
using namespace std;
using namespace sf;
class Player
{
	float movhorizontal;
	float movvertical;
	float x;
	float y;

	array<float, ANCHO_PANTALLA> verrayos;

	Sprite map_player_sprite;
	Texture map_player_texture;

public:
	Player(float i_x, float i_y);

	void draw_map(RenderWindow &i_ventana);
	void draw_screen(RenderWindow &i_ventana);
	void set_position(float i_x , float i_y);
	void update(const array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA> &i_map, const RenderWindow &i_ventana);
};