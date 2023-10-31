#pragma once
#include <SFML/Graphics.hpp>
#include <array> 
#include "Global.hpp"
#include "Enemigo.hpp"

using namespace std;
class Player
{
	float movhorizontal;
	float movvertical;
	float x;
	float y;

	array<float, ANCHO_PANTALLA> verrayos{};

	sf::Sprite map_player_sprite;
	sf::Sprite enemigo_sprite;
	sf::Sprite wall_sprite;

	sf::Texture map_player_texture;
	sf::Texture enemigo_texture;
	sf::Texture wall_texture;

public:
	Player(float i_x, float i_y);

	void draw_map(sf::RenderWindow& i_ventana);
	void bibujar_ventana(sf::RenderWindow& i_ventana, const Enemigo& i_enemigo);
	void set_position(float i_x , float i_y);
	void update(const array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA>& i_map, const sf::RenderWindow& i_ventana);
};