#include <SFML/Graphics.hpp>
#include <Core/Vector2.h>
#include <Core/Commons.h>
#include <Core/Scene.h>
#include <Core/GameObject.h>
#include <Core/InputManager.h>
#include <Core/Event.h>
#include <Core/Physics.h>
#include <Core/Time.h>
#include <Core/Components.h>
#include <functional>
#include <string>
#include <iostream>
#include <chrono>

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

#pragma region level load

	LLGP::Scene* _GameScene = new Scene("Scenes/testing.scene", "Testing");
	LLGP::GameObject* player = _GameScene->Instantiate("player");
	LLGP::Renderer* playerRenderer = player->AddComponent<LLGP::Renderer>();
	player->transform->SetPosition(LLGP::Vector2f(900.f, 450.f));
	playerRenderer->SetupQuad(LLGP::Vector2f(250.f, 250.f));
	playerRenderer->SetupTexture("Textures/tux.png", LLGP::Vector2u(8, 9));
	playerRenderer->SetupSpriteUV(LLGP::Vector2u(0, 5));
	LLGP::Rigidbody* playerRB = player->AddComponent<LLGP::Rigidbody>();
	playerRB->Mass = 10.f;
	playerRB->HasGravity = false;
	TEST::PlayerMovement* playerMove = player->AddComponent<TEST::PlayerMovement>();
	playerMove->SetSpeed(100.f);
	LLGP::Animator* playerAnim = player->AddComponent<LLGP::Animator>();
	playerAnim->SetAnimation("Animations/tuxWave.anim");

	player->OnStart();

	float timer = 0;
	int animIndex = 0;

	LLGP::GameObject* notPlayer = _GameScene->Instantiate("notPlayer");
	notPlayer->transform->SetPosition(LLGP::Vector2f(450.f, 225.f));
	LLGP::Renderer* notplayerRenderer = notPlayer->AddComponent<LLGP::Renderer>();
	notplayerRenderer->SetupQuad(LLGP::Vector2f(250.f, 250.f));
	notplayerRenderer->SetupTexture("Textures/tux.png", LLGP::Vector2u(8, 9));
	notplayerRenderer->SetupSpriteUV(LLGP::Vector2u(0, 5));

	notPlayer->OnStart();
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
			Physics::StepPhysics();
			Physics::CollectCollisions();
			Physics::DispatchCollisions();
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

		/*timer += Time::deltaTime;
		if (timer >= 0.3f)
		{
			animIndex = ++animIndex % 3;
			playerRenderer->SetupSpriteUV(LLGP::Vector2u(animIndex, 5));
			timer -= 0.3f;
		}*/
#pragma endregion

#pragma region rendering
		window.clear();
		LLGP::Renderer::OnRenderLayer(window, RenderLayers::DEFAULT);
		LLGP::Renderer::OnRenderLayer(window, RenderLayers::BACKGROUND);
		window.display();
#pragma endregion

		/*or (LLGP::GameObject* forDeath : LLGP::GameObject::s_PendingKillList)
		{
			delete(forDeath);
		}
		LLGP::GameObject::s_PendingKillList.clear();*/

		EndOfFrame.notify_all();
	}

	delete _GameScene;
	return 0;
}
