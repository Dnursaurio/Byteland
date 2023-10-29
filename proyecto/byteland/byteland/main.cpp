#include <SFML/Graphics.hpp>
#include <chrono>
#include <array>
#include "Global.hpp"
#include "Player.hpp"
#include "Enemigo.hpp"
#include "Convertir_escena.hpp"
using namespace std;
using namespace sf;
int main()
{
	bool dibujar_mapa = 1;

	array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA> map{};

	chrono::microseconds lag(0);

	chrono::steady_clock::time_point tiempo_previo;

	Event evento;

	RenderWindow ventana(VideoMode(REESCALADO_DE_PANTALLA * ANCHO_PANTALLA, REESCALADO_DE_PANTALLA * ALTURA_PANTALLA), "Byteland Project", Style::Close);
	ventana.setMouseCursorVisible(0);
	ventana.setView(View(FloatRect(0, 0, ANCHO_PANTALLA, ALTURA_PANTALLA)));

	Sprite casilla_de_la_grilla_del_mapa_sprite;
	Sprite pared_mapa_sprite;

	Texture casilla_de_la_grilla_del_mapa_textura;
	casilla_de_la_grilla_del_mapa_textura.loadFromFile("sprites/Grilla.png");

	Texture pared_mapa_textura;
	pared_mapa_textura.loadFromFile("sprites/mapa_pared" + to_string(TAMANO_CASILLA_EN_MAPA) + ".png");

	Player jugador(0, 0);
	Enemigo enemy(0, 0);
	map = convertir_escena(jugador, enemy);

	casilla_de_la_grilla_del_mapa_sprite.setTexture(casilla_de_la_grilla_del_mapa_textura);
	casilla_de_la_grilla_del_mapa_sprite.setTextureRect(IntRect(0, 0, TAMANO_CASILLA_DE_GRILLA_EN_MAPA, TAMANO_CASILLA_DE_GRILLA_EN_MAPA));

	tiempo_previo = chrono::steady_clock::now();

	while (1 == ventana.isOpen())
	{
		chrono::microseconds delta_time = chrono::duration_cast<chrono::microseconds>(chrono::steady_clock::now() - tiempo_previo);

		lag = delta_time;

		tiempo_previo += delta_time;

		while (DURACION_FOTOGRAMA <= lag)
		{
			lag -= DURACION_FOTOGRAMA;

			while (1 == ventana.pollEvent(evento))
			{
				switch (evento.type)
				{
				case Event::Closed:
				{
					ventana.close();

					break;
				}
				case Event::KeyPressed:
				{
				case Keyboard::H:
				{
					dibujar_mapa = 1 - dibujar_mapa;
				}
				}
				}
			}
			jugador.update(map, ventana);
			enemy.update(map, ventana);

			if (DURACION_FOTOGRAMA > lag)
			{
				ventana.clear(Color(73, 255, 255));

				jugador.draw_screen(ventana, enemy);

				if (1 == dibujar_mapa)
				{
					for (unsigned short a = 0; a < ceil(TAMANO_CASILLA_EN_MAPA * ANCHO_MAPA / static_cast<float>(TAMANO_CASILLA_DE_GRILLA_EN_MAPA)); a++)
					{
						for (unsigned short b = 0; b < ceil(TAMANO_CASILLA_EN_MAPA * ALTURA_MAPA / static_cast<float>(TAMANO_CASILLA_DE_GRILLA_EN_MAPA)); b++)
						{
							casilla_de_la_grilla_del_mapa_sprite.setPosition(static_cast<float>(TAMANO_CASILLA_EN_MAPA * a), static_cast<float>(TAMANO_CASILLA_EN_MAPA * b));

							ventana.draw(casilla_de_la_grilla_del_mapa_sprite);
						}
					}

					for (unsigned short a = 0; a < ANCHO_MAPA; a++)
					{
						for (unsigned short b = 0; b < ALTURA_MAPA; b++)
						{
							if (Cell::Wall == map[a][b])
							{
								pared_mapa_sprite.setPosition(static_cast<float>(TAMANO_CASILLA_EN_MAPA * a), static_cast<float>(TAMANO_CASILLA_EN_MAPA * b));

								ventana.draw(pared_mapa_sprite);
							}
						}
					}
					jugador.draw_map(ventana);

					enemy.dibujar_mapa(ventana);
				}
				ventana.display();
			}
		}
	}
}