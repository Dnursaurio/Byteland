
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 500), "ByteLand");
    window.setFramerateLimit(60);
    
    //DIBUJO
    sf::RectangleShape rect;

    sf::Vector2f rectanglePosition(0, 450);

    rect.setPosition(rectanglePosition);

    rect.setSize(sf::Vector2f(50, 50));




    //VENTANA
    while (window.isOpen()) {
        sf::Event event;
        //ventana se abre
        while (window.pollEvent(event)) {
            //ventana se cierra
            if (event.type == sf::Event::Closed) { window.close(); }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }
         }
    
        //RENDER
        window.clear();
        window.draw(rect);
        window.display();
    
    }

    




    return 0;
}