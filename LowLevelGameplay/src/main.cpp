#include <SFML/Graphics.hpp>
#include <Core/Vector2.h>
#include <Core/Event.h>
#include <Core/Physics.h>
#include <Core/Commons.h>
#include <Core/GameObject.h>
#include <Core/Renderer.h>
#include <functional>
#include <string>
#include <iostream>
#include <chrono>

using namespace LLGP;


int main()
{
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	float deltaTime = 0.f;
	float timeSincePhysicsStep = 0.f;

	sf::RenderWindow window(sf::VideoMode(1800, 900), "SFML Works!");
	/*Vector2<float> rectSize = Vector2<float>::one * 100;
	Vector2<float> rectPos = Vector2<float>(900, 450);
	sf::Texture rectTex; rectTex.loadFromFile("Textures/tux.png");
	Vector2i spritesInTex(8, 9);
	Vector2i rectTexSize(rectTex.getSize());
	sf::IntRect rectTexUV(0, (rectTexSize.y / spritesInTex.y) * 5, rectTexSize.x/spritesInTex.x, rectTexSize.y/spritesInTex.y);
	sf::RectangleShape shape(rectSize);
	shape.setTexture(&rectTex);
	shape.setTextureRect(rectTexUV);
	shape.setOrigin(rectSize / 2);
	shape.setPosition(rectPos);*/

	//Initialisation
	LLGP::GameObject* player = new GameObject();
	LLGP::Renderer* playerRenderer = player->AddComponent<LLGP::Renderer>();
	playerRenderer->SetupQuad(LLGP::Vector2f(500.f, 500.f));
	playerRenderer->SetupTexture("Textures/tux.png", LLGP::Vector2u(8, 9));
	playerRenderer->SetupSpriteUV(LLGP::Vector2u(0, 5));
	LLGP::Transform* playerTransform = player->GetComponent<LLGP::Transform>();
	playerTransform->SetPosition(LLGP::Vector2f(900.f, 450.f));
	float timer = 0;
	int animIndex = 0;

	LLGP::GameObject* notPlayer = new GameObject();
	LLGP::Renderer* notplayerRenderer = notPlayer->AddComponent<LLGP::Renderer>();
	notplayerRenderer->SetupQuad(LLGP::Vector2f(500.f, 500.f));
	notplayerRenderer->SetupTexture("Textures/tux.png", LLGP::Vector2u(8, 9));
	notplayerRenderer->SetupSpriteUV(LLGP::Vector2u(0, 5));
	LLGP::Transform* notplayerTransform = notPlayer->GetComponent<LLGP::Transform>();
	notplayerTransform->SetPosition(LLGP::Vector2f(450.f, 225.f));

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

		timeSincePhysicsStep += deltaTime;
		while (timeSincePhysicsStep > FIXED_FRAMERATE)
		{
			Physics::StepPhysics();
			Physics::CollectCollisions();
			Physics::DispatchCollisions();
			timeSincePhysicsStep -= FIXED_FRAMERATE;
		}

		//run update
		timer += deltaTime;
		if (timer >= 0.3f)
		{
			animIndex = ++animIndex % 3;
			playerRenderer->SetupSpriteUV(LLGP::Vector2u(animIndex, 5));
			timer -= 0.3f;
		}
		

		window.clear();
		//window.draw(shape);
		LLGP::Renderer::OnRenderLayer(window, RenderLayers::DEFAULT);
		LLGP::Renderer::OnRenderLayer(window, RenderLayers::BACKGROUND);
		window.display();
	}

	return 0;
}
