#include <array>
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Global.hpp"
#include "Player.hpp"
#include "Convertir_escena.hpp"

using namespace std;
using namespace sf;

array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA> convertir_escena(Player& i_player, Enemigo& i_enemigo)
{
    array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA> mapa_salida{};

    Image mapa;
    mapa.loadFromFile("mapa.png");

    for (unsigned char a = 0; a < ALTURA_MAPA; a++) {
        for (unsigned char b = 0; b < ANCHO_MAPA; b++) {
            Color pixel = mapa.getPixel(a, b);
            if (pixel == Color(0, 0, 0)) {
                mapa_salida[a][b] = Cell::Wall;}
            else if (pixel == Color(255, 0, 0)) {
                i_player.set_position(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
            }
            else if (pixel == Color(0, 0, 255))
            {
                i_enemigo.set_position(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));
            }
            else
            {
                mapa_salida[a][b] = Cell::Empty;
            }
        }
    }

    return mapa_salida;
}