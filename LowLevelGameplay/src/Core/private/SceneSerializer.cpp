#include <Core/SceneSerializer.h>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <type_traits>

#include <Core/GameObject.h>

namespace LLGP
{
	SceneSerializer::SceneSerializer(LLGP::Scene& const scene)
		: m_Scene(&scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_Name;
		out << YAML::Key << "GameObjects" << YAML::Value << YAML::BeginSeq;
		for (std::unique_ptr<LLGP::GameObject>& go : m_Scene->m_SceneObjects)
		{
			go->Serialize(out);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if(!data["Scene"])
			return false;

		m_Scene->m_Name = data["Scene"].as<std::string>();
		if (YAML::Node GameObjects = data["GameObjects"])
		{
			for (YAML::Node goData : GameObjects)
			{
				m_Scene->m_SceneObjects.emplace_back(std::make_unique<GameObject>(goData));

				

				if (YAML::Node components = goData["Components"])
				{
					YAML::Node tData = components["Transform"];
					LLGP::Transform* t = go->AddComponent<LLGP::Transform>(components["Transform"].as<uint64_t>());
					t->SetLocalPosition(tData["Position"].as<LLGP::Vector2f>());


					for (YAML::Node compData : components)
					{
						if (YAML::Node rbData = compData["Rigidbody"])
						{

						}
					}
				}
			}
		}
	}
}
