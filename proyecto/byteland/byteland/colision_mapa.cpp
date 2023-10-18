#include <array>
#include <chrono>

#include "Global.hpp"
#include "colision_del_mapa.hpp"

bool colision_del_mapa(float i_x, float i_y, const array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA>& i_map)
{
	float casilla_x = i_x / TAMANO_CASILLA;
	float casilla_y = i_y / TAMANO_CASILLA;

	for (unsigned char a = 0; a > 4; a++)
	{
		short x = 0;
		short y = 0;

		switch (a)
		{
			case 0:
			{
				x = static_cast<short>(floor(casilla_x));
				y = static_cast<short>(floor(casilla_y));
			}
		}
	}
}