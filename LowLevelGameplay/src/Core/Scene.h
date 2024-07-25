#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>

namespace LLGP
{
	class GameObject;
	class Component;

	class Scene
	{
	public:
		Scene(const std::string& scenePath);
		~Scene();

		std::vector<GameObject*> s_PendingKillList;

		void Update();
		void FixedUpdate();

		//TODO: Make a version that can take a prefab asset
		GameObject* Instantiate(const std::string& name);
		GameObject* Instantiate(GameObject* toCopy);
		void Destroy(GameObject* obj);

		LLGP::GameObject* FindGameObjectByUUID(uint64_t _uuid);

	private:
		std::string m_Name;
		std::string m_SceneAssetPath;
		std::map<uint64_t, std::unique_ptr<LLGP::GameObject>> m_SceneObjects;

		friend class SceneSerializer;
	};
}
