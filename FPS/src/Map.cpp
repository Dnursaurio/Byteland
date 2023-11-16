#include "Headers/Map.hpp"
#include <cmath>
#include "SFML/Graphics.hpp"

Map::Map(float frame_angle, float shifted_direccion) :
this-> frame_angle = frame_angle;
this-> shifted_direction = shifted_direction;
{}

virtual void Map::draw_map(sf::RenderWindow& i_window)
{

}

