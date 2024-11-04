#pragma once

#include <Core/Scene.h>
#include <unordered_map>
#include <functional>

namespace YAML
{
	class Node;
}

namespace LLGP
{
	struct LinkRequest;
	class Prefab;

	class SceneSerializer
	{
	public:
		SceneSerializer(LLGP::Scene& const scene);

		void Serialize(std::filesystem::path  filePath);
		YAML::Node TempSerializeGameObject(LLGP::GameObject* toSerialize);

		bool DeserializeScene(std::filesystem::path  filePath);
		LLGP::GameObject* DeserializePrefab(const LLGP::Prefab& prefab);
		LLGP::GameObject* DeserializeGameObjects(YAML::Node GameObjects);

	private:
		std::vector<LinkRequest> m_LinkRequests;
		std::map<uint64_t, LLGP::Component*> m_Token2PtrLUT;
		LLGP::Scene* m_Scene;
	};
}
