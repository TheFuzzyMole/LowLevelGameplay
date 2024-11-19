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
#include <chrono>
#include <Core/Maths/Matrix3x3.h>
#include <Core/Assets/AssetManager.h>
#include <Core/Assets/Prefab.h>
#include <Utils/Debug.h>


using namespace LLGP;

std::condition_variable_any EndOfFrame;
std::condition_variable_any NextUpdate;
std::condition_variable_any NextFixedUpdate;

int main()
{
	std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
	Time::deltaTime = 0.f;
	float timeSincePhysicsStep = 0.f;
	sf::View view;

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML Works!");

#pragma region Input Initialisation
	LLGP::InputAction* moveAction = LLGP::InputManager::AddAction("Move", LLGP::ActionType::Vector);
	moveAction->AddBinding(new LLGP::InputBinding_Vector(
			LLGP::CompositeButtonVector(sf::Keyboard::S, sf::Keyboard::W, sf::Keyboard::A, sf::Keyboard::D),
			1.f, 0));
#pragma endregion

	Debug::Log("doing stuff");
	Debug::LogWarning("and things");
	Debug::LogError("wbu?");
	Debug::Assert(true, "it broke");

#pragma region level load

	LLGP::Scene* _GameScene = new Scene("Scenes/testing.scene");
	LLGP::GameObject* playerRef = _GameScene->FindGameObjectByName("player");
	std::shared_ptr<LLGP::Prefab> ballPrefab = LLGP::AssetManager::GetAsset<LLGP::Prefab>("Prefabs/BouncyBall.prefab");
	LLGP::GameObject* newBall = _GameScene->Instantiate(*ballPrefab);
	newBall->transform->SetPosition({1000, 450});
	//LLGP::GameObject* Ball = _GameScene->Instantiate(*ballPrefab);
	//Ball->transform->SetPosition({ 1000, 600 });


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
			LLGP::GameObject::OnWorldFixedUpdate();
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
		LLGP::Transform* cameraT = LLGP::Camera::main->GetGameObject()->transform;
		view.reset(
			sf::FloatRect(
				cameraT->GetPosition() - LLGP::Vector2f{SCREEN_WIDTH/2, SCREEN_HEIGHT/2},
				{ SCREEN_WIDTH, SCREEN_HEIGHT }));
		view.rotate(cameraT->GetRotation() * RAD2DEG);
		window.setView(view);
		LLGP::Renderer::OnRenderLayer(window, RenderLayers::DEFAULT);
		LLGP::Renderer::OnRenderLayer(window, RenderLayers::BACKGROUND);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
		{
			LLGP::Renderer::OnRenderLayer(window, RenderLayers::DEBUG);
		}
		window.setView(window.getDefaultView());
		//Render UI???
		window.display();

#pragma endregion

		EndOfFrame.notify_all();
	}

	delete _GameScene;
	return 0;
}
