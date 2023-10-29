#pragma once
#include <array>
#include "Global.hpp"
#include "Player.hpp"
#include "Enemigo.hpp"
using namespace std;

array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA> convertir_escena(Player &i_player, Enemigo &i_enemigo);