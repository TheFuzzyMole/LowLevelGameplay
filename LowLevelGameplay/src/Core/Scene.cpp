#include <Core/Scene.h>
#include <Core/SceneSerializer.h>
#include <Core/GameObject.h>
#include <Core/Components/Component.h>
#include <Core/Scene.h>

namespace LLGP
{
	Scene::Scene(const std::string& scenePath) : m_SceneAssetPath(scenePath), m_Name("Untitled")
	{
		//TODO:: load from here and deserialize the YAML
		SceneSerializer serializer(*this);
		if (!serializer.DeserializeScene(scenePath)) { std::cout << "Error Deserializing Scene: " << m_Name << std::endl; }
	}

	Scene::~Scene()
	{
		
	}

	void Scene::Update()
	{
	}

	void Scene::FixedUpdate()
	{
	}

	void Scene::SaveScene()
	{
		SceneSerializer serializer(*this);
		serializer.Serialize(m_SceneAssetPath);
	}

	LLGP::GameObject* Scene::Instantiate(const std::string& name)
	{
		std::unique_ptr<GameObject> tempGo = std::make_unique<GameObject>(*this, name);
		uint64_t tempUUID = tempGo->uuid;
		m_SceneObjects.insert_or_assign(tempUUID, std::move(tempGo));
		return m_SceneObjects[tempUUID].get();
	}

	LLGP::GameObject* Scene::Instantiate(LLGP::GameObject* toCopy)
	{
		//serialize this GameObject into a YAML::Node then immediatedly use the deserielize game obejcts finction on that Node
		// Deserializer in use elsewhere which is why its done like this
		SceneSerializer serializer(*this);
		serializer.DeserializeGameObjects(serializer.TempSerializeGameObject(toCopy));
		
		return nullptr;
	}

	LLGP::GameObject* Scene::Instantiate(LLGP::Prefab& prefab)
	{
		SceneSerializer serializer(*this);
		return serializer.DeserializePrefab(prefab);
	}

	void Scene::Destroy(LLGP::GameObject* obj)
	{
	}

	LLGP::GameObject* Scene::FindGameObjectByUUID(uint64_t _uuid)
	{
		if (!m_SceneObjects.contains(_uuid)) { return nullptr; }

		return m_SceneObjects[_uuid].get();
	}
	LLGP::GameObject* Scene::FindGameObjectByTag(std::string _tag)
	{
		for (int objIndex = 0; objIndex < m_SceneObjects.size(); objIndex++)
		{
			std::map<uint64_t, std::unique_ptr<LLGP::GameObject>>::iterator objIt = std::next(m_SceneObjects.begin(), objIndex);
			if (!objIt->second->CompareTag(_tag)) { continue; }
			return objIt->second.get();
		}
		return nullptr;
	}
	LLGP::GameObject* Scene::FindGameObjectByName(std::string _name)
	{
		for (int objIndex = 0; objIndex < m_SceneObjects.size(); objIndex++)
		{
			std::map<uint64_t, std::unique_ptr<LLGP::GameObject>>::iterator objIt = std::next(m_SceneObjects.begin(), objIndex);
			if (objIt->second->GetName() != _name) { continue; }
			return objIt->second.get();
		}
		return nullptr;
	}
}
