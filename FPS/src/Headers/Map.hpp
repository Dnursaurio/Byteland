#pragma once
#ifndef MAP_H
#define MAP_H

class Map
{
public:
	Map(float frame_angle, float shifted_direction);

	virtual void draw_map(sf::RenderWindow& i_window);
private:
	float frame_angle;
	float shifted_direction;
};

#endif