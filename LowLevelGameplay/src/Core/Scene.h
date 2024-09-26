#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>

namespace LLGP
{
	class GameObject;
	class Component;
	class Asset;

	class Scene
	{
	public:
		Scene(const std::string& scenePath);
		~Scene();

		std::vector<GameObject*> s_PendingKillList;

		void Update();
		void FixedUpdate();

		void SaveScene();

		//TODO: Make a version that can take a prefab asset
		LLGP::GameObject* Instantiate(const std::string& name);
		LLGP::GameObject* Instantiate(LLGP::GameObject* toCopy);
		void Destroy(LLGP::GameObject* obj);

		LLGP::GameObject* FindGameObjectByUUID(uint64_t _uuid);
		LLGP::GameObject* FindGameObjectByTag(std::string _tag);
		LLGP::GameObject* FindGameObjectByName(std::string _name);

	private:
		std::string m_Name;
		std::string m_SceneAssetPath;
		std::map<uint64_t, std::unique_ptr<LLGP::GameObject>> m_SceneObjects;
		std::vector<std::shared_ptr<LLGP::Asset>> m_SceneTextures;
		std::vector<std::shared_ptr<LLGP::Asset>> m_SceneAnimations;

		friend class SceneSerializer;
	};
}
