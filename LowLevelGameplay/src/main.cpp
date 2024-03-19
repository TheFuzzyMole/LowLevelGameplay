#include <SFML/Graphics.hpp>
#include <Core/Vector2.h>
#include <Core/Event.h>
#include <functional>
#include <string>
#include <iostream>
#include <chrono>

#define FIXEDFRAMERATE 0.02f

using namespace LLGP;

class b2World;

b2World* m_World = nullptr;

void thing(int a, int b)
{
	std::cout << a << " and " << b << std::endl;
}

int main()
{
	Event<int, int>eventTest;

	eventTest.AddListener(&thing);
	eventTest.AddListener(&thing);
	eventTest.PrintListenerCount();
	eventTest.Invoke(5, 6);
	
	eventTest.RemoveListener(&thing);
	eventTest.PrintListenerCount();


	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.f;
	float timeSincePhysicsStep = 0.f;

	sf::RenderWindow window(sf::VideoMode(1800, 900), "SFML Works!");
	Vector2<float> rectSize = Vector2<float>::one * 100;
	Vector2<float> rectPos = Vector2<float>(900, 450);
	sf::Texture rectTex; rectTex.loadFromFile("Textures/tux.png");
	Vector2i spritesInTex(8, 9);
	Vector2i rectTexSize(rectTex.getSize());
	sf::IntRect rectTexUV(0, (rectTexSize.y / spritesInTex.y) * 5, rectTexSize.x/spritesInTex.x, rectTexSize.y/spritesInTex.y);
	sf::RectangleShape shape(rectSize);
	shape.setTexture(&rectTex);
	shape.setTextureRect(rectTexUV);
	shape.setOrigin(rectSize / 2);
	shape.setPosition(rectPos);

	while (window.isOpen())
	{
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 1000000.f;
		lastTime = now;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		//run update

		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXEDFRAMERATE)
		{
			//step the physics
			//collect collision info
			//dispatch collisions
			timeSincePhysicsStep -= FIXEDFRAMERATE;
		}


		window.clear();
		window.draw(shape);
		window.display();
	}

	return 0;
}
