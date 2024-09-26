#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <Core/Maths/Vector2.h>
#include <Core/Commons.h>
#include <Core/Scene.h>
#include <Core/GameObject.h>
#include <Core/InputManager.h>
#include <Core/Event.h>
#include <Core/Physics.h>
#include <Core/Time.h>
#include <Core/Components/Components.h>
#include <functional>
#include <string>
#include <iostream>
#include <chrono>
#include <Core/Maths/Matrix3x3.h>

using namespace LLGP;

std::condition_variable_any EndOfFrame;
std::condition_variable_any NextUpdate;
std::condition_variable_any NextFixedUpdate;

int main()
{
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	Time::deltaTime = 0.f;
	float timeSincePhysicsStep = 0.f;

	sf::RenderWindow window(sf::VideoMode(1800, 900), "SFML Works!");

#pragma region Input Initialisation
	LLGP::InputAction* moveAction = LLGP::InputManager::AddAction("Move", LLGP::ActionType::Vector);
	moveAction->AddBinding(new LLGP::InputBinding_Vector(
			LLGP::CompositeButtonVector(sf::Keyboard::S, sf::Keyboard::W, sf::Keyboard::A, sf::Keyboard::D),
			1.f));
#pragma endregion

	//LLGP::Mat3f l2w = LLGP::Mat3f::FromPS(LLGP::Vector2f{900, 450}, LLGP::Vector2u{ 3, 6 });
	//LLGP::Mat3f local = LLGP::Mat3f::FromPos(LLGP::Vector2f{100,-300});
	//LLGP::Mat3f world = l2w * local;
	//LLGP::Vector2f pos; float rot{ 0 }; LLGP::Vector2u scale;
	//LLGP::Mat3f::Decompose(world, pos, rot, scale);

#pragma region level load

	LLGP::Scene* _GameScene = new Scene("Scenes/testing.scene");
	LLGP::GameObject* playerGO = _GameScene->FindGameObjectByName("player");
	_GameScene->Instantiate(playerGO);

#pragma endregion

	while (window.isOpen())
	{
		std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		Time::deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime).count() / 1000000.f;
		lastTime = now;

#pragma region window events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
#pragma endregion

#pragma region Fixed Update
		timeSincePhysicsStep += Time::deltaTime;
		while (timeSincePhysicsStep > FIXED_FRAMERATE)
		{
			NextFixedUpdate.notify_all();
			for (int iteration = 0; iteration < PHYSICS_SOLVER_ITERATIONS; iteration++)
			{
				Physics::StepPhysics();
				Physics::CollectCollisions();
				Physics::ResolveCollisionOverlaps();
			}
			Physics::DispatchCollisions();
			Physics::DispatchOverlaps();
			timeSincePhysicsStep -= FIXED_FRAMERATE;
		}
#pragma endregion

#pragma region Input Processing
		LLGP::InputManager::ProcessInput();
#pragma endregion

#pragma region Update
		//std::cout << "MainThread delta: " << LLGP::Time::deltaTime << std::endl;
		LLGP::Time::coroutineDeltaTime.store(LLGP::Time::deltaTime);
		NextUpdate.notify_all();

		LLGP::GameObject::OnWorldUpdate();

#pragma endregion

#pragma region rendering
		window.clear();
		LLGP::Renderer::OnRenderLayer(window, RenderLayers::DEFAULT);
		LLGP::Renderer::OnRenderLayer(window, RenderLayers::BACKGROUND);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			LLGP::Renderer::OnRenderLayer(window, RenderLayers::DEBUG);
		}
		window.display();
#pragma endregion

		EndOfFrame.notify_all();
	}

	delete _GameScene;
	return 0;
}
