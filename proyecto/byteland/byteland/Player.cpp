#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Grados_a_Radianes.hpp"
#include "Tomar_Grados.hpp"
#include "Global.hpp"
#include "colision_del_mapa.hpp"
#include "Radianes_a_Gradis.hpp"
#include "Enemigo.hpp"
#include "Player.hpp"

Player::Player(float i_x, float i_y) :
	movhorizontal(0),
	movvertical(0),
	x(i_x),
	y(i_y),
	map_player_sprite(map_player_texture),
	wall_sprite(wall_texture)
{

}