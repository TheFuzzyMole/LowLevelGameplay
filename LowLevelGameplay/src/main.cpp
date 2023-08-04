#include <SFML/Graphics.hpp>
#include <Core/Vector2.h>
#include <string>

using namespace LLGP;

class b2World;

b2World* m_World = nullptr;

int main()
{
	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML Works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	Vector2<float> vf = Vector2<float>::one;
	vf = vf * 6.f;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}
