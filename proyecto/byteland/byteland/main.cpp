#include <SFML/Graphics.hpp>
using namespace sf;
int main()
{
	RenderWindow ventana(VideoMode(800,600),"Byteland Project");

	while (ventana.isOpen())
	{
		Event evento;
		while (ventana.pollEvent(evento))
		{
			if(evento.type == Event::Closed)
			{
				ventana.close();
			}
		}

		ventana.clear();
		ventana.display();
	}
	return 0;
}