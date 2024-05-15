#pragma once

#include <Core/Scene.h>

namespace LLGP
{
	class SceneSerializer
	{
	public:
		SceneSerializer(LLGP::Scene& const scene);

		void Serialize(const std::string& filePath);

		bool Deserialize(const std::string& filePath);

	private:
		LLGP::Scene* m_Scene;
	};
}
