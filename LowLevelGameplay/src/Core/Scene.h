#pragma once
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <filesystem>

namespace LLGP
{
	class GameObject;
	class Component;
	class Asset;
	class Prefab;

	class Scene
	{
	public:
		Scene(std::filesystem::path scenePath);
		~Scene();

		std::vector<GameObject*> s_PendingKillList;

		void Update();
		void FixedUpdate();

		void SaveScene();

		LLGP::GameObject* Instantiate(const std::string& name);
		LLGP::GameObject* Instantiate(LLGP::GameObject* toCopy);
		LLGP::GameObject* Instantiate(LLGP::Prefab& prefab);
		void Destroy(LLGP::GameObject* obj);

		LLGP::GameObject* FindGameObjectByUUID(uint64_t _uuid);
		LLGP::GameObject* FindGameObjectByTag(const std::string& _tag);
		LLGP::GameObject* FindGameObjectByName(const std::string& _name);

	private:
		std::string m_Name;
		std::filesystem::path m_SceneAssetPath;
		std::map<uint64_t, std::unique_ptr<LLGP::GameObject>> m_SceneObjects;
		std::vector<std::shared_ptr<LLGP::Asset>> m_SceneTextures;
		std::vector<std::shared_ptr<LLGP::Asset>> m_SceneAnimations;
		std::vector<std::shared_ptr<LLGP::Asset>> m_ScenePrefabs;

		friend class SceneSerializer;
	};
}
