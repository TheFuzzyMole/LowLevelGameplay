#include <Core/Scene.h>
#include <Core/SceneSerializer.h>
#include <Core/GameObject.h>
#include <Core/Scene.h>

namespace LLGP
{
	Scene::Scene(const std::string& scenePath, const std::string& name) : m_SceneAssetPath(scenePath), m_Name(name)
	{
		//TODO:: load from here and deserialize the YAML
		SceneSerializer serializer(*this);
		if (!serializer.Deserialize(scenePath)) { std::cout << "Error Deserializing Scene: " << m_Name << std::endl; }
	}

	Scene::~Scene()
	{
		SceneSerializer serializer(*this);
		serializer.Serialize(m_SceneAssetPath);
	}

	void Scene::Update()
	{
	}

	void Scene::FixedUpdate()
	{
	}

	GameObject* Scene::Instantiate(const std::string& name)
	{
		std::unique_ptr<GameObject> tempGo = std::make_unique<GameObject>(*this, name);
		m_SceneObjects.insert_or_assign(tempGo->uuid, std::move(tempGo));
		return std::prev(m_SceneObjects.end())->second.get();
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
	LLGP::GameObject* Scene::FindGameObjectByUUID(uint64_t _uuid)
	{
		if (!m_SceneObjects.contains(_uuid)) { return nullptr; }

		return m_SceneObjects[_uuid].get();
	}
}
