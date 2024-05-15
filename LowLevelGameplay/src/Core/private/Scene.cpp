#include <Core/Scene.h>
#include <Core/SceneSerializer.h>
#include <Core/GameObject.h>
#include <Core/Scene.h>

namespace LLGP
{
	Scene::Scene(const std::string& scenePath, const std::string& name) : m_SceneAssetPath(scenePath), m_Name(name)
	{
		//TODO:: load from here and deserialize the YAML
	}

	Scene::~Scene()
	{
		//SceneSerializer serializer(*this);
		//serializer.Serialize(m_SceneAssetPath);
	}

	void Scene::Update()
	{
	}

	void Scene::FixedUpdate()
	{
	}

	GameObject* Scene::Instantiate(const std::string& name)
	{
		m_SceneObjects.emplace_back(std::make_unique<GameObject>(name));
		return m_SceneObjects.back().get();
	}

	GameObject* Scene::Instantiate(GameObject* toCopy)
	{
		//m_SceneObjects.emplace_back(std::make_unique<GameObject>(toCopy));
		//TODO:: sort out actually making sure components are copied;
		//return m_SceneObjects.back().get();
		return nullptr;
	}

	void Scene::Destroy(GameObject* obj)
	{
	}
}