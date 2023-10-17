#pragma once
#include <array>
#include "Global.hpp"
#include "Player.hpp"
using namespace std;

array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA> convetir_ecsena(Player &i_player);