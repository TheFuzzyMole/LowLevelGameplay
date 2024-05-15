#pragma once
#include <memory>
#include <vector>
#include <string>

namespace LLGP
{
	class GameObject;

	class Scene
	{
	public:
		Scene(const std::string& scenePath, const std::string& name);
		~Scene();

		std::vector<GameObject*> s_PendingKillList;

		void Update();
		void FixedUpdate();

		//TODO: Make a version that can take a prefab asset
		GameObject* Instantiate(const std::string& name);
		GameObject* Instantiate(GameObject* toCopy);
		void Destroy(GameObject* obj);

	private:
		std::string m_Name;
		std::string m_SceneAssetPath;
		std::vector<std::unique_ptr<GameObject>> m_SceneObjects;

		friend class SceneSerializer;
	};
}
