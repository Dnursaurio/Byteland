#include <array>
#include <chrono>
#include <cmath>

#include "Global.hpp"
#include "Colision_mapa.hpp"

bool colision_mapa(float i_x, float i_y, const array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA>& i_map)
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

                break;
			}
            case 1:
            {
                x = static_cast<short>(ceil(casilla_x));
                y = static_cast<short>(floor(casilla_y));

                break;
            }
            case 2:
            {
                x = static_cast<short>(floor(casilla_x));
                y = static_cast<short>(ceil(casilla_y));

                break;
            }
            case 3:
            {
                x = static_cast<short>(ceil(casilla_x));
                y = static_cast<short>(ceil(casilla_y));
            }
		}
        if (0 <= x && 0 <= y && ALTURA_MAPA > y && ANCHO_MAPA > x)
        {
            if (Cell::Wall == i_map[x][y])
            {
                return 1;
            }
        }
	}
    return 0;
}