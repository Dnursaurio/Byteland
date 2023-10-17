#include <SFML/Graphics.hpp>
#include <chrono>
#include <array>
#include "Global.hpp"
#include "Player.hpp"
#include "Convertir_escena.hpp"
using namespace std;
using namespace sf;
int main()
{
	//escondemos el HUD
	bool dibujar_mapa = 1;

	array<array<Cell, ALTURA_MAPA>, ANCHO_MAPA> mapa{};
	
	//independizamos la tasa de fotogramas del juego
	chrono::microseconds lag(0);

	chrono::steady_clock::time_point tiempo_previo;
	
	Event evento;

	//la ventana del proyecyo Byteland
	RenderWindow ventana(VideoMode(REESCALADO_DE_PANTALLA * ANCHO_PANTALLA, REESCALADO_DE_PANTALLA * ALTURA_PANTALLA),"Byteland Project", Style::Close);
	ventana.setMouseCursorVisible(0);
	ventana.setView(View(FloatRect(0,0, ANCHO_PANTALLA,ALTURA_PANTALLA)));
	
	Sprite casilla_de_la_grilla_del_mapa_sprite;
	
	Texture casilla_de_la_grilla_del_mapa_textura;
	casilla_de_la_grilla_del_mapa_textura.loadFromFile("sprites/Grilla.png");
	
	Player jugador(0, 0);

	casilla_de_la_grilla_del_mapa_sprite.setTexture(casilla_de_la_grilla_del_mapa_textura);
	casilla_de_la_grilla_del_mapa_sprite.setTextureRect(IntRect(0,0,TAMANO_CASILLA_DE_GRILLA_EN_MAPA,TAMANO_CASILLA_DE_GRILLA_EN_MAPA));

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
		}
		//jugador.update(ventana);

		ventana.clear();
		ventana.display();
	}
	return 0;
}