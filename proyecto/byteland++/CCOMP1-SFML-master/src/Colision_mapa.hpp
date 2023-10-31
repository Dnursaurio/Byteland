#pragma once
#include <array>
#include "Global.hpp"
#include "Player.hpp"
using namespace std;
bool colision_mapa(float i_x, float i_y, const array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA> &i_map);