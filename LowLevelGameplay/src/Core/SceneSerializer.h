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

	class SceneSerializer
	{
	public:
		SceneSerializer(LLGP::Scene& const scene);

		void Serialize(const std::string& filePath);
		YAML::Node TempSerializeGameObject(LLGP::GameObject* toSerialize);

		bool DeserializeScene(const std::string& filePath);
		bool DeserializePrefab(const std::string& filePath);
		bool DeserializeGameObjects(YAML::Node GameObjects);

	private:
		std::vector<LinkRequest> m_LinkRequests;
		std::map<uint64_t, LLGP::Component*> m_Token2PtrLUT;
		LLGP::Scene* m_Scene;
	};
}
